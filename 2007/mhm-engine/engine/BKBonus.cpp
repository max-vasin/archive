//==========================================================================================
// BIOKRIZ bonus manager
//==========================================================================================
#include "mhmbase.h"
#include "BKBonus.h"
#include "BKBonusManager.h"

// Bonus constructor
//=======================================================
clsBKBonus::clsBKBonus(clsEntityFactory *pFactory, 
					   const clsString &sType,
					   const clsString &sName):
		    clsEntity(pFactory, sType, sName)
{
	m_eBonusType		 = enUnknown;
	m_iAppearanceFactor	 = 0;
	m_fBonusScale        = 1.0f;
	m_fAltitude			 = 0.0f;
	m_fRotationSpeed	 = 2.0f;
	m_fAmplitude		 = 0.0f;
	m_fAmplifySpeed		 = 0.0f;
	m_pBonusParticles    = mmnull;
	m_fCurrentAppearTime = 0.0f;
	m_fAppearTime		 = 0.0f;
	m_fDisappearTime	 = 0.0f;
	m_fCurrentLifeTime	 = 0.0f;
	m_fLifeTime			 = 0.0f;
	m_bNeedCollide		 = mmtrue;
	m_pPickupSound		 = mmnull;
	m_pBonusManager		 = mmnull;
}

// Bonus destructor
//=======================================================
clsBKBonus::~clsBKBonus()
{

}

// Bonus initializing
//=======================================================
mmVoid clsBKBonus::OnInitialize(mmBool bClone)
{
	if (!bClone)
	{
		m_bRenderable		= mmfalse;
		m_bNeedUpdateEvent  = mmfalse;
		m_pPickupSound      = m_pEFactory->GetEngine()->GetSample(m_sPickupSoundFile);
//		EnumChilds("SpriteParticles", this);
//		if (m_pBonusParticles)
//			m_pBonusParticles->Res = Renderer->GetResourceManager()->GetResource("bonus_aura.apd", enResBaseParticles);
		m_mxWorld.Identity();
	}
	else
	{
		m_bRenderable		= mmtrue;
		m_bNeedUpdateEvent  = mmtrue;
		m_vPosition		    = m_mxWorld.GetTranslation();
		m_vPosition.z       = m_fAltitude;
		m_fCurrentRot		= 0.0f;
		clsMatrix mxScale, mxTrans;
		mxScale.Scale(0.0f, 0.0f, 0.0f);
		mxTrans.Translation(m_vPosition);
		m_mxWorld =  mxScale * mxTrans;
		m_eStatus	  = enBonusAppear;
		m_fCurrentAppearTime = 0.0f;
//		EnumChilds("SpriteParticles", this);
//		if (m_pBonusParticles)
//		{
//			m_pBonusParticles->Res = Renderer->GetResourceManager()->GetResource("bonus_aura.apd", enResBaseParticles);
//			if (m_pBonusParticles->Res)
//				((clsBaseParticles*)m_pBonusParticles->Res)->ResetEmitter();
//		}
	}
	g_plog->Out(m_sBonusType + " bonus initialized, appearance factor = " + clsString(m_iAppearanceFactor), enLCWhite);
}

// Bonus cloning
//=======================================================
mmVoid clsBKBonus::OnClone(clsEntity* pInstance)
{
	clsBKBonus* pBonus = (clsBKBonus*)pInstance;
	m_iAppearanceFactor = pBonus->m_iAppearanceFactor;
	m_fBonusScale		= pBonus->m_fBonusScale;
	m_fAltitude			= pBonus->m_fAltitude;
	m_fRotationSpeed	= pBonus->m_fRotationSpeed;
	m_fAmplitude		= pBonus->m_fAmplitude;
	m_fAmplifySpeed	    = pBonus->m_fAmplifySpeed;
	m_fAppearTime		= pBonus->m_fAppearTime;
	m_fDisappearTime	= pBonus->m_fDisappearTime;
	m_fLifeTime			= pBonus->m_fLifeTime;
	m_pPickupSound	    = pBonus->m_pPickupSound;
//	EnumChilds("SpriteParticles", this);
//	if (m_pBonusParticles)
//		m_pBonusParticles->Res = pBonus->m_pBonusParticles->Res;

}


