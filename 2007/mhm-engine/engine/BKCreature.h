//==========================================================================================
// BIOKRIZ creatures base class
//==========================================================================================
#ifndef BK_CREATURE_INCLUDED
#define BK_CREATURE_INCLUDED


class clsBKCreature: public clsEntity
{
public:
	enum enCreatureStatus
	{
		enWait	 = 0,
		enMove	 = 1,
		enAttack = 2,
		enDeath  = 3,
		enFear   = 4,
		enPanic  = 5
	};

protected:
	mmBool				m_bIntstance;
	enCreatureStatus	m_eStatus;
	// creature parameters block
	mmInt				m_nLife;
	mmFloat				m_fScale;
	mmFloat				m_fAltitude;
	mmFloat				m_fSpeed;
	
	
	// bonus manager death collector
	clsEntity*			m_pBonusManager;
	
	// creature structure
	clsEntity*			m_pCreatureMesh;
	clsEntity*			m_pBloodParticles;
	
	// creature life
	mmFloat				m_fCurrentLifeTime;	
	clsVector3			m_vHeroPosition;
	clsVector3			m_vPosition;
	clsVector3			m_vDeathHeroPosition;
	
	
	// sounds
	clsSound*			m_pHitScream;
	
	
	
	
public:
	clsBKCreature(clsEntityFactory* pFactory,
				  const clsString& sType,
				  const clsString& sName);
    virtual ~clsBKCreature();
    
    mmVoid OnInitialize(mmBool bClone);
    mmVoid OnClone(clsEntity* pInstance);
    mmVoid OnPostLoading();
    mmVoid OnEnumChild(clsEntity* pEntity);
    mmVoid OnDepencyUpdate(clsEntity* pEntity, mmBool bMeDead);
    mmVoid OnCalcSpeed(const clsTimer* pTimer);
    mmVoid OnCollide(clsEntity* pEntity, 
					 mmFloat fFirstTime,
					 mmFloat fSecondTime);
    mmVoid OnUpdate(const clsTimer* pTimer);
    
   
	mmVoid ApplyProjectileDamage(mmInt nAmount,
								 const clsVector3& vHitPoint,
								 const clsRay&	   HitRay);
	
	mmVoid ApplyExplodeDamage(mmInt nAmount,
							  const clsVector3& vExplodePoint,
							  mmFloat fFalloff);
	
	mmVoid ApplyConstantDamage(mmInt nAmount,
							   mmFloat fDamageTime);
	
protected:
	virtual mmVoid CloneCreature(clsEntity* pInstance)		{}
	virtual mmVoid OnApplyProjectileDamage(mmInt& nAmout,
										   clsVector3& vHitPoint,
										   clsRay& HitRay)		{}
	virtual mmVoid OnApplyExplodeDamage(mmInt& nAmount,
										clsVector3& vExplodePoint,
										mmFloat fFalloff)		{}
	virtual mmVoid OnApplyConstantDamage(mmInt& nAmount,
										 mmFloat fDamageTime)	{}
	virtual mmVoid LoadCreature(const clsString& sFile)			{}
	virtual mmVoid InitCreature(mmBool bClone)					{}
	virtual mmVoid CalcSpeed(const clsTimer* pTimer)			{}
	virtual mmVoid UpdateCreature(const clsTimer* pTimer)		{}
	virtual mmVoid OnAnalyzeStructure(clsEntity* pEntity)		{}
	virtual mmVoid OnHeroMove(const clsVector3& vNewPosition)	{}
	virtual mmVoid OnDeath()									{KillCreature();}
	
	mmVoid  KillCreature();

private:
	mmVoid ApplyDamage(mmInt nAmount);
};


#endif // BK_CREATURE_INCLUDED
