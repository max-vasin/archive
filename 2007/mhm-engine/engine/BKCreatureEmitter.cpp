//==========================================================================================
// BIOKRIZ creature emitter base class
//==========================================================================================
#include "mhmbase.h"
#include "BKCreatureEmitter.h"

// Emitter constructor
//==========================================================================================
clsBKCreatureEmitter::clsBKCreatureEmitter(clsEntityFactory *pFactory,
										   const clsString &sType,
										   const clsString &sName):
					  clsEntity(pFactory, sType, sName)
{
	m_bRenderable	   = mmfalse;	// disable emitter rendering
	m_bNeedUpdateEvent = mmtrue;	// enable OnUpdate event generation
	m_fEmitTime		   = 10.0f;
	m_fCurrentEmitTime = 0.0f;
	m_nEmitCounter	   = 0;
	
	m_pSucker          = mmnull;
	m_pFlyer		   = mmnull;
	m_pOrc			   = mmnull;
	m_pBug			   = mmnull;
}

// Emitter destructor
//==========================================================================================
clsBKCreatureEmitter::~clsBKCreatureEmitter()
{

}

// Emitter initialization
//==========================================================================================
mmVoid clsBKCreatureEmitter::OnInitialize(mmBool bClone)
{
	m_pFlyer  = (clsBKCreature*)AddChildNode("Flyer", "Flyer", mmtrue, mmtrue);
	m_pSucker = (clsBKCreature*)AddChildNode("Sucker", "Sucker", mmtrue, mmtrue);
	m_pOrc    = (clsBKCreature*)AddChildNode("Orc", "Orc", mmtrue, mmtrue);
	m_pBug    = (clsBKCreature*)AddChildNode("Bug", "Bug", mmtrue, mmtrue);
	
	m_pFlyer->SetOwner(mmnull);
	m_pSucker->SetOwner(mmnull);
	m_pOrc->SetOwner(mmnull);
	m_pBug->SetOwner(mmnull);
}

// Emitter update
//==========================================================================================
mmVoid clsBKCreatureEmitter::OnUpdate(const clsTimer *pTimer)
{
	m_fCurrentEmitTime += pTimer->GetDeltaSecs();
	if (m_fCurrentEmitTime >= m_fEmitTime)
	{
		m_fCurrentEmitTime = 0.0f;
		m_fEmitTime -= 0.5f;
		if (m_fEmitTime < 5.0f)
			m_fEmitTime = 5.0f;
		EmitCreature();
	}	
}

// EmitCreature: emits new creature
//==========================================================================================
mmVoid clsBKCreatureEmitter::EmitCreature()
{
//	static mmInt nCount = 0;
//	if (nCount < 8)
//	{
	clsBKCreature* pClone;
	mmFloat fRand = g_pEnvironment->m_cRand.frand(1.0f);
	if   (fRand <= 0.25f)
	{
		pClone = (clsBKCreature*)m_pFlyer->Clone("Flyer", mmtrue, &m_mxFull);
	}
	else if ((fRand > 0.25f) && (fRand <= 0.5f))
	{
		pClone = (clsBKCreature*)m_pSucker->Clone("Sucker", mmtrue, &m_mxFull);	
	}
	else if ((fRand > 0.5f) && (fRand <= 0.75f))
	{
		pClone = (clsBKCreature*)m_pOrc->Clone("Orc", mmtrue, &m_mxFull);	
	}
	else if (fRand > 0.75f)
	{
		pClone = (clsBKCreature*)m_pBug->Clone("Bug", mmtrue, &m_mxFull);	
	}
		
	//FindRoot()->DumpHierarchy(" ");
//	nCount++;
//	}
	pClone->SetOwner(FindRoot());
	pClone->ProcessInitialize(mmtrue);
	pClone->m_bDrawBoundSphere = m_bDrawBoundSphere;
	
	m_nEmitCounter++;
}



