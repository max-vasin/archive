//==========================================================================================
// BIOKRIZ creature emitter base class
//==========================================================================================
#ifndef BK_CREATURE_EMITTER_INCLUDED
#define BK_CREATURE_EMITTER_INCLUDED

#include "BKCreature.h"
//#include "BKSucker.h"

class clsBKCreatureEmitter: public clsEntity
{
public:
	clsBKCreature*	m_pFlyer;
	clsBKCreature*	m_pSucker;
	clsBKCreature*	m_pOrc;
	clsBKCreature*	m_pBug;
	
	mmFloat		m_fEmitTime;
	mmFloat		m_fCurrentEmitTime;
	mmUint		m_nEmitCounter;
	
public:
	clsBKCreatureEmitter(clsEntityFactory* pFactory,
						 const clsString& sType,
						 const clsString& sName);
    virtual ~clsBKCreatureEmitter();
    
    mmVoid OnInitialize(mmBool bClone);
    mmVoid OnUpdate(const clsTimer* pTimer);

protected:
	mmVoid EmitCreature();
};
											 

#endif // BK_CREATURE_EMITTER_INCLUDED
