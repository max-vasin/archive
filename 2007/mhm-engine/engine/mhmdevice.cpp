//==========================================================================================
// Mayhem engine device class implementation
// Coded:	MozG
// Created:	9 may 2006
//==========================================================================================
#include "mhmbase.h"

//=================================================================
// Video mode list class implementation
//=================================================================
clsVideoModeList::clsVideoModeList()	{}
clsVideoModeList::~clsVideoModeList()	{}

// Sort: sort modes by dimensions
//=================================================================
mmVoid clsVideoModeList::Sort()
{
	if (m_head == mmnull)
		return;
	clsListNode *pnodeA, *pnodeB;
	pnodeA = m_head;
	do
	{
		pnodeB = pnodeA->m_next;
		while(pnodeB)
		{
			// compare
			if ((pnodeB->m_data->m_width * pnodeB->m_data->m_height) <
				(pnodeA->m_data->m_width * pnodeA->m_data->m_height))
			{
				clsVideoMode* pmode = pnodeA->m_data;
				pnodeA->m_data = pnodeB->m_data;
				pnodeB->m_data = pmode;
			}
			pnodeB = pnodeB->m_next;
		}
		pnodeA = pnodeA->m_next;
	}while(pnodeA);
}

//=================================================================
// Device class implementation
//=================================================================

// Device constructor
//==========================================================================================
clsDevice::clsDevice(clsRenderer* pren, MHMDirect3D* pd3d)
{
	m_pd3d     = pd3d;
	m_renderer = pren;
	m_pdev     = mmnull;
	m_presman  = mmnull;
	m_ininame  = mmnull;
	m_pconfig  = mmnull;
	m_proj_tm.Identity();
	m_view_tm.Identity();
	m_wrld_tm.Identity();
	m_nMaxSamples = 0;
}

// Device destructor
//==========================================================================================
clsDevice::~clsDevice()
{
	Shutdown();
}

// Create: creates new device
//==========================================================================================
mmRes clsDevice::Create(HWND hwnd, const mmString inifile)
{
	if (hwnd == mmnull)
	{
		g_plog->Out("clsDevice::Create >> invalid window handle passed", enLCRed);
		return mmFail;
	}
	Shutdown();
	CheckCaps();
	m_ininame = inifile;
	m_hwnd    = hwnd;
	mmRes res = LoadConfig();
	if (res != mmOk)
	{
		g_plog->Out("clsDevice::Create >> cant load device configuration", enLCRed);
		return res;
	}
	
	res = MakeModesLists();	
	if (MHMFAIL(res))
	{
		g_plog->Out("clsDevice::Create >> cant make modes lists", enLCRed);
		return mmFail;
	}
	res = FindMaxSamples();
	if (MHMFAIL(res))
	{
		g_plog->Out("clsDevice::Create >> cant find max sample value", enLCRed);
		return mmFail;
	}
		
	// prepare window size
	RECT Rect;
	Rect.left    = 10;
	Rect.top     = 10;
	Rect.right   = Rect.left + m_pconfig->m_width;
	Rect.bottom  = Rect.top + m_pconfig->m_height;
	LONG dwStyle = GetWindowLong(m_hwnd, GWL_STYLE);
	AdjustWindowRect(&Rect, dwStyle, FALSE);
	// recalc window rect to place it in the topleft corner
	Rect.right  = Rect.right - Rect.left;
	Rect.bottom = Rect.bottom - Rect.top;
	Rect.left	= 0;
	Rect.top	= 0;
	
	// check multisample
	if (m_pconfig->m_nSamples > m_nMaxSamples)
		m_pconfig->m_nSamples = m_nMaxSamples;
	
//	D3DPRESENT_PARAMETERS	pp;// Pink
	mmZeroVar(pp);
	pp.AutoDepthStencilFormat     = m_pconfig->m_dsfmt;
	pp.BackBufferCount            = 0;

#ifdef MHM_USE_DX8
	D3DDISPLAYMODE	dmode;
	m_pd3d->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &dmode);
	pp.BackBufferFormat           = (m_pconfig->m_windowed) ? dmode.Format : m_pconfig->m_bbfmt;
#else
	pp.BackBufferFormat           = m_pconfig->m_bbfmt;
#endif

	if (m_pconfig->m_windowed)
	{
		
		// pink
	/*	
		tagRECT rect_d;
		HWND desk;
		desk = GetDesktopWindow();
		GetWindowRect(desk, &rect_d);
		mmInt x,y;
		x = (rect_d.right - m_width) / 2;
		y = (rect_d.bottom - m_height) / 2;


		SetWindowPos(m_hwnd, HWND_DESKTOP, Rect.left+x, Rect.top+y, Rect.right+x, Rect.bottom+y, 0);
		GetClientRect(m_hwnd, &Rect);
		pp.BackBufferWidth  = Rect.right - Rect.left;
		pp.BackBufferHeight = Rect.bottom - Rect.top;
	*/	
		
	}
	else
	{
		pp.BackBufferWidth            =  m_pconfig->m_width;
		pp.BackBufferHeight		      =  m_pconfig->m_height;
	}
	pp.EnableAutoDepthStencil     = mmtrue;
	pp.hDeviceWindow			  = m_hwnd;
	pp.Flags				      = 0;
	pp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	pp.MultiSampleType			  = (m_pconfig->m_nSamples > 0) ? D3DMULTISAMPLE_TYPE(m_pconfig->m_nSamples + 1) : D3DMULTISAMPLE_NONE;
