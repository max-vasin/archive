//==========================================================================================
// BIOKRIZ bug creature
//==========================================================================================
#ifndef BK_BUG_INCLUDED
#define BK_BUG_INCLUDED

class clsBKBug: public clsBKCreature
{
protected:
	clsEntity*		m_pMeatParticles;
	
public:
	clsBKBug(clsEntityFactory* pFactory,
			   const clsString& sType,
			   const clsString& sName);
   ~clsBKBug();

protected:
	mmVoid InitCreature(mmBool bClone);
	mmVoid LoadCreature(const clsString& sFile);
	mmVoid CalcSpeed(const clsTimer* pTimer);
	mmVoid OnAnimationKey(mmInt key);

	mmVoid OnApplyProjectileDamage(mmInt& nAmout,
								   clsVector3& vHitPoint,
								   clsRay& HitRay);
};

#endif // BK_SUCKER_INCLUDED