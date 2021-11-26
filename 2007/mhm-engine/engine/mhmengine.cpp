//==========================================================================================
// Mayhem engine root class implementation
// Coded:	MozG
// Created:	25 june 2006
//==========================================================================================
#include "mhmbase.h"

// Engine constructor
//==========================================================================================

clsEngine::clsEngine(HINSTANCE hinst)
{
	m_hInst        = hinst;
	m_pRenderer    = mmnull;
	m_pInput       = mmnull;
	m_pEFactory    = mmnull;
	m_pActiveScene = mmnull;
	m_pRenderArray	=	new clsRenderArray(1024,1056);
	m_pLightsArray	=	new clsRenderArray(128,256);
	m_pRenderArray->AutoTruncate = mmfalse;
	m_pLightsArray->AutoTruncate = mmfalse;
	m_cSuicides.AutoTruncate     = mmfalse;
	m_cCollides.AutoTruncate     = mmfalse;
	m_pSoundManager = mmnull;
//	m_pDataProvider = mmnull;
}

// Destructor
//==========================================================================================
clsEngine::~clsEngine()
{
	mmSafeDelete(m_pLightsArray);
	mmSafeDelete(m_pRenderArray);
	m_hInst = mmnull;
	Shut();
}

#pragma region CPU
mmVoid clsEngine::DefineCPU()
{
	g_pEnvironment->m_eCpuType = enCPU_Old;

#ifdef WIN32
	do{
		__try
		{
			__asm
			{
				mov     eax,0       ; function 0 = manufacturer string
					CPUID
			}
		}
		__except (1)
		{
			break;
		}

		int cp = 0;
		__asm
		{

			test	eax,eax
				jz	no_f

				; Load up the features and (where appropriate) extended features flags
				mov     eax,1               ; Check for processor features
				CPUID

				shr	edx,26
				jnc	no_s

				mov	eax,1
				mov	[cp],eax   ; SSE

no_s:

			mov     eax,080000000h      ; Check for support of extended functions.
				CPUID
				; Check which extended functions can be called
				cmp     eax,080000001h      ; Extended Feature Bits
				jb      no_f                ; jump if not supported

				mov     eax,080000001h      ; Select function 0x80000001
				CPUID
				//        mov     [ext_features],edx  ; Store extended features bits
				add	edx,edx
				jnc	no_f

				mov	eax,2
				mov	[cp],eax   ; 3DNOW

no_f:
		}

		g_pEnvironment->m_eCpuType = (enCpu)cp;
		break;
	}
	while(1);
#endif
}
#pragma endregion

// Init: creates and initializes all engine subsystems
//==========================================================================================
mmRes clsEngine::Init(HWND hwnd, const mmString inifile)
{
	m_sModulePath.Alloc(MAX_PATH);
	if (GetModuleFileName((HMODULE)m_hInst, m_sModulePath, MAX_PATH) == 0)
	{
		g_plog->Out("clsEngine::Init >> cant get module file name", enLCRed);
		return mmFail;
	}
	m_sModulePath.Truncate();
	clsString filename = m_sModulePath.CutBy('\\', mmfalse);
	m_sModulePath = m_sModulePath.Left(m_sModulePath.Length() - filename.Length());
	
	DefineCPU();
	
	clsString FullINI = m_sModulePath + inifile;

	m_pRenderer = new clsRenderer(this);
	if (!m_pRenderer)
		mmThrow(enExceptOutOfMemory, clsEngine, Init, Renderer object alloc failed);
	if (MHMFAIL(m_pRenderer->Init(hwnd, FullINI)))
	{
		g_plog->Out("clsEngine::Init >> cant initialize renderer", enLCRed);
		return mmFail;
	}
	
	m_pInput = new clsInput(this);
	if (!m_pInput)
		mmThrow(enExceptOutOfMemory, clsEngine, Init, Input object alloc failed);
	if (MHMFAIL(m_pInput->Init(hwnd)))
	{
		g_plog->Out("clsEngine::Init >> cant initialize input subsystem",enLCRed);
		return mmFail;
	}
	m_timer.Initialization();
	
	m_pSoundManager = new clsSoundManager();
	if (m_pSoundManager == mmnull)
		mmThrow(enExceptOutOfMemory, clsEngine, Init, Sound manager object alloc failed);
	
	//fyrex new
/*
	m_pDataProvider = new clsDataProvider();
	m_pDataProvider->E=this; // they are friends !
*/

	m_cSuicides.SetAddSize(1024);
	m_cCollides.SetAddSize(1024);
	
	return mmOk;
}

