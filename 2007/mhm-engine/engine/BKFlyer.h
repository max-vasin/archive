//==========================================================================================
// BIOKRIZ flyer creature
//==========================================================================================
#ifndef BK_FLYER_INCLUDED
#define BK_FLYER_INCLUDED

class clsBKFlyer: public clsBKCreature
{
protected:
	clsEntity*		m_pMeatParticles;
	
public:
	clsBKFlyer(clsEntityFactory* pFactory,
			   const clsString& sType,
			   const clsString& sName);
   ~clsBKFlyer();

protected:
//	mmRes  OnRender()
//	{
//		return mmOk;
//	}
	mmVoid InitCreature(mmBool bClone);
	mmVoid LoadCreature(const clsString& sFile);
	mmVoid CloneCreature(clsEntity* pInstance);
	mmVoid CalcSpeed(const clsTimer* pTimer);
	mmVoid OnAnimationKey(mmInt key);

	mmVoid OnApplyProjectileDamage(mmInt& nAmout,
								   clsVector3& vHitPoint,
								   clsRay& HitRay);
	mmVoid OnDeath();
};

#endif // BK_FLYER_INCLUDED