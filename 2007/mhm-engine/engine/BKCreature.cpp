//==========================================================================================
// BIOKRIZ creatures base class
//==========================================================================================
#include "mhmbase.h"
#include "BKDefs.h"
#include "BKHero.h"
#include "BKParticleEmitter.h"
#include "BKCreature.h"
#include "BKCreatureInstance.h"
#include "BKSpriteParticles.h"
#include "BKBonusManager.h"


// Creature constructor
//==========================================================================================
clsBKCreature::clsBKCreature(clsEntityFactory *pFactory, 
							 const clsString &sType,
							 const clsString &sName):
			   clsEntity(pFactory, sType, sName)
{
	m_bNeedUpdateEvent		  = mmtrue;
	m_bNeedLoad				  = mmtrue;
	m_bDisableHiddenHierarchy = mmtrue;
	m_bNeedCollide			  = mmtrue;
	m_bNeedSpeedCalc		  = mmtrue;
	m_bBlockRender			  = mmtrue;
	
	m_nLife			   = 0;
	m_eStatus		   = enWait;
	m_fCurrentLifeTime = 0.0f;
	m_fScale		   = 0.0f;
	m_fAltitude		   = 0.0f;
	m_fSpeed		   = 0.0f;
	
	m_pCreatureMesh    = mmnull;
	m_pBloodParticles  = mmnull;
	m_pHitScream	   = mmnull;
	m_pBonusManager    = mmnull;
	m_bIntstance	   = mmfalse;
	m_sSuperType	   = "CREATURE";
}

// Creature destructor
//==========================================================================================
clsBKCreature::~clsBKCreature()
{
	if (m_bIntstance)
		mmSafeDelete(m_pHitScream);
}

// Creature initialization
//==========================================================================================
mmVoid clsBKCreature::OnInitialize(mmBool bClone)
{
	EnumChilds("", this);
	if (m_pCreatureMesh == mmnull)
		g_plog->Out("Creature [" + Name + "] no creature mesh subnode founded", enLCRed);
	else
	{
		m_cBoundSphere = m_pCreatureMesh->m_cBoundSphere;
		m_eBoundType   = m_pCreatureMesh->m_eBoundType;
		m_cBoundBox    = m_pCreatureMesh->m_cBoundBox;
		//m_pCreatureMesh->m_bEnableSphereTest = mmfalse;
		//m_pCreatureMesh->m_bNeedCollide      = mmfalse;
	}
	
	if (m_pBloodParticles == mmnull)
		g_plog->Out("Creature [" + Name + "] no creature blood particles subnode founded", enLCRed);
	else
	{
		m_pBloodParticles->Res = Renderer->GetResourceManager()->GetResource("blood_hit.apd", enResBaseParticles);
		m_pBloodParticles->m_bRenderable = mmfalse;
	}
	
	if (bClone)
	{
		clsMatrix mxScale, mxTrans;
		m_vPosition = m_mxWorld.GetTranslation();
		mxScale.Scale(m_fScale, m_fScale, m_fScale);
		mxTrans.Translation(m_vPosition);
		m_mxWorld = mxScale * mxTrans;
		clsEntity* pHero = FindRoot()->FindChild("Hero");
		pHero->LinkUpdateDepency(this);
		m_vHeroPosition = pHero->m_mxWorld.GetTranslation();
		m_pBloodParticles->SetOwner(FindRoot());
	}
	else
	{
		m_bIntstance = mmtrue;
		FindRoot()->EnumChilds("BonusManager", this);
	}
	InitCreature(bClone);
}

