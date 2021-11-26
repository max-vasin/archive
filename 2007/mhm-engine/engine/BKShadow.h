//==========================================================================================
// BIOKRIZ shadow entity
//==========================================================================================
#ifndef BK_SHADOW_INCLUDED
#define BK_SHADOW_INCLUDED


class clsBKShadow: public clsEntity
{
public:
	clsBKShadow(clsEntityFactory* pFactory,
				const clsString& sType,
				const clsString& sName);
   ~clsBKShadow();
    
    mmVoid OnInitialize();
    mmRes  OnRender();
};



#endif // BKSHADOW_INCLUDED
