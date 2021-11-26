//==========================================================================================
// BIOKRIZ ground orc creature
//==========================================================================================
#ifndef BK_ORC_INCLUDED
#define BK_ORC_INCLUDED

class clsBKOrc: public clsBKCreature
{
protected:
	clsEntity*		m_pMeatParticles;
	mmBool			m_bRun;
	
public:
	clsBKOrc(clsEntityFactory* pFactory,
			   const clsString& sType,
			   const clsString& sName);
   ~clsBKOrc();

protected:
	mmVoid InitCreature(mmBool bClone);
	mmVoid LoadCreature(const clsString& sFile);
	mmVoid CalcSpeed(const clsTimer* pTimer);
	mmVoid OnAnimationKey(mmInt key);
	mmVoid UpdateCreature(const clsTimer* pTimer);

	mmVoid OnApplyProjectileDamage(mmInt& nAmout,
								   clsVector3& vHitPoint,
								   clsRay& HitRay);
	
	mmVoid OnDeath();
};

#endif // BK_ORC_INCLUDED