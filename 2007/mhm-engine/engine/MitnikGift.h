
class clsMitnikEntityFactory: public clsEntityFactory
{
public:
	clsMitnikEntityFactory(clsEngine* pEngine);
   ~clsMitnikEntityFactory();
	
	clsEntity* CreateEntity(const clsString& type,
							const clsString& name);
};

enum enState
{
	enWords,
	enBegining
};


class clsMitnikApp: public clsApp
{
protected:
	clsText*	m_pText;
	clsEntity*	m_pBarrelScene;
	clsEntity*	m_pStairsScene;
	clsEntity*	m_pAutoCamera;
	enState		m_eState;
	mmUint		m_nWordCount;
	mmFloat		m_fWordTime;
	mmFloat		m_fWordAppearTime;
	clsMatrix	m_mxProj;
	clsCamera	m_Cam;
	clsBaseMesh* m_pBarrel;
	clsMatrix	m_mxWorld;
	clsMatrix	m_mxBSphere;
	clsSound*	m_pMusic;
	
	
	
public:
	clsMitnikApp();
   ~clsMitnikApp();
    
    mmRes InitApp();
    mmRes ShutApp();
    mmRes IdleLoop();
    mmRes WindowSize();
    
protected:
	mmRes ModifyWindowStyle(stAppWindowStyle* pStyle);
	clsEntityFactory* CreateEntityFactory(clsEngine* pengine);
	mmVoid BuildWords();
	
	
};

USE_APPLICATION(clsMitnikApp)
