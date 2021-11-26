//==========================================================================================
// BIOKRIZ bonus
//==========================================================================================
#ifndef BK_BONUS_INCLUDED
#define BK_BONUS_INCLUDED

class clsBKBonus: public clsEntity
{
friend class clsBonusManager;
public:
	enum enBonusType
	{
		enUnknown		= 0,
		enRocketStrike  = 1
	};
	
	enum enBonusStatus
	{
		enBonusAppear	 = 0,
		enBonusStay		 = 1,
		enBonusDisappear = 2
	};

public:
	clsString		m_sPickupSoundFile;
	clsSound*		m_pPickupSound;
	clsEntity*		m_pBonusParticles;
	clsEntity*		m_pBonusManager;
	enBonusType		m_eBonusType;
	enBonusStatus	m_eStatus;
	clsString		m_sBonusType;
	mmInt			m_iAppearanceFactor;
	mmFloat			m_fBonusScale;
	clsVector3		m_vPosition;
	mmFloat			m_fAltitude;
	mmFloat			m_fRotationSpeed;
	mmFloat			m_fAmplitude;
	mmFloat			m_fAmplifySpeed;
	mmFloat			m_fCurrentRot;
	mmFloat			m_fCurrentAppearTime;
	mmFloat			m_fAppearTime;
	mmFloat			m_fDisappearTime;
	mmFloat			m_fCurrentLifeTime;
	mmFloat			m_fLifeTime;
	
public:
	clsBKBonus(clsEntityFactory* pFactory,
					  const clsString& sType,
					  const clsString& sName);
   ~clsBKBonus();
   
   mmVoid OnInitialize(mmBool bClone);
   mmVoid OnLoadProps(clsVarList* pVarList);
   mmVoid OnUpdate(const clsTimer* pTimer);
   mmVoid OnClone(clsEntity* pInstance);
   mmVoid OnEnumChild(clsEntity* pEntity);
   
   // bonus interfaces
   enBonusType GetType();
   mmInt       GetAppearanceFactor();
   mmVoid PickUp(clsEntity* pEntity);

protected:
   mmVoid KillBonus();
};

#endif // BK_BONUS_INCLUDED
