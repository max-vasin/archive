//==========================================================================================
// BIOKRIZ ground creature
//==========================================================================================
#ifndef BK_SUCKER_INCLUDED
#define BK_SUCKER_INCLUDED

class clsBKSucker: public clsBKCreature
{
protected:
	clsEntity*		m_pMeatParticles;
	
public:
	clsBKSucker(clsEntityFactory* pFactory,
			   const clsString& sType,
			   const clsString& sName);
   ~clsBKSucker();

protected:
	mmVoid InitCreature(mmBool bClone);
	mmVoid LoadCreature(const clsString& sFile);
	mmVoid CalcSpeed(const clsTimer* pTimer);
	mmVoid OnAnimationKey(mmInt key);

	mmVoid OnApplyProjectileDamage(mmInt& nAmout,
								   clsVector3& vHitPoint,
								   clsRay& HitRay);
	mmVoid OnDeath();
};

#endif // BK_SUCKER_INCLUDED