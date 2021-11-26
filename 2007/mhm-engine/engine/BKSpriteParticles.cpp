//==========================================================================================
// BIOKRIZ particles entity
//==========================================================================================
#include "mhmbase.h"
#include "BKSpriteParticles.h"

// Particles constructor
//================================================
clsBKSpriteParticles::clsBKSpriteParticles(clsEntityFactory* pFactory,
							  const clsString& sType,
							  const clsString& sName):
				clsEntity(pFactory, sType, sName)
{
	m_bDeathCounter = mmfalse;
	m_fDeathTime    = 0.0f;
	m_fCurTime      = 0.0f;
}

// Particles destructor
//================================================
clsBKSpriteParticles::~clsBKSpriteParticles()
{

}

// Particles update
//================================================
mmVoid clsBKSpriteParticles::OnUpdate(const clsTimer* pTimer)
{
	if (m_bDeathCounter)
	{
		m_fCurTime += pTimer->GetDeltaSecs();
		if (m_fCurTime > m_fDeathTime)
			m_bKillMe = mmtrue;
	}
}

// Particles death counter set
//================================================
mmVoid clsBKSpriteParticles::SetDeathCounter(mmFloat fSecs)
{
	m_bDeathCounter = mmtrue;
	m_fDeathTime    = fSecs;
	m_bNeedUpdateEvent = mmtrue;
}