// Shut: shutdowns engine subsystems
//==========================================================================================
mmRes clsEngine::Shut()
{
//	mmSafeDelete(m_pDataProvider);
	mmSafeDelete(m_pSoundManager);
	mmSafeDelete(m_pInput);
	mmSafeDelete(m_pRenderer);
	m_pEFactory    = mmnull;
	m_pActiveScene = mmnull;
	return mmOk;
}

// GetRenderer: returns engine renderer
//==========================================================================================
clsRenderer* clsEngine::GetRenderer()
{
	return m_pRenderer;
}

// GetInput: returns engine input subsystem
//==========================================================================================
clsInput* clsEngine::GetInput()
{
	return m_pInput;
}

// GetTimer: returns engine timer
//==========================================================================================
clsTimer* clsEngine::GetTimer()
{
	return &m_timer;
}

// Update: must be called in idle loop
//==========================================================================================
mmRes clsEngine::Update()
{
	// reset environment buffers checkes
	g_pEnvironment->m_dwLastFVF = 0;
	g_pEnvironment->m_pLastVB = mmnull;
	g_pEnvironment->m_pLastIB = mmnull;
	
	m_timer.Update();
	m_pRenderer->Update();
	m_pInput->Snapshot();
	mmUint i = 0;
	mmUint j = 0;
	if (m_pActiveScene != mmnull)
	{
		// first kill every who wanted it very very
		m_cSuicides.Size(0);
		m_cCollides.Size(0);
		m_pActiveScene->Collect(m_cSuicides, m_cCollides);
		for (mmUint z = 0; z < m_cSuicides.Size(); z++)
		{
			clsEntity* pEntity = m_cSuicides[z];
			pEntity->ProcessUnbindDepencies();
			delete pEntity;
		}
		
		
		clsMouse* pMouse = m_pInput->GetMouse();
		if (pMouse->IsAcquired())
		{
			const clsMouseState* pState = pMouse->GetMouseState();
			const clsMouseEvent* pEvents = pState->GetEventsPointer();
			for (i = 0; i < pState->GetNumEvents(); i++)
				m_pActiveScene->ProcessBufferedMouseEvent(&pEvents[i], i==0);
			m_pActiveScene->ProcessMouseEvent(pState->GetSnapshot());
		}
		clsKeyboard* pKeyboard = m_pInput->GetKeyboard();
		if (pKeyboard->IsAcquired())
		{
			const clsKeyboardState* pKBState = pKeyboard->GetState();
			const clsKeyState* pKBStates = pKBState->GetKeysStatePointer();
			for (j = 0; j < pKBState->GetNumAffectedKeys(); j++)
				m_pActiveScene->ProcessBufferedKeyboardEvent(&pKBStates[j], j==0);
		}
		//if ((i != 0) || (j != 0))
		m_pActiveScene->ProcessEndBufferedEvents(i, j);
		
		m_pActiveScene->ProcessSpeedCalc(&m_timer);
		
		for (mmUint a = 0; a < m_cCollides.Size(); a++)
			for (mmUint b = a + 1; b < m_cCollides.Size(); b++)
			{
				clsEntity* pEntityA = m_cCollides[a];
				clsEntity* pEntityB = m_cCollides[b];
				pEntityA->CheckCollision(pEntityB);
			}
		
		m_pActiveScene->ProcessUpdate(&m_timer);
	}
	
	return mmOk;
}

// Resize: resizes all device objects
//==========================================================================================
mmRes clsEngine::Resize()
{
	if (MHMFAIL(m_pRenderer->Resize()))
	{
		g_plog->Out("clsEngine::Resize >> renderer resizing failed", enLCRed);
		return mmFail;
	}
	if (MHMFAIL(m_pInput->Resize()))
	{
		g_plog->Out("clsEngine::Resize >> input subsystem resizing failed", enLCRed);
		return mmFail;
	}
	return mmOk;
}

// GetHinstance: returns engine module handler
//==========================================================================================
HINSTANCE clsEngine::GetHinstance()
{
	return m_hInst;
}

// GetModulePath: returns module path
//==========================================================================================
const clsString& clsEngine::GetModulePath()
{
	return m_sModulePath;
}

