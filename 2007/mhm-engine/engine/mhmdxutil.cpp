//==========================================================================================
// Mayhem engine DirectX utilities class implementation
// Coded:	MozG
// Created:	9 may 2006
//==========================================================================================
#include "mhmbase.h"

// FormatToString: converts Direct3D format value into string
//==========================================================================================
mmRes clsDXUtils::FormatToString(clsString &str, D3DFORMAT &fmt)
{
	switch(fmt)
	{
		case D3DFMT_UNKNOWN:			str = "D3DFMT_UNKNOWN";			break;           
	    case D3DFMT_R8G8B8:				str = "D3DFMT_R8G8B8";			break;
		case D3DFMT_A8R8G8B8:			str = "D3DFMT_A8R8G8B8";		break;
		case D3DFMT_X8R8G8B8:			str = "D3DFMT_X8R8G8B8";		break;
		case D3DFMT_R5G6B5:				str = "D3DFMT_R5G6B5";			break;
		case D3DFMT_X1R5G5B5:			str = "D3DFMT_X1R5G5B5";		break;
		case D3DFMT_A1R5G5B5:			str = "D3DFMT_A1R5G5B5";		break;
		case D3DFMT_A4R4G4B4:			str = "D3DFMT_A4R4G4B4";		break;
		case D3DFMT_R3G3B2:             str = "D3DFMT_R3G3B2";			break; 
		case D3DFMT_A8:					str = "D3DFMT_A8";				break;
		case D3DFMT_A8R3G3B2:			str = "D3DFMT_A8R3G3B2";		break;
		case D3DFMT_X4R4G4B4:			str = "D3DFMT_X4R4G4B4";		break;
//		case D3DFMT_A2B10G10R10:		str = "D3DFMT_A2B10G10R10";		break;
//		case D3DFMT_G16R16:				str = "D3DFMT_G16R16";			break;
		case D3DFMT_A8P8:				str = "D3DFMT_A8P8";			break;
		case D3DFMT_P8:					str = "D3DFMT_P8";				break;
		case D3DFMT_L8:					str = "D3DFMT_L8";				break;
		case D3DFMT_A8L8:				str = "D3DFMT_A8L8";			break;
		case D3DFMT_A4L4:				str = "D3DFMT_A4L4";			break;
		case D3DFMT_V8U8:				str = "D3DFMT_V8U8";			break;
		case D3DFMT_L6V5U5:				str = "D3DFMT_L6V5U5";			break; 
		case D3DFMT_X8L8V8U8:			str = "D3DFMT_X8L8V8U8";		break;
		case D3DFMT_Q8W8V8U8:			str = "D3DFMT_Q8W8V8U8";		break;
		case D3DFMT_V16U16:				str = "D3DFMT_V16U16";			break;
//		case D3DFMT_A2W10V10U10:		str = "D3DFMT_A2W10V10U10";		break;
		case D3DFMT_UYVY:				str = "D3DFMT_UYVY";			break;
		case D3DFMT_YUY2:				str = "D3DFMT_YUY2";			break;
		case D3DFMT_DXT1:				str = "D3DFMT_DXT1";			break;
		case D3DFMT_DXT2:				str = "D3DFMT_DXT2";			break;
		case D3DFMT_DXT3:				str = "D3DFMT_DXT3";			break;
		case D3DFMT_DXT4:				str = "D3DFMT_DXT4";			break;
		case D3DFMT_DXT5:				str = "D3DFMT_DXT5";			break;
		case D3DFMT_D16_LOCKABLE:		str = "D3DFMT_D16_LOCKABLE";	break;
		case D3DFMT_D32:				str = "D3DFMT_D32";				break;
		case D3DFMT_D15S1:				str = "D3DFMT_D15S1";			break; 
		case D3DFMT_D24S8:				str = "D3DFMT_D24S8";			break;
		case D3DFMT_D24X8:				str = "D3DFMT_D24X8";			break;
		case D3DFMT_D24X4S4:			str = "D3DFMT_D24X4S4";			break;
		case D3DFMT_D16:				str = "D3DFMT_D16";				break;
		case D3DFMT_VERTEXDATA:			str = "D3DFMT_VERTEXDATA";		break; 
		case D3DFMT_INDEX16:			str = "D3DFMT_INDEX16";			break;
		case D3DFMT_INDEX32:			str = "D3DFMT_INDEX32";			break;

#ifndef MHM_USE_DX8
		case D3DFMT_A8B8G8R8:			str = "D3DFMT_A8B8G8R8";		break;
		case D3DFMT_X8B8G8R8:			str = "D3DFMT_X8B8G8R8";		break;
		case D3DFMT_A2R10G10B10:		str = "D3DFMT_A2R10G10B10";		break; 
		case D3DFMT_A16B16G16R16:		str = "D3DFMT_A16B16G16R16";	break;
		case D3DFMT_R8G8_B8G8:			str = "D3DFMT_R8G8_B8G8";		break;
		case D3DFMT_G8R8_G8B8:			str = "D3DFMT_G8R8_G8B8";		break; 
		case D3DFMT_D32F_LOCKABLE:		str = "D3DFMT_D32F_LOCKABLE";	break;
		case D3DFMT_D24FS8:				str = "D3DFMT_D24FS8";			break;
		case D3DFMT_L16:				str = "D3DFMT_L16";				break;
		case D3DFMT_Q16W16V16U16:		str = "D3DFMT_Q16W16V16U16";	break;
		case D3DFMT_MULTI2_ARGB8:		str = "D3DFMT_MULTI2_ARGB8";	break;
		case D3DFMT_R16F:				str = "D3DFMT_R16F";			break;
		case D3DFMT_G16R16F:			str = "D3DFMT_G16R16F";			break;
		case D3DFMT_A16B16G16R16F:		str = "D3DFMT_A16B16G16R16F";	break;
		case D3DFMT_R32F:               str = "D3DFMT_R32F";			break;
		case D3DFMT_G32R32F:			str = "D3DFMT_G32R32F";			break;
		case D3DFMT_A32B32G32R32F:		str = "D3DFMT_A32B32G32R32F";	break;
		case D3DFMT_CxV8U8:				str = "D3DFMT_CxV8U8";			break;
#endif // MHM_USE_DX8
		default: return MHMDXUTIL_INVALID_ARGS;
	}
	return mmOk;
}

