//==========================================================================================
// BIOKRIZ hero entity
//==========================================================================================
#include "mhmbase.h"
#include "BKHero.h"
#include "BKCrossHair.h"
#include "BKCreature.h"
#include "BKWeapon.h"
#include "BKBonus.h"

//==========================================================================
// Hero parameters key names
//==========================================================================
const mmString c_sHeroSection		= "Hero";
const mmString c_sHeroSpeedRunKey	= "SpeedRun";
const mmString c_sHeroSpeedBackKey	= "SpeedBack";
const mmString c_sHeroSpeedRightKey	= "SpeedRight";
const mmString c_sHeroSpeedLeftKey	= "SpeedLeft";

// Hero constructor
//==========================================================================================
clsBKHero::clsBKHero(clsEntityFactory *pFactory, 
					 const clsString &type,
					 const clsString &name):
		   clsEntity(pFactory, type, name),
		   m_vDirection(0.0f, 0.0f, 0.0f),
		   m_vLookDir(0.0f, 0.0f, 0.0f)
{
	m_bNeedBufferedKeyboardEvent = mmtrue;
	m_bNeedBufferedMouseEvent	 = mmtrue;
	m_bNeedUpdateEvent			 = mmtrue;
	m_bNeedLoad					 = mmtrue;
	m_bMoveLeft			= mmfalse;
	m_bMoveRight		= mmfalse;
	m_bMoveForward		= mmfalse;
	m_bMoveBackward		= mmfalse;
	m_fSpeed			= 90.0f;
	
	
	m_vBoundMin.Set(0.0f);
	m_vBoundMax.Set(0.0f);
	m_bNeedCollide		= mmtrue;
	m_bNeedSpeedCalc	= mmtrue;
	m_mxHalfSector.RotationZ(MHM_PI / 4.0f);
	m_bRenderable		= mmtrue;
	//m_bFinishUpdate		= mmtrue;
	m_pWeaponScene      = mmfalse;
	m_pRightGunfire     = mmnull;
	m_pLeftGunfire		= mmnull;
	m_bLButtonPressed   = mmfalse;
	mmZeroVar(m_stParams);
	
}


// Hero destructor
//==========================================================================================
clsBKHero::~clsBKHero()
{
	mmSafeDelete(m_pWeaponScene);
}

// Hero initializtion
//==========================================================================================
mmVoid clsBKHero::OnInitialize(mmBool bClone)
{
	LoadParamBlock();

	clsEntity* pEntity = FindRoot()->FindChild("CrossHair");
	pEntity->LinkUpdateDepency(this);
//	m_pHelper = (clsText*)m_pEFactory->GetEngine()->GetRenderer()->GetResourceManager()->GetResource("myfont.mhmFont2D", MHMRES_MISC_ID);
//	m_pHelper->SetPos(0.0f, 200.0f);
//	m_pHelper->SetColor(0xFF0000FF);
	m_pGroundMesh = (clsBaseMesh*)FindRoot()->FindChild("Ground")->Res;
	
	clsEntity* pBoundMin = FindRoot()->FindChild("BoundCheckerMin");
	clsEntity* pBoundMax = FindRoot()->FindChild("BoundCheckerMax");
	if ((pBoundMin == mmnull) || (pBoundMax == mmnull))
		g_plog->Out("Hero: one of bounds checkers not founded!", enLCRed);
	else
	{
		m_vBoundMin = pBoundMin->m_mxWorld.GetTranslation();
		m_vBoundMax	= pBoundMax->m_mxWorld.GetTranslation();
	}
	
	pEntity = FindChild("HeroMesh");
	m_cBoundSphere = pEntity->m_cBoundSphere;
	m_cBoundBox    = pEntity->m_cBoundBox;
	m_eBoundType   = pEntity->m_eBoundType;
	m_bEnableSphereTest = mmtrue;
	g_plog->Out("Hero entity created", enLCBlue);
	LoadAnimationKeys("hero.mhmAnim");
	AnimationFrame(0, 0);
	AnimationRangeStop(mmtrue);
	PlayAnimationRange("stay_2pistols", enAnimPingPong);
	m_eAnimState		= enHeroAnimStay;
	// weapons placeholders
	m_pLeftHolder = FindChild("LeftWeaponHolder");
	m_pRightHolder = FindChild("RightWeaponHolder");
	//weapons
	m_pWeaponScene->SetOwner(mmnull);
	m_pWeaponScene->EnumChilds("Weapon", this);
//	m_pFirstPistol = m_pWeaponScene->FindChild("Pistol");
	//m_pFirstPistol->m_mxWorld.Identity();
	m_pSecondPistol = m_pPistol->Clone("SecondPistol", mmtrue);
	//clsEntity* pWeapon = m_pFirstPistol->FindChild("WeaponMesh");
	//m_pFirstPistol = pWeapon->Clone("FirstPistol", mmtrue);
	//m_pSecondPistol = pWeapon->Clone("SeconPistol", mmtrue);
	m_eState = enHeroFree;
	
	
	m_eWeapon			= enHeroWeaponTwoPistols;
	SelectWeapon();
}