// Creature loading
//==========================================================================================
mmVoid clsBKCreature::OnPostLoading()
{
	clsINIFile ini(BK_GAME_PARAMS_INI_NAME);
	if (!ini.IsExist())
	{
		g_plog->Out("Creature [" + Name + "] no game config file founded", enLCRed);
		return;
	}
	
	clsString sFile;
	if (MHMFAIL(ini.ReadString(m_sType, "SourceScene", sFile)))
	{
		g_plog->Out("Creature [" + Name + "] source scene config key not founded", enLCRed);
		return;
	}
	if (MHMFAIL(ini.ReadFloat(m_sType, "Scale", &m_fScale)))
	{
		g_plog->Out("Creature [" + Name + "] scale config key not founded", enLCRed);
		m_fScale = 1.0f;
		return;
	}
	if (MHMFAIL(ini.ReadFloat(m_sType, "Altitude", &m_fAltitude)))
	{
		g_plog->Out("Creature [" + Name + "] altitude config key not founded", enLCRed);
		m_fScale = 1.0f;
		return;
	}
	if (MHMFAIL(ini.ReadFloat(m_sType, "Speed", &m_fSpeed)))
	{
		g_plog->Out("Creature [" + Name + "] speed config key not founded", enLCRed);
		m_fScale = 1.0f;
		return;
	}
	LoadCreature(sFile);
}

// Creature cloning
//==========================================================================================
mmVoid clsBKCreature::OnClone(clsEntity *pInstance)
{
	clsBKCreature* pCreature = (clsBKCreature*)pInstance;
	// copy creature basic parameters
	m_nLife			= pCreature->m_nLife;
	m_fScale		= pCreature->m_fScale;
	m_pHitScream	= pCreature->m_pHitScream;
	m_fAltitude		= pCreature->m_fAltitude;
	m_fSpeed		= pCreature->m_fSpeed;
	m_pBonusManager = pCreature->m_pBonusManager;
	
	CloneCreature(pInstance);
}

// Creature structure analyzing
//==========================================================================================
mmVoid clsBKCreature::OnEnumChild(clsEntity *pEntity)
{
	clsString sUType = pEntity->m_sType;
	sUType.UpperCase();
	if (sUType == "CREATUREMESH")
	{
		if (m_pCreatureMesh != mmnull)
			g_plog->Out("Creature [" + Name + "] duplicate creature mesh subnode founded", enLCYellow);
		m_pCreatureMesh = pEntity;
	}
	if (sUType == "BLOODPARTICLES")
	{
		if (m_pBloodParticles != mmnull)
			g_plog->Out("Creature [" + Name + "] duplicate blood particles subnode founded", enLCYellow);
		m_pBloodParticles = pEntity;
	}
	if (sUType == "BONUSMANAGER")
	{
		if (m_pBonusManager != mmnull)
			g_plog->Out("Creature [" + Name + "] duplicate bonus managers founded", enLCYellow);		
		else
			g_plog->Out("Creature: bonus manager founded", enLCBlue);
		m_pBonusManager = pEntity;
	}
	
	OnAnalyzeStructure(pEntity);
}

// Creature hero position tracker
//==========================================================================================
mmVoid clsBKCreature::OnDepencyUpdate(clsEntity *pEntity, mmBool bMeDead)
{
	if (pEntity->m_sType == "Hero")
	{
		clsVector3 vNewPosition = pEntity->m_mxFull.GetTranslation();
		if (m_vHeroPosition != vNewPosition)
		{
			OnHeroMove(vNewPosition);
			m_vHeroPosition = vNewPosition;
		}
	}
}

// Creature updating
//==========================================================================================
mmVoid clsBKCreature::OnUpdate(const clsTimer *pTimer)
{
	m_fCurrentLifeTime += pTimer->GetDeltaSecs();
	if (m_eStatus == enMove)
	{
		m_vPosition += m_vSpeed;
		m_vSpeed.NormalizeFast();
		clsMatrix mxScale, mxRot, mxPos;
		mxPos.Translation(m_vPosition);
		mxRot.RotationVectorZ(m_vSpeed);
		mxScale.Scale(m_fScale, m_fScale, m_fScale);
		m_mxWorld = mxScale * mxRot * mxPos;
	}
	if (m_eStatus == enDeath)
	{
		m_vPosition += m_vSpeed;
		m_vSpeed.NormalizeFast();
		m_vSpeed.Negate();
		clsMatrix mxScale, mxRot, mxPos;
		mxPos.Translation(m_vPosition);
		mxRot.RotationVectorZ(m_vSpeed);
		mxScale.Scale(m_fScale, m_fScale, m_fScale);
		m_mxWorld = mxScale * mxRot * mxPos;
	}
	UpdateCreature(pTimer);
	
}

