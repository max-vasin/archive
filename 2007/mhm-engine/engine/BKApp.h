//==========================================================================================
// BIOKRIZ application
// Coded:	MozG
// Created:	7 july 2006
//==========================================================================================
#ifndef BIOAPP_INCLUDED
#define BIOAPP_INCLUDED

//==============================================================
// Various game modes
//==============================================================
enum enBKGameMode
{
	enBKStartup	 = 0,
	enBKMenu	 = 1,
	enBKSurvival = 2
};

//==============================================================
// Biokriz application
//==============================================================
class clsBiokrizApp: public clsApp
{
protected:
	enBKGameMode		m_eMode;
	clsBiokrizSurvival*	m_pSurvivalMode;

public:
	clsBiokrizApp();
   ~clsBiokrizApp();
    
    mmRes InitApp();
    mmRes ShutApp();
    mmRes IdleLoop();
    mmRes WindowSize();
    
protected:
	mmRes ModifyWindowStyle(stAppWindowStyle* pStyle);
	clsEntityFactory* CreateEntityFactory(clsEngine* pengine);
};

USE_APPLICATION(clsBiokrizApp)

#endif // BIOAPP_INCLUDED