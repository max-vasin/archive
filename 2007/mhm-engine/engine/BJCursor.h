//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// Brick journey shareware project source code
// (c) Mayhem Projex 2006
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

#ifndef BJ_CURSOR_INCLUDED
#define BJ_CURSOR_INCLUDED

class clsBJCursor: public clsEntity
{
protected:
	clsSprite*	m_pSprite;
	clsVector3	m_vPosition;
	mmBool		m_bLClick;
	mmBool		m_bRClick;
	mmBool		m_bSelected;

public:
	clsBJCursor(clsEntityFactory* pEFactory,
				const clsString& sType,
				const clsString& sName);
   ~clsBJCursor();
   
    
    mmVoid OnInitialize(mmBool bClone);
    mmVoid OnPostLoading();
    mmVoid OnBufferedMouseEvent(const clsMouseEvent* pEvent, mmBool bFirst);
    mmVoid OnEndBufferedEvents(mmUint nNumMouseEvents, mmUint nNumKeybordEvents);
    mmRes  OnRender();
    
	mmVoid OnSphereRayIntersect(clsEntity* pEntity, 
								const clsVector3& vNear,
								const clsVector3& vFar);

private:
	mmVoid OnLeftClick();
	mmVoid OnRightClick();

};

#endif // BJ_CURSOR_INCLUDED