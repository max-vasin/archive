// ParticlesEngine.h: interface for the ParticlesEngine class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PARTICLESENGINE_H__2537E50F_2A90_46D2_9F5C_42F777D6D841__INCLUDED_)
#define AFX_PARTICLESENGINE_H__2537E50F_2A90_46D2_9F5C_42F777D6D841__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "..\Amath.h"

#define LPTEXTURE int

class CDevice2_PE
{
public:
	CDevice2_PE();
	virtual ~CDevice2_PE();

	virtual LPTEXTURE CreateTexture(char *n, int a, int mip);
	virtual void DeleteTexture(LPTEXTURE tex);

	virtual void RenderColorVB(float *vb, LPTEXTURE t, int n, D3DXMATRIX *m);
	virtual void RenderColorVB2(float *vb, LPTEXTURE t, int n, D3DXMATRIX *m, int mode);

	virtual void SetPhase(int i);
	virtual int getPhase();

	virtual void * LoadFileToMemory(char *);
	virtual void FreeMemoryOfFile(void *);
	virtual unsigned int GetFileSize();
	virtual int FileExist(char *);
};


class ParticleEngine  
{
private:
	void *List;
public:
	ParticleEngine();
	virtual ~ParticleEngine();

	virtual void __stdcall setDevice(CDevice2_PE *);

	virtual unsigned int __stdcall CreateEmitter(char *filename);
	
	virtual void __stdcall MaskEmitter(unsigned int e, unsigned int mask);
	virtual void __stdcall SpeedEmitter(unsigned int e, float speed);
	virtual void __stdcall ScaleEmitter(unsigned int e, float size);
	virtual void __stdcall ViewEmitter(unsigned int e, D3DXMATRIX *vm);
	virtual void __stdcall MatrixEmitter(unsigned int, D3DXMATRIX *);
	virtual void __stdcall RenderEmitter(unsigned int e);
	virtual void __stdcall ResetEmitter(unsigned int);
	virtual void __stdcall ClearEmitter(unsigned int); // reset automatically !!!
	virtual void __stdcall AnimateEmitter(unsigned int, float);
	virtual void __stdcall DeleteEmitters();
	virtual void __stdcall DeleteEmitter(unsigned int);
// new, oct2006
	virtual void __stdcall StopEmitter(unsigned int);
	virtual unsigned int __stdcall CountEmitter(unsigned int);
// new, jan2007
	virtual int __stdcall LoopCountEmitter(unsigned int);
// new, apr2007
	virtual void __stdcall ColorEmitter(unsigned int e, unsigned int c);
	
};


#define PEDECLARE_STATIC static HINSTANCE particleengine_dll = NULL;\
extern "C"\
{\
ParticleEngine * (WINAPI *CreateParticleEngine)();\
};

#define PEDECLARE_OPEN(name) if ((particleengine_dll=(HINSTANCE)LoadLibrary(name))) {\
       CreateParticleEngine = (ParticleEngine*(__stdcall *)())GetProcAddress(particleengine_dll,"CreateParticleEngine");\
}

#define PEDECLARE_ISOK particleengine_dll

#define PEDECLARE_CLOSE if(particleengine_dll)\
{	FreeLibrary(particleengine_dll);\
	particleengine_dll=0;\
}
	





#endif // !defined(AFX_PARTICLESENGINE_H__2537E50F_2A90_46D2_9F5C_42F777D6D841__INCLUDED_)