// Creature speed calculations
//==========================================================================================
mmVoid clsBKCreature::OnCalcSpeed(const clsTimer *pTimer)
{
	CalcSpeed(pTimer);
}

// Creature collides
//==========================================================================================
mmVoid clsBKCreature::OnCollide(clsEntity* pEntity, mmFloat fFirstTime, mmFloat fSecondTime)
{

}


// Projectile damage applying (pistols, rifles, miniguns)
//==========================================================================================
mmVoid clsBKCreature::ApplyProjectileDamage(mmInt nAmount, 
											const clsVector3 &vHitPoint,
											const clsRay &HitRay)
{
	mmInt		_nAmount   = nAmount;
	clsVector3	_vHitPoint = vHitPoint;
	clsRay		_HitRay	   = HitRay;
	OnApplyProjectileDamage(_nAmount, _vHitPoint, _HitRay);
	// make some blood here
	((clsBaseParticles*)m_pBloodParticles->Res)->ResetEmitter();
	clsMatrix mxTrans, mxRot, mxRotDir;
	mxTrans.Translation(m_mxFull.GetTranslation());
	mxRot.RotationY(-MHM_PI / 1.5f);
	_HitRay.m_vDirection.NormalizeFast();
	mxRotDir.RotationVectorZ(_HitRay.m_vDirection);
	m_pBloodParticles->m_mxWorld = mxRot * mxRotDir * mxTrans;
	m_pBloodParticles->m_bRenderable = mmtrue;
	ApplyDamage(_nAmount);
}

// Explode damage applying (rockets, hand grenades)
//==========================================================================================
mmVoid clsBKCreature::ApplyExplodeDamage(mmInt nAmount,
								  		 const clsVector3& vExplodePoint,
										 mmFloat fFalloff)
{
	mmInt		_nAmount	   = nAmount;
	clsVector3	_vExplodePoint = vExplodePoint;
	mmFloat		_fFalloff	   = fFalloff;
	OnApplyExplodeDamage(_nAmount, _vExplodePoint, _fFalloff);
	ApplyDamage(_nAmount);
}

// Constant damage applying (poison, radiation)
//==========================================================================================
mmVoid clsBKCreature::ApplyConstantDamage(mmInt nAmount,
										  mmFloat fDamageTime)
{
	mmInt		_nAmount	   = nAmount;
	mmFloat		_fDamageTime   = fDamageTime;
	OnApplyConstantDamage(_nAmount, _fDamageTime);
	ApplyDamage(_nAmount);
}




//==========================================================================================
// Creature iternal block
//==========================================================================================
mmVoid clsBKCreature::ApplyDamage(mmInt nAmount)
{
	m_nLife -= nAmount;
	if (m_nLife < 0)
	{
		if (m_eStatus != enDeath)
		{
			m_vDeathHeroPosition = m_vHeroPosition;
			OnDeath();
		}
		
	}
	if (m_pHitScream)
		m_pHitScream->Play((mmDword)this, 90);
}
mmVoid clsBKCreature::KillCreature()
{
	m_bKillMe = mmtrue;
	if (m_pBloodParticles)
		((clsBKSpriteParticles*)m_pBloodParticles)->SetDeathCounter(2.0f);
	if (m_pBonusManager)
		((clsBKBonusManager*)m_pBonusManager)->RegisterDeath(this);
}






