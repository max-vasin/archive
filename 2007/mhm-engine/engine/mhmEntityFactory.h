//==========================================================================================
// Mayhem engine entity factory class interface
// Coded:	MozG
// Created:	26 july 2006
//==========================================================================================
#ifndef MHMENTITY_FACTORY_INCLUDED
#define MHMENTITY_FACTORY_INCLUDED

class clsEntityFactory
{
protected:
	clsEngine* m_pEngine;
	
public:
	clsEntityFactory(clsEngine* pEngine);
    virtual ~clsEntityFactory();
    
    virtual clsEntity* CreateEntity(const clsString& type,
									const clsString& name);
	clsEngine* GetEngine();
};

#endif // MHMENTITY_FACTORY_INCLUDED