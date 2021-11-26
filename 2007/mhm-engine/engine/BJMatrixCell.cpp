//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// Brick journey shareware project source code
// (c) Mayhem Projex 2006
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
#include "mhmbase.h"
#include "BJMatrixCell.h"

// Matrix cell constructor
//--------------------------------------------------
clsBJMatrixCell::clsBJMatrixCell(clsEntityFactory *pEFactory,
								 const clsString &sType,
								 const clsString &sName):
				 clsEntity(pEFactory, sType, sName)
{

}

// Matrix cell destructor
//--------------------------------------------------
clsBJMatrixCell::~clsBJMatrixCell()
{

}

// Matrix cell props loading
//--------------------------------------------------
mmVoid clsBJMatrixCell::OnLoadProps(clsVarList* pVarList)
{
	clsVariable* pVar = pVarList->FindVariable("PosX");
	if (pVar == mmnull)
	{
		g_plog->Out("Matrix cell: missed X position variable in properties", enLCRed);
	}
	else
	{
		m_nX = *pVar;	
	}
	pVar = pVarList->FindVariable("PosY");
	if (pVar == mmnull)
	{
		g_plog->Out("Matrix cell: missed Y position variable in properties", enLCRed);
	}
	else
	{
		m_nY = *pVar;	
	}
}




