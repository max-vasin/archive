//==========================================================================================
// BIOKRIZ game camera entity 
//==========================================================================================
#ifndef BK_GAME_CAMERA_INCLUDED
#define BK_GAME_CAMERA_INCLUDED

#define BK_GAME_CAMERA_SMOOTH	0.5f

class clsBKGameCamera: public clsEntity
{
public:
	clsVector3	m_vTargetPoint;
	clsVector3	m_vHeroPosition;
	clsVector3	m_vCrossHairPosition;
	clsVector3	m_vPosition;
	clsVector3	m_vAt;
	clsVector3	m_vUp;
	
	clsVector3	m_vSpeed;
	
	mmFloat		m_fSmoothDist;
	mmFloat		m_fSmoothSpeed;
	mmFloat		m_fSmoothCurDist;
	mmFloat		m_fSmoothDisCurDist;
	mmFloat		m_fSmoothDisDist;
	mmFloat		m_fSmoothTime;
	mmFloat		m_fDistort;
	
public:
	clsBKGameCamera(clsEntityFactory* pEFactory, 
					const clsString& type,
					const clsString& name);
   ~clsBKGameCamera();
    
    mmVoid OnInitialize(mmBool bClone);
    mmVoid OnBufferedMouseEvent(const clsMouseEvent* pEvent, mmBool bFirst);
    mmVoid OnEndBufferedEvents(mmUint nNumMouseEvents, mmUint nNumKeyboardEvents);
    mmVoid OnUpdate(const clsTimer* pTimer);
    mmVoid OnDepencyUpdate(clsEntity* pEntity, mmBool bMeDead);
};


#endif // BK_GAME_CAMERA_INCLUDED