//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// Brick journey shareware project source code
// (c) Mayhem Projex 2006
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

#ifndef BJ_GAMEMATRIX_INCLUDED
#define BJ_GAMEMATRIX_INCLUDED

#include "BJBrick.h"

class clsBJGameMatrix: public clsEntity
{
protected:
	clsVector3	m_arCells[BJ_GAME_MATRIX_HEIGHT][BJ_GAME_MATRIX_WIDTH];
	clsBJBrick*	m_arBricks[BJ_GAME_MATRIX_HEIGHT][BJ_GAME_MATRIX_WIDTH];
	clsEntity*	m_pBrick;
	
public:
	clsBJGameMatrix(clsEntityFactory* pEFactory,
					const clsString& sType,
					const clsString& sName);
   ~clsBJGameMatrix();
   
    mmVoid OnInitialize(mmBool bClone);
    mmVoid OnPostLoading();
    mmVoid OnUpdate(const clsTimer* pTimer);
    
    clsEntity* GetCell(mmUint x, mmUint y);
    clsVector3 GetCellPos(mmUint x, mmUint y);
    mmVoid SetBrick(clsBJBrick* pBrick, mmUint x, mmUint y);

private:
    mmVoid CollapseMatrix();
    mmVoid GenerateBrick(mmUint x);
    mmVoid GeneratePositions();
    mmVoid GenerateClones();
};

#endif // BJ_GAMEMATRIX_INCLUDED