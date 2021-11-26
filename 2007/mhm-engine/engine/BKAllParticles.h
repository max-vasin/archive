//==========================================================================================
// BIOKRIZ all particles definitions
//==========================================================================================
#ifndef BK_ALLPARTICLES_INCLUDED
#define BK_ALLPARTICLES_INCLUDED

class clsBKMeatParticleEmitter: public clsBKParticleEmitter
{
public:
	clsBKMeatParticleEmitter(clsEntityFactory* pFactory,
							 const clsString& sType,
							 const clsString& sName);
   ~clsBKMeatParticleEmitter();
   
    mmVoid OnPostLoading();	
};


#endif //BK_ALLPARTICLES_INCLUDED

