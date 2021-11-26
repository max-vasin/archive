//==========================================================================================
// Biocrisis application
// Coded:	MozG
// Created:	7 july 2006
//==========================================================================================
#ifndef BIOAPP_INCLUDED
#define BIOAPP_INCLUDED

class clsBiocrisisApp: public clsApp
{
protected:
	clsText*			m_pText;
	clsText*			m_pTextStatus;
	clsText*			m_pTextMouse;
	clsBaseLight*		m_pLight;
	clsBaseMesh*		m_pHero;
	clsRenderer*		m_pRenderer;
	clsResourceManager*	m_pResMan;
	clsCamera			m_cCamera;
	clsMatrix			m_mxHeroRotation;
	clsEntity*			m_SceneHeroRun;
	
	// Controllers
	clsLinearController				m_cLinearCtrl;
	clsConstantRotationController	m_cConstRotCtrl;
	clsConstantRotationController	m_cMiniGunRot;
	
	// My tests
	mmFloat				m_fMoveSpeed;
	mmFloat				m_fStrafeSpeed;
	clsVector3			m_fAngles;

public:
	clsBiocrisisApp();
   ~clsBiocrisisApp();
    
    mmRes InitApp();
    mmRes IdleLoop();
    mmRes WindowSize();
    
    mmRes UpdateObjects();
    mmRes RenderObjects();

protected:
	mmRes ModifyWindowStyle(stAppWindowStyle* pStyle);
};

USE_APPLICATION(clsBiocrisisApp)

#endif // BIOAPP_INCLUDED