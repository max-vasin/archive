//==========================================================================================
// BIOKRIZ crosshair
//==========================================================================================
#ifndef BK_CORSSHAIR_INCLUDED
#define BK_CORSSHAIR_INCLUDED

class clsBKCrosshair: public clsEntity
{
protected:
	clsVector3	m_vPosition;
	clsVector3	m_vTargetPoint;
	clsVector3	m_vSpeed;
	mmFloat		m_fSmoothTime;
	mmFloat		m_fSmoothDist;
	mmFloat		m_fSmoothCurDist;
	mmFloat		m_fSmoothSpeed;
	
	
	mmFloat		m_fMouseSens;
	mmFloat		m_fRotationSpeed;
	mmFloat		m_fScale;
	mmFloat		m_fScaleSpeed;
	mmBool		m_bLBPressed;
	mmDword		m_nColor;
	
	mmUint		m_nKillCounter;
	
	clsText*	m_pHelper;
	clsEntity*	m_pHero;
	
public:
	clsBKCrosshair(clsEntityFactory* pFactory,
				   const clsString& type,
				   const clsString& name);
   ~clsBKCrosshair();
    
    clsRay GetRay();
    
    mmVoid OnInitialize(mmBool bClone);
    mmRes OnRender();
    mmVoid OnBufferedMouseEvent(const clsMouseEvent* pEvent, mmBool bFirst);
    mmVoid OnEndBufferedEvents(mmUint nNumMouseEvents, mmUint nNumKeyboardEvents);
    mmVoid OnUpdate(const clsTimer* pTimer);
    mmVoid OnSphereRayIntersect(clsEntity* pEntity);
};

#endif // BK_CORSSHAIR_INCLUDED