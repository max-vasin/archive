//==========================================================================================
// BIOKRIZ survival mode class implementation
// Coded:	MozG
// Created:	29 july 2006
//==========================================================================================
#include "mhmbase.h"
#include "BKDefs.h"
#include "BKCreatureEmitter.h"
#include "BKGameCamera.h"
#include "BKSurvival.h"

// Survival mode constructor
//==========================================================================================
clsBiokrizSurvival::clsBiokrizSurvival(clsEngine *pEngine)
{
	// Retrieving engine objects
	m_pEngine = pEngine;
	if (m_pEngine == mmnull)
		mmThrow(enExceptObjectMissed, clsBiokrizSurvival, constructor, Engine object missed);
	
	m_pRenderer = m_pEngine->GetRenderer();
	if (m_pRenderer == mmnull)
		mmThrow(enExceptObjectMissed, clsBiokrizSurvival, constructor, Renderer object missed);
	
	m_pInput    = m_pEngine->GetInput();
	if (m_pInput == mmnull)
		mmThrow(enExceptObjectMissed, clsBiokrizSurvival, constructor, Input object missed);
	
	m_pKeyboard = m_pInput->GetKeyboard();
	if (m_pKeyboard == mmnull)
		mmThrow(enExceptObjectMissed, clsBiokrizSurvival, constructor, Keyboard object missed);

	m_pMouse = m_pInput->GetMouse();
	if (m_pMouse == mmnull)
		mmThrow(enExceptObjectMissed, clsBiokrizSurvival, constructor, Mouse object missed);
	
	m_pTimer = m_pEngine->GetTimer();
	if (m_pTimer == mmnull)
		mmThrow(enExceptObjectMissed, clsBiokrizSurvival, constructor, Timer object missed);
	
	// Initialize mode objects
	m_sMeshPath  = BK_SURVIVAL_MESH_PATH;
	m_pModeScene = mmnull;
	m_pMusic     = mmnull;
	m_bSlow      = mmfalse;
}

// Survival mode destructor
//==========================================================================================
clsBiokrizSurvival::~clsBiokrizSurvival()
{
	m_pEngine	= mmnull;
	m_pRenderer	= mmnull;
	m_pInput	= mmnull;
	m_pKeyboard	= mmnull;
	m_pMouse	= mmnull;
	mmSafeDelete(m_pMusic);
}

// LoadObjects: loads all mode resources
//==========================================================================================
mmRes clsBiokrizSurvival::LoadObjects()
{
	g_plog->Out("Survival: loading mode objects", enLCBlue);
	g_plog->Out("Survival: loading main scene", enLCBlue);
	// loading logo
	m_pLoadingSprite = (clsSprite*)m_pRenderer->GetResourceManager()->GetResource("loading.spr", enResSprite);
	clsViewport* pVP = m_pRenderer->GetViewport();
	m_pLoadingSprite->SetPosition(clsVector3((mmFloat)pVP->m_nWidth / 2.0f,
											 (mmFloat)pVP->m_nHeight / 2.0f,
											 0.0f));
	m_pLoadingSprite->SetBlendType(enBlendAlpha);
	m_pLoadingSprite->ReBuild();
	m_pRenderer->StartRender();
	m_pRenderer->Clear(mmtrue);
	m_pLoadingSprite->Render();
	m_pRenderer->StopRender();
	m_pRenderer->Present();
	
	m_pModeScene = m_pEngine->CreateScene("survival");
	if (m_pModeScene == mmnull)
	{
		g_plog->Out("Survival: out of memory while creating main scene object", enLCRed);
		return mmFail;	
	}
	if (MHMFAIL(m_pModeScene->LoadMJ("Meshes\\survival.mj")))
	{
		g_plog->Out("Survival: loading main mode scene failed", enLCRed);
		return mmFail;	
	}
	m_pGameCamera = (clsBKGameCamera*)m_pModeScene->FindChild("GameCamera");
//	clsResource* pRes = m_pRenderer->GetResourceManager()->GetResource("Test particles", enResBaseParticles);
//	mmSafeRelease(pRes);	
	//m_pModeScene->EnableBoundSphereTest(mmfalse, mmtrue);
	//m_pModeScene->EnableBoundSphereDraw(mmtrue, mmtrue);
	m_pTextStatistic = (clsText*)m_pRenderer->GetResourceManager()->GetResource("myfont.mhmFont2D",
																	  MHMRES_MISC_ID);
	m_pTextStatistic->SetAlignX(enLeft);
	m_pTextStatistic->SetAlignY(enTop);
//	m_pTextStatistic = (clsFont2D*)m_pRenderer->GetResourceManager()->GetResource("sysfont.tga",
//																	  enResFont2D);
/*	stFontRect Rect;
	Rect.m_left = 0;
	Rect.m_right = 300;
	Rect.m_bottom = 100;
	Rect.m_top  = 0;
	m_pTextStatistic->SetRect(&Rect, 0xFF0000FF);*/
	
	m_pMusic = m_pEngine->GetStream("Sounds\\inhell.xm");
	return mmOk;
}

