#pragma once
//==========================================================================================
// Mayhem raw Node class implementation
// Coded:   fyrex
// Created:	1 july 2006
//==========================================================================================

//=================================================================
// Entity type
//=================================================================
enum enType
{
	enUndefined = 0,
	enScene		= 1,
	enMesh		= 2,
	enHelper	= 3,
	enCamera	= 4,
	enLight		= 5,
	enParticles	= 6,

	enDword = 0x7fffffff
};

class clsEntity;

/* ??????
//=================================================================
// Intersection handler
//=================================================================
class ICollisionHandler
{
public:
	virtual mmVoid HandleSphereCollision(clsEntity* pa, clsEntity* pb);
};*/


typedef tmpRefArr<clsEntity*> clsEntityArray;

//=================================================================
// Bounding volumes types
//=================================================================
enum enBoundingType
{
	enBoundNone		= 0,
	enBoundSphere	= 1,
	enBoundBox		= 2,
	enBoundMesh		= 3
};


//=================================================================
// Entity class
//=================================================================
class clsEntity
{
friend class clsEngine;
public:
	enType			 Type;
	clsRenderer*	 Renderer;
	
	// Entity transformations
	clsMatrix		 m_mxFull;
	clsMatrix		 m_mxWorld;
	clsMatrix		 m_mxLocal;
	
	// Entity controllers
	IAnimController* m_pAnimController;
	
	// Entity system
	clsEntity	*Owner;
	clsEntity	*Next;
	clsEntity	*Child;
	clsEntity	*List;
	
	// Update depencies
	clsEntity*	m_pNextDepency;
	clsEntity*	m_pDepency;
	clsEntity*	m_pDepencyMaster;
	clsEntity*	m_pNextDepencyMaster;
	
	// Entity renderer resource
	clsResource *Res;
	clsResource *m_bs;
	
	// Entity world bound volumes
	enBoundingType		 m_eBoundType;
	clsBoundBox			 m_cBoundBox;
	clsBoundSphere		 m_cBoundSphere;
	mmFloat				 m_fBoundScale;
	
	
	mmBool			m_bEnableSphereTest;
	mmBool			m_bRenderable;
	
	// Entity misc
	clsMatrix*		m_pTformHolder;
	
	// Entity events flags
	// set needed flags to cath needed events in your class
	mmBool			m_bNeedBufferedMouseEvent;
	mmBool			m_bNeedBufferedKeyboardEvent;
	mmBool			m_bNeedMouseEvent;
	mmBool			m_bNeedUpdateEvent;
	mmBool			m_bNeedSpeedCalc;
	
	mmBool			m_bKillMe;
	mmBool			m_bKillProtection;
	mmBool			m_bInitialized;
	mmBool			m_bDrawBoundSphere;
	mmBool			m_bNeedLoad;
	mmBool			m_bOutOfView;
	mmBool			m_bNeedCollide;
	mmBool			m_bFinishUpdate;
	mmBool			m_bDisableHiddenHierarchy;
	mmBool			m_bRayCollide;
	mmBool			m_bBlockRender;
	
	clsVector3		m_vSpeed;
	
	// debug
	clsBoundSphere	m_AffSphere;


	

public:	// <-- temporary
	clsString		Name;
	clsString		m_sType;
	clsString		m_sSuperType;
	PathTrack		Path;
	clsAnimState	m_AnimState;
	clsEntityFactory* m_pEFactory;
	

	
public:
				clsEntity(clsEntityFactory* pfact, 
						  const clsString& type,
						  const clsString& name);
	virtual	   ~clsEntity(void);

	mmVoid		SetOwner(clsEntity *O);
	clsEntity	*FindNext(const mmString name);
	clsEntity	*FindList(const mmString name);
	clsEntity	*FindRoot();
	clsEntity	*FindChild(const clsString& Name);
	clsEntity	*FindChild(const mmString Name);
//	clsEntity	*FindChildByType(const clsString& Name);

	mmVoid		  AnimationBuild();
	mmVoid		  AnimationReset();
	mmVoid		  AnimationFrame(mmFloat ms, mmFloat delta);
	

	// Pink \/

	clsAnimKeys * m_AnimKeys;	
	clsAnimationRageController * m_AnimationRageController;

	mmRes		  LoadAnimationKeys(const mmString name , mmBool childs = mmfalse);//

	mmVoid		  AnimationRangeStop(mmBool childs);

	mmVoid 		  AnimationRangeBuild(mmFloat delta); //

	mmRes		  SetAnimationPos(const mmString key);//
	mmRes		  SetAnimationPos(mmInt key);//
	mmRes		  SetAnimationPosMiSec(mmFloat time);//

	mmRes		  PlayAnimationRange(const mmString name, enAnimLooping Loop = enAnimLoop, mmInt LoopCount = 0);
	mmRes		  PlayAnimationRange(const mmString start, const mmString end, enAnimLooping Loop = enAnimLoop, mmInt LoopCount = 0);
	mmRes		  PlayAnimationRange(mmInt start, mmInt end, enAnimLooping Loop = enAnimLoop, mmInt LoopCount = 0);
	mmRes		  PlayAnimationRangeMiSec(mmFloat start, mmFloat end, enAnimLooping Loop = enAnimLoop, mmInt LoopCount = 0);

	mmRes		  PlayAnimationRangeStart(const mmString name, enAnimLooping Loop = enAnimLoop, mmInt LoopCount = 0);
	mmRes		  PlayAnimationRangeStart(const mmString start, const mmString end, enAnimLooping Loop = enAnimLoop, mmInt LoopCount = 0);
	mmRes		  PlayAnimationRangeStart(mmInt start, mmInt end, enAnimLooping Loop = enAnimLoop, mmInt LoopCount = 0);
	mmRes		  PlayAnimationRangeMiSecStart(mmFloat start, mmFloat end, enAnimLooping Loop = enAnimLoop, mmInt LoopCount = 0);

