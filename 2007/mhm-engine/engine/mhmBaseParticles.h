//==========================================================================================
// Mayhem engine particles resource class
// Coded:	fyrex
// Created:	5 august 2006

//==========================================================================================
#ifndef MHMPARTICLES_INCLUDED
#define MHMPARTICLES_INCLUDED

//========================================================================
// Particles emitter, used to spawn particles
//========================================================================
class clsBaseParticles: public clsResource
{
friend class clsBaseParticlesProxy;
protected:
	mmDword		m_Handle;	
	mmFloat		m_fEmitterScale;
	mmBool		m_resize;
public:
	clsBaseParticles(clsResourceManager* presman, clsString name, enResType type = enResBaseParticles);
    virtual ~clsBaseParticles();

    virtual mmRes Create(clsResourceParams* params);
   	virtual mmRes Render();
   	virtual clsResource* OnAddReference();
	void    SetColor(mmDword c);

//    virtual mmRes Activate(clsResource* powner = mmnull);
//    virtual mmRes Deactivate(clsResource* powner = mmnull);
	mmVoid SetEmitterScale(mmFloat fScale);
	mmVoid ResetEmitter();
	mmVoid ClearEmitter();
	mmVoid StopEmitter();
	mmUint CountEmitter();
	mmBool LoopEmitter();

};

extern void PEDLL_Open(clsDevice *);
extern void PEDLL_Close();

#endif // MHMPARTICLES_INCLUDED