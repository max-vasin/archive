//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// Brick journey shareware project source code
// (c) Mayhem Projex 2006
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

#ifndef BJ_APP_INCLUDED
#define BJ_APP_INCLUDED

class clsBJApp: public clsApp
{
protected:
	clsCamera	m_Camera;
//

	clsText*	m_pText;
	clsSprite*	m_pSprite;
//	clsFont2D*	m_pText;
	clsEntity*	m_pGameScene;
	
public:
	clsBJApp();
   ~clsBJApp();
    
    mmRes InitApp();
    mmRes ShutApp();
    mmRes IdleLoop();
    mmRes WindowSize();
    
protected:
	mmRes ModifyWindowStyle(stAppWindowStyle* pStyle);
	clsEntityFactory* CreateEntityFactory(clsEngine* pengine);
};

USE_APPLICATION(clsBJApp)

#endif // BJ_APP_INCLUDED