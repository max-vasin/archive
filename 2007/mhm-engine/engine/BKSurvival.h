//==========================================================================================
// BIOKRIZ survival mode class
// Coded:	MozG
// Created:	29 july 2006
//==========================================================================================
#ifndef BK_SURVIVAL_INCLUDED
#define BK_SURVIVAL_INCLUDED

class clsBiokrizSurvival
{
protected:
	// engine objects
	clsEngine*			m_pEngine;
	clsRenderer*		m_pRenderer;
	clsInput*			m_pInput;
	clsKeyboard*		m_pKeyboard;
	clsMouse*			m_pMouse;
	clsTimer*			m_pTimer;
	clsSound*			m_pMusic;
	
	// Helpers
	clsString			m_sMeshPath;
	clsCamera			m_cCamera;
	clsBKGameCamera*	m_pGameCamera;
	clsEntity*			m_pModeScene;
	clsText*			m_pTextStatistic;
	mmBool				m_bSlow;
	//clsFont2D*		m_pTextStatistic;
	clsSprite*			m_pLoadingSprite;
	
	
public:
	clsBiokrizSurvival(clsEngine* pEngine);
   ~clsBiokrizSurvival();
    
    mmRes LoadObjects();
    mmRes FirstTimeInit();
    mmRes UpdateObjects();
    mmRes RenderObjects();
    mmRes LeaveMode();
    mmRes ResizeViewport();

protected:
	mmVoid UpdateStatistics();
};



#endif //BK_SURVIVAL_INCLUDED
