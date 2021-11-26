//==========================================================================================
// BIOKRIZ mesh particles emitter
//==========================================================================================
#ifndef BK_PARTICLESEMITTER_INCLUDED
#define BK_PARTICLESEMITTER_INCLUDED

class clsBKParticleEmitter: public clsEntity
{
public:
	clsString		m_sSourceSceneName;
	mmFloat			m_fLifeTime;
	clsVector3		m_vPulsePoint;
	clsVector3		m_vGravity;
	clsPlane		m_cBouncingPlane;
	mmFloat			m_fSpeedVariance;
	mmFloat			m_fBouncing;
	mmFloat			m_fPulseForce;
	mmFloat			m_fCurrentLife;
	mmFloat			m_fRotationSpeed;
	mmFloat			m_fRotationVariance;
	clsEntityArray	m_arParticles;
	
	//mmFloat	m_fRotation

public:
	clsBKParticleEmitter(clsEntityFactory* pFactory,
						 const clsString& sName,
						 const clsString& sType);
   ~clsBKParticleEmitter();
   
   mmVoid OnInitialize(mmBool bClone);
   mmVoid OnUpdate(const clsTimer* pTimer);
   mmVoid OnLoadProps(clsVarList* pvlist);
   mmVoid OnPostLoading();
   
   // emitter interface
   mmVoid SetPulsePoint(const clsVector3& vPoint);
   mmVoid SetSpeedVariance(mmFloat fVary);
   mmVoid SetLifeTime(mmFloat fLifeTime);
   mmVoid SetBouncing(mmFloat fBouncing);
   
   mmVoid Start();
};

#endif // BK_PARTICLESEMITTER_INCLUDED