// Hero post loading
//==========================================================================================
mmVoid clsBKHero::OnPostLoading()
{
	LoadMJ("Meshes\\hero.mj");
	m_pWeaponScene = AddChildNode("Scene", "Weapons");
	m_pWeaponScene->LoadMJ("Meshes\\weapons.mj");

	
	
}



// Helper rendering
//==========================================================================================
mmRes clsBKHero::OnRender()
{
	return mmOk;
}

// Hero keyboard moving
//==========================================================================================
mmVoid clsBKHero::OnBufferedKeyboardEvent(const clsKeyState* pEvent, mmBool bFirst)
{
	if (pEvent->m_code == enKeyW)
		m_bMoveForward = pEvent->m_pressed;
	
	if (pEvent->m_code == enKeyS)
		m_bMoveBackward = pEvent->m_pressed;
		
	if (pEvent->m_code == enKeyA)
		m_bMoveLeft = pEvent->m_pressed;
		
	if (pEvent->m_code == enKeyD)
		m_bMoveRight = pEvent->m_pressed;
	
}

// Process mouse events
//==========================================================================================
mmVoid clsBKHero::OnBufferedMouseEvent(const clsMouseEvent *pEvent, mmBool bFirst)
{
	if (pEvent->m_buttons[enMouseButton0] == enMouseButtonDown)
		m_bLButtonPressed = mmtrue;
	if (pEvent->m_buttons[enMouseButton0] == enMouseButtonUp)
		m_bLButtonPressed = mmfalse;

}

// Childrens cross
//==========================================================================================
mmVoid clsBKHero::OnEnumChild(clsEntity *pEntity)
{
	clsBKWeapon* pWeapon = (clsBKWeapon*)pEntity;
	if (pWeapon->GetType() == clsBKWeapon::enPistol)
	{
		m_pPistol = pEntity;	
	}
}



// Fix direction
//==========================================================================================
mmVoid clsBKHero::OnEndBufferedEvents(mmUint nNumMouseEvents, mmUint nNumKeyboardEvents)
{
	if (nNumKeyboardEvents == 0)
		return;
	m_vDirection.Set(0.0f);
	if (m_bMoveLeft)
		m_vDirection.x -= 1.0f;
	if (m_bMoveRight)
		m_vDirection.x += 1.0f;
	if (m_bMoveForward)
		m_vDirection.y += 1.0f;
	if (m_bMoveBackward)
		m_vDirection.y -= 1.0f;
	
	
	m_vDirection.NormalizeFast();
}

// OnCalcSpeed
//==========================================================================================
mmVoid clsBKHero::OnCalcSpeed(const clsTimer* pTimer)
{
	m_vSpeed = m_vDirection * m_fSpeed * pTimer->GetDeltaSecs();
	clsVector3 vPosition = m_mxFull.GetTranslation() + m_vSpeed;
	if ((vPosition.x < m_vBoundMin.x) || (vPosition.x > m_vBoundMax.x))
		m_vSpeed.x = 0.0f;
	if ((vPosition.y < m_vBoundMin.y) || (vPosition.y > m_vBoundMax.y))
		m_vSpeed.y = 0.0f;
}


// Update hero
//==========================================================================================
mmVoid clsBKHero::OnUpdate(const clsTimer* pTimer)
{

	// check velocity vector and hero rotation
	if ((m_vDirection.AlmostZero()) && (m_eAnimState != enHeroAnimStay))
	{
		SetAnimationPos("s_stay_2pistols");
		PlayAnimationRange("stay_2pistols", enAnimPingPong);
		m_eAnimState = enHeroAnimStay;
	}
	else
	{
		mmFloat fCosSector = m_vDirection.Dot(m_vLookDir);
		mmFloat fZDir = (m_vDirection * m_vLookDir).z;
		if ((fCosSector > 0.7f))
		{
			if (m_eAnimState != enHeroAnimRun)
			{
				if (m_eAnimState == enHeroAnimStay)
					SetAnimationPos("s_run_2pistols");
				m_eAnimState = enHeroAnimRun;
				PlayAnimationRange("run_2pistols", enAnimLoop);
				m_fSpeed = m_stParams.m_fHeroSpeedRun;
			}
		}
		if ((fCosSector < -0.7f))
		{
			if (m_eAnimState != enHeroAnimBack)
			{
				if (m_eAnimState == enHeroAnimStay)
					SetAnimationPos("s_back_2pistols");
				m_eAnimState = enHeroAnimBack;
				PlayAnimationRange("back_2pistols", enAnimLoop);
				m_fSpeed = m_stParams.m_fHeroSpeedBack;
			}
		}
		if ((fCosSector > -0.7f) && (fCosSector < 0.7f) && (fCosSector != 0.0f))
		{
			if (fZDir < 0.0f)
			{
				if (m_eAnimState != enHeroAnimLeft)
				{
					if (m_eAnimState == enHeroAnimStay)
						SetAnimationPos("s_left_2pistols");
					m_eAnimState = enHeroAnimLeft;
					PlayAnimationRange("left_2pistols", enAnimLoop);
					m_fSpeed = m_stParams.m_fHeroSpeedLeft;
				}
			}
			else
			{
				if (m_eAnimState != enHeroAnimRight)
				{
					if (m_eAnimState == enHeroAnimStay)
						SetAnimationPos("s_right_2pistols");
					m_eAnimState = enHeroAnimRight;
					PlayAnimationRange("right_2pistols", enAnimLoop);
					m_fSpeed = m_stParams.m_fHeroSpeedRight;
				}
			}
		}
	}
	clsBKWeapon* m_pWeapon = (clsBKWeapon*)m_pPistol;
	if (m_pWeapon && m_bLButtonPressed)
	{
		if (m_pWeapon->GetState() == clsBKWeapon::enReady)
		{
			m_pWeapon->SetState(clsBKWeapon::enFiring);
		}
		if (m_pWeapon->GetState() == clsBKWeapon::enWaiting)
		{
			m_pWeapon = (clsBKWeapon*)m_pSecondPistol;
			if (m_pWeapon)
			{
				if (m_pWeapon->GetState() == clsBKWeapon::enReady)
					m_pWeapon->SetState(clsBKWeapon::enFiring);
			}
		}
	}
	
	m_mxWorld.SetTranslation(m_mxWorld.GetTranslation() + m_vSpeed);
}

