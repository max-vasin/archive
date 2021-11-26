//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// Brick journey shareware project source code
// (c) Mayhem Projex 2006
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

#ifndef BJ_EFACTORY_INCLUDED
#define BJ_EFACTORY_INCLUDED

class clsBJEntityFactory: public clsEntityFactory
{
public:
	clsBJEntityFactory(clsEngine* pEngine);
   ~clsBJEntityFactory();
	
	clsEntity* CreateEntity(const clsString& sType, const clsString& sName);
							
};

#endif // BJ_EFACTORY_INCLUDED