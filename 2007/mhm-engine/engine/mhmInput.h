//==========================================================================================
// Mayhem engine input class
// Coded:	MozG
// Created:	5 july 2006
//==========================================================================================

#ifndef MHMINPUT_INCLUDED
#define MHMINPUT_INCLUDED

//=========================================================
//  Input device types
//=========================================================
enum enInputDeviceType
{
	enIDMouse		= 0,
	enIDKeybord		= 1,
	enIDJoystick	= 2,
	enIDSocket		= 3,
	
	enIDDword		= 0xFFFFFFFF
};

//=========================================================
// Input class
//=========================================================
class clsEngine;
class clsInput
{
protected:
	clsEngine*		m_pengine;
	IDirectInput8*	m_pdi;
	HWND			m_hwnd;
	clsKeyboard*	m_pkeyboard;
	clsMouse*		m_pmouse;
	mmBool			m_initialized;
	mmBool			m_autocapture;
	
public:
	clsInput(clsEngine* pengine);
   ~clsInput();
    
    mmRes Init(HWND hwnd);
    mmRes Shut();
    HWND  GetWindow();
    
    clsKeyboard* GetKeyboard();
    clsMouse*    GetMouse();
   
    mmRes CaptureKeyboard();
    mmRes CaptureMouse();
    
    mmRes FreeKeyboard();
    mmRes FreeMouse();
    
    mmRes Resize();
    mmRes Snapshot();
    clsEngine* GetEngine();
    
    operator IDirectInput8*();

	mmRes AutoCapture(mmBool onoff);
	mmRes AutoCaptureMouse();

	mmRes t_Init();
	void t_Shut();
};

#endif // MHMINPUT_INCLUDED
