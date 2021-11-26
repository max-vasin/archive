//==========================================================================================
// Mayhem engine renderer class definition
// Coded:	MozG
// Created:	31 may 2006
//==========================================================================================
#ifndef _MHMRENDERER_INCLUDED
#define _MHMRENDERER_INCLUDED


//========================================================
// Help graphics structures
//========================================================
struct stVectorVertex
{
	clsVector3	m_vPos;
	mmDword		m_dwColor;
};


class clsEngine;
class clsRenderer
{
protected:
// DirectX vary block start
#ifdef MHM_USE_DX8
	IDirect3D8*				m_pd3d;
#else
	MHMDirect3D*				m_pd3d;
#endif
// DirectX vary block end
	clsDevice*	m_pdevice;
	mmBool		m_binit;
	clsCamera*	m_pcam;
	clsEngine*	m_engine;
	stVectorVertex m_VectorVerts[2];

	clsCamera *	m_pcam2d;
	clsCamera *	m_pcam_tmp;
	
public:

	mmVoid SetCam2D(mmFloat p_width, mmFloat p_height, mmFloat p_near, mmFloat p_far );

	clsRenderer(clsEngine *pengine);
   ~clsRenderer();

	mmVoid Start2D();
	mmVoid End2D();
    
    mmRes Init(HWND hwnd, clsString inifile);
    mmRes Shut();
    clsDevice*			GetDevice();
    clsResourceManager* GetResourceManager();
    clsEngine*			GetEngine();
    clsViewport*		GetViewport();
    mmRes SetAmbientLight(mmDword value);
    
    //=============================================
    // This methods maps to device
    //=============================================
    mmRes StartRender();
    mmRes StopRender();
    mmRes Clear(mmBool bTarget=mmfalse);
    mmRes Present();
    mmRes Resize();
    
    mmUint GetWidth();
    mmUint GetHeight();
    
    mmRes Update();
// Renderer camera control [added by MozG 6 june 2006]
	mmRes SetCamera(clsCamera* pcam);
    clsCamera*			GetCamera();
    
    clsRay UnProject(const clsPoint2D& pt);
    // help graphics control
    mmVoid DrawVector(const clsVector3& Pos,
					  const clsVector3& Dir,
					  mmDword dwColor,
					  mmFloat fScale);

    
};


#endif // _MHMRENDERER_INCLUDED