#ifdef MHM_USE_DX8
	pp.FullScreen_PresentationInterval  = (m_pconfig->m_windowed) ? D3DPRESENT_INTERVAL_DEFAULT : m_pconfig->m_interval;
#else
	pp.PresentationInterval		  = m_pconfig->m_interval;
	pp.MultiSampleQuality		  = 0;
#endif // MHM_USE_DX8
	pp.SwapEffect				  = D3DSWAPEFFECT_DISCARD;
	pp.Windowed					  = m_pconfig->m_windowed;
	
	
	mmDword dwCreFlags = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
	if (m_pconfig->m_bHardVertProcessing)
		dwCreFlags = D3DCREATE_HARDWARE_VERTEXPROCESSING;
	if (m_pconfig->m_bPureDevice)
		dwCreFlags |= D3DCREATE_PUREDEVICE;


	// pink
//	HRESULT hres = m_pd3d->CreateDevice(D3DADAPTER_DEFAULT,D3DDEVTYPE_HAL,m_hwnd,dwCreFlags,&pp,&m_pdev);

	m_FormatS.Size(0);


	m_FormatS.Add(strFormatS(D3DFMT_R5G6B5,D3DFMT_D24S8));
	m_FormatS.Add(strFormatS(D3DFMT_R5G6B5,D3DFMT_D24X8));
	m_FormatS.Add(strFormatS(D3DFMT_R5G6B5,D3DFMT_D24X4S4));

	m_FormatS.Add(strFormatS(D3DFMT_X1R5G5B5,D3DFMT_D24S8));
	m_FormatS.Add(strFormatS(D3DFMT_X1R5G5B5,D3DFMT_D24X8));
	m_FormatS.Add(strFormatS(D3DFMT_X1R5G5B5,D3DFMT_D24X4S4));

	m_FormatS.Add(strFormatS(D3DFMT_A1R5G5B5,D3DFMT_D24S8));
	m_FormatS.Add(strFormatS(D3DFMT_A1R5G5B5,D3DFMT_D24X8));
	m_FormatS.Add(strFormatS(D3DFMT_A1R5G5B5,D3DFMT_D24X4S4));

	m_FormatS.Add(strFormatS(D3DFMT_X8R8G8B8,D3DFMT_D24S8));
	m_FormatS.Add(strFormatS(D3DFMT_X8R8G8B8,D3DFMT_D24X8));
	m_FormatS.Add(strFormatS(D3DFMT_X8R8G8B8,D3DFMT_D24X4S4));

	m_FormatS.Add(strFormatS(D3DFMT_A8R8G8B8,D3DFMT_D24S8));
	m_FormatS.Add(strFormatS(D3DFMT_A8R8G8B8,D3DFMT_D24X8));
	m_FormatS.Add(strFormatS(D3DFMT_A8R8G8B8,D3DFMT_D24X4S4));

	m_FormatS.Add(strFormatS(D3DFMT_R8G8B8,D3DFMT_D24S8));
	m_FormatS.Add(strFormatS(D3DFMT_R8G8B8,D3DFMT_D24X8));
	m_FormatS.Add(strFormatS(D3DFMT_R8G8B8,D3DFMT_D24X4S4));

	m_FormatS.Add(strFormatS(D3DFMT_R5G6B5,D3DFMT_D16));
	m_FormatS.Add(strFormatS(D3DFMT_X1R5G5B5,D3DFMT_D16));
	m_FormatS.Add(strFormatS(D3DFMT_A1R5G5B5,D3DFMT_D16));
	m_FormatS.Add(strFormatS(D3DFMT_X8R8G8B8,D3DFMT_D16));
	m_FormatS.Add(strFormatS(D3DFMT_A8R8G8B8,D3DFMT_D16));
	m_FormatS.Add(strFormatS(D3DFMT_R8G8B8,D3DFMT_D16));

	HRESULT hres = m_pd3d->CreateDevice(D3DADAPTER_DEFAULT,D3DDEVTYPE_HAL,m_hwnd,dwCreFlags,&pp,&m_pdev);
	if (FAILED(hres))
	{
		for(mmUint z = 0; z < m_FormatS.Size(); z++)
		{
			pp.BackBufferFormat = m_FormatS[z].m_BackBufferFormat;
			pp.AutoDepthStencilFormat = m_FormatS[z].m_AutoDepthStencilFormat;
			hres = m_pd3d->CreateDevice(D3DADAPTER_DEFAULT,D3DDEVTYPE_HAL,m_hwnd,dwCreFlags,&pp,&m_pdev);
			if (!(FAILED(hres)))
				break;
		}
	}


	if (FAILED(hres))
	{
		g_plog->Out("clsDevice::Create >> failed to create Direct3D device with given PP", enLCRed);
		return MHMDEVICE_DX_ERR;
	}

	m_width  = pp.BackBufferWidth;
	m_height = pp.BackBufferHeight;
	
	m_vport.m_nX = 0; 
	m_vport.m_nY = 0; 
	m_vport.m_nWidth  = pp.BackBufferWidth;
	m_vport.m_nHeight = pp.BackBufferHeight;
	
	g_plog->Out("Device: Direct3D device created successfully");	
	m_presman = new clsResourceManager(this);
	if (m_presman == mmnull)
		mmThrow(enExceptOutOfMemory, clsDevice, Create, Resource manager object alloc failed);
	if (MHMFAIL(m_presman->Init(inifile)))
	{
		g_plog->Out("Device: cant initialize resource manager", enLCRed);
		return mmFail;
	}
	if (MHMFAIL(SetBaseStates()))
	{
		g_plog->Out("Device: cant set base device states");
		return mmFail;
	}
	g_plog->Out("Device: resource manager created successfully");
	return mmOk;
}