	mmRes		  PlayAnimationToKey(const mmString key);//
	mmRes		  PlayAnimationToKey(mmInt key);//
	mmRes		  PlayAnimationToMiSec(mmFloat time);//


	virtual mmVoid OnAnimationKey(mmInt key){};
	virtual mmVoid OnAnimationPlay(){};

	virtual mmVoid OnNoSphereTest(){};

	//pink /\


	PathTrack*	  GetPathTrack();
	clsAnimState* GetAnimState();
	mmVoid	      AssignController(IAnimController* pctrl, mmBool recursive = mmfalse);

	mmVoid		  LoadPath(const mmString name);
	mmRes		  LoadMJ(const mmString FileName);
	
	

	mmVoid		RayBoundsIntersection(const clsRay& ray, clsEntity* pEntity, clsVector3* pPosSelect = mmnull, mmFloat fScale = 1.0f);
	mmVoid		Animate(clsCamera* pCam = mmnull);

	
	// MozG expansion
	mmVoid		SetTransformHolder(clsMatrix* pMx);
	clsEntity*	AddChildNode(const clsString& type,
							 const clsString& name,
							 mmBool bInitialize = mmfalse,
							 mmBool bPostLoad = mmfalse);
	clsEntity*	AddChildNode(clsEntity* pEntity);
	
	mmVoid		LinkUpdateDepency(clsEntity* pEntity);
	mmVoid		UnlinkUpdateDepency(clsEntity* pEntity, mmBool bRemoveMaster = mmtrue);
	clsEntity*  Clone(const clsString& sNewName, 
					  mmBool bCloneChildrens, clsMatrix* pTForm = mmnull);
	clsEntity*  Clone(const clsString& sNewName,
					  const clsString& sNewType,
					  mmBool bCloneChidrens,
					  clsMatrix* pTForm = mmnull,
					  mmBool bFirst = mmtrue);
	mmVoid		EnableBoundSphereTest(mmBool bEnable, mmBool bRecursive = mmfalse);
	mmVoid		EnableBoundSphereDraw(mmBool bEnable, mmBool bRecursive = mmfalse);
	mmVoid		EnableRendering(mmBool bEnable, mmBool bRecursive = mmfalse);
	mmVoid		OutOfView(mmBool bOut, mmBool bRecursive = mmfalse);
	mmVoid		CheckCollision(clsEntity* pEntity);
	const clsString& GetSuperType();
	
	virtual	mmRes  Render();
	virtual clsEntity* GetCollideEntity()												{return this;}
	
	// MozG events expansion
	virtual mmVoid OnBufferedMouseEvent(const clsMouseEvent* pEvent, mmBool bFirst)		{}
	virtual mmVoid OnBufferedKeyboardEvent(const clsKeyState* pEvent, mmBool bFirst)	{}
	virtual mmVoid OnMouseEvent(const clsMouseEvent* pEvent)							{}
	virtual mmVoid OnUpdate(const clsTimer* pTimer)										{}
	virtual mmVoid OnEndBufferedEvents(mmUint nNumMouseEvents, mmUint nNumKeyboardEvents){}
	virtual mmVoid OnInitialize(mmBool bClone=mmfalse)									{}
	virtual mmRes  OnRender()					{return mmOk;}
	virtual mmVoid OnSphereRayIntersect(clsEntity* pEntity, 
										const clsVector3& vNear,
										const clsVector3& vFar)						{}
	virtual mmVoid OnDepencyUpdate(clsEntity* pEntity,	mmBool	bMeDead=mmfalse)		{}
	virtual mmVoid OnPostLoading()									{}
	virtual mmVoid OnClone(clsEntity* pInstance)					{}
	virtual mmVoid OnCollide(clsEntity* pEntity, mmFloat fFirstTime, mmFloat fSecondTime){}
	virtual mmVoid OnCalcSpeed(const clsTimer* pTimer)				{}

	virtual mmVoid OnLoadProps(clsVarList*)												{}
	virtual mmVoid OnLoadPropsPost(clsVarList*)												{}
	virtual mmVoid OnEnumChild(clsEntity* pEntity)										{}
	

public:
	mmVoid	ProcessBufferedMouseEvent(const clsMouseEvent* pEvent, mmBool bFirst);
	mmVoid  ProcessBufferedKeyboardEvent(const clsKeyState* pEvent, mmBool bFirst);
	mmVoid  ProcessEndBufferedEvents(mmUint nNumMouseEvents, mmUint nNumKeyboardEvents);
	mmVoid	ProcessMouseEvent(const clsMouseEvent* pEvent);
	mmVoid	ProcessUpdate(const clsTimer* ptimer);
	mmVoid	ProcessInitialize(mmBool bClone=mmfalse);
	mmVoid  ProcessSpeedCalc(const clsTimer* pTimer);
	mmVoid	ProcessLoading();
	mmVoid  Collect(clsEntityArray& cSuicides, clsEntityArray& cCollides);
	
	mmVoid  ProcessUnbindDepencies();
	mmVoid  EnumChilds(const clsString& sType, clsEntity* pCallback);
	
	// debug
	mmVoid  DumpHierarchy(clsString sSpace);

	clsVector3	GetPosition();
	clsVector3	GetWorldPosition();

	mmVoid  ProcessCount(mmUint& nTotal);
	


	//Animation

	mmVoid AnimationRangeBuildChild(clsAnimState * AnimState);
};
