//==========================================================================================
// BIOKRIZ all particles definitions
//==========================================================================================
#include "mhmbase.h"
#include "BKParticle.h"
#include "BKParticleEmitter.h"
#include "BKAllParticles.h"

//==========================================================================================
// Monsters meat particles
//==========================================================================================
clsBKMeatParticleEmitter::clsBKMeatParticleEmitter(clsEntityFactory *pFactory,
												   const clsString &sType,
												   const clsString &sName):
						  clsBKParticleEmitter(pFactory, sType, sName)
{
	m_bNeedLoad = mmtrue;
}
clsBKMeatParticleEmitter::~clsBKMeatParticleEmitter()
{
}
mmVoid clsBKMeatParticleEmitter::OnPostLoading()
{
	LoadMJ("Meshes\\meat_particles.mj");
}

