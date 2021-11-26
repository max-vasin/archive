//==========================================================================================
// BIOKRIZ flyer creature
//==========================================================================================
#include "mhmbase.h"
#include "BKCreature.h"
#include "BKSucker.h"

// Flyer constructor
//==========================================================================================
clsBKSucker::clsBKSucker(clsEntityFactory* pFactory,
					   const clsString& sType,
					   const clsString& sName):
			clsBKCreature(pFactory, sType, sName)
{
	m_nLife			   = 150;
	m_pMeatParticles   = mmnull;
}

// Flyer destructor
//==========================================================================================
clsBKSucker::~clsBKSucker()
{

}

// Flyer initializing
//==========================================================================================
mmVoid clsBKSucker::InitCreature(mmBool bClone)
{
	m_vPosition.z = m_fAltitude;
	SetAnimationPos(0);
	PlayAnimationRangeStart("run");
	m_eStatus = enMove;
	g_plog->Out("Sucker [" + Name + "] initialized", enLCBlue);
}

// Flyer loading
//==========================================================================================
mmVoid clsBKSucker::LoadCreature(const clsString& sFile)
{
	LoadMJ(sFile.Data());
	LoadAnimationKeys("sucker.mhmAnim");
	m_pHitScream = m_pEFactory->GetEngine()->GetSample("sounds\\flyer_hit_scream.ogg");
	g_plog->Out("Sucker [" + Name + "] loaded from [" + clsString(sFile) + "]", enLCBlue);
}

// Calculate flyer speed
//==========================================================================================
mmVoid clsBKSucker::CalcSpeed(const clsTimer* pTimer)
{
	if (m_eStatus == enMove)
	{
		clsVector3 vHeroPosition = m_vHeroPosition;
		vHeroPosition.z = m_vPosition.z;
		m_vSpeed = vHeroPosition - m_vPosition;
		m_vSpeed.NormalizeFast();
		m_vSpeed *= (m_fSpeed * pTimer->GetDeltaSecs());
	}
	if (m_eStatus == enDeath)
	{
		clsVector3 vHeroPosition = m_vDeathHeroPosition;
		vHeroPosition.z = m_vPosition.z;
		m_vSpeed = m_vPosition - vHeroPosition;
		m_vSpeed.NormalizeFast();
		m_vSpeed *= (m_fSpeed * 1.5f * pTimer->GetDeltaSecs());
	}
}

// Switch animations
//==========================================================================================
mmVoid clsBKSucker::OnAnimationKey(mmInt key)
{
	if (key == 3)
	{
		PlayAnimationRange("run");
		m_eStatus = enMove;
	}
	if (key == 5) // force death animation ended
	{
		KillCreature();
		m_eStatus = enDeath;
	}
}

// Projectile damage
//==========================================================================================
mmVoid clsBKSucker::OnApplyProjectileDamage(mmInt &nAmout, 
										   clsVector3 &vHitPoint,
										   clsRay &HitRay)
{
	if (m_eStatus != enDeath)
	{
		PlayAnimationRangeStart("projectile_hit");
		m_eStatus = enWait;
	}
}

// Projectile damage
//==========================================================================================
mmVoid clsBKSucker::OnDeath()
{
	PlayAnimationRangeStart("force_death");
	m_eStatus = enDeath;
}




