//==========================================================================================
// BIOKRIZ bonus manager
//==========================================================================================
#ifndef BK_BONUSMANAGER_INCLUDED
#define BK_BONUSMANAGER_INCLUDED

#include "BKBonus.h"

class clsBKBonusManager: public clsEntity
{
protected:
	clsEntityArray	m_arBonuses;
	mmInt			m_nDeathCounter;
	mmInt			m_nTotalQualifier;
	mmInt			m_nScore;
	clsText*		m_pText;
	
public:
	clsBKBonusManager(clsEntityFactory* pFactory,
					  const clsString& sType,
					  const clsString& sName);
   ~clsBKBonusManager();
   
   mmRes  OnRender();
   mmVoid OnInitialize(mmBool bClone);
   mmVoid OnPostLoading();
   mmVoid OnEnumChild(clsEntity* pEntity);
   
   // bonus manager collector
   mmVoid RegisterDeath(clsEntity* pEntity);
   mmVoid AddPoints(mmInt nCount);

protected:
   mmVoid GenerateBonus(const clsVector3& vWhere);
};

#endif // BK_BONUSMANAGER_INCLUDED
