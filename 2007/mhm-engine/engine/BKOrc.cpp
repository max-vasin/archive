//==========================================================================================
// BIOKRIZ orc creature
//==========================================================================================
#include "mhmbase.h"
#include "BKCreature.h"
#include "BKOrc.h"

// Orc constructor
//==========================================================================================
clsBKOrc::clsBKOrc(clsEntityFactory* pFactory,
					   const clsString& sType,
					   const clsString& sName):
			clsBKCreature(pFactory, sType, sName)
{
	m_nLife			   = 110;
	m_pMeatParticles   = mmnull;
	m_bRun			   = mmfalse;
}

// Orc destructor
//==========================================================================================
clsBKOrc::~clsBKOrc()
{

}

// Orc initializing
//==========================================================================================
mmVoid clsBKOrc::InitCreature(mmBool bClone)
{
	m_vPosition.z = m_fAltitude;
	SetAnimationPos(0);
	PlayAnimationRangeStart("walk");
	m_eStatus = enMove;
	g_plog->Out("Orc [" + Name + "] initialized", enLCBlue);
}

// Orc loading
//==========================================================================================
mmVoid clsBKOrc::LoadCreature(const clsString& sFile)
{
	LoadMJ(sFile.Data());
	LoadAnimationKeys("orc.mhmAnim");
	m_pHitScream = m_pEFactory->GetEngine()->GetSample("sounds\\flyer_hit_scream.ogg");
	g_plog->Out("Orc [" + Name + "] loaded from [" + clsString(sFile) + "]", enLCBlue);
}

// Calculate orc speed
//==========================================================================================
mmVoid clsBKOrc::CalcSpeed(const clsTimer* pTimer)
{
	if (m_eStatus == enMove)
	{
		clsVector3 vHeroPosition = m_vHeroPosition;
		vHeroPosition.z = m_vPosition.z;
		m_vSpeed = vHeroPosition - m_vPosition;
		m_vSpeed.NormalizeFast();
		mmFloat fSpeed = m_fSpeed;
		if (m_bRun)
			fSpeed *= 4.0f;
		m_vSpeed *= (fSpeed * pTimer->GetDeltaSecs());
	}
	if (m_eStatus == enDeath)
	{
		clsVector3 vHeroPosition = m_vDeathHeroPosition;
		vHeroPosition.z = m_vPosition.z;
		m_vSpeed = m_vPosition - vHeroPosition;
		m_vSpeed.NormalizeFast();
		m_vSpeed *= (m_fSpeed * 4.0f * pTimer->GetDeltaSecs());
	}
}

mmVoid clsBKOrc::UpdateCreature(const clsTimer* pTimer)
{
	if (m_eStatus == enMove)
	{
		clsVector3 vDir = m_vHeroPosition - m_vPosition;
		if ((vDir.Length() < 200.0f) && (!m_bRun))
		{
			PlayAnimationRangeStart("run");
			m_bRun = mmtrue;
		}
		if ((vDir.Length() > 200.0f) && (m_bRun))
		{
			PlayAnimationRangeStart("walk");
			m_bRun = mmfalse;
		}
	}
}

// Switch animations
//==========================================================================================
mmVoid clsBKOrc::OnAnimationKey(mmInt key)
{
	if (key == 3) // force death animation ended
	{
		KillCreature();
		m_eStatus = enDeath;
	}
	if (key == 5) // end of projectile hit animation
	{
		m_eStatus = enMove;
		PlayAnimationRangeStart("walk");
	}
}

// Projectile damage
//==========================================================================================
mmVoid clsBKOrc::OnApplyProjectileDamage(mmInt &nAmout, 
										   clsVector3 &vHitPoint,
										   clsRay &HitRay)
{
	if (m_eStatus != enDeath)
	{
		PlayAnimationRangeStart("projectile_hit");
		m_eStatus = enWait;
	}
}

// Death
//==========================================================================================
mmVoid clsBKOrc::OnDeath()
{
	PlayAnimationRangeStart("force_death");
	m_eStatus = enDeath;
}




