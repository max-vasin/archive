//==========================================================================================
// BIOKRIZ weapon entity class
//==========================================================================================

#ifndef BK_WEAPON_INCLUDED
#define BK_WEAPON_INCLUDED

class clsBKWeapon: public clsEntity
{
public:
	enum enWeaponType
	{
		enUndefined			= 0,
		enPistol			= 1,
		enMinigun			= 2,
		enRocketLauncher	= 3
	};
	
	enum enWeaponState
	{
		enReady		= 0,
		enFiring	= 1,
		enWaiting	= 2,
		enReloading = 3
	};

protected:
	// weapon characteristics
	clsEntity*		m_pFireBeam;
	clsEntity*		m_pGunFire;
	mmFloat			m_fFirePause;
	mmFloat			m_fFireTime;
	mmFloat			m_fReloadPause;
	enWeaponType	m_eType;
	enWeaponState	m_eState;
	mmInt			m_nAmmoCount;
	mmFloat			m_fCurTime;
	
	clsEntity*		m_pHitEntity;
	clsVector3		m_vHitPoint;
	clsVector3		m_vBloodPoint;
	mmFloat			m_fMinDistance;
	
	clsSound*		m_pShotSound;
	
	clsVector3		m_vTarget;
	
	// pistol block
	
	//debug
	clsRay			m_HitRay;
	
	
public:
	clsBKWeapon(clsEntityFactory* pFactory,
				const clsString& sType,
				const clsString& sName);
   ~clsBKWeapon();
    
    mmRes  OnRender();
    mmVoid OnInitialize(mmBool bClone);
    mmVoid OnLoadProps(clsVarList* pVarList);
    mmVoid OnClone(clsEntity* pInstance);
    mmVoid OnUpdate(const clsTimer* pTimer);
    mmVoid OnDepencyUpdate(clsEntity* pEntity, mmBool bMeDead);
    mmVoid OnSphereRayIntersect(clsEntity* pEntity,
								const clsVector3& pNear,
								const clsVector3& pFar);
    
    mmFloat			GetFirePause();
    mmFloat			GetReloadPause();
    enWeaponState	GetState();
    enWeaponType	GetType();
    mmVoid			SetState(enWeaponState eState);
    mmVoid			SetTarget(const clsVector3& vTarget);
    
    mmVoid			PistolReady();
    mmVoid			PistolFire();
    mmVoid			PistolReload();
    mmVoid			PistolWait();
    
};

#endif // BK_WEAPON_INCLUDED
