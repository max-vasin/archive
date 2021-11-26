//==========================================================================================
// BIOKRIZ mesh particles emitter
//==========================================================================================
#include "mhmbase.h"
#include "BKParticle.h"
#include "BKParticleEmitter.h"

// Emitter constructor
//==========================================================================================
clsBKParticleEmitter::clsBKParticleEmitter(clsEntityFactory *pFactory,
										   const clsString &sName,
										   const clsString &sType):
					  clsEntity(pFactory, sName, sType),
					  m_vPulsePoint(0.0f, 0.0f, 0.0f),
					  m_fLifeTime(0.0f),
					  m_fSpeedVariance(0.0f),
					  m_vGravity(0.0f, 0.0f, 0.0f),
					  m_cBouncingPlane(0.0f, 0.0f, 0.0f, 0.0f),
					  m_fPulseForce(0.0f),
					  m_fCurrentLife(0.0f),
					  m_fRotationSpeed(0.0f),
					  m_fRotationVariance(0.0f)
					  
{
	m_arParticles.SetAddSize(1000);
	m_bNeedLoad = mmtrue;
	m_bFinishUpdate = mmtrue;
	
}


// Emitter destructor
//==========================================================================================
clsBKParticleEmitter::~clsBKParticleEmitter()
{

}

// Properties loading
//==========================================================================================
mmVoid clsBKParticleEmitter::OnLoadProps(clsVarList* pvlist)
{
	if (pvlist == mmnull)
		return;
	
	clsVariable* pvar = pvlist->FindVariable("SourceScene");
	if (pvar == mmnull)
		g_plog->Out("Particle emitter: no source scene specified", enLCYellow);
	else
		m_sSourceSceneName = (mmString)*pvar;
	
	

}

// Emitter initialization
//==========================================================================================
mmVoid clsBKParticleEmitter::OnInitialize(mmBool bClone)
{
	clsEntity* pChilds = Child;
	clsString sType;
	while(pChilds)
	{
		sType = pChilds->m_sType;
		sType.UpperCase();
		if (sType == "PARTICLE")
		{
			pChilds->m_bNeedUpdateEvent  = mmfalse;
			pChilds->m_bRenderable       = mmfalse;
			pChilds->m_bEnableSphereTest = mmfalse;
			m_arParticles.Add(pChilds);
		}
		pChilds = pChilds->Next;
	}	
}

// Emitter update
//==========================================================================================
mmVoid clsBKParticleEmitter::OnUpdate(const clsTimer* pTimer)
{
	m_fCurrentLife += pTimer->GetDeltaSecs();
	if (m_fCurrentLife > m_fLifeTime)
		m_bKillMe = mmtrue;
}

// Emitter particles scene post loading
//==========================================================================================
mmVoid clsBKParticleEmitter::OnPostLoading()
{
	if (m_sSourceSceneName.IsEmpty())
		return;
	
	if (MHMFAIL(LoadMJ(m_sSourceSceneName)))
		g_plog->Out("Particle emitter: failed to load source scene [" + m_sSourceSceneName + "]", enLCYellow);
}

// Start: starts paticles emit
//==========================================================================================
mmVoid clsBKParticleEmitter::Start()
{
	for (mmUint i = 0; i < m_arParticles.Size(); i++)
	{
		clsEntity* pChilds = m_arParticles[i];
		pChilds->m_bNeedUpdateEvent  = mmtrue;
		pChilds->m_bRenderable       = mmtrue;
		pChilds->m_bEnableSphereTest = mmtrue;
		clsBKParticle* pParticle = (clsBKParticle*)pChilds;
		// calculate speed vector
		pParticle->m_vSpeed = pParticle->m_mxFull.GetTranslation() - m_vPulsePoint;
		pParticle->m_vSpeed.NormalizeFast();
		pParticle->m_vSpeed *= m_fPulseForce * (1.0f - g_pEnvironment->m_cRand.frand(m_fSpeedVariance));
		pParticle->m_vGravity = m_vGravity;
		pParticle->m_cReflectPlane = m_cBouncingPlane;
		pParticle->m_fBouncing = m_fBouncing;
		pParticle->m_fLifeTime = m_fLifeTime;
		pParticle->m_vRotationAxis = clsVector3(m_fRotationSpeed * (1.0f - g_pEnvironment->m_cRand.frand(m_fRotationVariance)),
												m_fRotationSpeed * (1.0f - g_pEnvironment->m_cRand.frand(m_fRotationVariance)),
												m_fRotationSpeed * (1.0f - g_pEnvironment->m_cRand.frand(m_fRotationVariance)));
		pParticle->m_mxWorld = pParticle->m_mxFull;
		pParticle->SetOwner(FindRoot());
		pParticle->InitParticle();
	}
	m_bNeedUpdateEvent = mmtrue;
}


// SetSpeedVector: sets particles speed vector
//==========================================================================================
mmVoid clsBKParticleEmitter::SetPulsePoint(const clsVector3 &vPoint)
{
	m_vPulsePoint = vPoint;
}

// SetSpeedVariance: sets speed variance
//==========================================================================================
mmVoid clsBKParticleEmitter::SetSpeedVariance(mmFloat fVary)
{
	m_fSpeedVariance = fVary;
}

// SetLifeTimer: sets particles lifr time
//==========================================================================================
mmVoid clsBKParticleEmitter::SetLifeTime(mmFloat fLifeTime)
{
	m_fLifeTime = fLifeTime;
}

// SetBouncing: sets particles plane bouncing
//==========================================================================================
mmVoid clsBKParticleEmitter::SetBouncing(mmFloat fBouncing)
{
	m_fBouncing = fBouncing;
}




