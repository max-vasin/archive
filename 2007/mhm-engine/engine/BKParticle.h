//==========================================================================================
// BIOKRIZ mesh particle
//==========================================================================================
#ifndef BK_PARTICLE_INCLUDED
#define BK_PARTICLE_INCLUDED

class clsBKParticle: public clsEntity
{
friend class clsBKParticleEmitter;
protected:
	clsVector3	m_vRotationAxis;	// particle rotation by x -> y -> z
	clsVector3	m_vSpeed;			// current particle speed
	clsVector3	m_vGravity;			// particle gravity
	clsPlane	m_cReflectPlane;	// reflect (bouncing) plane
	mmFloat		m_fBouncing;		// bouncing (falloff) factor
	clsVector3	m_vPosition;
	mmFloat		m_fLifeTime;
	mmFloat		m_fCurrentLife;
	clsMatrix	m_mxInitial;
	mmFloat		m_fAlpha;
	clsBaseParticles* m_pParticles;
	clsEntity*	m_pInstance;
	//clsString	m_pParticles;

public:
	clsBKParticle(clsEntityFactory* pFactory,
				  const clsString& sType,
				  const clsString& sName);
   ~clsBKParticle();
    
    mmRes OnRender();
    mmVoid OnInitialize(mmBool bClone);
    mmVoid OnLoadProps(clsVarList* pvlist);
    mmVoid OnUpdate(const clsTimer* pTimer);
    mmVoid OnClone(clsEntity* pInstance);
    mmVoid InitParticle();
};

#endif // BK_PARTICLE_INCLUDED
