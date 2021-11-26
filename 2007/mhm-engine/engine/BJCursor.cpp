//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// Brick journey shareware project source code
// (c) Mayhem Projex 2006
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
#include "mhmbase.h"
#include "BJCursor.h"
#include "BJBrick.h"

// Cursor constructor
//--------------------------------------------------
clsBJCursor::clsBJCursor(clsEntityFactory* pEFactory,
						 const clsString& sType,
						 const clsString& sName):
			 clsEntity(pEFactory, sType, sName)
{
	m_pSprite = mmnull;
	m_bNeedLoad = mmtrue;
	m_bNeedBufferedMouseEvent = mmtrue;
	m_bLClick = mmfalse;
	m_bRClick = mmfalse;
}

// Cursor destructor
//--------------------------------------------------
clsBJCursor::~clsBJCursor()
{

}

// Cursor initalizing
//--------------------------------------------------
mmVoid clsBJCursor::OnInitialize(mmBool bClone)
{
	m_vPosition.Set(0.0f);
	m_pSprite->SetPosition(m_vPosition);
}

// Cursor loading
//--------------------------------------------------
mmVoid clsBJCursor::OnPostLoading()
{
	Res = Renderer->GetResourceManager()->GetResource("BJ_cursor.spr", enResSprite);
	m_pSprite = (clsSprite*)Res;
	m_pSprite->SetBlendType(enBlendAlpha);
}

// Calculate cursor position
//--------------------------------------------------
mmVoid clsBJCursor::OnBufferedMouseEvent(const clsMouseEvent *pEvent, mmBool bFirst)
{
	m_vPosition.x += pEvent->m_axis[enMouseX];// * m_fMouseSens;
	m_vPosition.y += pEvent->m_axis[enMouseY];// * m_fMouseSens;
	
	// clip by viewport
	clsViewport* pViewport = m_pEFactory->GetEngine()->GetRenderer()->GetDevice()->GetViewport();
	if (m_vPosition.x < 0.0f)
		m_vPosition.x = 0.0f;
	if (m_vPosition.y < 0.0f)
		m_vPosition.y = 0.0f;
	if (m_vPosition.x > (mmFloat)pViewport->m_nWidth)
		m_vPosition.x = (mmFloat)pViewport->m_nWidth;
	if (m_vPosition.y > (mmFloat)pViewport->m_nHeight)
		m_vPosition.y = (mmFloat)pViewport->m_nHeight;
	
	if (pEvent->m_buttons[enMouseButton0] == enMouseButtonUp)
		m_bLClick = mmtrue;
	if (pEvent->m_buttons[enMouseButton1] == enMouseButtonUp)
		m_bRClick = mmtrue;
}

// Fix cursor position
//--------------------------------------------------
mmVoid clsBJCursor::OnEndBufferedEvents(mmUint nNumMouseEvents, mmUint nNumKeybordEvents)
{
//	if (nNumMouseEvents != 0)
//	{
		m_pSprite->SetPosition(m_vPosition);
		m_pSprite->ReBuild();
		
		if (m_bLClick)
			OnLeftClick();
		if (m_bRClick)
			OnRightClick();
//	}
	m_bLClick = mmfalse;
	m_bRClick = mmfalse;
}

// Render cursor
//--------------------------------------------------
mmRes clsBJCursor::OnRender()
{
	m_pSprite->Render();
	return mmFail;
}


// Left mouse click handler
//--------------------------------------------------
mmVoid clsBJCursor::OnLeftClick()
{
	clsRay ray = Renderer->UnProject(clsPoint2D((mmInt)m_vPosition.x, (mmInt)m_vPosition.y));
	ray.m_vDirection *= 10000.0f;
	m_bSelected = mmfalse;
	FindRoot()->RayBoundsIntersection(ray, this);
}

// Right mouse click handler
//--------------------------------------------------
mmVoid clsBJCursor::OnRightClick()
{
	m_pSprite->SetColor(0xFFFFFFFF);
}

// Bricks selection
//--------------------------------------------------
mmVoid clsBJCursor::OnSphereRayIntersect(clsEntity* pEntity,
										 const clsVector3& vNear,
										 const clsVector3& vFar)
{
	if (!m_bSelected)
	{
		((clsBJBrick*)pEntity)->MouseSelect();
		m_bSelected = mmtrue;
	}
}

