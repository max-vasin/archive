//==========================================================================================
// BIOKRIZ crosshair
//==========================================================================================
#include "mhmbase.h"
#include "BKCrossHair.h"
#include "BKCreature.h"

// Constructor
//==========================================================================================
clsBKCrosshair::clsBKCrosshair(clsEntityFactory* pFactory,
							   const clsString& type,
							   const clsString& name):
				clsEntity(pFactory, type, name),
				m_vPosition(0.0f, 0.0f, 0.0f),
				m_vTargetPoint(0.0f, 0.0f, 0.0f),
				m_vSpeed(0.0f, 0.0f, 0.0f)
{
	m_bNeedBufferedMouseEvent = mmtrue;
	m_bNeedUpdateEvent		  = mmtrue;
	m_bEnableSphereTest		  = mmfalse;
	m_fRotationSpeed		  = 1.0f;
	m_fScale				  = 1.0f;
	m_bLBPressed			  = 0.0f;
	m_fScaleSpeed			  = 10.0f;
	m_fMouseSens			  = 2.0f;
	m_fSmoothTime			  = 0.03f;
	m_fSmoothDist			  = 0.0f;
	m_fSmoothCurDist		  = 0.0f;
	m_fSmoothSpeed			  = 0.0f;
	m_pHelper				  = mmnull;
	m_nKillCounter			  = 0;
}

// Destructor
//==========================================================================================
clsBKCrosshair::~clsBKCrosshair()
{
	mmSafeRelease(m_pHelper);
}

// GetOrigin: returns crosshair 3D origin
//==========================================================================================
clsRay clsBKCrosshair::GetRay()
{
	return m_pEFactory->GetEngine()->GetRenderer()->UnProject(clsPoint2D((mmInt)m_vPosition.x,
																		 (mmInt)m_vPosition.y));
}

// OnInitialize
//==========================================================================================
mmVoid clsBKCrosshair::OnInitialize(mmBool bClone)
{
	Res = m_pEFactory->GetEngine()->GetRenderer()->GetResourceManager()->GetResource("crosshair.spr", MHMRES_MISC_ID);
	if (Res)
		Res->SetBlendType(enBlendAlpha);
	m_pHelper = (clsText*)m_pEFactory->GetEngine()->GetRenderer()->GetResourceManager()->GetResource("myfont.mhmFont2D", MHMRES_MISC_ID);
	m_pHelper->SetPos(0, 320);
	m_pHelper->SetColor(0xFFFF0000);
	m_pHero = FindRoot()->FindChild("Hero");
	m_mxWorld.SetTranslation(clsVector3(0.0f, 0.0f, 100.0f));
}

// OnRender
//==========================================================================================
mmRes clsBKCrosshair::OnRender()
{
//	if (Res)
//		Res->Render();

//	m_pHelper->SetText("KILLS:" + clsString(m_nKillCounter));
//	m_pHelper->Render();
	return mmOk;
}

// Mouse control
//==========================================================================================
mmVoid clsBKCrosshair::OnBufferedMouseEvent(const clsMouseEvent* pEvent, mmBool bFirst)
{
	m_vTargetPoint.x += pEvent->m_axis[enMouseX] * m_fMouseSens;
	m_vTargetPoint.y += pEvent->m_axis[enMouseY] * m_fMouseSens;
	
	// clip by viewport
	clsViewport* pViewport = m_pEFactory->GetEngine()->GetRenderer()->GetDevice()->GetViewport();
	if (m_vTargetPoint.x < 0.0f)
		m_vTargetPoint.x = 0.0f;
	if (m_vTargetPoint.y < 0.0f)
		m_vTargetPoint.y = 0.0f;
	if (m_vTargetPoint.x > (mmFloat)pViewport->m_nWidth)
		m_vTargetPoint.x = (mmFloat)pViewport->m_nWidth;
	if (m_vTargetPoint.y > (mmFloat)pViewport->m_nHeight)
		m_vTargetPoint.y = (mmFloat)pViewport->m_nHeight;

	
	if (!m_bLBPressed && pEvent->m_buttons[enMouseButton0] == enMouseButtonDown)
		m_bLBPressed = mmtrue;
	if (m_bLBPressed && pEvent->m_buttons[enMouseButton0] == enMouseButtonUp)
		m_bLBPressed = mmfalse;
}	

// Fixup position
//==========================================================================================
mmVoid clsBKCrosshair::OnEndBufferedEvents(mmUint nNumMouseEvents, mmUint nNumKeyboardEvents)
{
	m_vSpeed = m_vTargetPoint - m_vPosition;
	m_fSmoothDist  = m_vSpeed.Length();
	m_fSmoothSpeed = m_fSmoothDist / m_fSmoothTime;
	m_vSpeed.Normalize();
	m_fSmoothCurDist = 0.0f;
}

// Update
//==========================================================================================
mmVoid clsBKCrosshair::OnUpdate(const clsTimer* pTimer)
{
	// smooth mouse
	if (!mmAlmostZero(m_fSmoothDist))
	{
		mmFloat fStep = m_fSmoothSpeed * pTimer->GetDeltaSecs();
		if ((m_fSmoothCurDist + fStep) > m_fSmoothDist)
			fStep = m_fSmoothDist - m_fSmoothCurDist;
		
		m_vPosition += m_vSpeed * fStep;
		m_fSmoothCurDist += fStep;
	}
	
	m_nColor = 0xFF00FF00;
	m_fRotationSpeed = 1.0f;
	clsEntity* pRoot = FindRoot();
	clsRay ray = m_pEFactory->GetEngine()->GetRenderer()->UnProject(clsPoint2D((mmInt)m_vPosition.x,
																			   (mmInt)m_vPosition.y));
//	pRoot->RayBoundsIntersection(ray, this);
	
/*	if (m_bLBPressed)
		m_fScale += pTimer->GetDeltaSecs() * m_fScaleSpeed;
	else
		m_fScale -= pTimer->GetDeltaSecs() * m_fScaleSpeed * 2;
	
	if (m_fScale < 1.0f)
		m_fScale = 1.0f;*/
	
	clsSprite* pSprite = (clsSprite*)Res;
	pSprite->SetColor(m_nColor);
	pSprite->SetPosition(m_vPosition);
	pSprite->SetScale(clsVector3(m_fScale, m_fScale, m_fScale));
	pSprite->SetRotation(pTimer->GetSecs() * m_fRotationSpeed);
	pSprite->ReBuild();
}

// Sphere intersection handler
//==========================================================================================
mmVoid clsBKCrosshair::OnSphereRayIntersect(clsEntity* pEntity)
{
	clsString sType = pEntity->m_sType;
	sType.UpperCase();
	if (sType == "MONSTER")
	{
		clsBKCreature* pCreature = (clsBKCreature*)pEntity;
		clsRay ray = m_pEFactory->GetEngine()->GetRenderer()->UnProject(clsPoint2D((mmInt)m_vPosition.x,
			(mmInt)m_vPosition.y));
			m_nColor = 0xFFFF0000;
			//m_fRotationSpeed *= 5.0f;


//			if (m_bLBPressed)
//			{
//				pCreature->ApplyDamage(m_pHero);
//				m_nKillCounter++;
//			}
//		}
	}
}