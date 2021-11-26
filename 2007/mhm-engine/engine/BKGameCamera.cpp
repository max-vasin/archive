//==========================================================================================
// BIOKRIZ game camera entity 
//==========================================================================================
#include "mhmbase.h"
#include "BKCrossHair.h"
#include "BKGameCamera.h"

// Camera constructor
//==========================================================================================
clsBKGameCamera::clsBKGameCamera(clsEntityFactory *pEFactory,
								 const clsString& type,
								 const clsString& name):
				 clsEntity(pEFactory, type, name),
				 m_vTargetPoint(0.0f, 0.0f, 0.0f),
				 m_vHeroPosition(0.0f, 0.0f, 0.0f),
				 m_vCrossHairPosition(0.0f, 0.0f, 0.0f),
				 m_vSpeed(0.0f, 0.0f, 0.0f),
				 m_vAt(0.0f, 0.0f, 0.0f)
{
	m_bNeedBufferedMouseEvent = mmtrue;
	m_bNeedUpdateEvent		  = mmtrue;
	m_fSmoothDist			  = 0.0f;
	m_fSmoothSpeed			  = 0.0f;
	m_fSmoothCurDist		  = 0.0f;
	m_fSmoothTime			  = 0.1f;
}

// Camera destructor
//==========================================================================================
clsBKGameCamera::~clsBKGameCamera()
{

}

// Game camera initialization
//==========================================================================================
mmVoid clsBKGameCamera::OnInitialize(mmBool bClone)
{
	// we are depency of crosshair and hero position, so take control
	clsEntity* pEnt = FindRoot()->FindChild("Hero");
	if (pEnt == mmnull)
		g_plog->Out("Game camera: hero entity lost, camera not working properly", enLCRed);
	else
		pEnt->LinkUpdateDepency(this);
	pEnt = FindRoot()->FindChild("CrossHair");
	if (pEnt == mmnull)
		g_plog->Out("Game camera: crosshair entity lost, camera not working properly", enLCRed);
	else
		pEnt->LinkUpdateDepency(this);
		
	// store current position
	clsCamera Cam;
	Cam.Up(0.0f, 1.0f, 0.0f);
	Cam.SetViewMatrix(m_mxWorld);
	Cam.BuildMatrixs();
	m_vPosition = Cam.Eye();
	m_vAt		= Cam.At();
	m_vAt.z     = 0.0f;
	m_vUp		= Cam.Up();
}


// Mouse control
//==========================================================================================
mmVoid clsBKGameCamera::OnBufferedMouseEvent(const clsMouseEvent *pEvent, mmBool bFirst)
{
	m_vPosition.z += pEvent->m_axis[enMouseWheel] * 0.1f;
}
// Fix buffered events
//==========================================================================================
mmVoid clsBKGameCamera::OnEndBufferedEvents(mmUint nNumMouseEvents, mmUint nNumKeyboardEvents)
{
}
// Rebuild matrices
//==========================================================================================
mmVoid clsBKGameCamera::OnUpdate(const clsTimer* pTimer)
{
	if (!mmAlmostZero(m_fSmoothDist))
	{
		mmFloat fStep = m_fSmoothSpeed * pTimer->GetDeltaSecs();
		if ((m_fSmoothCurDist + fStep) > m_fSmoothDist)
			fStep = m_fSmoothDist - m_fSmoothCurDist;
		
		m_vAt.x += m_vSpeed.x * fStep;
		m_vAt.y += m_vSpeed.y * fStep;
		m_vPosition += m_vSpeed * fStep;
		m_fSmoothCurDist += fStep;
	}
	m_mxWorld.SetTranslation(m_vPosition);
}

// Make new camera position here
//==========================================================================================
mmVoid clsBKGameCamera::OnDepencyUpdate(clsEntity *pEntity, mmBool bMeDead)
{
	if (pEntity->Name == "Hero")
		m_vHeroPosition = pEntity->m_mxWorld.GetTranslation();
	
	if (pEntity->Name == "CrossHair")
	{
		clsBKCrosshair* pCH = (clsBKCrosshair*)pEntity;
		clsRay ray = pCH->GetRay();
		clsPlane plane;
		plane.FromPointNormal(m_vHeroPosition, clsVector3(0.0f, 0.0f, 1.0f));
		plane.IntersectRay(m_vCrossHairPosition, ray);	
	}
	// now sets new camera target point
	m_vTargetPoint = m_vHeroPosition + ((m_vCrossHairPosition - m_vHeroPosition) * 0.5f);
	m_vTargetPoint.z = m_vPosition.z;
	m_vSpeed = m_vTargetPoint - m_vPosition;
	m_fSmoothDist  = m_vSpeed.Length();
	m_fSmoothSpeed = m_fSmoothDist / m_fSmoothTime;
	m_vSpeed.Normalize();
	m_fSmoothCurDist = 0.0f;
}



