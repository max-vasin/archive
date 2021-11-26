//==========================================================================================
// BIOKRIZ creature emitter instances class
//==========================================================================================
#include "mhmbase.h"
#include "BKCreatureInstance.h"

// Instance constructor
//==========================================================================================
clsBKCreatureInstance::clsBKCreatureInstance(clsEntityFactory *pFactory,
											 const clsString &sType,
											 const clsString &sName):
					   clsEntity(pFactory, sName, sType)
{
	m_bRenderable = mmfalse;
	m_bNeedLoad   = mmtrue;
}


// Instance destructor
//==========================================================================================
clsBKCreatureInstance::~clsBKCreatureInstance()
{

}

// Instance initialization
//==========================================================================================
mmVoid clsBKCreatureInstance::OnInitialize(mmBool bClone)
{
	//m_pHitScream = m_pEFactory->GetEngine()->GetSample("Sounds\\flyer_hit_scream.ogg");
}

mmVoid clsBKCreatureInstance::OnPostLoading()
{
/*	LoadMJ("Meshes\\flyer.mj");
	clsEntity* pSkin = FindChild("MonsterSkin");
	m_cBoundSphere = pSkin->m_cBoundSphere;
	m_cBoundBox    = pSkin->m_cBoundBox;
	m_eBoundType   = enBoundSphere;
	LoadAnimationKeys("flyer.mhmAnim");
	//Res = Renderer->GetResourceManager()->GetResource(pSkin->Res);
	//pSkin->m_bKillMe = mmtrue;*/
}

