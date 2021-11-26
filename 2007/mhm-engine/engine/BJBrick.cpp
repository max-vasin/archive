//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// Brick journey shareware project source code
// (c) Mayhem Projex 2006
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
#include "mhmbase.h"
#include "BJDefs.h"
#include "BJBrick.h"
#include "BJGameMatrix.h"

// Brick constructor
//--------------------------------------------------
clsBJBrick::clsBJBrick(clsEntityFactory *pEFactory, 
					   const clsString &sType,
					   const clsString &sName):
			clsEntity(pEFactory, sType, sName)
					   
{
	m_dwBrickType		= 0;
	m_eBrickState		= enBrickBlocked;
	m_bEnableSphereTest = mmfalse;
	m_bNeedLoad			= mmtrue;
	m_bNeedUpdateEvent	= mmtrue;
	m_bRayCollide		= mmtrue;
	m_vPosition.Set(0.0f);
}


// Brick destructor
//--------------------------------------------------
clsBJBrick::~clsBJBrick()
{

}

// Brick resources loading
//--------------------------------------------------
mmVoid clsBJBrick::OnPostLoading()
{
	Res = Renderer->GetResourceManager()->GetResource(BJ_BRICK_MESH, MHMRES_MESH_ID);
	((clsBaseMesh*)Res)->ComputeBounds(&m_cBoundBox, &m_cBoundSphere, &m_mxWorld);
	m_eBoundType = enBoundSphere;
	g_plog->Out("Brick loaded", enLCGreen);
}

// Brick initializing
//--------------------------------------------------
mmVoid clsBJBrick::OnInitialize(mmBool bClone)
{
	m_vPosition = m_mxWorld.GetTranslation();
//	m_bDrawBoundSphere = mmtrue;
}

// Brick updating
//--------------------------------------------------
mmVoid clsBJBrick::OnUpdate(const clsTimer *pTimer)
{
	if (m_eBrickState == enBrickFalling)
	{
		mmFloat fDelta = pTimer->GetDeltaSecs();
		mmFloat fStep;
		m_fFallTime += fDelta;
		if (m_fFallTime > BJ_BRICK_FALL_TIME)
		{
			fDelta = m_fFallTime - BJ_BRICK_FALL_TIME;
			m_fFallTime = BJ_BRICK_FALL_TIME;
			fStep = m_fFallDist - m_fFallCurDist;
			m_eBrickState = enBrickReady;
		}
		else
			fStep = (m_fFallDist / BJ_BRICK_FALL_TIME) * fDelta;
		m_fFallCurDist += fStep;
		m_vPosition += m_vSpeed * fStep;
		m_mxWorld.SetTranslation(m_vPosition);
	}
	if (m_eBrickState == enBrickReady)
		CheckBrickState();
}


// Matrix sets itself
//--------------------------------------------------
mmVoid clsBJBrick::SetGameMatrix(clsBJGameMatrix *pMatrix)
{
	m_pMatrix = pMatrix;
}

// Matrix sets cells
//--------------------------------------------------
mmVoid clsBJBrick::SetCell(mmUint x, mmUint y)
{
	m_nX = x;
	m_nY = y;
	m_vSpeed    = m_pMatrix->GetCellPos(x, y) - m_vPosition;
	m_fFallDist = m_vSpeed.Length();
	m_fFallCurDist = 0.0f;
	m_vSpeed.NormalizeFast();
	m_fFallTime = 0.0f;
	m_eBrickState = enBrickFalling;
	m_pMatrix->SetBrick(this, x, y);
	if (y != 0)
		m_pMatrix->SetBrick(mmnull, x, y - 1);
}

// Returns brick type
//--------------------------------------------------
mmDword clsBJBrick::GetBrickType()
{
	return m_dwBrickType;
}

// Returns brick state
//--------------------------------------------------
clsBJBrick::enBrickState clsBJBrick::GetBrickState()
{
	return m_eBrickState;
}

// Sets new brick type
//--------------------------------------------------
mmVoid clsBJBrick::SetBrickType(mmDword dwNewType)
{
	m_dwBrickType = dwNewType;
}

// Checks brick current state
//--------------------------------------------------
mmVoid clsBJBrick::CheckBrickState()
{
	if (m_nY == BJ_GAME_MATRIX_HEIGHT - 1)
		return;
	if (m_pMatrix->GetCell(m_nX, m_nY + 1) == mmnull)
		SetCell(m_nX, m_nY + 1);
}

// Mouse selection
//--------------------------------------------------
mmVoid clsBJBrick::MouseSelect()
{
//	m_vPosition.z = 30.0f;
//	m_mxWorld.SetTranslation(m_vPosition);
	if (m_eBrickState != enBrickReady)
		return;
	m_pMatrix->SetBrick(mmnull, m_nX, m_nY);
	m_bKillMe = mmtrue;
}






