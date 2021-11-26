//==========================================================================================
// BIOKRIZ director
//==========================================================================================
#ifndef BK_DIRECTOR_INCLUDED
#define BK_DIRECTOR_INCLUDED

class clsBKDirector: public clsEntity
{
protected:
	clsVector3	m_vTargetPoint;
	clsVector3	m_vDirection;
	clsVector3	m_vNewDirection;
	clsVector3	m_vPosition;
	clsMatrix	m_mxInitial;
	
	mmFloat		m_fRotateTime;
	mmFloat		m_fRotateSpeed;
	mmFloat		m_fRotateCurrent;

public:
	clsBKDirector(clsEntityFactory* pFactory,
				  const clsString& sType,
				  const clsString& sName);
   ~clsBKDirector();

    mmVoid OnInitialize(mmBool bClone);
    mmVoid OnUpdate(const clsTimer* pTimer);
    mmVoid OnDepencyUpdate(clsEntity* pEntity, mmBool bMeDead);
};

#endif // BK_DIRECTOR_INCLUDED