// StringToFormat: converts string value to Direct3D format value
//==========================================================================================
mmRes clsDXUtils::StringToFormat(clsString &str, D3DFORMAT &fmt)
{
	if (str == "D3DFMT_UNKNOWN")		{fmt = D3DFMT_UNKNOWN;			return mmOk;}
	if (str == "D3DFMT_R8G8B8")			{fmt = D3DFMT_R8G8B8;			return mmOk;}
	if (str == "D3DFMT_A8R8G8B8")		{fmt = D3DFMT_A8R8G8B8;			return mmOk;}
	if (str == "D3DFMT_X8R8G8B8")		{fmt = D3DFMT_X8R8G8B8;			return mmOk;}
	if (str == "D3DFMT_R5G6B5")			{fmt = D3DFMT_R5G6B5;			return mmOk;}
	if (str == "D3DFMT_X1R5G5B5")		{fmt = D3DFMT_X1R5G5B5;			return mmOk;}
	if (str == "D3DFMT_A1R5G5B5")		{fmt = D3DFMT_A1R5G5B5;			return mmOk;}
	if (str == "D3DFMT_A4R4G4B4")		{fmt = D3DFMT_A4R4G4B4;			return mmOk;}
	if (str == "D3DFMT_R3G3B2")			{fmt = D3DFMT_R3G3B2;         	return mmOk;}
	if (str == "D3DFMT_A8")				{fmt = D3DFMT_A8;				return mmOk;}
	if (str == "D3DFMT_A8R3G3B2")		{fmt = D3DFMT_A8R3G3B2;			return mmOk;}
	if (str == "D3DFMT_X4R4G4B4")		{fmt = D3DFMT_X4R4G4B4;			return mmOk;}
//	if (str == "D3DFMT_A2B10G10R10")	{fmt = D3DFMT_A2B10G10R10;		return mmOk;}
//	if (str == "D3DFMT_G16R16")			{fmt = D3DFMT_G16R16;			return mmOk;}
	if (str == "D3DFMT_A8P8")			{fmt = D3DFMT_A8P8;				return mmOk;}
	if (str == "D3DFMT_P8")				{fmt = D3DFMT_P8;				return mmOk;}
	if (str == "D3DFMT_L8")				{fmt = D3DFMT_L8;				return mmOk;}
	if (str == "D3DFMT_A8L8")			{fmt = D3DFMT_A8L8;				return mmOk;}
	if (str == "D3DFMT_A4L4")			{fmt = D3DFMT_A4L4;				return mmOk;}
	if (str == "D3DFMT_V8U8")			{fmt = D3DFMT_V8U8;				return mmOk;}
	if (str == "D3DFMT_L6V5U5")			{fmt = D3DFMT_L6V5U5;			return mmOk;}
	if (str == "D3DFMT_X8L8V8U8")		{fmt = D3DFMT_X8L8V8U8;			return mmOk;}
	if (str == "D3DFMT_Q8W8V8U8")		{fmt = D3DFMT_Q8W8V8U8;			return mmOk;}
	if (str == "D3DFMT_V16U16")			{fmt = D3DFMT_V16U16;			return mmOk;}
//	if (str == "D3DFMT_A2W10V10U10")	{fmt = D3DFMT_A2W10V10U10;		return mmOk;}
	if (str == "D3DFMT_UYVY")			{fmt = D3DFMT_UYVY;				return mmOk;}
	if (str == "D3DFMT_YUY2")			{fmt = D3DFMT_YUY2;				return mmOk;}
	if (str == "D3DFMT_DXT1")			{fmt = D3DFMT_DXT1;				return mmOk;}
	if (str == "D3DFMT_DXT2")			{fmt = D3DFMT_DXT2;				return mmOk;}
	if (str == "D3DFMT_DXT3")			{fmt = D3DFMT_DXT3;				return mmOk;}
	if (str == "D3DFMT_DXT4")			{fmt = D3DFMT_DXT4;				return mmOk;}
	if (str == "D3DFMT_DXT5")			{fmt = D3DFMT_DXT5;				return mmOk;}
	if (str == "D3DFMT_D16_LOCKABLE")	{fmt = D3DFMT_D16_LOCKABLE;		return mmOk;}
	if (str == "D3DFMT_D32")			{fmt = D3DFMT_D32;				return mmOk;}
	if (str == "D3DFMT_D15S1")			{fmt = D3DFMT_D15S1;			return mmOk;}
	if (str == "D3DFMT_D24S8")			{fmt = D3DFMT_D24S8;			return mmOk;}
	if (str == "D3DFMT_D24X8")			{fmt = D3DFMT_D24X8;			return mmOk;}
	if (str == "D3DFMT_D24X4S4")		{fmt = D3DFMT_D24X4S4;			return mmOk;}
	if (str == "D3DFMT_D16")			{fmt = D3DFMT_D16;				return mmOk;}
	if (str == "D3DFMT_VERTEXDATA")		{fmt = D3DFMT_VERTEXDATA;		return mmOk;}
	if (str == "D3DFMT_INDEX16")		{fmt = D3DFMT_INDEX16;			return mmOk;}
	if (str == "D3DFMT_INDEX32")		{fmt = D3DFMT_INDEX32;			return mmOk;}

#ifndef MHM_USE_DX8
	if (str == "D3DFMT_A8B8G8R8")		{fmt = D3DFMT_A8B8G8R8;			return mmOk;}
	if (str == "D3DFMT_X8B8G8R8")		{fmt = D3DFMT_X8B8G8R8;			return mmOk;}
	if (str == "D3DFMT_A2R10G10B10")	{fmt = D3DFMT_A2R10G10B10;		return mmOk;}
	if (str == "D3DFMT_A16B16G16R16")	{fmt = D3DFMT_A16B16G16R16;		return mmOk;}
	if (str == "D3DFMT_R8G8_B8G8")		{fmt = D3DFMT_R8G8_B8G8;		return mmOk;}
	if (str == "D3DFMT_G8R8_G8B8")		{fmt = D3DFMT_G8R8_G8B8;		return mmOk;}
	if (str == "D3DFMT_D32F_LOCKABLE")	{fmt = D3DFMT_D32F_LOCKABLE;	return mmOk;}
	if (str == "D3DFMT_D24FS8")			{fmt = D3DFMT_D24FS8;			return mmOk;}
	if (str == "D3DFMT_L16")			{fmt = D3DFMT_L16;				return mmOk;}
	if (str == "D3DFMT_Q16W16V16U16")	{fmt = D3DFMT_Q16W16V16U16;		return mmOk;}
	if (str == "D3DFMT_MULTI2_ARGB8")	{fmt = D3DFMT_MULTI2_ARGB8;		return mmOk;}
	if (str == "D3DFMT_R16F")			{fmt = D3DFMT_R16F;				return mmOk;}
	if (str == "D3DFMT_G16R16F")		{fmt = D3DFMT_G16R16F;			return mmOk;}
	if (str == "D3DFMT_A16B16G16R16F")	{fmt = D3DFMT_A16B16G16R16F;	return mmOk;}
	if (str == "D3DFMT_R32F")			{fmt = D3DFMT_R32F;           	return mmOk;}
	if (str == "D3DFMT_G32R32F")		{fmt = D3DFMT_G32R32F;			return mmOk;}
	if (str == "D3DFMT_A32B32G32R32F")	{fmt = D3DFMT_A32B32G32R32F;	return mmOk;}
	if (str == "D3DFMT_CxV8U8")			{fmt = D3DFMT_CxV8U8;			return mmOk;}
#endif // MHM_USE_DX8

	return MHMDXUTIL_INVALID_ARGS;
}