// Shutdown: releases device resources
//==========================================================================================
mmRes clsDevice::Shutdown()
{
	mmSafeDelete(m_pconfig);
	mmSafeDelete(m_presman);
	mmSafeRelease(m_pdev);
	return mmOk;
}

// Reset: resets device
//==========================================================================================
mmRes clsDevice::Reset(mmInt pWidth, mmInt pHeight)
{
	if (m_pdev == mmnull)
	{
		g_plog->Out("clsDevice::Reset >> no device", enLCRed);
		return mmFail;
	}

	if(pWidth >= 0) 
		m_pconfig->m_width = pWidth;
	if(pHeight >= 0)
		m_pconfig->m_height = pHeight;

	pp.BackBufferWidth            = m_pconfig->m_width;
	pp.BackBufferHeight		      = m_pconfig->m_height;
	pp.Windowed					  = m_pconfig->m_windowed;
	m_vport.m_nX = 0; 
	m_vport.m_nY = 0; 
	m_vport.m_nWidth  = pp.BackBufferWidth;
	m_vport.m_nHeight = pp.BackBufferHeight;
	
	/*
	if (FAILED(m_pdev->Reset(&pp)))
	{
		g_plog->Out("clsDevice::Reset >> device method call failed", enLCRed);
		return mmFail;
	}
	*/


	HRESULT hres = m_pdev->Reset(&pp);
	if (FAILED(hres))
	{
		for(mmUint z = 0; z < m_FormatS.Size(); z++)
		{
			pp.BackBufferFormat = m_FormatS[z].m_BackBufferFormat;
			pp.AutoDepthStencilFormat = m_FormatS[z].m_AutoDepthStencilFormat;
			hres = m_pdev->Reset(&pp);
			if (!(FAILED(hres)))
				break;
		}
	}
	if (FAILED(hres))
	{
		g_plog->Out("clsDevice::Reset >> device method call failed", enLCRed);
		return mmFail;
	}



	m_width  = pp.BackBufferWidth;
	m_height = pp.BackBufferHeight;

	if (m_pconfig->m_windowed)
	{
		tagRECT rect_d;
		HWND desk;
		desk = GetDesktopWindow();
		GetWindowRect(desk, &rect_d);
		mmInt x,y;
		x = (rect_d.right - m_width) / 2;
		y = (rect_d.bottom - m_height) / 2;
		
		SetWindowPos(m_hwnd, HWND_NOTOPMOST, x, y, m_width, m_height+10, 0);
		//SetWindowPos(m_hwnd, HWND_DESKTOP, 0, 0, m_width, m_height, 0);

		ShowWindow(m_hwnd, SW_SHOW);
		UpdateWindow(m_hwnd);
	}




/*	if (m_pdev == mmnull)
	{
		g_plog->Out("clsDevice::Reset >> no device", enLCRed);
		return mmFail;
	}
	//return mmOk;
	D3DPRESENT_PARAMETERS	pp;
	mmZeroVar(pp);
		
	pp.AutoDepthStencilFormat     = m_pconfig->m_dsfmt;
	pp.BackBufferCount            = 0;
	pp.BackBufferFormat           = m_pconfig->m_bbfmt;
	if (m_pconfig->m_windowed)
	{
		RECT rc;
		GetClientRect(m_hwnd, &rc);
		pp.BackBufferWidth  = rc.right - rc.left;
		pp.BackBufferHeight = rc.bottom - rc.top;
	}
	else
	{
		pp.BackBufferWidth            = m_pconfig->m_width;
		pp.BackBufferHeight		      = m_pconfig->m_height;
	}
	pp.EnableAutoDepthStencil     = mmtrue;
	pp.hDeviceWindow			  = m_hwnd;
	pp.Flags				      = 0;
	pp.FullScreen_RefreshRateInHz = 0;
	pp.MultiSampleType			  = (m_pconfig->m_nSamples > 0) ? D3DMULTISAMPLE_TYPE(m_pconfig->m_nSamples + 1) : D3DMULTISAMPLE_NONE;
	pp.SwapEffect				  = D3DSWAPEFFECT_DISCARD;
	pp.Windowed					  = m_pconfig->m_windowed;
#ifdef MHM_USE_DX8
	pp.FullScreen_PresentationInterval  = m_pconfig->m_interval;
#else
	pp.PresentationInterval		  = m_pconfig->m_interval;
	pp.MultiSampleQuality		  = 0;
#endif // MHM_USE_DX8

	m_vport.m_nX = 0; 
	m_vport.m_nY = 0; 
	m_vport.m_nWidth  = pp.BackBufferWidth;
	m_vport.m_nHeight = pp.BackBufferHeight;
	
	if (FAILED(m_pdev->Reset(&pp)))
	{
		g_plog->Out("clsDevice::Reset >> device method call failed", enLCRed);
		return mmFail;
	}
	m_width  = pp.BackBufferWidth;
	m_height = pp.BackBufferHeight;
	if (FAILED(SetBaseStates()))
	{
		g_plog->Out("clsDevice::Reset >> cant set base states", enLCRed);
		return mmFail;
	}
	
/*	D3DVIEWPORT9 vp;
	vp.X      = m_vport.m_nX;
	vp.Y      = m_vport.m_nY;
	vp.Width  = m_vport.m_nWidth;
	vp.Height = m_vport.m_nHeight;
	vp.MinZ   = 0.0f;
	vp.MaxZ   = 1.0f;
	m_pdev->SetViewport(&vp);*/
	g_plog->Out("Device reset");
	return mmOk;
}

