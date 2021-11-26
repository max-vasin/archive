//==========================================================================================
// Mayhem engine Direct3D device class
// Coded:	MozG
// Created:	7 may 2006
//==========================================================================================

#ifndef _MHMDEVICE_INCLUDED
#define _MHMDEVICE_INCLUDED

#include "mhmdevcfg.h"	// device configuration holder

// Device error codes
#define MHMDEVICE_NO_INI	mmOk + 1	// device cant find specified ini file
#define MHMDEVICE_DX_ERR	mmOk + 2
#define MHMDEVICE_NOT_INIT	mmOk + 3	// device not initialized\created

// Direct3D device invalidate resources event structure class
//==========================================================================================
class clsDeviceInvalidateEvent
{
public:
	mmDword	m_reserved;
};

// Direct3D device restore resource event structure class
//==========================================================================================
class clsDeviceRestoreEvent
{
public:
	mmDword m_reserved;
};

// Video mode class
//==========================================================================================
class clsVideoMode
{
public:
	mmUint		m_width;
	mmUint		m_height;
	D3DFORMAT	m_fmt;
	
public:
	clsVideoMode(mmUint width, mmUint height, D3DFORMAT fmt)
	{
		m_width  = width;
		m_height = height;
		m_fmt	 = fmt;
	}
};

//=================================================================
// Video mode list class
//=================================================================
class clsVideoModeList: public tmpPtrList<clsVideoMode*>
{
public:
	clsVideoModeList();
   ~clsVideoModeList();
    mmVoid Sort();
};

//=================================================================
// Video mode bit per pixel enumerator
//=================================================================
enum enBitsPerPixel
{
	enBPPDontCare	= 0,
	enBPP16			= 1,
	enBPP32			= 2
};

//=================================================================
// Device viewport class
//=================================================================
class clsViewport
{
public:
	mmUint m_nX, m_nY, m_nWidth, m_nHeight;

public:
	clsViewport():m_nX(0), m_nY(0), m_nWidth(0), m_nHeight(0){}
	clsViewport(mmUint x, mmUint y, mmUint width, mmUint height):
				 m_nX(x), m_nY(y), m_nWidth(width), m_nHeight(height){}
	
	mmFloat GetAspect()
	{
		return (mmFloat)m_nWidth / (mmFloat)m_nHeight;
	}

};

//#define MHMDEVICE_MAX_LIGHTS	10


// Device class
//==========================================================================================
class clsResourceManager;
class clsRenderer;
class clsDevice
{
protected:
	MHMDirect3D*			m_pd3d;
	MHMDirect3DDevice*		m_pdev;
	clsString				m_ininame;
	clsResourceManager*		m_presman;
	clsRenderer*			m_renderer;
//	clsDeviceConfig*		m_pconfig;
	HWND					m_hwnd;
	clsViewport				m_vport;
	
	mmUint					m_width;
	mmUint					m_height;
	mmUint					m_nMaxSamples;
	
	/*
	clsVideoModeList		m_modes16;
	clsVideoModeList		m_modes32;
	clsVideoModeList		m_modes16alpha;
	clsVideoModeList		m_modes32alpha;
	*/
	
	// Current transforms
	clsMatrix				m_proj_tm;
	clsMatrix				m_view_tm;
	clsMatrix				m_wrld_tm;
	
	// State changes controllers
	mmBool					m_bWireFrameEnabled;
	mmBool					m_bCulling;
	mmBool					m_bSpecular;
	mmBool					m_bLighting;
	mmBool					m_bZ;


	D3DPRESENT_PARAMETERS	pp;

	struct strFormatS
	{
		strFormatS(){};
		strFormatS(D3DFORMAT bbf, D3DFORMAT dsf)
		{
			m_BackBufferFormat = bbf;
			m_AutoDepthStencilFormat = dsf;
		};
		D3DFORMAT m_BackBufferFormat;
		D3DFORMAT m_AutoDepthStencilFormat;
	};

	tmpRefArr <strFormatS> m_FormatS;

public:
	clsDevice(clsRenderer* pren, MHMDirect3D* pd3d);
   ~clsDevice();
    
    mmRes				Create(HWND hwnd, const mmString inifile);
    mmRes				Shutdown();
    mmRes				Reset(mmInt pWidth = -1, mmInt pHeight = -1);
    mmRes				Clear(mmBool bTarget=mmfalse);
    mmRes				StartRender();
    mmRes				StopRender();
    mmRes				Present(HWND hwnd = mmnull);
    mmRes				SetProjectionTM(clsMatrix* mx);
    mmRes				SetViewTM(clsMatrix* mx);
    mmRes				SetModelTM(clsMatrix* mx);
    mmRes				EnableLight(MHMLight* plight, mmBool enable, mmInt Number=0);
    clsMatrix*			GetProjectionTM();
    clsMatrix*			GetViewTM();
    clsMatrix*			GetModelTM();
    mmUint				GetWidth();
    mmUint				GetHeight();
    clsViewport*		GetViewport();
    HWND				GetWindow();

    clsResourceManager* GetResourceManager();
    clsRenderer*		GetRenderer();
    mmRes				Update();
    mmRes				SetAmbientLight(mmDword value);
    mmUint				GetMaxSamples();
	mmRes				RenderVB(mmFloat * VB, mmUint stride, mmUint count, clsResource *material, mmInt blend_type);
	mmVoid				DepthWrite(mmBool b);
	mmRes				SetFog(mmBool enable, mmDword Color=0x00000000, mmFloat Start = 100.0f, mmFloat End = 2000.0f);
	mmRes				SetBaseStates();
	mmRes				SetFVF(mmDword fvf);
	
	// state changes
	mmRes				EnableWireframe(mmBool bEnable);
	mmRes				EnableCulling(mmBool bEnable);
	mmRes				EnableSpecular(mmBool bEnable);
	mmRes				EnableLighting(mmBool bEnable);
	mmRes				EnableZ(mmBool bEnable);

	clsVideoModeList		m_modes16;
	clsVideoModeList		m_modes32;
	clsVideoModeList		m_modes16alpha;
	clsVideoModeList		m_modes32alpha;

	clsDeviceConfig*		m_pconfig;
	
    
	operator MHMDirect3DDevice*();

protected:
    mmRes LoadConfig();
    mmRes MakeModesLists();
    mmRes FindMaxSamples();
    mmRes GetNumModes(mmUint adapter, enBitsPerPixel bpp, mmBool alpha,
					  D3DFORMAT& fmt, mmUint& nummodes);
	mmRes BuildModeList(mmUint adapter, mmUint num, D3DFORMAT fmt,
					    clsVideoModeList *plist);

	mmRes CheckCaps();


// Debug only block

#ifdef _DEBUG
	mmVoid DumpVideoModes();
	mmVoid DumpVideoResolutions(clsVideoModeList & ml);
#endif // _DEBUG
};

#endif // _MHMDEVICE_INCLUDED