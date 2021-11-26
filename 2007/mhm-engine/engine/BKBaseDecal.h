//==========================================================================================
// BIOKRIZ base decals class
//==========================================================================================
#ifndef BK_BASEDECAL_INCLUDED
#define BK_BASEDECAL_INCLUDED

class clsBKBaseDecal: public clsEntity
{
protected:
	mmFloat	m_fAlphaValue;
	mmFloat	m_fFadeTime;
	mmFloat m_fCurrentFade;
	
	
public:
	clsBKBaseDecal(clsEntityFactory* pFactory,
				   const clsString& sType,
				   const clsString& sName);
	virtual ~clsBKBaseDecal();
	
	mmVoid OnInitialize(mmBool bClone);
	mmRes OnRender();
	mmVoid OnUpdate(const clsTimer* pTimer);
};

#endif // BK_BASEDECAL_INCLUDED