// GetFVFSize: returns flexible vertex format size
//==========================================================================================
mmUint clsDXUtils::GetFVFSize(mmDword fvf)
{
	mmUint size = 0;
	if (fvf & D3DFVF_DIFFUSE)	size += sizeof(mmDword);
	if (fvf & D3DFVF_SPECULAR)	size += sizeof(mmDword);
	if (fvf & D3DFVF_XYZ)		size += sizeof(mmFloat) * 3;
	if (fvf & D3DFVF_NORMAL)	size += sizeof(mmFloat) * 3;
	size += sizeof(mmFloat) * ((fvf & 0xF00) >> 8) * 2;
//	if (fvf & D3DFVF_XYZB1)		size += sizeof(mmFloat) * 4;
//	if (fvf & D3DFVF_XYZB2)		size += sizeof(mmFloat) * 5;
//	if (fvf & D3DFVF_XYZB3)		size += sizeof(mmFloat) * 6;
//	if (fvf & D3DFVF_XYZB4)		size += sizeof(mmFloat) * 7;
//	if (fvf & D3DFVF_XYZB5)		size += sizeof(mmFloat) * 8;
	if (fvf & D3DFVF_XYZRHW)	size += sizeof(mmFloat) * 4;
//	if (fvf & D3DFVF_XYZW)		size += sizeof(mmFloat) * 4;
	return size;
}

