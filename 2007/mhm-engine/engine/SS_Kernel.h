// SS_Kernel.h: interface for the SS_Kernel class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SS_KERNEL_H__B9726ABB_E7AB_44F9_A370_779617B646B8__INCLUDED_)
#define AFX_SS_KERNEL_H__B9726ABB_E7AB_44F9_A370_779617B646B8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define SS_HANDLE unsigned long
#ifndef DWORD
#define DWORD unsigned long
#endif



class SS_Kernel  
{
public:
	virtual void _stdcall SetMainKernel(void *);

	virtual int __stdcall IsSoundPlaying(SS_HANDLE ssh);
	virtual void __stdcall StreamLoop(SS_HANDLE ssh);
	virtual void __stdcall StreamStop(SS_HANDLE ssh);
	virtual void __stdcall StreamPlay(SS_HANDLE ssh);
	virtual void __stdcall StreamDestroy(SS_HANDLE ssh);
	virtual SS_HANDLE __stdcall StreamCreate(char *filename);
	virtual void __stdcall Free();
	virtual int __stdcall Init(unsigned int Hz);

	virtual SS_HANDLE __stdcall StreamGenerator( void*, unsigned int);

	SS_Kernel();
	virtual ~SS_Kernel();

	virtual		SS_HANDLE	SampleCreate(char *filename);
	virtual		void	SamplePlay(SS_HANDLE ssh, unsigned int id, unsigned int Volume);
	virtual		void	SampleStop(SS_HANDLE ssh, unsigned int id);
	virtual		void	SampleDestroy(SS_HANDLE ssh);

	virtual void __stdcall StreamVolume(SS_HANDLE ssh, unsigned int Volume);

	//   65536 = 1.0
	// 0x20000 = 2.0 etc
	//
	virtual void __stdcall BoostXM(unsigned int);

	
private:
	void Clear();
	void SetPrimaryBufferFormat( DWORD dwPrimaryChannels, 
                                               DWORD dwPrimaryFreq, 
                                               DWORD dwPrimaryBitRate );
};
/*
#define SSLOADFUNCTION(f,s) {\
		register char *func=(char*)(s);\
		register void *x=(void*)(GetProcAddress);\
		__asm {push func}\
		__asm {push soundserver_dll}\
		__asm {mov eax,x}\
		__asm {call eax}\
		__asm {mov f,eax}\
	}
*/
#define SSDECLARE_STATIC static HINSTANCE soundserver_dll = NULL;\
extern "C"\
{\
SS_Kernel * (WINAPI *CreateSoundServer)();\
};

#define SSDECLARE_OPEN(name) if ((soundserver_dll=(HINSTANCE)LoadLibrary(name))) {\
       CreateSoundServer = (SS_Kernel*(__stdcall *)())GetProcAddress(soundserver_dll,"CreateSoundServer");\
}

#define SSDECLARE_ISOK soundserver_dll

#define SSDECLARE_CLOSE if(soundserver_dll)\
{	FreeLibrary(soundserver_dll);\
	soundserver_dll=0;\
}
	



#endif // !defined(AFX_SS_KERNEL_H__B9726ABB_E7AB_44F9_A370_779617B646B8__INCLUDED_)
