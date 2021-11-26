
#include "mhmbase.h"
#include "ParticlesEngine.h"

PEDECLARE_STATIC
static clsFile * DLL;
static  ParticleEngine * PE = mmnull;
static	CDevice2_PE	*Dev;
static	clsFile *CurFile;
static	clsDevice *D;
static  mmUint ParticleColor;


void PEDLL_Open(clsDevice *cDev)
{
	clsString PE_tmpFile = "mhm-pe.dll";
	D = cDev;

	Dev = mmnull;
	PE = mmnull;

	DLL = new clsFile(PE_tmpFile);
	if(DLL)
	{
		if(MHMSUCC(DLL->Open()))
		{
			if(DLL->m_zipsize)
			{
				PE_tmpFile = DLL->ReadToTemp();
				DLL->NewName(PE_tmpFile);
				DLL->m_zipsize=1; // some tricky !!!
			}
			else
			{
				DLL->Close();
			}

			PEDECLARE_OPEN(PE_tmpFile);
			if(PEDECLARE_ISOK)
			{
				PE = CreateParticleEngine();
				if(PE)
				{
					PE->setDevice(Dev=new CDevice2_PE());
				}
			}
		}
	}
	if(PE==mmnull)
	{
		g_plog->Out(">>> mhm-pe.dll not found !");
	}

}

void PEDLL_Close()
{
	PEDECLARE_CLOSE	
	if(DLL->m_zipsize)
	{
		DLL->Delete();
	}
	mmSafeDelete(Dev);
	PE = mmnull;
	mmSafeDelete(DLL);
}

CDevice2_PE::CDevice2_PE()
{
	CurFile = new clsFile("");
}

CDevice2_PE::~CDevice2_PE()
{
	mmSafeDelete(CurFile);
}

int CDevice2_PE::CreateTexture(char *n, int a, int mip)
{
	mmBool	Failed;
	clsBaseMaterial * ma;
	
	clsBaseTextureParams pp;
	pp.m_levels = 1;
//	pp.m_depth = enTexDepth32; //DEFAULT (16)
	pp.m_alpha = a?mmtrue:mmfalse;

	clsBaseTexture* ttt = (clsBaseTexture*)D->GetResourceManager()->GetResource(n, MHMRES_TEXTURE_ID, &pp, &Failed );

	if(Failed) {mmSafeRelease(ttt);return 0;}

	ma = (clsBaseMaterial *)D->GetResourceManager()->GetResource("", enResBaseMaterial);
	
	ma->SetColorLong(&ma->m_dxmtl.Ambient, 0);
	ma->SetColorLong(&ma->m_dxmtl.Diffuse, 0);
	ma->SetColorLong(&ma->m_dxmtl.Specular, 0);
	ma->SetColorLong(&ma->m_dxmtl.Emissive, 0);
	if(a ){
		ma->SetBlendType(enBlendAlpha);
	}
	ma->m_dxmtl.Power = 0;
	ma->m_bTwoSided = mmtrue;//(a==1);
	ma->SetBlendType(enBlendAdditive);

	ma->m_map_diffuse.m_texture = ttt;
	ma->m_map_diffuse.m_enabled=mmtrue;
	ma->m_map_diffuse.m_wrapping_enable = mmfalse;

	return (int)ma;
}

void CDevice2_PE::DeleteTexture(int n)
{
	clsResource * R = ((clsResource*)(n));
	mmSafeRelease(R);
}

void CDevice2_PE::RenderColorVB2(float *vb, int t, int n, D3DXMATRIX *m, int mode)
{
	D->SetModelTM((clsMatrix*)m);
	((clsBaseMaterial *)(t))->SetColorLong(&((clsBaseMaterial *)(t))->m_dxmtl.Emissive , ParticleColor);
	D->RenderVB(vb,24,n,(clsBaseMaterial *)t, (mode&65535));
	
}

void CDevice2_PE::RenderColorVB(float *vb, int t, int n, D3DXMATRIX *m)
{
	RenderColorVB2(vb,t,n,m,0);
}


void CDevice2_PE::SetPhase(int i)
{

}

int CDevice2_PE::getPhase()
{
	return 2;
}