// FirstTimeInit: initialized mode first time (once)
//==========================================================================================
mmRes clsBiokrizSurvival::FirstTimeInit()
{
	g_plog->Out("Survival: initializing mode objects", enLCBlue);
	// camera projection
	m_cCamera.UsePerspective();
	m_cCamera.Fov(mmToRadian(70.000f));//84.136f));
	clsViewport* pViewPort = m_pRenderer->GetDevice()->GetViewport();
	m_cCamera.Aspect(pViewPort->GetAspect());
	// capture mouse
	m_pMouse->Acquire();
	m_pKeyboard->Acquire();
	// set active scene
	m_pEngine->SetActiveScene(m_pModeScene);
	clsEntity* pAnimController = m_pModeScene->FindChild("AnimationController");
	if (pAnimController)
	{
		pAnimController->LoadAnimationKeys("survival.mhmAnim");
		pAnimController->PlayAnimationRangeStart("play", enAnimPingPong);
	}
	//m_pRenderer->SetAmbientLight(0xFFA0A0A0);
	//m_pMusic->Play((mmDword)this, 100);
	return mmOk;
}

// UpdateObjects: updates all mode objects
//==========================================================================================
mmRes clsBiokrizSurvival::UpdateObjects()
{
	if (m_pKeyboard->SnapshotKeyPressed(enKeyEscape))
		return mmFail;
	
	if (m_pKeyboard->SnapshotKeyPressed(enKeySpace))
	{
		if (m_bSlow)
			m_pTimer->SetDeltaFactor(0.2f);
		else
			m_pTimer->SetDeltaFactor(1.0f);
		
		m_bSlow = !m_bSlow;
	}
	
	if (m_pKeyboard->IsKeyReleased(enKeyZ))
		m_pModeScene->DumpHierarchy(" ");
	
	if (m_pKeyboard->SnapshotKeyPressed(enKey1))
		m_pMouse->Acquire();
	if (m_pKeyboard->SnapshotKeyPressed(enKey2))
		m_pMouse->Unacquire();
	
	
	m_pModeScene->AnimationRangeBuild(m_pTimer->GetDeltaMiSecs());

//	m_pModeScene->AnimationFrame(m_pTimer->GetMiSecs(),
//								 m_pTimer->GetDeltaMiSecs());
	m_pModeScene->Animate(&m_cCamera);
	
	// Move camera
	clsEntity* pEntity = m_pModeScene->FindChild("GameCamera");
	if (pEntity == mmnull)
	{
		g_plog->Out("Survival: cant find game camera entity", enLCRed);
		return mmFail;	
	}
	
	//m_pModeScene->FindChild("Ground")->m_bRenderable = mmfalse;
	
	m_cCamera.At(m_pGameCamera->m_vAt);
	m_cCamera.Eye(m_pGameCamera->m_vPosition);
	m_cCamera.Up(m_pGameCamera->m_vUp);
	m_cCamera.BuildMatrixs();
	//m_cCamera.SetViewMatrix(pEntity->m_mxFull);
	return mmOk;
}

// RenderObjects: renders all objects
//==========================================================================================
mmRes clsBiokrizSurvival::RenderObjects()
{
	m_pRenderer->SetCamera(&m_cCamera);
	m_pEngine->RenderScene(m_pModeScene);
	UpdateStatistics();
	m_pTextStatistic->Render();
	return mmOk;
}

// LeaveMode: destroys all mode objects
//==========================================================================================
mmRes clsBiokrizSurvival::LeaveMode()
{
	g_plog->Out("Survival: leave mode", enLCBlue);
	m_pEngine->DestroyScene(m_pModeScene);
	m_pModeScene = mmnull;
	return mmOk;
}

