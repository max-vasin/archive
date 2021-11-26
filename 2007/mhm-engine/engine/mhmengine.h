//==========================================================================================
// Mayhem engine root class interfaces
// Coded:	MozG
// Created:	25 june 2006
//==========================================================================================
#ifndef _MHMENGINE_INCLUDED
#define _MHMENGINE_INCLUDED

enum enCpu
{
	enCPU_Old = 0,
	enCPU_SSE = 1, // p3/4
	enCPU_3DNow = 2, // amd

	enCPUDword			= 0xFFFFFFFF

};

//===============================================
// Render array
//===============================================
class clsRenderObject;

class clsRenderObject
{
public:
	clsEntity	*E;
	float		dist;
	__inline mmBool operator > (const clsRenderObject &o)const {return dist > o.dist;};
	__inline mmBool operator < (const clsRenderObject &o)const {return dist < o.dist;};
};
typedef tmpRefArr<clsRenderObject> clsRenderArray;


//=======================================================================

//=======================================================================
class clsApp;
class clsEnvironment
{
public:
	clsLinearController		m_cLinearController;
	clsRand					m_cRand;	
	clsApp					*m_pMyApp;
	clsMemoryPool*			m_pStringPool;
	clsVertexBuffer*		m_pLastVB;
	clsIndexBuffer*			m_pLastIB;
	mmDword					m_dwLastFVF;
//-----------------------------------------------------------------------------
// Name: CPU
// Desc: 0 = old cpu   |   1 = SSE cpu  |   2 = 3DNOW cpu
//-----------------------------------------------------------------------------
	enCpu					m_eCpuType;


	clsEnvironment()
	{
		m_pStringPool	= mmnull;
		m_pLastVB		= mmnull;
		m_pLastIB		= mmnull;
		m_dwLastFVF		= mmnull;
	}
};

class clsEntity;

class clsEngine
{
protected:
	HINSTANCE			m_hInst;
	clsRenderer*		m_pRenderer;
	clsInput*			m_pInput;
	clsTimer			m_timer;
	clsSoundManager*	m_pSoundManager;
	clsString			m_sModulePath;
	clsEntityFactory*	m_pEFactory;
	clsEntity*			m_pActiveScene;
	//fyrex new
	//clsDataProvider*	m_pDataProvider;

	// default linear animation controller
	clsLinearController	m_cLinearCtrl;

	clsRenderArray*		m_pRenderArray;
	clsRenderArray*		m_pLightsArray;
	clsRenderObject		m_cCurrentRenderObject;
	clsMatrix*			m_pmxCurrentView;
	clsMatrix			m_mxCurrentMatrix;
	clsEntityArray		m_cSuicides;
	clsEntityArray		m_cCollides;

public:
	clsEngine(HINSTANCE hinst);
   ~clsEngine();
   mmVoid DefineCPU();
//	__inline int CPU(){return cpu_type;};
    
    mmRes Init(HWND hwnd, const mmString inifile);
    mmRes Shut();
    clsRenderer* GetRenderer();
    clsInput*    GetInput();
    clsTimer*	 GetTimer();
    mmRes Update();
    HINSTANCE    GetHinstance();
    const clsString& GetModulePath();
    mmRes Resize();
    mmUint GetViewWidth();
    mmUint GetViewHeight();
    mmRes  SetEntityFactory(clsEntityFactory* pfact);
	
	clsEntity * CreateScene(const clsString& name);
	mmVoid    DestroyScene(clsEntity* pRoot);
	mmVoid    SetActiveScene(clsEntity* pRoot);
	mmVoid    RenderScene(clsEntity * pScene = mmnull);
	mmVoid    CollectRenderables(clsEntity *);
	mmBool	CheckSM();
	clsSound* GetSample(const clsString& sFileName);
	clsSound* GetSound(const clsString& sFileName);
	clsSound* GetStream(const clsString & sFileName);
	void	SoundVolume(const mmFloat V);
	void	StreamVolume(const mmFloat V);

	clsSoundManager* GetSM();
	//clsDataProvider* GetDP();
};

extern clsEnvironment* g_pEnvironment;


#endif // _MHMENGINE_INCLUDED