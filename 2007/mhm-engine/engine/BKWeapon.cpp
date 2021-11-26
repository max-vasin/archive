//==========================================================================================
// BIOKRIZ weapon entity class
//==========================================================================================
#include "mhmbase.h"
#include "BKWeapon.h"
#include "BKCrosshair.h"
#include "BKCreature.h"

// Weapon construtor
//==================================================
clsBKWeapon::clsBKWeapon(clsEntityFactory* pFactory,
						 const clsString& sType,
						 const clsString& sName):
			 clsEntity(pFactory, sType, sName)
{
	m_pFireBeam		= mmnull;
	m_fFirePause	= 0.0f;
	m_fFireTime		= 0.0f;
	m_fReloadPause	= 0.0f;
	m_pFireBeam		= mmnull;
	m_eType			= enUndefined;
	m_eState		= enReady;
	m_nAmmoCount	= 0;
	m_fCurTime      = 0.0f;
	m_bNeedUpdateEvent = mmtrue;
}

// Weapon destrutor
//==================================================
clsBKWeapon::~clsBKWeapon()
{
}

// Weapon properties loader
//==================================================
mmVoid clsBKWeapon::OnLoadProps(clsVarList* pVarList)
{
	clsVariable* var = pVarList->FindVariable("FirePause");
	if (var)
		m_fFirePause = *var;
	else
		m_fFirePause = 10.0f;
	
	var = pVarList->FindVariable("ReloadPause");
	if (var)
		m_fReloadPause = *var;
	else
		m_fReloadPause = 10.0f;
	
	var = pVarList->FindVariable("FireTime");
	if (var)
		m_fFireTime = *var;
	else
		m_fFireTime = 10.0f;
	
	var = pVarList->FindVariable("AmmoCount");
	if (var)
		m_nAmmoCount = *var;
	else
		m_nAmmoCount = 1;
		
	m_eType = enUndefined;
	var = pVarList->FindVariable("WeaponType");
	if (var)
	{
		clsString sType((mmString)*var);
		sType.UpperCase();
		if (sType == "PISTOL")
			m_eType = enPistol;
		if (sType == "MINIGUN")
			m_eType = enMinigun;
		if (sType == "ROCKETLAUNCHER")
			m_eType = enRocketLauncher;
	}
}

// Weapon initialization
//==================================================
mmVoid clsBKWeapon::OnInitialize(mmBool bClone)
{
	g_plog->Out("Initializing weapon [" + Name + "]", enLCBlue);
	if (!bClone)
	{
		m_pShotSound = m_pEFactory->GetEngine()->GetSample("Sounds\\pistol_shot.ogg");
	}
	m_pFireBeam = FindChild("FireBeam");
	m_pGunFire  = FindChild("Gunfire");
	m_pGunFire->m_bRenderable = mmfalse;
	EnableBoundSphereTest(mmfalse, mmtrue);
	m_mxWorld.Identity();
}

// Weapon updating
//==================================================
mmVoid clsBKWeapon::OnUpdate(const clsTimer* pTimer)
{
	switch(m_eState)
	{
	case enReady:
		switch (m_eType)
		{
		case enPistol:
			PistolReady();
			break;
		}
		break;
	case enWaiting:
		switch (m_eType)
		{
		case enPistol:
			PistolWait();
			break;
		}
		break;
	case enFiring:
		switch (m_eType)
		{
		case enPistol:
			PistolFire();
			break;
		}
		break;
	case enReloading:
		switch (m_eType)
		{
		case enPistol:
			PistolReload();
			break;
		}
		break;
	}
	m_fCurTime += pTimer->GetDeltaMiSecs();
}

// Weapon target point
//==================================================
mmVoid clsBKWeapon::OnDepencyUpdate(clsEntity* pEntity, mmBool bMeDead)
{
	clsBKCrosshair* pCrosshair = (clsBKCrosshair*)pEntity;
	clsRay ray = pCrosshair->GetRay();
	clsPlane plane;
	clsVector3 vPosition = m_mxWorld.GetTranslation();
	plane.FromPointNormal(vPosition, clsVector3(0.0f, 0.0f, 1.0f));
	clsVector3 vIntersect;
	plane.IntersectRay(m_vTarget, ray);
}

