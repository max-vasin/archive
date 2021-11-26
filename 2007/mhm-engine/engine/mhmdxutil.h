//==========================================================================================
// Mayhem engine DirectX utilities static class
// Coded:	MozG
// Created:	9 may 2006
//==========================================================================================

#ifndef _MHMDXUTIL_INCLUDED
#define _MHMDXUTIL_INCLUDED


// Error codes
#define MHMDXUTIL_INVALID_ARGS	mmOk + 1

class clsDXUtils
{
public:
	static mmRes     FormatToString(clsString& str, D3DFORMAT& fmt);
	static mmRes     StringToFormat(clsString& str, D3DFORMAT& fmt);
	static mmDword   StringToInterval(const clsString& sValue);
	static clsString IntervalToString(mmDword interval);
	static mmUint    GetFVFSize(mmDword fvf);
};



#endif //_MHMDXUTIL_INCLUDED