#include "mhmbase.h"
#include "mhmSoundManager.h"

SSDECLARE_STATIC
SS_Kernel *SS;

/* OLD VERSION
static mmDword SoundVolume; // 0..64 !!!
static mmDword StreamVolume; // 0..64 !!!
*/
static mmFloat fSoundVolume=1.0F;
static mmFloat fStreamVolume=1.0F;

clsSound::clsSound(clsString name, mmBool bSmp)
{
	clsString S1fn="";
	S1f = new clsFile(name);

	if(MHMSUCC(S1f->Open()))
	{
		S1fn = S1f->ReadToTemp();
	}
	S1f->NewName(S1fn);

	if(S1fn != "")
	{
		bSample = bSmp;
		if(bSmp)
		{
			sh = SS->SampleCreate(S1fn);
		}
		else
		{
			sh = SS->StreamCreate(S1fn);
		}
	}
	else sh = mmnull;

	S1f->Close();
}

mmVoid clsSound::Play(mmDword id, mmFloat Volume)
{
	if(bSample)
	{
	/*	__try
		{*/
			SS->SamplePlay(sh,id,LinearToDb((mmDword)(Volume*fSoundVolume*65.0f)));
	/*	}
		__except (EXCEPTION_EXECUTE_HANDLER)
        {
			g_plog->Out("clsSound:Play >> ERROR", enLCRed);
           // MessageBox(NULL, "Обработчик исключения %lX\n", "*", MB_OK); 
            //Sleep(2000);
        }*/
	}
	else
	{
		SS->StreamPlay(sh);
		SS->StreamLoop(sh);
		SS->StreamVolume(sh, LinearToDb((mmDword)(Volume*fStreamVolume*65.0f)));
	}
}


mmVoid clsSound::Volume(mmDword Volume)
{
	if(bSample)
	{
	}
	else
	{
		SS->StreamVolume(sh, LinearToDb((mmDword)(fStreamVolume * Volume)));
	}
}

mmVoid clsSound::Volume(mmFloat Volume)
{
	if(bSample)
	{
	}
	else
	{
		SS->StreamVolume(sh, LinearToDb((mmDword)(Volume*fStreamVolume*65.0f)));
	}
}

mmVoid clsSound::Stop(mmDword id)
{
	if(bSample)
	{
		SS->SampleStop(sh,id);
	}
	else
	{
		SS->StreamStop(sh);
	}
}

clsSound::~clsSound()
{
	if(bSample)
	{
		SS->SampleDestroy(sh);
	}
	else
	{
		SS->StreamDestroy(sh);
	}
	S1f->Delete();
	mmSafeDelete(S1f);
}

clsSoundManager::clsSoundManager(void)
{
	SS = mmnull;
	clsString m_tmpname;

	DLL = new clsFile("mhm-snd.dll");

	if(DLL)
	{
		if(MHMSUCC(DLL->Open()))
		{
			m_tmpname = DLL->ReadToTemp();
			DLL->NewName(m_tmpname);
			//DLL->Close();
			SSDECLARE_OPEN(m_tmpname);
			if(SSDECLARE_ISOK)
			{
				SS = CreateSoundServer();
				SS->Init(44100);
			}
		}
//		mmSafeDelete(DLL);
	}
//	SoundVolume = 64;
//	StreamVolume = 64;

	fSoundVolume = 1;
	fStreamVolume = 1;
}

clsSound * clsSoundManager::GetSound(const clsString& Name)
{
	if(SS)
	{
		return new clsSound(Name, mmfalse);
	}
	return mmnull;
}

clsSound * clsSoundManager::GetSample(const clsString& Name)
{
	if(SS)
	{
		return new clsSound(Name, mmtrue);
	}
	return mmnull;
}

//
// V = 0..64 
//
void clsSoundManager::GlobalSoundVolume(const mmDword V)
{
	fSoundVolume = (1.0f/64.0f) * V;
}

//
// V = 0..64 
//
void clsSoundManager::GlobalStreamVolume(const mmDword V)
{
	fSoundVolume = (1.0f/64.0f) * V;
}

void clsSoundManager::GlobalSoundVolume(const mmFloat V)
{
	fSoundVolume = V;
}

void clsSoundManager::GlobalStreamVolume(const mmFloat V)
{
	fSoundVolume = V;
}

void clsSoundManager::GlobalBoostXM(const mmFloat BF)
{
	SS->BoostXM((mmDword)(BF*65536.0f));
}

clsSoundManager::~clsSoundManager(void)
{
	if(SS)
	{
		SS->Free();
		SSDECLARE_CLOSE
	}
	DLL->Delete();
	mmSafeDelete(DLL);
}

mmDword clsSound::LinearToDb(mmDword Volume)// [0..64]
{
	if(Volume>64)Volume=64;
	mmFloat v = 65537 - (1024.0f * Volume);
	mmFloat Db = 20.0f*log10( 1.0f/v );
	Db = (96.0f+Db)/96.0f;
	v = ((1.0f - v/65536.0f)+Db)*0.5f;
	return (mmDword)(v*64.0f);
}
