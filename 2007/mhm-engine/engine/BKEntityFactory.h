//==========================================================================================
// BIOKRIZ entity factory class
// Coded:	MozG
// Created:	29 july 2006
//==========================================================================================
#ifndef BK_ENTITY_FACTORY_INCLUDED
#define BK_ENTITY_FACTORY_INCLUDED

class clsBiokrizEntityFactory: public clsEntityFactory
{
public:
	clsBiokrizEntityFactory(clsEngine* pEngine);
   ~clsBiokrizEntityFactory();
	
	clsEntity* CreateEntity(const clsString& type,
							const clsString& name);
};

#endif // BK_ENTITY_FACTORY_INCLUDED
