//==========================================================================================
// Mayhem engine 3DS Max tools plugin
// Coded:	MozG
// Created:	1 june 2006
//==========================================================================================
#include "mhmtool.h"

extern ClassDesc* GetMayhemToolDesc();

HINSTANCE	g_hinst	   = NULL;
bool		g_ctrlinit = false;

// GetString: return string resource by ID
//==========================================================================================
TCHAR* GetString(int id)
{
	static TCHAR buf[256];
	if (g_hinst)
		return LoadString(g_hinst, id, buf, 256) ? buf : NULL;
	return NULL;	
}

// DLL main
//==========================================================================================
BOOL WINAPI DllMain(HINSTANCE hinst, ULONG reason, LPVOID reserved)
{
	g_hinst = hinst;
	if (!g_ctrlinit)
	{
		InitCustomControls(g_hinst);
		InitCommonControls();
	}
	switch(reason)
	{
	case DLL_PROCESS_ATTACH:
		break;
	case DLL_THREAD_ATTACH:
		break;
	case DLL_THREAD_DETACH:
		break;
	case DLL_PROCESS_DETACH:
		break;
	};
	return TRUE;
}

//==========================================================================================
// Plugin max interface
//==========================================================================================

__declspec(dllexport) const TCHAR* LibDescription()
{
	return GetString(IDS_LIBDESC);
}

__declspec(dllexport) int LibNumberClasses()
{
	return 1;
}

__declspec(dllexport) ClassDesc* LibClassDesc(int i)
{
	switch(i)
	{
	case 0: return GetMayhemToolDesc();
	}
	return NULL;
}

__declspec(dllexport) ULONG LibVersion()
{
	return VERSION_3DSMAX; 
}

__declspec(dllexport) ULONG CanAutoDefer()
{
	return 1;
}

