//==========================================================================================
// Mayhem engine input keyboard device class
// Coded:	MozG
// Created:	5 july 2006
//==========================================================================================

#ifndef MHMKEYBOARD_INLUDED
#define MHMKEYBOARD_INLUDED

#define MHM_KEYBOARD_SAMPLER_SIZE	256

//==================================
// Keyboard scan codes
//==================================
#pragma region Keyboard scan codes
enum enKeyScanCode
{
	enKeyEscape		= 0x01,	enKey1			= 0x02,	enKey2			= 0x03,	
	enKey3			= 0x04,	enKey4			= 0x05,	enKey5			= 0x06,
	enKey6			= 0x07,	enKey7			= 0x08,	enKey8			= 0x09,
	enKey9			= 0x0A,	enKey0			= 0x0B,	enKeyMinus		= 0x0C,
	enKeyEquals		= 0x0D,	enKeyBack		= 0x0E,	enKeyTab		= 0x0F,
	enKeyQ			= 0x10,	enKeyW			= 0x11,	enKeyE			= 0x12,
	enKeyR			= 0x13,	enKeyT			= 0x14,	enKeyY			= 0x15,
	enKeyU			= 0x16,	enKeyI			= 0x17,	enKeyO			= 0x18,
	enKeyP			= 0x19,	enKeyLBracket	= 0x1A,	enKeyRBracket	= 0x1B,
	enKeyReturn		= 0x1C,	enKeyLControl	= 0x1D,	enKeyA			= 0x1E,
	enKeyS			= 0x1F,	enKeyD			= 0x20,	enKeyF			= 0x21,
	enKeyG			= 0x22,	enKeyH			= 0x23,	enKeyJ			= 0x24,
	enKeyK			= 0x25,	enKeyL			= 0x26,	enKeySemicolon  = 0x27,
	enKeyApostrophe = 0x28,	enKeyGrave		= 0x29,	enKeyLShift		= 0x2A,
	enKeyBackSlash	= 0x2B,	enKeyZ			= 0x2C,	enKeyX			= 0x2D,
	enKeyC			= 0x2E,	enKeyV			= 0x2F,	enKeyB			= 0x30,
	enKeyN			= 0x31,	enKeyM			= 0x32,	enKeyComma		= 0x33,
	enKeyPeriod		= 0x34,	enKeySlash		= 0x35,	enKeyRShift		= 0x36,
	enKeyMultiply	= 0x37,	enKeyLAlt		= 0x38,	enKeySpace		= 0x39,
	enKeyCapital	= 0x3A,	enKeyF1			= 0x3B,	enKeyF2			= 0x3C,
	enKeyF3			= 0x3D,	enKeyF4			= 0x3E,	enKeyF5			= 0x3F,
	enKeyF6			= 0x40,	enKeyF7			= 0x41,	enKeyF8			= 0x42,
	enKeyF9			= 0x43,	enKeyF10		= 0x44,	enKeyNumlock	= 0x45,
	enKeyScrollLock	= 0x46,	enKeyNumpad7	= 0x47,	enKeyNumpad8	= 0x48,
	enKeyNumpad9	= 0x49,	enKeyNumpadMinus= 0x4A,	enKeyNumpad4	= 0x4B,
	enKeyNumpad5	= 0x4C,	enKeyNumpad6	= 0x4D,	enKeyNumpadPlus	= 0x4E,
	enKeyNumpad1	= 0x4F,	enKeyNumpad2	= 0x50,	enKeyNumpad3	= 0x51,
	enKeyNumpad0	= 0x52,	enKeyNumpadDot	= 0x53,	enKeyOEM102		= 0x56,
	enKeyF11		= 0x57,	enKeyF12		= 0x58,	enKeyF13		= 0x64,
	enKeyF14		= 0x65,	enKeyF15		= 0x66,	enKeyNumpapEquals = 0x8D,
	enKeyNumpadEnter= 0x9C, enKeyRControl	= 0x9D,	enKeyNumpadDivide = 0xB5,
	enKeySysRQ		= 0xB7,	enKeyRAlt		= 0xB8,	enKeyPause		= 0xC5,
	enKeyHome		= 0xC7,	enKeyUp			= 0xC8,	enKeyPageUp		= 0xC9,
	enKeyLeft		= 0xCB,	enKeyRight		= 0xCD,	enKeyEnd		= 0xCF,
	enKeyDown		= 0xD0,	enKeyPageDown	= 0xD1,	enKeyInsert		= 0xD2,
	enKeyDelete		= 0xD3,	enKeyPower		= 0xDE,	enKeySleep		= 0xDF,
	enKeyWake		= 0xE3
};
#pragma endregion

//==================================
// Key state class
//==================================
class clsKeyState
{
public:
	mmByte	m_code;
	mmBool	m_pressed;
};

//==================================
// Keyboard state class
//==================================

class clsKeyboardState
{
friend class clsKeyboard;
protected:
	clsKeyboard*	m_pkeyboard;
	mmUint			m_numkeys;		// number of keys pressed
	clsKeyState*	m_pstate;		// affected keys states (256 max)
	mmBool*			m_psnapshot;	// all keys status
	mmVoid			Copy(clsKeyboardState *ns);
	
public:
	clsKeyboardState(clsKeyboard* pkeyboard);
   ~clsKeyboardState();
    
    mmUint GetNumAffectedKeys() const;				// returns number of affected keys
    const clsKeyState* GetKeysStatePointer() const;	// returns keys state direct
    const clsKeyState* GetKeyState(mmUint index) const;
    mmBool IsKeyPressed(enKeyScanCode code) const;	// returns true if specified key pressed
    mmBool IsKeyReleased(enKeyScanCode code) const;	// returns true if specified key released
	mmBool IsKeyAffected(enKeyScanCode code) const;	// returns true if specified key affected 
	mmBool SnapshotKeyPressed(enKeyScanCode code) const;	// returns true if snapshoted key pressed
};

//==================================
// Keyboard class
//==================================
class clsInput;
class clsKeyboard
{
	friend class clsKeyboardState;
	friend DWORD WINAPI InputProc( LPVOID lpParameter );
	friend class clsInput;

protected:
	clsInput*				m_pinput;
	IDirectInputDevice8*	m_pdevice;
	clsKeyboardState*		m_pstate;
	clsKeyboardState*		m_pstate2;
	mmBool					m_acquired;
	
	mmRes CopyState();
    mmRes Create();
    mmRes Snapshot();

public:
	clsKeyboard(clsInput* pinput);
   ~clsKeyboard();
    
    mmRes Acquire();
    mmRes Unacquire();
    mmBool IsAcquired();
    mmBool IsKeyPressed(enKeyScanCode code);
    mmBool IsKeyReleased(enKeyScanCode code);
    mmBool IsKeyAffected(enKeyScanCode code);
    mmBool SnapshotKeyPressed(enKeyScanCode code);
    const clsKeyboardState* GetState();
};


#endif // MHMKEYBOARD_INLUDED
