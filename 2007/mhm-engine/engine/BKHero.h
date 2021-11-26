//==========================================================================================
// BIOKRIZ hero entity
//==========================================================================================

#ifndef BK_HERO_INCLUDED
#define BK_HERO_INCLUDED


struct stHeroParamBlock
{
	mmFloat m_fHeroSpeedRun;
	mmFloat m_fHeroSpeedBack; 
	mmFloat m_fHeroSpeedRight;
	mmFloat m_fHeroSpeedLeft; 
};

enum enHeroAnimState
{
	enHeroAnimStay	= 0,
	enHeroAnimRun	= 1,
	enHeroAnimLeft	= 2,
	enHeroAnimRight	= 3,
	enHeroAnimBack	= 4
};

enum enHeroWeapon
{
	enHeroWeaponPistol			= 0,
	enHeroWeaponTwoPistols		= 1,
	enHeroWeaponMinigun			= 2,
	enHeroWeaponRocketLauncher	= 3,
	
	enHeroWeaponCount
};

enum enHeroState
{
	enHeroFree		= 0,
	enHeroFiring	= 1,
	enHeroReloading = 2
};

class clsBKHero: public clsEntity
{
public:
	clsEntity*		m_pHeroRunScene;
	clsVector3		m_vBoundMin;
	clsVector3		m_vBoundMax;
	
	
	
	mmBool			m_bMoveLeft;
	mmBool			m_bMoveRight;
	mmBool			m_bMoveForward;
	mmBool			m_bMoveBackward;
	mmBool			m_bLButtonPressed;
	stHeroParamBlock m_stParams;
	
	mmFloat			m_fSpeed;
	clsVector3		m_vDirection;
	clsVector3		m_vLookDir;
	clsBaseMesh*	m_pGroundMesh;
	clsMatrix		m_mxBuffer;
	clsMatrix		m_mxHalfSector;
	enHeroAnimState m_eAnimState;
	enHeroState		m_eState;
	enHeroWeapon	m_eWeapon;
	clsEntity*		m_pLeftHolder;
	clsEntity*		m_pRightHolder;
	clsEntity*		m_pWeaponScene;
	
	// hero weapons
	clsEntity*		m_pPistol;
	clsEntity*		m_pSecondPistol;
	clsEntity*		m_pMinigun;
	clsEntity*		m_pRocketLauncher;
	
	
	
	clsEntity*		m_pLeftGunfire;
	clsEntity*		m_pRightGunfire;
	
	// helpers
	clsText*		m_pHelper;
	clsVector3		m_vRelDir;
	
	

public:
	clsBKHero(clsEntityFactory* pFactory,
			  const clsString& type,
			  const clsString& name);
   ~clsBKHero();
    
    mmVoid OnInitialize(mmBool bClone);
    mmVoid OnPostLoading();
    mmRes  OnRender();
    mmVoid OnBufferedKeyboardEvent(const clsKeyState* pEvent, mmBool bFirst);
    mmVoid OnBufferedMouseEvent(const clsMouseEvent* pEvent, mmBool bFirst);
    mmVoid OnEndBufferedEvents(mmUint nNumMouseEvents, mmUint nNumKeyboardEvents);
    mmVoid OnCalcSpeed(const clsTimer* pTimer);
    mmVoid OnUpdate(const clsTimer* pTimer);
    mmVoid OnDepencyUpdate(clsEntity* pEntity, mmBool bMeDead);
    mmVoid OnCollide(clsEntity* pEntity, mmFloat fFirstTime, mmFloat fSecondTime);
    mmVoid OnEnumChild(clsEntity* pEntity);
    
 protected:
    mmVoid SelectWeapon();
    mmVoid SelectState(enHeroState eState);
    mmVoid LoadParamBlock();
};

#endif // BK_HERO_INCLUDED