// Clear: clears device back buffer
//==========================================================================================
mmRes clsDevice::Clear(mmBool bTarget)
{
	m_pdev->Clear(0, NULL, bTarget?D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER:D3DCLEAR_ZBUFFER, 0x00000000, 1.0f, 0);
	return mmOk;
}

// StartRender: start vertex processing
//==========================================================================================
mmRes clsDevice::StartRender()
{
	if (m_pdev == mmnull)
	{
		g_plog->Out("clsDevice::StartRender >> no device", enLCRed);
		return mmFail;
	}
	if (FAILED(m_pdev->BeginScene()))
	{
		g_plog->Out("clsDevice::StartRender >> device method call failed", enLCRed);
		return mmFail;
	}
	return mmOk;
}

// StopRender: stops vertex processing
//==========================================================================================
mmRes clsDevice::StopRender()
{
	if (m_pdev == mmnull)
	{
		g_plog->Out("clsDevice::StopRender >> no device", enLCRed);
		return mmFail;
	}
	if (FAILED(m_pdev->EndScene()))
	{
		g_plog->Out("clsDevice::StopRender >> device method call failed", enLCRed);
		return mmFail;
	}
	return mmOk;
}

// Present: presents back buffer
//==========================================================================================
mmRes clsDevice::Present(HWND hwnd)
{
	m_pdev->Present(NULL, NULL, hwnd, NULL);
	return mmOk;
}

// SetProjectionTM: sets current projection trasformation
//==========================================================================================
mmRes clsDevice::SetProjectionTM(clsMatrix *mx)
{
	if (m_pdev == mmnull)
		return MHMDEVICE_NOT_INIT;
	if (FAILED(m_pdev->SetTransform(D3DTS_PROJECTION, (D3DMATRIX*)mx)))
		return MHMDEVICE_DX_ERR;
	if (mx != mmnull)
		m_proj_tm = *mx;
	else
		m_proj_tm.Identity();
	return mmOk;
}


// SetViewTM: sets current view transformation
//==========================================================================================
mmRes clsDevice::SetViewTM(clsMatrix *mx)
{
	if (m_pdev == mmnull)
		return MHMDEVICE_NOT_INIT;
	if (FAILED(m_pdev->SetTransform(D3DTS_VIEW, (D3DMATRIX*)mx)))
		return MHMDEVICE_DX_ERR;
	if (mx != mmnull)
		m_view_tm = *mx;
	else
		m_view_tm.Identity();
	return mmOk;
}

// EnableLight: enables light
//==========================================================================================
mmRes clsDevice::EnableLight(MHMLight* plight, mmBool enable, mmInt Number)
{
	if(enable) m_pdev->SetLight(Number, plight);
	m_pdev->LightEnable(Number, enable);
	return mmOk;	
}

// EnableLight: enables light
//==========================================================================================
mmRes clsDevice::SetFog(mmBool enable, mmDword Color, mmFloat Start, mmFloat End)
{
	if(FAILED(m_pdev->SetRenderState(
                    D3DRS_FOGENABLE,
					enable?TRUE:FALSE)))
					return mmFail;
	if(enable)
	{
		m_pdev->SetRenderState(
                    D3DRS_FOGCOLOR,
                    Color); // Highest 8 bits are not used.
        m_pdev->SetRenderState(D3DRS_FOGVERTEXMODE, D3DFOG_LINEAR);
        m_pdev->SetRenderState(D3DRS_FOGSTART, *(DWORD *)(&Start));
        m_pdev->SetRenderState(D3DRS_FOGEND,   *(DWORD *)(&End));
	}
	return mmOk;	
}

