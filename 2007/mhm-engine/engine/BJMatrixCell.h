//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// Brick journey shareware project source code
// (c) Mayhem Projex 2006
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

#ifndef BJ_MATRIXCELL_INCLUDED
#define BJ_MATRIXCELL_INCLUDED

class clsBJMatrixCell: public clsEntity
{
friend class clsBJGameMatrix;
protected:
	mmInt	m_nX;
	mmInt	m_nY;
	
public:
	clsBJMatrixCell(clsEntityFactory* pEFactory,
				    const clsString& sType,
				    const clsString& sName);
   ~clsBJMatrixCell();
   
    mmVoid OnLoadProps(clsVarList* pVarList);
};

#endif // BJ_GAMECELL_INCLUDED