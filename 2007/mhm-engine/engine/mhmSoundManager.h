#pragma once

#include "SS_Kernel.h"

class clsSound
{
private:
	clsFile * S1f;
	SS_HANDLE sh;
	mmBool	bSample;
public:
	clsSound(clsString name, mmBool bSample);
	mmVoid Play(mmDword id=0, mmFloat Volume=1.0f);
	mmVoid Stop(mmDword id=0);
	mmVoid Volume(mmDword Volume);
	mmVoid Volume(mmFloat Volume); 

	mmDword LinearToDb(mmDword Volume);// [0..64]

	~clsSound();
};

class clsSoundManager
{
private:
	//clsString m_tmpname;
	clsFile * DLL;
public:
	clsSoundManager(void);
	~clsSoundManager(void);
	clsSound * GetSound(const clsString & Name);
	clsSound * GetSample(const clsString& Name);
	void GlobalSoundVolume(const mmDword V);
	void GlobalStreamVolume(const mmDword V);
	void GlobalSoundVolume(const mmFloat V);
	void GlobalStreamVolume(const mmFloat V);
	void GlobalBoostXM(const float BF);
};