// SetModelTM: sets current model transformation
//==========================================================================================
mmRes clsDevice::SetModelTM(clsMatrix *mx)
{
	if (m_pdev == mmnull)
		return MHMDEVICE_NOT_INIT;
	if (mx == mmnull)
	{
		m_wrld_tm.Identity();
		if (FAILED(m_pdev->SetTransform(D3DTS_WORLD, (D3DMATRIX*)&m_wrld_tm)))
			return MHMDEVICE_DX_ERR;
		
		return mmOk;
	}
	
	if (FAILED(m_pdev->SetTransform(D3DTS_WORLD, (D3DMATRIX*)mx)))
		return MHMDEVICE_DX_ERR;
	m_wrld_tm = *mx;
	return mmOk;
}
// SetModelTM: sets current model transformation
//==========================================================================================
mmRes clsDevice::RenderVB(mmFloat * VB, mmUint stride, mmUint count, clsResource *material, mmInt blend_type)
{
/*	static clsVertexBuffer * vb = mmnull;
	if (vb == mmnull)
	{
		vb = GetRenderer()->GetResourceManager()->CreateVertexBuffer();
		vb->Create(10000, D3DFVF_XYZ|D3DFVF_DIFFUSE|D3DFVF_TEX1, D3DPOOL_DEFAULT);
	}*/
	

	if (m_pdev == mmnull)
		return MHMDEVICE_NOT_INIT;

	clsBaseMaterial * M = (clsBaseMaterial *)material;
	EnableLighting(mmfalse);

	M->Activate(mmnull);

	if(blend_type)
	{
		m_pdev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		m_pdev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	}
	else
	{
	//	m_pdev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE );
		m_pdev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		m_pdev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	}

	if(stride==24)
	{
		SetFVF((D3DFVF_XYZ|D3DFVF_DIFFUSE|D3DFVF_TEX1));
	}
	else if(stride==32)
	{
		SetFVF((D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_TEX1));
	}
	m_pdev->DrawPrimitiveUP(D3DPT_TRIANGLELIST, count, VB, stride);
/*	
	mmVoid* pMem;
	vb->Lock(&pMem);
	memcpy(pMem, VB, (count+1) * stride);
	vb->Unlock();
	vb->SetStream(0);
	m_pdev->DrawPrimitive(D3DPT_TRIANGLELIST, 0, count);
*/

	M->Deactivate(mmnull);

	EnableLighting(mmtrue);
	return mmOk;
}

// GetProjectionTM: returns current projection trasformation
//==========================================================================================
mmVoid clsDevice::DepthWrite(mmBool b)
{
	m_pdev->SetRenderState(D3DRS_ZWRITEENABLE, b?TRUE:FALSE);
}

clsMatrix* clsDevice::GetProjectionTM()
{
	return &m_proj_tm;
}

// GetViewTM: returns current view trasformation
//==========================================================================================
clsMatrix* clsDevice::GetViewTM()
{
	return &m_view_tm;
}

// GetProjectionTM: returns current projection trasformation
//==========================================================================================
clsMatrix* clsDevice::GetModelTM()
{
	return &m_wrld_tm;
}

// GetWidth: returns device width in pixels
//==========================================================================================
mmUint clsDevice::GetWidth()
{
	return m_width;
}

// GetHeight: returns device height in pixels
//==========================================================================================
mmUint clsDevice::GetHeight()
{
	return m_height;
}

// GetViewport: returns device viewport
//==========================================================================================
clsViewport* clsDevice::GetViewport()
{
	return &m_vport;
}

// GetWindow: returns device window
//==========================================================================================
HWND clsDevice::GetWindow()
{
	return m_hwnd;
}


// GetResourceManager: returns resource manager associated with device
//==========================================================================================
clsResourceManager* clsDevice::GetResourceManager()
{
	return m_presman;
}

// GetRenderer: returns renderer owner
//==========================================================================================
clsRenderer* clsDevice::GetRenderer()
{
	return m_renderer;
}

// Update: updates renderer depencies
//==========================================================================================
mmRes clsDevice::Update()
{
	m_presman->Update();
	return mmOk;
}

// SetAmbientLight: sets device ambient light value
//==========================================================================================
mmRes clsDevice::SetAmbientLight(mmDword value)
{
	if (FAILED(m_pdev->SetRenderState(D3DRS_AMBIENT, value)))
		return mmFail;
	return mmOk;
}


// LoadConfig: loads configuration file
//==========================================================================================
mmRes clsDevice::LoadConfig()
{
	mmRes res;
	mmSafeDelete(m_pconfig);
	m_pconfig = new clsDeviceConfig(m_ininame);
	clsFile file(m_ininame);
	if (!file.IsExist())
	{
		g_plog->Out("clsDevice::Create >> device ini file not exist or could not be opened now", enLCYellow);
		g_plog->Out("clsDevice::Create >> trying to create new default config file", enLCYellow);
		res = m_pconfig->Save();	
		if (res != mmOk)
		{
			g_plog->Out("clsDevice::Create >> failed to create new ini file", enLCRed);			
			return MHMDEVICE_NO_INI;
		}
		return mmOk;
	}
	res = m_pconfig->Load();
	if (res != mmOk)
	{
		g_plog->Out("clsDevice::Create >> cant load configuration, trying resave defaults", enLCRed);
		res = m_pconfig->Save();
		if (res != mmOk)
		{
			g_plog->Out("clsDevice::Create >> default configuration writing failed", enLCRed);			
			return MHMDEVICE_NO_INI;
		}
	}
	return mmOk;
}

