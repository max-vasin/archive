//==========================================================================================
// Mayhem engine renderer class implementation
// Coded:	MozG
// Created:	31 may 2006
// 
// * fyrex, 16 june 2006
//==========================================================================================
#include "mhmbase.h"
#include "mhmRenderer.h"

// Renderer constructor
//==========================================================================================
clsRenderer::clsRenderer(clsEngine* pengine)
{
	m_binit   = mmfalse;
	m_pdevice = mmnull;
	m_pd3d    = mmnull;
	m_pcam    = mmnull;
	m_engine  = pengine;

	m_pcam2d = new clsCamera;
	m_pcam2d->UseOrto();
	SetCam2D(1,1, 1, 0);
}

// Renderer destructor
//==========================================================================================
clsRenderer::~clsRenderer()
{
	Shut();
	delete m_pcam2d;
}

mmVoid clsRenderer::Start2D()
{
	m_pcam_tmp = m_pcam;
	SetCamera(m_pcam2d);
}
mmVoid clsRenderer::End2D()
{
	SetCamera(m_pcam_tmp);
}
mmVoid clsRenderer::SetCam2D(mmFloat p_width, mmFloat p_height, mmFloat p_near, mmFloat p_far )
{
	m_pcam2d->Width(p_width);
	m_pcam2d->Height(p_height);
	m_pcam2d->Near(p_near);
	m_pcam2d->Far(p_far);
}

// Init: initializes the renderer
//==========================================================================================
mmRes clsRenderer::Init(HWND hwnd, clsString inifile)
{
	g_plog->OutBreakLine();
	g_plog->Out("Renderer: initialization started", enLCWhite);
// Direct3D vary block

#ifdef MHM_USE_DX8
	m_pd3d = Direct3DCreate8(D3D_SDK_VERSION);
	if (m_pd3d == mmnull)
	{
		g_plog->Out("clsRenderer::Init >> Direct3DCreate8 failed", enLCRed);
		return mmFail;
	}
	m_pdevice = new clsDevice(this, m_pd3d);
	if (m_pdevice == mmnull)
		mmThrow(enExceptOutOfMemory, clsRenderer, Init, Device object alloc failed);
#else
	m_pd3d = Direct3DCreate9(D3D_SDK_VERSION);
	if (m_pd3d == mmnull)
	{
		g_plog->Out("clsRenderer::Init >> Direct3DCreate9 failed", enLCRed);
		return mmFail;
	}
	m_pdevice = new clsDevice(this, m_pd3d);
	if (m_pdevice == mmnull)
		mmThrow(enExceptOutOfMemory, clsRenderer, Init, Device object alloc failed);
#endif
	
	mmRes res = m_pdevice->Create(hwnd, inifile);
	if (MHMFAIL(res))
	{
		g_plog->Out("clsRenderer::Init >> cant create device", enLCRed);
		return res;
	}
	g_plog->Out("Renderer: device created", enLCWhite);
	m_binit = mmtrue;
	g_plog->Out("Renderer: initialization completed successfully", enLCWhite);
	g_plog->OutBreakLine();
	return mmOk;
}

// Shut: deinitializes the renderer if needed
//==========================================================================================
mmRes clsRenderer::Shut()
{
	if (!m_binit)
		return mmOk;
	g_plog->Out("Renderer: shutdown started", enLCWhite);
	mmSafeRelease(m_pd3d);
	mmSafeDelete(m_pdevice);
	m_binit = mmfalse;
	g_plog->Out("Renderer: shutdown completed", enLCWhite);
	return mmOk;
}

// SetCamera: sets current renderer camera
//==========================================================================================
mmRes clsRenderer::SetCamera(clsCamera* pcam)
{
	m_pcam = pcam;
	if ((m_pdevice != mmnull) && (m_pcam != mmnull))
	{
		m_pdevice->SetProjectionTM((clsMatrix*)m_pcam->GetProjection());
		m_pdevice->SetViewTM((clsMatrix*)m_pcam->GetView());
	}
	return mmOk;
}

// GetCamera: returns current renderer camera
//==========================================================================================
clsCamera* clsRenderer::GetCamera()
{
	return m_pcam;
}

// GetDevice: returns renderer device
//==========================================================================================
clsDevice* clsRenderer::GetDevice()
{
	return m_pdevice;
}