// GetIntervalFromString: returns interval from string
//==========================================================================================
mmDword clsDXUtils::StringToInterval(const clsString& sValue)
{
	clsString sCopy(sValue);
	sCopy.UpperCase();
	if      (sCopy == "DEFAULT")
		return D3DPRESENT_INTERVAL_DEFAULT;
	else if (sCopy == "ONE")
		return D3DPRESENT_INTERVAL_ONE;
	else if (sCopy == "TWO")
		return D3DPRESENT_INTERVAL_TWO;
	else if (sCopy == "THREE")
		return D3DPRESENT_INTERVAL_THREE;
	else if (sCopy == "FOUR")
		return D3DPRESENT_INTERVAL_FOUR;
	else if (sCopy == "IMMEDIATE")
		return D3DPRESENT_INTERVAL_IMMEDIATE;
	return D3DPRESENT_INTERVAL_IMMEDIATE;
	
}

// IntervalToString: returns string interval
//==========================================================================================
clsString clsDXUtils::IntervalToString(mmDword interval)
{
	switch(interval)
	{
	case D3DPRESENT_INTERVAL_DEFAULT:
		return "Default";
	case D3DPRESENT_INTERVAL_ONE:
		return "One";
	case D3DPRESENT_INTERVAL_TWO:
		return "Two";
	case D3DPRESENT_INTERVAL_THREE:
		return "Three";
	case D3DPRESENT_INTERVAL_FOUR:
		return "Four";
	case D3DPRESENT_INTERVAL_IMMEDIATE:
		return "Immediate";
	}
	return "Immediate";
}

