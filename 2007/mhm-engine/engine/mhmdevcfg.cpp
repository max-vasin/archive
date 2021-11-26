//==========================================================================================
// Mayhem engine configuration holder class implementation
// Coded:	MozG
// Created:	9 may 2006
//==========================================================================================
#include "mhmbase.h"

// Device configuration constructor
//==========================================================================================
clsDeviceConfig::clsDeviceConfig(const mmString ininame)
{
	#ifdef _PINK_FIRE

	m_ininame	= ininame;
	m_windowed	= mmfalse;
	m_width		= 800;
	m_height    = 600;
	m_bbfmt		= D3DFMT_R5G6B5;
	m_dsfmt		= D3DFMT_D24S8;
	m_interval	= D3DPRESENT_INTERVAL_IMMEDIATE;
	m_nSamples  = 0;
	m_bPureDevice = mmfalse;
	m_bHardVertProcessing = mmfalse;
	m_SoundVolume = 0.5f;
	m_StreamVolume = 1.0f;

	#else

	m_ininame	= ininame;
	m_windowed	= mmtrue;
	m_width		= 640;
	m_height    = 480;
	m_bbfmt		= D3DFMT_A8R8G8B8;
	m_dsfmt		= D3DFMT_D24S8;
	m_interval	= D3DPRESENT_INTERVAL_IMMEDIATE;
	m_nSamples  = 0;
	m_bPureDevice = mmfalse;
	m_bHardVertProcessing = mmfalse;
	m_SoundVolume = 0.5f;
	m_StreamVolume = 1.0f;

	#endif


}

// Device configuration destructor
//==========================================================================================
clsDeviceConfig::~clsDeviceConfig()
{
	Save(); // autosave
}

// Load: loads configuration from ini file
//==========================================================================================
mmRes clsDeviceConfig::Load()
{
	clsDeviceConfig trans("");
	clsINIFile		ini(m_ininame);
	
	mmRes res = ini.ReadBool(MHMDEVCFG_DEVICE_SECTION, "Windowed", &trans.m_windowed);
	if (res != mmOk)
		return res;
	res = ini.ReadInt(MHMDEVCFG_DEVICE_SECTION, "Width", (mmInt*)&trans.m_width);
	if (res != mmOk)
		return res;
	res = ini.ReadInt(MHMDEVCFG_DEVICE_SECTION, "Height", (mmInt*)&trans.m_height);
	if (res != mmOk)
		return res;
	
	clsString fmtstr;
	res = ini.ReadString(MHMDEVCFG_DEVICE_SECTION, "BackBufferFormat", fmtstr);
	if (res != mmOk)
		return res;	
	res = clsDXUtils::StringToFormat(fmtstr, trans.m_bbfmt);
	if (res != mmOk)
		return res;
	res = ini.ReadString(MHMDEVCFG_DEVICE_SECTION, "DepthStencilFormat", fmtstr);
	if (res != mmOk)
		return res;	
	res = clsDXUtils::StringToFormat(fmtstr, trans.m_dsfmt);
	if (res != mmOk)
		return res;
	res = ini.ReadString(MHMDEVCFG_DEVICE_SECTION, "Interval", fmtstr);
	if (MHMFAIL(res))
		return res;
	trans.m_interval = clsDXUtils::StringToInterval(fmtstr);
	
	res = ini.ReadInt(MHMDEVCFG_DEVICE_SECTION, "Samples", (mmInt*)&trans.m_nSamples);
	if (MHMFAIL(res))
		return res;
	
	res = ini.ReadBool(MHMDEVCFG_DEVICE_SECTION, "PureDevice", &trans.m_bPureDevice);
	if (MHMFAIL(res))
		return res;
	
	res = ini.ReadBool(MHMDEVCFG_DEVICE_SECTION, "HardVertexProcessing", &trans.m_bHardVertProcessing);
	if (MHMFAIL(res))
		return res;

	res = ini.ReadFloat(MHMDEVCFG_DEVICE_SECTION, "SoundVolume", &m_SoundVolume);
	if (MHMFAIL(res))
		return res;

	res = ini.ReadFloat(MHMDEVCFG_DEVICE_SECTION, "StreamVolume", &m_StreamVolume);
	if (MHMFAIL(res))
		return res;

	// transaction completed
	m_windowed = trans.m_windowed;
	m_width    = trans.m_width;
	m_height   = trans.m_height;
	m_bbfmt    = trans.m_bbfmt;
	m_dsfmt    = trans.m_dsfmt;
	m_interval = trans.m_interval;
	m_nSamples = trans.m_nSamples;
	m_bPureDevice = trans.m_bPureDevice;
	m_bHardVertProcessing = trans.m_bHardVertProcessing;
	return mmOk;
}

// Save: saves current config to file
//==========================================================================================
mmRes clsDeviceConfig::Save()
{
	clsINIFile		ini(m_ininame);
	mmRes res = ini.WriteBool(MHMDEVCFG_DEVICE_SECTION, "Windowed", m_windowed);
	if (res != mmOk)
		return res;
	res = ini.WriteInt(MHMDEVCFG_DEVICE_SECTION, "Width", (mmInt)m_width);
	if (res != mmOk)
		return res;
	res = ini.WriteInt(MHMDEVCFG_DEVICE_SECTION, "Height", (mmInt)m_height);
	if (res != mmOk)
		return res;	
	clsString fmtstr;
	res = clsDXUtils::FormatToString(fmtstr, m_bbfmt);
	if (res != mmOk)
		return res;
	res = ini.WriteString(MHMDEVCFG_DEVICE_SECTION, "BackBufferFormat", fmtstr);
	if (res != mmOk)
		return res;	
	res = clsDXUtils::FormatToString(fmtstr, m_dsfmt);
	if (res != mmOk)
		return res;
	res = ini.WriteString(MHMDEVCFG_DEVICE_SECTION, "DepthStencilFormat", fmtstr);
	if (res != mmOk)
		return res;	
	res = ini.WriteString(MHMDEVCFG_DEVICE_SECTION, "Interval", clsDXUtils::IntervalToString(m_interval));
	if (res != mmOk)
		return res;
	res = ini.WriteInt(MHMDEVCFG_DEVICE_SECTION, "Samples", m_nSamples);
	if (MHMFAIL(res))
		return res;
	res = ini.WriteBool(MHMDEVCFG_DEVICE_SECTION, "PureDevice", m_bPureDevice);
	if (MHMFAIL(res))
		return res;
	res = ini.WriteBool(MHMDEVCFG_DEVICE_SECTION, "HardVertexProcessing", m_bHardVertProcessing);
	if (MHMFAIL(res))
		return res;
	res = ini.WriteFloat(MHMDEVCFG_DEVICE_SECTION, "SoundVolume", m_SoundVolume);
	if (MHMFAIL(res))
		return res;
	res = ini.WriteFloat(MHMDEVCFG_DEVICE_SECTION, "StreamVolume", m_StreamVolume);
	if (MHMFAIL(res))
		return res;
	return mmOk;
}