// Depencies updating
//==========================================================================================
mmVoid clsBKHero::OnDepencyUpdate(clsEntity* pEntity, mmBool bMeDead)
{
	clsBKCrosshair* pCrosshair = (clsBKCrosshair*)pEntity;
	clsRay ray = pCrosshair->GetRay();
	clsPlane plane;
	clsVector3 vPosition = m_mxWorld.GetTranslation();
	plane.FromPointNormal(vPosition, clsVector3(0.0f, 0.0f, 1.0f));
	clsVector3 vIntersect;
	//mmBool bisect = plane.IntersectLine(vIntersect, ray.m_vOrigin, ray.m_vDirection * 10000.0f);
	mmBool bisect = plane.IntersectRay(vIntersect, ray);
	clsBKWeapon* pWeapon = (clsBKWeapon*)m_pPistol;
	pWeapon->SetTarget(vIntersect);
	pWeapon = (clsBKWeapon*)m_pSecondPistol;
	pWeapon->SetTarget(vIntersect);
	
	m_vLookDir = vIntersect - vPosition;
	m_vLookDir.z = 0.0f;
	m_vLookDir.Normalize();
	m_mxLocal.RotationVectorZ(m_vLookDir);
}

mmVoid clsBKHero::OnCollide(clsEntity* pEntity, mmFloat fFirstTime, mmFloat fSecondTime)
{
	if (fFirstTime < 1.0f)
	{
		if (pEntity->m_sType == "Bonus")
		{
			clsBKBonus* pBonus = (clsBKBonus*)pEntity;
			pBonus->PickUp(this);
		}
	}
}

mmVoid clsBKHero::SelectWeapon()
{
	switch(m_eWeapon)
	{
	case enHeroWeaponPistol:
		break;
	case enHeroWeaponTwoPistols:
		m_pPistol->SetOwner(m_pRightHolder);
		m_pSecondPistol->SetOwner(m_pLeftHolder);
		m_pRightGunfire = m_pPistol->FindChild("Gunfire");
		m_pLeftGunfire = m_pSecondPistol->FindChild("Gunfire");
		break;
	case enHeroWeaponMinigun:
		break;
	case enHeroWeaponRocketLauncher:
		break;
	}
/*	if (m_pRightGunfire)
		m_pRightGunfire->m_bRenderable = mmfalse;
	if (m_pLeftGunfire)
		m_pLeftGunfire->m_bRenderable = mmfalse;*/
		
}

mmVoid clsBKHero::SelectState(enHeroState eState)
{
	if (eState == m_eState)
		return;
	switch(eState)
	{
	case enHeroFree:
		break;
	case enHeroFiring:
		break;
	case enHeroReloading:
		break;
	}
}

// Hero parameters block loading
//==================================================================================
mmVoid clsBKHero::LoadParamBlock()
{
	clsINIFile	ini("config\\game_params.ini");
	mmRes Res = 0;
	Res += ini.ReadFloat(c_sHeroSection,c_sHeroSpeedRunKey,&m_stParams.m_fHeroSpeedRun);
	Res += ini.ReadFloat(c_sHeroSection,c_sHeroSpeedBackKey,&m_stParams.m_fHeroSpeedBack);
	Res += ini.ReadFloat(c_sHeroSection,c_sHeroSpeedLeftKey,&m_stParams.m_fHeroSpeedLeft);
	Res += ini.ReadFloat(c_sHeroSection,c_sHeroSpeedRightKey,&m_stParams.m_fHeroSpeedRight);
		
	if (MHMFAIL(Res))
		g_plog->Out("Hero: parameters block loading error", enLCRed);
}