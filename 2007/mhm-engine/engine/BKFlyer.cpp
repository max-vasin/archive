//==========================================================================================
// BIOKRIZ flyer creature
//==========================================================================================
#include "mhmbase.h"
#include "BKCreature.h"
#include "BKParticle.h"
#include "BKParticleEmitter.h"
#include "BKFlyer.h"

// Flyer constructor
//==========================================================================================
clsBKFlyer::clsBKFlyer(clsEntityFactory* pFactory,
					   const clsString& sType,
					   const clsString& sName):
			clsBKCreature(pFactory, sType, sName)
{
	m_nLife			   = 110;
	m_pMeatParticles   = mmnull;
}

// Flyer destructor
//==========================================================================================
clsBKFlyer::~clsBKFlyer()
{

}

// Flyer initializing
//==========================================================================================
mmVoid clsBKFlyer::InitCreature(mmBool bClone)
{
	m_vPosition.z = m_fAltitude;
	SetAnimationPos(0);
	PlayAnimationRangeStart("fly");
	m_eStatus = enMove;
	g_plog->Out("Flyer [" + Name + "] initialized", enLCBlue);
}

// Flyer loading
//==========================================================================================
mmVoid clsBKFlyer::LoadCreature(const clsString& sFile)
{
	LoadMJ(sFile.Data());
	LoadAnimationKeys("flyer.mhmAnim");
	m_pHitScream = m_pEFactory->GetEngine()->GetSample("sounds\\flyer_hit_scream.ogg");
	m_pMeatParticles = AddChildNode("ParticleEmitter", "MeatParticles");
	m_pMeatParticles->SetOwner(mmnull);
	m_pMeatParticles->LoadMJ("meshes\\meat_particles.mj");
	m_pMeatParticles->ProcessInitialize();
	g_plog->Out("Flyer [" + Name + "] loaded from [" + clsString(sFile) + "]", enLCBlue);
}

// Flyer cloning
//==========================================================================================
mmVoid clsBKFlyer::CloneCreature(clsEntity* pInstance)
{
	clsBKFlyer* pFlyer = (clsBKFlyer*)pInstance;
	m_pMeatParticles = pFlyer->m_pMeatParticles;
}

// Calculate flyer speed
//==========================================================================================
mmVoid clsBKFlyer::CalcSpeed(const clsTimer* pTimer)
{
	clsVector3 vHeroPosition = m_vHeroPosition;
	vHeroPosition.z = m_vPosition.z;
	m_vSpeed = vHeroPosition - m_vPosition;
	m_vSpeed.NormalizeFast();
	m_vSpeed *= (m_fSpeed * pTimer->GetDeltaSecs());
}

// Switch animations
//==========================================================================================
mmVoid clsBKFlyer::OnAnimationKey(mmInt key)
{
	if (key == 3)
	{
		PlayAnimationRange("fly");
		m_eStatus = enMove;
	}
}

// Projectile damage
//==========================================================================================
mmVoid clsBKFlyer::OnApplyProjectileDamage(mmInt &nAmout, 
										   clsVector3 &vHitPoint,
										   clsRay &HitRay)
{
	SetAnimationPos("start_hit");
	PlayAnimationRange("hit");
	m_eStatus = enWait;
}

// Flyer death
//==========================================================================================
mmVoid clsBKFlyer::OnDeath()
{
	if (m_pMeatParticles)
	{
		clsBKParticleEmitter* pEmitter = (clsBKParticleEmitter*)m_pMeatParticles->Clone("FlyerMeat", mmtrue);
		pEmitter->SetOwner(FindRoot());
		pEmitter->m_mxWorld = m_mxFull;
		pEmitter->ProcessInitialize(mmtrue);
		pEmitter->Animate();
		
		
		// calc pulse point
		clsVector3 vHitVec = m_vDeathHeroPosition - m_vPosition;
		vHitVec.NormalizeFast();
		vHitVec *= 30.0f;
		vHitVec += m_vPosition;
		
		// emitter params
		pEmitter->m_cBouncingPlane    = clsPlane(0.0f, 0.0f, 1.0f, 0.0f);
		pEmitter->m_fRotationSpeed    = 1.0f;
		pEmitter->m_fPulseForce       = 200.0f;
		pEmitter->m_vGravity          = clsVector3(0.0f, 0.0f, -3.0f);
		pEmitter->m_fBouncing         = 0.6f;
		pEmitter->m_fRotationVariance = 0.6f;
		pEmitter->m_fSpeedVariance	  = 0.6f;
		pEmitter->m_vPulsePoint       = vHitVec;
		pEmitter->m_fLifeTime         = 5.0f;
		pEmitter->Start();
	}
	KillCreature();
}