// Type cast operator
//==========================================================================================
clsDevice::operator MHMDirect3DDevice*()
{
	return m_pdev;
}

// MakeModesLists: makes device modes lists
//==========================================================================================
mmRes clsDevice::MakeModesLists()
{
	D3DFORMAT Format32, Format16, Format32Alpha, Format16Alpha;
	mmUint	  Number32, Number16, Number32Alpha, Number16Alpha;
	GetNumModes(D3DADAPTER_DEFAULT, enBPP32, mmfalse, Format32, Number32);
	if (Number32 == 0)
		g_plog->Out("No device 32 bit display modes support");
	else
		g_plog->Out("Device supports [" + clsString(Number32) + "] 32 bit modes");
	
	GetNumModes(D3DADAPTER_DEFAULT, enBPP32, mmtrue, Format32Alpha, Number32Alpha);
	if (Number32Alpha == 0)
		g_plog->Out("No device 32 bit alpha display modes support");
	else
		g_plog->Out("Device supports [" + clsString(Number32) + "] 32 bit alpha modes");
	
	GetNumModes(D3DADAPTER_DEFAULT, enBPP16, mmfalse, Format16, Number16);
	if (Number16 == 0)
		g_plog->Out("No device 16 bit display modes support");
	else
		g_plog->Out("Device supports [" + clsString(Number32) + "] 16 bit modes");
	
	GetNumModes(D3DADAPTER_DEFAULT, enBPP16, mmtrue, Format16Alpha, Number16Alpha);
	if (Number16Alpha == 0)
		g_plog->Out("No device 16 bit alpha display modes support");
	else
		g_plog->Out("Device supports [" + clsString(Number32) + "] 16 bit alpha modes");
	
	// 32 bit no alpha mode list
	if (Number32 > 0)
		if (MHMFAIL(BuildModeList(D3DADAPTER_DEFAULT, 
								  Number32,
								  Format32,
								  &m_modes32)))
		{
			g_plog->Out("clsDevice::MakeModesLists >> cant build 32 bit mode list", enLCRed);
			return mmFail;
		}
	
	if (Number32Alpha > 0)
		if (MHMFAIL(BuildModeList(D3DADAPTER_DEFAULT, 
								  Number32Alpha,
								  Format32Alpha,
								  &m_modes32alpha)))
		{
			g_plog->Out("clsDevice::MakeModesLists >> cant build 32 bit alpha mode list", enLCRed);
			return mmFail;
		}

	if (Number16 > 0)
		if (MHMFAIL(BuildModeList(D3DADAPTER_DEFAULT,
								  Number16,
								  Format16,
								  &m_modes16)))
		{
			g_plog->Out("clsDevice::MakeModesLists >> cant build 16 bit mode list", enLCRed);
			return mmFail;
		}

	if (Number16Alpha > 0)
		if (MHMFAIL(BuildModeList(D3DADAPTER_DEFAULT,
								  Number16Alpha,
								  Format16Alpha,
								  &m_modes16alpha)))
		{
			g_plog->Out("clsDevice::MakeModesLists >> cant build 16 bit mode list", enLCRed);
			return mmFail;
		}
	m_modes16.Sort();
	m_modes32.Sort();
	m_modes16alpha.Sort();
	m_modes32alpha.Sort();
#ifdef _DEBUG
	DumpVideoModes();
#endif // _DEBUG
	return mmOk;
}

//GetMaxSamples: returns max multisample type samples value
//==========================================================================================
mmUint clsDevice::GetMaxSamples()
{
	return m_nMaxSamples;
}

// FindMaxSamples: find max supported samples on this adapter
//==========================================================================================
mmRes clsDevice::FindMaxSamples()
{
#ifdef MHM_USE_DX8
	g_plog->Out("FindMaxSamples not supported on dx 8", enLCRed);
#else
	for (mmUint i = 2; i < 17; i++)
		if (FAILED(m_pd3d->CheckDeviceMultiSampleType(D3DADAPTER_DEFAULT,
													  D3DDEVTYPE_HAL,
													  m_pconfig->m_bbfmt,
													  m_pconfig->m_windowed,
													  D3DMULTISAMPLE_TYPE(i),
													  NULL)))
			break;
	
	m_nMaxSamples = i - 2;
	g_plog->Out("clsDevice: max samples [" + clsString(m_nMaxSamples) + "]", enLCWhite);
#endif // MHM_USE_DX8
	return mmOk;											  
}

// GetNumModes: returns number of device display modes
//==========================================================================================
mmRes clsDevice::GetNumModes(mmUint adapter, enBitsPerPixel bpp, mmBool alpha,
							 D3DFORMAT &fmt, mmUint &nummodes)
{
#ifdef MHM_USE_DX8
	nummodes = m_pd3d->GetAdapterModeCount(adapter);
#else
	switch(bpp)
	{
	case enBPP16:
		if (alpha)
			fmt = D3DFMT_A1R5G5B5;
		else
			fmt = D3DFMT_R5G6B5;
		break;
	case enBPP32:
		if (alpha)
			fmt	= D3DFMT_A8R8G8B8;
		else
			fmt	= D3DFMT_X8R8G8B8;
		break;
	};
	nummodes = m_pd3d->GetAdapterModeCount(adapter, fmt);
#endif // MHM_USE_DX8
	return mmOk;
}

