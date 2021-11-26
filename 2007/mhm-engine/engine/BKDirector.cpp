//==========================================================================================
// BIOKRIZ director
//==========================================================================================
#include "mhmbase.h"
#include "BKDirector.h"


//==========================================================================================
clsBKDirector::clsBKDirector(clsEntityFactory *pFactory,
							 const clsString &sType,
							 const clsString &sName):
			   clsEntity(pFactory, sType, sName),
			   m_vDirection(0.0f, 0.0f, 0.0f),
			   m_vNewDirection(0.0f, 0.0f, 0.0f),
			   m_vTargetPoint(0.0f, 0.0f, 0.0f),
			   m_vPosition(0.0f, 0.0f, 0.0f)
{
	m_bNeedUpdateEvent = mmtrue;
	m_bRenderable      = mmtrue; // <-- ???
	m_fRotateTime	   = 0.1f;
	m_fRotateSpeed	   = 0.0f;
	m_fRotateCurrent   = 0.0f;	
}
//==========================================================================================
clsBKDirector::~clsBKDirector()
{

}
//==========================================================================================
mmVoid clsBKDirector::OnInitialize(mmBool bClone)
{
	SetOwner(FindRoot());
	clsEntity* pHero = FindRoot()->FindChild("Hero");
	if (pHero != mmnull)
	{
		pHero->LinkUpdateDepency(this);
		m_vDirection = m_vNewDirection = pHero->m_mxWorld.GetTranslation() - m_vTargetPoint;
		m_vDirection.Normalize();
		m_vNewDirection.Normalize();
	}
	
	m_mxInitial = m_mxWorld;
	m_mxInitial.SetTranslation(clsVector3(-20.0f, 0.0f, 0.0f));	
}
//==========================================================================================
mmVoid clsBKDirector::OnUpdate(const clsTimer* pTimer)
{
	if (m_fRotateCurrent != m_fRotateTime)
	{
		m_fRotateCurrent += pTimer->GetDeltaSecs();
		clsVector3 Lerp;
		if (m_fRotateCurrent > m_fRotateTime)
		{
			m_fRotateCurrent = m_fRotateTime;
			Lerp = m_vDirection = m_vNewDirection;
		}
		else
		{
			Lerp = m_vDirection.Lerp(m_vNewDirection, m_fRotateCurrent / m_fRotateTime);
		}
		Lerp.Normalize();
		clsMatrix mxRotation, mxTranslation;
		mxRotation.RotationVectorZ(Lerp);
		//mxRotation.SetTranslation(clsVector3(-100.0f, 0.0f, 0.0f));
		mxTranslation.Translation(m_vPosition);
		m_mxWorld = m_mxInitial * mxRotation * mxTranslation;// * mxTranslation;
		
		//m_mxWorld.SetTranslation(m_vPosition);
	}
}
//==========================================================================================
mmVoid clsBKDirector::OnDepencyUpdate(clsEntity* pEntity, mmBool bMeDead)
{
	m_vPosition = pEntity->m_mxFull.GetTranslation();
	m_vNewDirection = m_vPosition - m_vTargetPoint;
	m_vNewDirection.z = m_vDirection.z;
	m_vNewDirection.Normalize();
	if (m_vNewDirection != m_vDirection)
		m_fRotateCurrent = 0.0f;
}
//==========================================================================================
//==========================================================================================
//==========================================================================================