// Bonus properties loading
//=======================================================
mmVoid clsBKBonus::OnLoadProps(clsVarList* pVarList)
{
	clsVariable* pVar = pVarList->FindVariable("BonusType");
	if (pVar)
	{
		m_sBonusType = (mmString)*pVar;
		m_sBonusType.UpperCase();
		if (m_sBonusType == "ROCKETSTRIKE")
			m_eBonusType = enRocketStrike;
		
		clsINIFile ini("config\\game_params.ini");
		if (!ini.IsExist())
		{
			g_plog->Out("Bonus: parameters file not exist", enLCRed);
			return;
		}
		
		if (MHMFAIL(ini.ReadInt(m_sBonusType, "AppearanceFactor", &m_iAppearanceFactor)))
			g_plog->Out(m_sBonusType + " bonus: appearance factor config not founded", enLCRed);
		if (MHMFAIL(ini.ReadFloat(m_sBonusType, "Scale", &m_fBonusScale)))
			g_plog->Out(m_sBonusType + " bonus: scale config not founded", enLCRed);
		if (MHMFAIL(ini.ReadFloat(m_sBonusType, "Altitude", &m_fAltitude)))
			g_plog->Out(m_sBonusType + " bonus: altitude config not founded", enLCRed);
		if (MHMFAIL(ini.ReadFloat(m_sBonusType, "RotationSpeed", &m_fRotationSpeed)))
			g_plog->Out(m_sBonusType + " bonus: rotation config not founded", enLCRed);
		if (MHMFAIL(ini.ReadFloat(m_sBonusType, "Amplitude", &m_fAmplitude)))
			g_plog->Out(m_sBonusType + " bonus: amplitude config not founded", enLCRed);
		if (MHMFAIL(ini.ReadFloat(m_sBonusType, "AmplifySpeed", &m_fAmplifySpeed)))
			g_plog->Out(m_sBonusType + " bonus: amplify speed config not founded", enLCRed);
		if (MHMFAIL(ini.ReadFloat(m_sBonusType, "AppearTime", &m_fAppearTime)))
			g_plog->Out(m_sBonusType + " bonus: appear time config not founded", enLCRed);
		if (MHMFAIL(ini.ReadFloat(m_sBonusType, "DisappearTime", &m_fDisappearTime)))
			g_plog->Out(m_sBonusType + " bonus: disappear time config not founded", enLCRed);
		if (MHMFAIL(ini.ReadFloat(m_sBonusType, "LifeTime", &m_fLifeTime)))
			g_plog->Out(m_sBonusType + " bonus: life time config not founded", enLCRed);
		if (MHMFAIL(ini.ReadString(m_sBonusType, "PickupSound", m_sPickupSoundFile)))
			g_plog->Out(m_sBonusType + " bonus: pickup sound config not founded", enLCRed);
	}
	else
		g_plog->Out("Bonus: udefined type bonus founded", enLCYellow);
}

// Updating bonus
//=======================================================
mmVoid clsBKBonus::OnUpdate(const clsTimer *pTimer)
{
	clsVector3 vOffPos = m_vPosition;
	mmFloat    fScale  = 1.0f;
	switch (m_eStatus)
	{
	case enBonusAppear:
		m_fCurrentAppearTime += pTimer->GetDeltaSecs();
		if (m_fCurrentAppearTime > m_fAppearTime)
		{
			m_fCurrentAppearTime = m_fAppearTime;
			m_eStatus = enBonusStay;
		}
		mmFloat fK;
		if (m_fAppearTime == 0.0f)
			fK = 1.0f;
		else
			fK = m_fCurrentAppearTime / m_fAppearTime;
		fScale = m_fBonusScale * fK;
		vOffPos.z = m_vPosition.z * fK;
		if (m_eStatus != enBonusAppear)
			m_fCurrentAppearTime = m_fDisappearTime;
		break;
	
	case enBonusStay:
		m_fCurrentLifeTime += pTimer->GetDeltaSecs();
		if (m_fCurrentLifeTime > m_fLifeTime)
		{
			m_fCurrentLifeTime	 = m_fLifeTime;
			m_eStatus			 = enBonusDisappear;
		}
		fScale = m_fBonusScale;
		vOffPos.z = sinf(m_fCurrentRot * m_fAmplifySpeed) * m_fAmplitude + m_vPosition.z;
		break;
	
	case enBonusDisappear:
		m_fCurrentAppearTime -= pTimer->GetDeltaSecs();
		if (m_fCurrentAppearTime < 0.0f)
		{
			m_fCurrentAppearTime = 0.0f;
			KillBonus();
		}
		if (m_fDisappearTime == 0.0f)
			fK = 0.0f;
		else
			fK = m_fCurrentAppearTime / m_fDisappearTime;
		fScale = m_fBonusScale * fK;
		vOffPos.z = m_vPosition.z * fK;
		break;
	}
	clsMatrix  mxRot, mxScale, mxTrans;
	m_fCurrentRot += pTimer->GetDeltaSecs() * m_fRotationSpeed;
	mxRot.RotationZ(m_fCurrentRot);
	mxScale.Scale(fScale, fScale, fScale);
	mxTrans.Translation(vOffPos);
	m_mxWorld = mxScale * mxRot * mxTrans;
}

// Analyzing bonus structure
//=======================================================
mmVoid clsBKBonus::OnEnumChild(clsEntity *pEntity)
{
	m_pBonusParticles = pEntity;
}

// GetType: returns bonus type
//=======================================================
clsBKBonus::enBonusType clsBKBonus::GetType()
{
	return m_eBonusType;
}

// GetAppearanceFactor: returns bonus randomize factor
//=======================================================
mmInt clsBKBonus::GetAppearanceFactor()
{
	return m_iAppearanceFactor;
}

// PickUp: entity pickups this bonus
//=======================================================
mmVoid clsBKBonus::PickUp(clsEntity *pEntity)
{
	m_eStatus      = enBonusDisappear;
	m_bNeedCollide = mmfalse;
	if (m_pPickupSound)
		m_pPickupSound->Play((mmDword)this, 100);
	
	if (m_pBonusManager)
		((clsBKBonusManager*)m_pBonusManager)->AddPoints(50);
}

// KillBonus
//=======================================================
mmVoid clsBKBonus::KillBonus()
{
	m_bKillMe = mmtrue;
}
