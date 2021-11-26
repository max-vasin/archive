//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
USERES("TColorGradient.res");
USEPACKAGE("vcl50.bpi");
USEUNIT("ColorGradient.cpp");
USEUNIT("..\TSplineEditor\SplineEditor.cpp");
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------

//   Package source.
//---------------------------------------------------------------------------

#pragma argsused
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void*)
{
	return 1;
}
//---------------------------------------------------------------------------