// ResizeViewport: resizes game camera
//==========================================================================================
mmRes clsBiokrizSurvival::ResizeViewport()
{
	clsViewport* pViewPort = m_pRenderer->GetDevice()->GetViewport();
	m_cCamera.Aspect(pViewPort->GetAspect());
	return mmOk;
}


// UpdateStatistic: makes stats string
//==========================================================================================
mmVoid clsBiokrizSurvival::UpdateStatistics()
{
	clsString sOut, sFloat;
	sFloat.FromFloat(m_pTimer->GetFPS());
	sOut = "FPS:" + sFloat;
	// collect emmiters info
//	sOut += "\nEmitters info";
	
/*	clsBKCreatureEmitter* pEmitter = (clsBKCreatureEmitter*)m_pModeScene->FindChild("Respawner01");
	if (pEmitter != mmnull)
	{
//		sOut += "\n01:" + clsString(pEmitter->m_nEmitCounter);
		nTotal += pEmitter->m_nEmitCounter;
	}
	pEmitter = (clsBKCreatureEmitter*)m_pModeScene->FindChild("Respawner02");
	if (pEmitter != mmnull)
	{
//		sOut += "\n02:" + clsString(pEmitter->m_nEmitCounter);
		nTotal += pEmitter->m_nEmitCounter;
	}
	pEmitter = (clsBKCreatureEmitter*)m_pModeScene->FindChild("Respawner03");
	if (pEmitter != mmnull)
	{
//		sOut += "\n03:" + clsString(pEmitter->m_nEmitCounter);
		nTotal += pEmitter->m_nEmitCounter;
	}
	pEmitter = (clsBKCreatureEmitter*)m_pModeScene->FindChild("Respawner04");
	if (pEmitter != mmnull)
	{
//		sOut += "\n04:" + clsString(pEmitter->m_nEmitCounter);
		nTotal += pEmitter->m_nEmitCounter;
	}
	pEmitter = (clsBKCreatureEmitter*)m_pModeScene->FindChild("Respawner05");
	if (pEmitter != mmnull)
	{
//		sOut += "\n05:" + clsString(pEmitter->m_nEmitCounter);
		nTotal += pEmitter->m_nEmitCounter;
	}
	pEmitter = (clsBKCreatureEmitter*)m_pModeScene->FindChild("Respawner06");
	if (pEmitter != mmnull)
	{
//		sOut += "\n06:" + clsString(pEmitter->m_nEmitCounter);
		nTotal += pEmitter->m_nEmitCounter;
	}
	pEmitter = (clsBKCreatureEmitter*)m_pModeScene->FindChild("Respawner07");
	if (pEmitter != mmnull)
	{
//		sOut += "\n07:" + clsString(pEmitter->m_nEmitCounter);
		nTotal += pEmitter->m_nEmitCounter;
	}
	pEmitter = (clsBKCreatureEmitter*)m_pModeScene->FindChild("Respawner08");
	if (pEmitter != mmnull)
	{
//		sOut += "\n08:" + clsString(pEmitter->m_nEmitCounter);
		nTotal += pEmitter->m_nEmitCounter;
	}*/
	
//	clsString sOut = "BIOKRIZ\nCC6 placeholder only\nWASD - move hero\nLB - kill mazafakaz\nWait for flying monsters for a while:)";
	mmUint nTotal = 0;
	m_pModeScene->ProcessCount(nTotal);
	sOut += "\nTOTAL:" + clsString(nTotal);
	sOut += "\n" + m_pRenderer->GetResourceManager()->m_Stats.FormatStatistic();
	m_pTextStatistic->SetText(sOut);
/*	clsString sOut, sFloat;
	sFloat.FromFloat(m_pTimer->GetFPS());
	sOut = "FPS:" + sFloat;
	HWND hwnd = m_pEngine->GetRenderer()->GetDevice()->GetWindow();
	RECT rc;
	GetClientRect(hwnd, &rc);
	HDC hdc = GetDC(hwnd); 
	FillRect(hdc, &rc, (HBRUSH)GetStockObject(WHITE_BRUSH));
	DrawText(hdc, sOut, sOut.Length(), &rc, DT_VCENTER|DT_CENTER|DT_SINGLELINE);
	ReleaseDC(hwnd, hdc);*/
	
}



