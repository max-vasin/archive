//==========================================================================================
// BIOKRIZ particles entity
//==========================================================================================
#ifndef BK_PARTICLES_INCLUDED
#define BK_PARTICLES_INCLUDED

class clsBKSpriteParticles:public clsEntity
{
protected:
	mmBool	m_bDeathCounter;
	mmFloat	m_fDeathTime;
	mmFloat m_fCurTime;
	
public:
	clsBKSpriteParticles(clsEntityFactory* pFactory,
				   const clsString& sType,
				   const clsString& sName);
	virtual ~clsBKSpriteParticles();
	
	mmVoid OnUpdate(const clsTimer* pTimer);
	
	mmVoid SetDeathCounter(mmFloat fSecs);
};

#endif // BK_PARTICLES_INCLUDED