// BuildModeList: builds needed modes list
//==========================================================================================
mmRes clsDevice::BuildModeList(mmUint adapter, mmUint num, D3DFORMAT fmt,
							   clsVideoModeList *plist)
{
	D3DDISPLAYMODE mode;
	for (mmUint i = 0; i < num; i++)
	{
#ifdef MHM_USE_DX8
		if (FAILED(m_pd3d->EnumAdapterModes(adapter, i, &mode)))
		{
			g_plog->Out("clsDevice::BuildModeList >> enumerating modes failed", enLCRed);
			return mmFail;
		}
#else
		if (FAILED(m_pd3d->EnumAdapterModes(adapter, fmt, i, &mode)))
		{
			g_plog->Out("clsDevice::BuildModeList >> enumerating modes failed", enLCRed);
			return mmFail;
		}
#endif // MHM_USE_DX8
		mmBool founded = mmfalse;
		for (mmUint z = 0; z < plist->Count(); z++)
			if (((*plist)[z]->m_height == mode.Height) &&
				((*plist)[z]->m_width  == mode.Width))
			{
				founded = mmtrue;
				break;
			}
		if (founded)
			continue;
		plist->Add(new clsVideoMode(mode.Width, mode.Height, fmt));
	}
	return mmOk;
}

// SetBaseStates: sets device base states
//==========================================================================================
mmRes clsDevice::SetBaseStates()
{
	m_bZ = mmtrue;

	// fill mode state control
	m_bWireFrameEnabled = mmfalse;
	if (FAILED(m_pdev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID)))
		return mmFail;
	
	// culling state control
	m_bCulling = mmtrue;
	if (FAILED(m_pdev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW)))
		return mmFail;
	
	// specular state control
	m_bSpecular = mmfalse;
	if (FAILED(m_pdev->SetRenderState(D3DRS_SPECULARENABLE, FALSE)))
		return mmFail;
	if (FAILED(m_pdev->SetRenderState(D3DRS_SPECULARMATERIALSOURCE, D3DMCS_MATERIAL)))
		return mmFail;
	if (FAILED(m_pdev->SetRenderState(D3DRS_DIFFUSEMATERIALSOURCE, D3DMCS_MATERIAL)))
		return mmFail;
			
	m_bLighting = mmtrue;
	if (FAILED(m_pdev->SetRenderState(D3DRS_LIGHTING, TRUE)))
		return mmFail;
	
	
	m_pdev->SetRenderState(D3DRS_DITHERENABLE, FALSE);
	
	m_pdev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	//m_pdev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pdev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);
	m_pdev->SetRenderState(D3DRS_ALPHAREF, 0x0000009);
	//m_pdev->SetRenderState(D3DRS_DIFFUSEMATERIALSOURCE, D3DMCS_COLOR1);
	//	m_pdev->SetRenderState(D3DRS_STENCILWRITEENABLE, FALSE);
	
	return mmOk;
}


// CheckCaps: checks device caps
//==========================================================================================
mmRes clsDevice::CheckCaps()
{
#ifdef MHM_USE_DX8
	D3DCAPS8 caps;
	if (FAILED(m_pd3d->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &caps)))
	{
		g_plog->Out("Device: cant get device caps", enLCRed);
		return mmFail;
	}
#else
	D3DCAPS9 caps;

	if (FAILED(m_pd3d->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &caps)))
	{
		g_plog->Out("Device: cant get device caps", enLCRed);
		return mmFail;
	}
	if (caps.Caps2 && D3DCAPS2_CANAUTOGENMIPMAP)
		g_plog->Out("Device supports mimpmap autogen", enLCGreen);
	else
		g_plog->Out("Device dont supports mimpmap autogen", enLCRed);
	
	if (m_pd3d->CheckDeviceFormat(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, D3DFMT_X8R8G8B8, D3DUSAGE_AUTOGENMIPMAP, D3DRTYPE_TEXTURE, D3DFMT_X8R8G8B8) == D3DOK_NOAUTOGEN)
		g_plog->Out("Device supports mimpmap autogen for textures", enLCRed);
	else
		g_plog->Out("Device supports mimpmap autogen for textures", enLCGreen);

#endif
	
	return mmOk;
}



// SetFVF: sets device current vertex format
//==========================================================================================
mmRes clsDevice::SetFVF(mmDword fvf)
{
#ifdef MHM_USE_DX8
	if (FAILED(m_pdev->SetVertexShader(fvf)))
		return mmFail;
#else
	if (FAILED(m_pdev->SetFVF(fvf)))
		return mmFail;
#endif // MHM_USE_DX8
	return mmOk;
}

//==========================================================================================
// State changes optimization block
//==========================================================================================