// GetViewWidth: returns device width
//==========================================================================================
mmUint clsEngine::GetViewWidth()
{
	if (m_pRenderer == mmnull)
		return 0;
	return m_pRenderer->GetWidth();
}

// GetViewHeight: returns device height
//==========================================================================================
mmUint clsEngine::GetViewHeight()
{
	if (m_pRenderer == mmnull)
		return 0;
	return m_pRenderer->GetHeight();
}

// SetEntityFactory: sets entity factory class
//==========================================================================================
mmRes clsEngine::SetEntityFactory(clsEntityFactory* pfact)
{
	m_pEFactory = pfact;
	return mmOk;
}

// CreateScene: just distribute Node
//==========================================================================================
clsEntity * clsEngine::CreateScene(const clsString& name)
{
	if (m_pEFactory == mmnull)
		return mmnull;
	clsEntity* e = m_pEFactory->CreateEntity("Scene", name);
	e->Type		 = enScene;
	e->Renderer  = m_pRenderer;
	return e;
}

// DestroyScene: scene killing
//==========================================================================================
mmVoid clsEngine::DestroyScene(clsEntity* pRoot)
{
	if (pRoot == mmnull)
		return;
	pRoot->ProcessUnbindDepencies();
	delete pRoot;
}


mmVoid clsEngine::CollectRenderables(clsEntity * s)
{
	clsEntity *e=s;
	mmBool Renderable;

	while(e)
	{
		Renderable = mmfalse;
		if(e->m_bRenderable)// && e->Res)
		{
			// at first lights
			if(e->Type == enLight)
			{
				m_cCurrentRenderObject.E = e;
				m_cCurrentRenderObject.dist = 0;
				((clsBaseLight*)e->Res)->SetPosition(clsVector3(e->m_mxFull._41,e->m_mxFull._42,e->m_mxFull._43));
				m_pLightsArray->Add(m_cCurrentRenderObject);
			}
			else
			{
				//// mozg' camera magic !
				clsCamera* pcam = m_pRenderer->GetCamera();
				Renderable = ((pcam == mmnull) || (!e->m_bEnableSphereTest));

				if(!Renderable)
				{
					clsBoundSphere bs = e->m_cBoundSphere;
					clsVector3 vRadius(1.0f, 0.0f, 0.0f);
					vRadius.TransformNormal(e->m_mxFull);
					bs.m_fRadius *= vRadius.Length();
					bs.m_cCenter.Transform(e->m_mxFull);
					Renderable = (pcam->SphereIn(bs.m_cCenter, bs.m_fRadius));
					if(!Renderable)
					{
						e->OnNoSphereTest();
					}
				}
				//Renderable = mmtrue;
				///

				if(Renderable)
				{
					m_mxCurrentMatrix = e->m_mxFull * *m_pmxCurrentView;
					m_cCurrentRenderObject.E = e;
					m_cCurrentRenderObject.dist = m_mxCurrentMatrix._43;
					m_pRenderArray->Add(m_cCurrentRenderObject);
				}
			}
		}
		if (!Renderable && e->m_bBlockRender)
		{
			e = e->Next;
			continue;
		}
		CollectRenderables(e->Child);
		e = e->Next;
	}
}

