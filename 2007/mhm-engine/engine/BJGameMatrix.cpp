//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// Brick journey shareware project source code
// (c) Mayhem Projex 2006
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
#include "mhmbase.h"
#include "BJDefs.h"
#include "BJGameMatrix.h"

// Game matrix constructor
//--------------------------------------------------
clsBJGameMatrix::clsBJGameMatrix(clsEntityFactory *pEFactory,
								 const clsString &sType,
								 const clsString &sName):
				 clsEntity(pEFactory, sType, sName)
{
	m_bNeedLoad			= mmtrue;
	m_bNeedUpdateEvent  = mmtrue;
	mmZeroVar(m_arCells);
	mmZeroVar(m_arBricks);
}
								 

// Game matrix destructor
//--------------------------------------------------
clsBJGameMatrix::~clsBJGameMatrix()
{
}

// Game matrix post loading
//--------------------------------------------------
mmVoid clsBJGameMatrix::OnPostLoading()
{
	m_pBrick = AddChildNode("Brick", "BrickInstance", mmtrue, mmtrue);
	m_pBrick->SetOwner(mmnull);
}



// Game matrix initializing
//--------------------------------------------------
mmVoid clsBJGameMatrix::OnInitialize(mmBool bClone)
{
	if (bClone)
		return;
	
	GeneratePositions();
	//GenerateClones();
	
	g_plog->Out("Game matrix successfully loaded and initialized", enLCGreen);
}

// Game matrix updating
//--------------------------------------------------
mmVoid clsBJGameMatrix::OnUpdate(const clsTimer* pTimer)
{
	CollapseMatrix();
}

// Returns cell entity by its index
//--------------------------------------------------
clsEntity* clsBJGameMatrix::GetCell(mmUint x, mmUint y)
{
	if ((x >= BJ_GAME_MATRIX_WIDTH) || (y > BJ_GAME_MATRIX_HEIGHT))
		return mmnull;
	return m_arBricks[y][x];
}

// Returns cell position by its index
//--------------------------------------------------
clsVector3 clsBJGameMatrix::GetCellPos(mmUint x, mmUint y)
{
	if ((x >= BJ_GAME_MATRIX_WIDTH) || (y > BJ_GAME_MATRIX_HEIGHT))
		return mmnull;
	return m_arCells[y][x];
}

// Callback from brick
//--------------------------------------------------
mmVoid clsBJGameMatrix::SetBrick(clsBJBrick* pBrick, mmUint x, mmUint y)
{
	if ((x >= BJ_GAME_MATRIX_WIDTH) || (y > BJ_GAME_MATRIX_HEIGHT))
		return;
	m_arBricks[y][x] = pBrick;
}


// Matrix positions generating
//--------------------------------------------------
mmVoid clsBJGameMatrix::GeneratePositions()
{
	mmFloat fStartX = 0.0f - (BJ_GAME_MATRIX_WIDTH  * BJ_GAME_MATRIX_SPACE / 2.0f) + BJ_GAME_MATRIX_SPACE / 2.0f;
	mmFloat fStartY = 0.0f + (BJ_GAME_MATRIX_HEIGHT * BJ_GAME_MATRIX_SPACE / 2.0f) + BJ_GAME_MATRIX_SPACE / 2.0f;
	
	for (mmUint y = 0; y < BJ_GAME_MATRIX_HEIGHT; y++)
		for (mmUint x = 0; x < BJ_GAME_MATRIX_WIDTH; x++)
		{
			m_arCells[y][x] = clsVector3(fStartX + (mmFloat)x * BJ_GAME_MATRIX_SPACE,
										 fStartY - (mmFloat)y * BJ_GAME_MATRIX_SPACE,
										 BJ_GAME_MATRIX_Z);
		}
}

// Bricks clones generation (test routine)
//--------------------------------------------------
mmVoid clsBJGameMatrix::GenerateClones()
{
	for (mmUint y = 0; y < BJ_GAME_MATRIX_HEIGHT; y++)
		for (mmUint x = 0; x < BJ_GAME_MATRIX_WIDTH; x++)
		{
			m_pBrick->SetOwner(this);
			clsEntity* pClone = m_pBrick->Clone("Brick", mmtrue);
			pClone->m_mxWorld.Translation(m_arCells[y][x]);
			pClone->m_mxFull = pClone->m_mxWorld;
			m_pBrick->SetOwner(mmnull);
		}
}

// Collapse matrix
//--------------------------------------------------
mmVoid clsBJGameMatrix::CollapseMatrix()
{
	for (mmUint x = 0; x < BJ_GAME_MATRIX_WIDTH; x++)
		for (mmUint y = 0; y < BJ_GAME_MATRIX_HEIGHT; y++)
		{
			if (m_arBricks[y][x] == mmnull)
			{
				if (y == 0)
					GenerateBrick(x);
			}
		}
}

// Generates new brick
//--------------------------------------------------
mmVoid clsBJGameMatrix::GenerateBrick(mmUint x)
{
	clsMatrix mxTrans;
	mxTrans.Translation(m_arCells[0][x]);
	mxTrans._42 += BJ_GAME_MATRIX_SPACE;
		
	m_pBrick->SetOwner(this);
	clsBJBrick* pNewBrick = (clsBJBrick*)m_pBrick->Clone("Brick", mmtrue, &mxTrans);
	pNewBrick->m_mxWorld.Translation(mxTrans.GetTranslation());
	pNewBrick->m_mxFull = pNewBrick->m_mxWorld;
	pNewBrick->SetGameMatrix(this);
	pNewBrick->SetCell(x, 0);
	//m_arBricks[0][x] = pNewBrick;
	m_pBrick->SetOwner(mmnull);
}