// Fill mode state change control
//==========================================================================================
mmRes clsDevice::EnableWireframe(mmBool bEnable)
{
	if (bEnable == m_bWireFrameEnabled)
		return mmOk;
	D3DFILLMODE eFill = bEnable ? D3DFILL_WIREFRAME : D3DFILL_SOLID;
	if (FAILED(m_pdev->SetRenderState(D3DRS_FILLMODE, eFill)))
		return mmFail;
	m_bWireFrameEnabled = bEnable;
	return mmOk;
}

// Culling state change control
//==========================================================================================
mmRes clsDevice::EnableCulling(mmBool bEnable)
{
	if (m_bCulling == bEnable)
		return mmOk;
	m_bCulling = bEnable;
	D3DCULL eCull = bEnable ? D3DCULL_CCW : D3DCULL_NONE;
	if (FAILED(m_pdev->SetRenderState(D3DRS_CULLMODE, eCull)))
		return mmFail;
	m_bCulling = bEnable;
	return mmOk;
}

// Specualar state change control
//==========================================================================================
mmRes clsDevice::EnableSpecular(mmBool bEnable)
{
	if (m_bSpecular == bEnable)
		return mmOk;
	if (FAILED(m_pdev->SetRenderState(D3DRS_SPECULARENABLE, bEnable)))
		return mmFail;
	if (FAILED(m_pdev->SetRenderState(D3DRS_SPECULARMATERIALSOURCE, D3DMCS_MATERIAL)))
		return mmFail;
	
	m_bSpecular = bEnable;
	return mmOk;
}
// Lighting state change control
//==========================================================================================
mmRes clsDevice::EnableLighting(mmBool bEnable)
{
	if (m_bLighting == bEnable)
		return mmOk;
	if (FAILED(m_pdev->SetRenderState(D3DRS_LIGHTING, bEnable)))
		return mmFail;
	m_bLighting = bEnable;
	return mmOk;
}

mmRes clsDevice::EnableZ(mmBool bEnable)
{
	if (m_bZ == bEnable)
		return mmOk;
	if (FAILED(m_pdev->SetRenderState(D3DRS_ZENABLE, bEnable)))
		return mmFail;
	/*
	if(bEnable)
	{
		if (FAILED(m_pdev->SetRenderState(D3DRS_ZFUNC, bEnable)))
			return mmFail;
	}else{
		if (FAILED(m_pdev->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS)))
			return mmFail;
	}
	*/
	m_bZ = bEnable;
	return mmOk;
}

#ifdef _DEBUG
// DumpVideoModes: dumps video modes list to log
//==========================================================================================
mmVoid clsDevice::DumpVideoModes()
{
	g_plog->OutBreakLine();

	g_plog->Out("Device video modes dump", enLCYellow);
	g_plog->Out("16 bit modes", enLCGreen);
	DumpVideoResolutions(m_modes16);
	g_plog->Out("32 bit modes", enLCGreen);
	DumpVideoResolutions(m_modes32);
	g_plog->Out("16 bit + alpha modes", enLCGreen);
	DumpVideoResolutions(m_modes16alpha);
	g_plog->Out("32 bit + alpha modes", enLCGreen);
	DumpVideoResolutions(m_modes32alpha);

	/*
		for (mmUint i = 0; i < m_modes16.Count(); i++)
			g_plog->Out(clsString(m_modes16[i]->m_width) + "x" + clsString(m_modes16[i]->m_height), enLCYellow);
	
	g_plog->Out("32 bit modes", enLCGreen);
	if (m_modes32.Count() == 0)
		g_plog->Out("No 32 bit device modes detected", enLCYellow);
	else
		for (mmUint i = 0; i < m_modes32.Count(); i++)
			g_plog->Out(clsString(m_modes32[i]->m_width) + "x" + clsString(m_modes32[i]->m_height), enLCYellow);
	
	g_plog->Out("16 bit + alpha modes", enLCGreen);
	if (m_modes16alpha.Count() == 0)
		g_plog->Out("No 16 bit + alpha device modes detected", enLCYellow);
	else
		for (mmUint i = 0; i < m_modes16alpha.Count(); i++)
			g_plog->Out(clsString(m_modes16alpha[i]->m_width) + "x" + clsString(m_modes16alpha[i]->m_height), enLCYellow);
	
	g_plog->Out("32 bit + alpha modes", enLCGreen);
	if (m_modes32alpha.Count() == 0)
		g_plog->Out("No 32 + alpha bit device modes detected", enLCYellow), enLCYellow;
	else
		for (mmUint i = 0; i < m_modes16.Count(); i++)
			g_plog->Out(clsString(m_modes32alpha[i]->m_width) + "x" + clsString(m_modes32alpha[i]->m_height));
	*/

	g_plog->OutBreakLine();
		
}

mmVoid clsDevice::DumpVideoResolutions(clsVideoModeList & ml)
{
	clsString str("");

	if(ml.Count()==0)
	{
		str = "No modes detected";
	}
	else
	{
		for(mmUint i=0; i<ml.Count(); i++)
		{
			str += (clsString(ml[i]->m_width) + "x" + clsString(ml[i]->m_height));
			if(i < (ml.Count()-1)) str += ", ";
		}
	}
	g_plog->Out(str, enLCYellow);;
}

#endif // _DEBUG