mmVoid clsEngine::RenderScene(clsEntity * s)
{
	if (s == mmnull)
		s = m_pActiveScene;
	
	if (s == mmnull)
		return;

	clsEntity *e;
	
	//m_pRenderer->GetDevice()->SetBaseStates();

	m_pmxCurrentView = m_pRenderer->GetDevice()->GetViewTM();
	m_pLightsArray->Size(0);
	m_pRenderArray->Size(0);
	CollectRenderables(s);
	m_pRenderArray->Sort();

	for(mmUint i=0; i < m_pLightsArray->Size(); i++)
	{
		e = (*m_pLightsArray)[i].E;

		((clsBaseLight*)(e->Res))->Activate();
	}

	m_pRenderer->GetDevice()->DepthWrite(mmtrue);

	// OPACITY
	//for(mmUint i=0; i < m_pRenderArray->Size(); i++)
	for(mmUint i=(m_pRenderArray->Size()-1); i!=(0-1); i--)
	{
		e = (*m_pRenderArray)[i].E;
		if(((e->Res) && (e->Res->GetBlendType() == enBlendNone)) || (!e->Res))
		{
			e->Render();
		}
	}

	m_pRenderer->GetDevice()->DepthWrite(mmfalse);

	// TRANSPARENT
	//for(mmUint i=(m_pRenderArray->Size()-1); i!=(0-1); i--)
	for(mmUint i=0; i < m_pRenderArray->Size(); i++)
	{
		e = (*m_pRenderArray)[i].E;
		if((e->Res) && (e->Res->GetBlendType() != enBlendNone))
		{
			e->Render();
		}
	}

	m_pRenderer->GetDevice()->DepthWrite(mmtrue);
	for(mmUint i=0; i < m_pLightsArray->Size(); i++)
	{
		e = (*m_pLightsArray)[i].E;
		((clsBaseLight*)(e->Res))->Deactivate();
	}

	/*
	// non alpha meshes
	do
	{
		if(e->Res)
		{
			m_pRenderer->GetDevice()->SetModelTM(&e->m_mxFull);
			if (e->Res)
			{
				if (mmHLID(e->Res->GetType()) == MHMRES_MESH_ID)
				{
					clsBaseMesh* pMesh = (clsBaseMesh*)e->Res;
					clsBaseMaterial* pMtl = (clsBaseMaterial*)pMesh->GetMaterial(0);
					if (pMtl->m_need_alphablend)
						continue;
				}
				clsCamera* pcam = m_pRenderer->GetCamera();
				if ((pcam == mmnull) || (!e->m_bEnableSphereTest))
				{
					e->Res->Render();
					e->OnRender();
				}
				else
				{
					clsBoundSphere bs = e->m_cBoundSphere;
					bs.m_cCenter.Transform(e->m_mxFull);
					if (pcam->SphereIn(bs.m_cCenter, bs.m_fRadius))
						e->Render();
					e->OnRender();
				}
			}
		}
	}while(e = e->List);
	e = s;
	do
	{
		if(e->Res)
		{
			m_pRenderer->GetDevice()->SetModelTM(&e->m_mxFull);
			if (e->Res)
			{
				if (mmHLID(e->Res->GetType()) == MHMRES_MESH_ID)
				{
					clsBaseMesh* pMesh = (clsBaseMesh*)e->Res;
					clsBaseMaterial* pMtl = (clsBaseMaterial*)pMesh->GetMaterial(0);
					if (!pMtl->m_need_alphablend)
						continue;
				}

				clsCamera* pcam = m_pRenderer->GetCamera();
				if ((pcam == mmnull) || (!e->m_bEnableSphereTest))
				{
					e->Res->Render();
					e->OnRender();
				}
				else
				{
					clsBoundSphere bs = e->m_cBoundSphere;
					bs.m_cCenter.Transform(e->m_mxFull);
					if (pcam->SphereIn(bs.m_cCenter, bs.m_fRadius))
						e->Render();
					e->OnRender();
				}
			}
		}
	}while(e = e->List);
*/	
}

// SetActiveScene: sets active scene
//==========================================================================================
mmVoid clsEngine::SetActiveScene(clsEntity *pRoot)
{
	m_pActiveScene = pRoot;
}



mmBool	clsEngine::CheckSM()
{
	if (!m_pSoundManager)
	{
		g_plog->Out("clsEngine::GetSample >> no sound manager created", enLCRed);
		return mmfalse;
	}
	return mmtrue;
}

// GetSample: returns buffered sound
//==========================================================================================
clsSound* clsEngine::GetSample(const clsString & sFileName)
{
	if(CheckSM())
		return m_pSoundManager->GetSample(sFileName);
	return mmnull;
}

// GetSound: returns sound
//==========================================================================================
clsSound* clsEngine::GetSound(const clsString & sFileName)
{
	if(CheckSM())
		return m_pSoundManager->GetSound(sFileName);
	return mmnull;
}

// GetSound: returns sound
//==========================================================================================
clsSound* clsEngine::GetStream(const clsString & sFileName)
{
	if(CheckSM())
		return m_pSoundManager->GetSound(sFileName);
	return mmnull;
}

void clsEngine::SoundVolume(const mmFloat V)
{
	if(CheckSM())
		m_pSoundManager->GlobalSoundVolume(V);
}
void clsEngine::StreamVolume(const mmFloat V)
{
	if(CheckSM())
		m_pSoundManager->GlobalSoundVolume(V);
}

//
// SoundManager object
//
clsSoundManager* clsEngine::GetSM()
{
	return m_pSoundManager;
}
//
/*
clsDataProvider* clsEngine::GetDP()
{
	return m_pDataProvider;
}
*/