void * CDevice2_PE::LoadFileToMemory(char *fn)
{
	void *fd = mmnull;

	CurFile->NewName(fn);
	if(MHMSUCC(	CurFile->Open()))
	{
		if(MHMSUCC(CurFile->ReadToMemory(&fd)))
		{
			return fd;
		}
	}
	return 0;
}

void CDevice2_PE::FreeMemoryOfFile(void *fd)
{
	CurFile->FreeMemory(fd);
}

unsigned int CDevice2_PE::GetFileSize()
{
	return CurFile->GetSize();
}

int CDevice2_PE::FileExist(char *fn)
{
	CurFile->NewName(fn);
	return CurFile->IsExist();
}

//  constructor
//==========================================================================================
clsBaseParticles::clsBaseParticles(clsResourceManager* presman, clsString name, enResType type):
			  clsResource(presman, name, type)
{
	if(PE)
	{
		SetBlendType(enBlendAdditive);
	}
	m_Handle		= mmnull;
	m_fEmitterScale = 1.0f;
	m_resize = mmtrue;
}

// destructor
//==========================================================================================
clsBaseParticles::~clsBaseParticles()
{
	if(PE)
	{
		PE->DeleteEmitter(m_Handle);
	}
}

//
// Create: creates new 
//==========================================================================================
mmRes clsBaseParticles::Create(clsResourceParams* params)
{
	if(PE)
	{
		m_Handle = PE->CreateEmitter(m_fullname);
		m_name += "@" + clsString((mmDword)(m_Handle));
//		m_name = (mmString)mmnull;
	}
	
	return mmOk;
}

// Cloning:
//==========================================================================================
clsResource* clsBaseParticles::OnAddReference()
{
	if (m_refs >= 1)
	{
		m_refs--;
		return m_presman->GetResource(m_name.CutBy('@'), enResBaseParticles);
	}
	return this;
}

void clsBaseParticles::SetColor(mmDword c)
{
	PE->ColorEmitter(m_Handle, c);
}

mmRes clsBaseParticles::Render()
{

	if((PE) && (m_fEmitterScale != 0.0f))
	{
		clsMatrix vm = *m_presman->GetDevice()->GetViewTM();
		vm.Inverse();
		PE->ViewEmitter(m_Handle, (D3DXMATRIX *)&vm);

		//vm.RotationX(MHM_PI);
		vm = *m_presman->GetDevice()->GetModelTM() ;
		//vm.Identity();
		PE->MatrixEmitter(m_Handle, (D3DXMATRIX *)&vm);
		PE->AnimateEmitter(m_Handle, m_presman->GetDevice()->GetRenderer()->GetEngine()->GetTimer()->GetDeltaSecs());
		if(m_resize)
		{
			PE->ScaleEmitter(m_Handle, m_fEmitterScale);
			m_resize = mmfalse;
		}
		PE->RenderEmitter(m_Handle);
	}

	return mmOk;
}

// SetEmitterScale: sets particles emitter scale
//==========================================================================================
mmVoid clsBaseParticles::SetEmitterScale(mmFloat fScale)
{
	m_resize = mmtrue;
	m_fEmitterScale = fScale;
}

// ResetEmitter: reset particles generation
//==========================================================================================
mmVoid clsBaseParticles::ResetEmitter()
{
	if(PE)
	{
		PE->ResetEmitter(m_Handle);
	}
}

//==========================================================================================
mmVoid clsBaseParticles::ClearEmitter()
{
	if(PE)
	{
		PE->ClearEmitter(m_Handle);
	}
}

// StopEmitter: stop particles generation
//==========================================================================================
mmVoid clsBaseParticles::StopEmitter()
{
	if(PE)
	{
		PE->StopEmitter(m_Handle);
	}
}

// CountEmitter: count particles for emitter
//==========================================================================================
mmUint clsBaseParticles::CountEmitter()
{
	if(PE)
	{
		return PE->CountEmitter(m_Handle);
	}
	else
	{
		return 0;
	}
}


// CountEmitter: count particles for emitter
//==========================================================================================
mmBool clsBaseParticles::LoopEmitter()
{
	if(PE)
	{
		return ((PE->LoopCountEmitter(m_Handle)==0)?mmtrue:mmfalse);
	}
	else
	{
		return mmfalse;
	}
}

