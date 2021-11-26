//==========================================================================================
// Mayhem device configuration holder class
// Coded:	MozG
// Created:	9 may 2006
//==========================================================================================

#ifndef _MHMDEVCFG_INCLUDED
#define _MHMDEVCFG_INCLUDED

#define MHMDEVCFG_DEVICE_SECTION	"Device"
#define MHMDEVCFG_INVALID_ARGS		mmOk + 1

class clsDeviceConfig
{
friend class clsDevice;
protected:
public:
	clsString	m_ininame;
	mmBool		m_windowed;	// windowed device flag
	mmUint		m_width;	// fullscreen device width
	mmUint		m_height;	// fullscreen device height
	D3DFORMAT	m_bbfmt;	// back buffer format
	D3DFORMAT	m_dsfmt;	// depth-stencil format
	mmDword		m_interval;	// presentation interval (vsync)
	mmUint		m_nSamples;	// multisample samples
	mmBool		m_bPureDevice;
	mmBool		m_bHardVertProcessing;
	mmFloat		m_SoundVolume;
	mmFloat		m_StreamVolume;

//public:
	clsDeviceConfig(const mmString ininame);
   ~clsDeviceConfig();
    
    mmRes Load();
    mmRes Save();
};



#endif // _MHMDEVCFG_INCLUDED