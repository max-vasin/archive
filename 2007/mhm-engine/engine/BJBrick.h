//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// Brick journey shareware project source code
// (c) Mayhem Projex 2006
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

#ifndef BJ_BRICK_INCLUDED
#define BJ_BRICK_INCLUDED

class clsBJGameMatrix;

class clsBJBrick: public clsEntity
{
public:
	enum enBrickState
	{
		enBrickBlocked	= 0,
		enBrickReady	= 1,
		enBrickFalling	= 2
	};
protected:
	mmDword				m_dwBrickType;
	enBrickState		m_eBrickState;
	clsBJGameMatrix*	m_pMatrix;
	clsVector3			m_vPosition;
	mmUint				m_nX;
	mmUint				m_nY;
	mmFloat				m_fFallSpeed;
	mmFloat				m_fFallDist;
	mmFloat				m_fFallTime;
	mmFloat				m_fFallCurDist;
	
public:
	clsBJBrick(clsEntityFactory* pEFactory,
			   const clsString& sType,
			   const clsString& sName);
   ~clsBJBrick();
   
/*    mmRes			OnRender()
    {
		return mmOk;
    }*/
    mmVoid			OnInitialize(mmBool bClone);
    mmVoid			OnPostLoading();
    mmVoid			OnUpdate(const clsTimer* pTimer);
   
    mmVoid			SetGameMatrix(clsBJGameMatrix* pMatrix);
    mmVoid			SetCell(mmUint x, mmUint y);
    
    mmDword			GetBrickType();
    enBrickState	GetBrickState();
    
    mmVoid			SetBrickType(mmDword dwNewType);
    mmVoid			MouseSelect();

private:
	mmVoid			CheckBrickState();
};
#endif // BJ_BRICK_INCLUDED