// Helpers rendering
//==================================================
mmRes clsBKWeapon::OnRender()
{
//	if (m_pFireBeam && m_eState == enFiring)
//	{
/*		clsVector3 vBeam = m_pFireBeam->m_mxFull.GetTranslation();
		clsVector3 vDir  = m_vTarget - vBeam;
		mmFloat fScale = vDir.Length();
		vDir.NormalizeFast();
		Renderer->DrawVector(vBeam, vDir, 0xFFFF0000, fScale * 10.0f); */
	//Renderer->DrawVector(m_HitRay.m_vOrigin, m_HitRay.m_vDirection, 0xFF0000FF, 1.0f);
//	}
	return mmOk;		 
}

// Cloning
//==================================================
mmVoid clsBKWeapon::OnClone(clsEntity *pInstance)
{
	clsBKWeapon* pWeapon = (clsBKWeapon*)pInstance;
	m_fFirePause	 = pWeapon->m_fFirePause;
	m_fFireTime		 = pWeapon->m_fFireTime;
	m_fReloadPause	 = pWeapon->m_fReloadPause;
	m_eType			 = pWeapon->m_eType;
	m_eState		 = pWeapon->m_eState;
	m_nAmmoCount	 = pWeapon->m_nAmmoCount;
	m_pShotSound	 = pWeapon->m_pShotSound;
}



// Weapon fire pause in misecs
//==================================================
mmFloat clsBKWeapon::GetFirePause()
{
	return m_fFirePause;
}

// Weapon reaload pause in misecs
//==================================================
mmFloat clsBKWeapon::GetReloadPause()
{
	return m_fReloadPause;
}

// Weapon state
//==================================================
clsBKWeapon::enWeaponState clsBKWeapon::GetState()
{
	return m_eState;
}

// Weapon type
//==================================================
clsBKWeapon::enWeaponType clsBKWeapon::GetType()
{
	return m_eType;
}

// Sets weapon state
//==================================================
mmVoid clsBKWeapon::SetState(clsBKWeapon::enWeaponState eState)
{
	m_eState = eState;
	if (m_eState == enFiring)
	{
		//clsRay Ray;
		if (m_pShotSound)
			m_pShotSound->Play((mmDword)this, 80);
		m_HitRay.m_vOrigin    = m_pFireBeam->m_mxFull.GetTranslation();
		m_HitRay.m_vOrigin.z  = 0;
		clsVector3 vTarget = m_vTarget;
		vTarget.z = 0;
		m_HitRay.m_vDirection = vTarget - m_HitRay.m_vOrigin;
		m_HitRay.m_vDirection.Normalize();
		m_HitRay.m_vDirection *= 10000.0f;
		m_pHitEntity     = mmnull;
		m_fMinDistance	 = 100000.0f;
		FindRoot()->RayBoundsIntersection(m_HitRay, this, &clsVector3(1.0f, 1.0f, 0.0f));
		if (m_pHitEntity != mmnull)
		{
			clsBKCreature* pCreature = (clsBKCreature*)m_pHitEntity;
			clsVector3 vBloodDir = m_HitRay.m_vDirection;
			vBloodDir.NormalizeFast();
			pCreature->ApplyProjectileDamage(60, m_vBloodPoint, m_HitRay);
		}
	}
}

// Sets vector target
//==================================================
mmVoid clsBKWeapon::SetTarget(const clsVector3& vTarget)
{
	m_vTarget = vTarget;
}



//==================================================
// PISTOL
//==================================================
mmVoid clsBKWeapon::PistolReady()
{
	m_fCurTime = 0.0f;
}
mmVoid clsBKWeapon::PistolFire()
{
	if (m_fCurTime >= m_fFireTime)
	{
		m_fCurTime = 0;
		m_eState = enWaiting;
		m_pGunFire->m_bRenderable = mmfalse;
		return;
	}
	m_pGunFire->m_bRenderable = mmtrue;
}
mmVoid clsBKWeapon::PistolReload()
{
	
}
mmVoid clsBKWeapon::PistolWait()
{
	if (m_fCurTime >= m_fFirePause)
	{
		m_fCurTime = 0;
		m_eState   = enReady;
		return;
	}
}

mmVoid clsBKWeapon::OnSphereRayIntersect(clsEntity *pEntity,
										 const clsVector3& vNear,
										 const clsVector3& vFar)
{
	if (pEntity->GetSuperType() != "CREATURE")
		return;
	clsVector3 vPosition = pEntity->m_mxFull.GetTranslation();
	mmFloat fDistance = (vPosition - m_pFireBeam->m_mxFull.GetTranslation()).Length();
	if (fDistance < m_fMinDistance)
	{
		m_pHitEntity   = pEntity;
		m_fMinDistance = fDistance;
		m_vHitPoint    = vNear;
		m_vBloodPoint  = vFar;
	}
	
	
}





						 