// GetResourceManager: returns resource manager associated with renderer device
//==========================================================================================
clsResourceManager* clsRenderer::GetResourceManager()
{
	if (m_pdevice)
		return m_pdevice->GetResourceManager();
	return mmnull;
}

// GetViewport: returns renderer viewport
//==========================================================================================
clsViewport* clsRenderer::GetViewport()
{
	return m_pdevice->GetViewport();
}

// GetEngine: returns engine object
//==========================================================================================
clsEngine* clsRenderer::GetEngine()
{
	return m_engine;
}

// StartRender: start device vertex processing
//==========================================================================================
mmRes clsRenderer::StartRender()
{
	if (!m_pdevice)
	{
		g_plog->Out("clsRenderer::StartRender >> renderer not properly initialized", enLCRed);
		return mmFail;
	}
	m_pdevice->EnableZ(mmtrue);//Pink
	m_pdevice->EnableLighting(mmtrue);
	return m_pdevice->StartRender();
}

// StopRender: stops device vertex processing
//==========================================================================================
mmRes clsRenderer::StopRender()
{
	if (!m_pdevice)
	{
		g_plog->Out("clsRenderer::StopRender >> renderer not properly initialized", enLCRed);
		return mmFail;
	}
	return m_pdevice->StopRender();
}

// Clear: clear device backbuffer surface
//==========================================================================================
mmRes clsRenderer::Clear(mmBool bTarget)
{
	if (!m_pdevice)
	{
		g_plog->Out("clsRenderer::Clear >> renderer not properly initialized", enLCRed);
		return mmFail;
	}
	return m_pdevice->Clear(bTarget);
}

// Present: presents device backbuffer surface
//==========================================================================================
mmRes clsRenderer::Present()
{
	if (!m_pdevice)
	{
		g_plog->Out("clsRenderer::Present >> renderer not properly initialized", enLCRed);
		return mmFail;
	}
	return m_pdevice->Present();
}

// Resize: resizes device
//==========================================================================================
mmRes clsRenderer::Resize()
{
	return m_pdevice->Reset();
}

// GetWidth: returns renderer device width
//==========================================================================================
mmUint clsRenderer::GetWidth()
{
	if (m_pdevice == mmnull)
		return 0;
	return m_pdevice->GetWidth();
}

// GetHeight: returns renderer device height
//==========================================================================================
mmUint clsRenderer::GetHeight()
{
	if (m_pdevice == mmnull)
		return 0;
	return m_pdevice->GetHeight();
}

// Update: update renderer depenciens
//==========================================================================================
mmRes clsRenderer::Update()
{
	m_pdevice->Update();
	return mmOk;
}

// UnProject: unprojects 2D point to 3D ray
//==========================================================================================
clsRay clsRenderer::UnProject(const clsPoint2D &pt)
{
	if (m_pcam == mmnull)
		return clsRay();
	return m_pcam->UnProject(pt, m_pdevice->GetViewport());
}

// SetAmbientLight: sets ambient light value
//==========================================================================================
mmRes clsRenderer::SetAmbientLight(mmDword value)
{
	return m_pdevice->SetAmbientLight(value);
}


//==========================================================================================
// Help graphics control
//==========================================================================================
mmVoid clsRenderer::DrawVector(const clsVector3 &Pos,
							   const clsVector3 &Dir,
							   mmDword dwColor, 
							   mmFloat fScale)
{
	m_VectorVerts[0].m_dwColor = dwColor;
	m_VectorVerts[1].m_dwColor = dwColor;
	m_VectorVerts[0].m_vPos    = Pos;
	m_VectorVerts[1].m_vPos    = Pos + (Dir * fScale);
	
	if (m_pdevice == mmnull)
		return;
	MHMDirect3DDevice* pDev = *m_pdevice;
	if (pDev == mmnull)
		return;
		
	clsMatrix mxIdentity;
	mxIdentity.Identity();
	m_pdevice->SetModelTM(&mxIdentity);
/*	pDev->SetFVF(D3DFVF_XYZ|D3DFVF_DIFFUSE);
	pDev->SetTexture(0, NULL);
	pDev->SetRenderState(D3DRS_LIGHTING, FALSE);
	pDev->DrawPrimitiveUP(D3DPT_LINELIST, 1, (mmVoid*)&m_VectorVerts, sizeof(stVectorVertex));
	pDev->SetRenderState(D3DRS_LIGHTING, TRUE);*/
}


