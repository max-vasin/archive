//==========================================================================================
// BIOKRIZ flyer creature
//==========================================================================================
#include "mhmbase.h"
#include "BKCreature.h"
#include "BKBug.h"

// Flyer constructor
//==========================================================================================
clsBKBug::clsBKBug(clsEntityFactory* pFactory,
					   const clsString& sType,
					   const clsString& sName):
			clsBKCreature(pFactory, sType, sName)
{
	m_nLife			   = 210;
	m_pMeatParticles   = mmnull;
}

// Flyer destructor
//==========================================================================================
clsBKBug::~clsBKBug()
{

}

// Flyer initializing
//==========================================================================================
mmVoid clsBKBug::InitCreature(mmBool bClone)
{
	m_vPosition.z = m_fAltitude;
	SetAnimationPos(0);
	PlayAnimationRangeStart("walk");
	m_eStatus = enMove;
	m_mxLocal.RotationZ(MHM_PI);
	g_plog->Out("Bug [" + Name + "] initialized", enLCBlue);
}

// Flyer loading
//==========================================================================================
mmVoid clsBKBug::LoadCreature(const clsString& sFile)
{
	LoadMJ(sFile.Data());
	LoadAnimationKeys("bug.mhmAnim");
	m_pHitScream = m_pEFactory->GetEngine()->GetSample("sounds\\flyer_hit_scream.ogg");
	g_plog->Out("Bug [" + Name + "] loaded from [" + clsString(sFile) + "]", enLCBlue);
}

// Calculate flyer speed
//==========================================================================================
mmVoid clsBKBug::CalcSpeed(const clsTimer* pTimer)
{
	clsVector3 vHeroPosition = m_vHeroPosition;
	vHeroPosition.z = m_vPosition.z;
	m_vSpeed = vHeroPosition - m_vPosition;
	m_vSpeed.NormalizeFast();
	m_vSpeed *= (m_fSpeed * pTimer->GetDeltaSecs());
}

// Switch animations
//==========================================================================================
mmVoid clsBKBug::OnAnimationKey(mmInt key)
{
//	if (key == 3)
//	{
//		PlayAnimationRange("run");
//		m_eStatus = enMove;
//	}
}

// Projectile damage
//==========================================================================================
mmVoid clsBKBug::OnApplyProjectileDamage(mmInt &nAmout, 
										   clsVector3 &vHitPoint,
										   clsRay &HitRay)
{
//	if (m_eStatus != enDeath)
//	{
//		PlayAnimationRangeStart("projectile_hit");
//		m_eStatus = enWait;
//	}
}




