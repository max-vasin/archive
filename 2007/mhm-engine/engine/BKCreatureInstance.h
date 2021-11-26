//==========================================================================================
// BIOKRIZ creature emitter instances class
//==========================================================================================
#ifndef BK_CREATURE_INSTANCE_INCLUDED
#define BK_CREATURE_INSTANCE_INCLUDED

class clsBKCreatureInstance: public clsEntity
{
public:
	// creature basic parameters
	clsSound*		m_pHitScream;
//	clsResource*	m_pBloodParticles;

public:
	clsBKCreatureInstance(clsEntityFactory* pFactory, 
						  const clsString& sType,
						  const clsString& sName);
   ~clsBKCreatureInstance();
    
    mmVoid OnInitialize(mmBool bClone);
    mmVoid OnPostLoading();
}; 

#endif // BK_CREATURE_INSTANCE_INCLUDED
