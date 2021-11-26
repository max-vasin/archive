//==========================================================================================
// Mayhem engine input keyboard device class
// Coded:	MozG
// Created:	5 july 2006
//==========================================================================================
#include "mhmbase.h"

//==========================================================================================
// Keyboard state class implementation
//==========================================================================================

// Keyboard state construction
//==========================================================================================
clsKeyboardState::clsKeyboardState(clsKeyboard* pkeyboard)
{
	m_pstate = new clsKeyState[MHM_KEYBOARD_SAMPLER_SIZE];
	if (m_pstate == mmnull)
		mmThrow(enExceptOutOfMemory, clsKeyboardState, constructor, Keyboard state alloc failed);
	m_psnapshot = new mmBool[256];
	if (m_psnapshot == mmnull)
		mmThrow(enExceptOutOfMemory, clsKeyboardState, constructor, Keyboard snapshot alloc failed);
	mmZeroMem(m_pstate, sizeof(clsKeyState) * MHM_KEYBOARD_SAMPLER_SIZE);
	mmZeroMem(m_psnapshot, sizeof(mmBool) * 256);
	
	m_numkeys = 0;
	m_pkeyboard = pkeyboard;
}

// Keyboard state destruction
//==========================================================================================
clsKeyboardState::~clsKeyboardState()
{
	mmSafeDeleteAr(m_pstate);
	mmSafeDeleteAr(m_psnapshot);
	m_numkeys = 0;
}

// Copy: copy to new state
//==========================================================================================
mmVoid clsKeyboardState::Copy(clsKeyboardState *ns)
{
	ns->m_numkeys = m_numkeys;
	mmCopyMem(ns->m_pstate, m_pstate, sizeof(clsKeyState) * m_numkeys);
	mmCopyMem(ns->m_psnapshot, m_psnapshot, 256 * sizeof(mmBool));
}

// GetNumAffectedKeys: returns number of affected keys
//==========================================================================================
mmUint clsKeyboardState::GetNumAffectedKeys() const
{
	return m_numkeys;
}

// GetKeysStatePoniter: returns keys state directly
//==========================================================================================
const clsKeyState* clsKeyboardState::GetKeysStatePointer() const
{
	return m_pstate;
}

// GetKeyState: returns key state by index in array
//==========================================================================================
const clsKeyState* clsKeyboardState::GetKeyState(mmUint index) const
{
	return &m_pstate[index];
}

// IsKeyPressed: returns true if key with specified scancode pressed
//==========================================================================================
mmBool clsKeyboardState::IsKeyPressed(enKeyScanCode code) const
{
	for (mmUint i = 0; i < m_numkeys; i++)
		if ((m_pstate[i].m_code == code) && (m_pstate[i].m_pressed))
			return mmtrue;
	return mmfalse;
}

// IsKeyReleased: returns true if key with specified scancode released
//==========================================================================================
mmBool clsKeyboardState::IsKeyReleased(enKeyScanCode code) const
{
	for (mmUint i = 0; i < m_numkeys; i++)
		if ((m_pstate[i].m_code == code) && (!m_pstate[i].m_pressed))
			return mmtrue;
	return mmfalse;
}

// IsKeyAffected: returns true if key was pressed or released during last snapshot
//==========================================================================================
mmBool clsKeyboardState::IsKeyAffected(enKeyScanCode code) const
{
	for (mmUint i = 0; i < m_numkeys; i++)
		if (m_pstate[i].m_code == code)
			return mmtrue;
	return mmfalse;
}

// SnapshotKeyPressed: returns true if snapshot key status is pressed
//==========================================================================================
mmBool clsKeyboardState::SnapshotKeyPressed(enKeyScanCode code) const
{
	return m_psnapshot[code];
}


// Keyboard device constructor
//==========================================================================================
clsKeyboard::clsKeyboard(clsInput *pinput)
{
	m_pinput   = pinput;
	m_pdevice  = mmnull;
	m_pstate   = mmnull;
	m_pstate2  = mmnull;
	m_acquired = mmfalse;
}

// Keyboard device destructor
//==========================================================================================
clsKeyboard::~clsKeyboard()
{
	mmSafeRelease(m_pdevice);
	mmSafeDelete(m_pstate);
	mmSafeDelete(m_pstate2);
	m_pinput = mmnull;
}

// CopyState: copy thread state to real state
//==========================================================================================
mmRes clsKeyboard::CopyState()
{
	m_pstate->Copy(m_pstate2);
	m_pstate->m_numkeys = 0; // flush
	return mmOk;
}

// Create: creates the keyboard
//==========================================================================================
mmRes clsKeyboard::Create()
{
	g_plog->Out("clsKeyboard::Create >> begin creating keyboard device", enLCWhite);
	IDirectInput8* pdi = *m_pinput;
	if (pdi == mmnull)
	{
		g_plog->Out("clsKeyboard::Create >> no IDirectInput8 interface", enLCRed);
		return mmFail;
	}
	
	if (FAILED(pdi->CreateDevice(GUID_SysKeyboard, &m_pdevice, NULL)))
	{
		g_plog->Out("clsKeyboard::Create >> cant create keyboard device interface", enLCRed);
		return mmFail;
	}
	
	if (FAILED(m_pdevice->SetDataFormat(&c_dfDIKeyboard)))
	{
		g_plog->Out("clsKeyboard::Create >> cant set keyboard device data format", enLCRed);
		return mmFail;
	}
	
	if (FAILED(m_pdevice->SetCooperativeLevel(m_pinput->GetWindow(), 
											  DISCL_FOREGROUND|DISCL_EXCLUSIVE)))
	{
		g_plog->Out("clsKeyboard::Create >> cant set keyboard behaivor", enLCRed);
		return mmFail;
	}
	
	DIPROPDWORD dipdw;
	
	dipdw.diph.dwSize       = sizeof(DIPROPDWORD);
	dipdw.diph.dwHeaderSize = sizeof(DIPROPHEADER);
	dipdw.diph.dwObj        = 0;
	dipdw.diph.dwHow        = DIPH_DEVICE;
	dipdw.dwData            = MHM_KEYBOARD_SAMPLER_SIZE; 
	if (FAILED(m_pdevice->SetProperty(DIPROP_BUFFERSIZE, &dipdw.diph)))
	{
		g_plog->Out("clsKeyboard::Create >> cant set keyboard device sampler buffer size", enLCRed);
		return mmFail;	
	}
	
	m_pstate = new clsKeyboardState(this);
	if (m_pstate == mmnull)
	{
		g_plog->Out("clsKeyboard::Create >> out of memory while alloc keyboard state", enLCRed);
		return mmFail;	
	}

	m_pstate2 = new clsKeyboardState(this);
	if (m_pstate2 == mmnull)
	{
		g_plog->Out("clsKeyboard::Create >> out of memory while alloc keyboard state #2", enLCRed);
		return mmFail;	
	}
	g_plog->Out("clsKeyboard::Create >> keyboard device created sucessfully", enLCWhite);
	return mmOk;
}

// Acquire: gain control over keyboard device
//==========================================================================================
mmRes clsKeyboard::Acquire()
{
	if (FAILED(m_pdevice->Acquire()))
	{
		g_plog->Out("clsKeyboard::Acquire >> cant gain control over keyboard", enLCRed);
		return mmFail;
	}
	m_acquired = mmtrue;
	return mmOk;
}

// Unacquire: frees keyboard control
//==========================================================================================
mmRes clsKeyboard::Unacquire()
{
	if (FAILED(m_pdevice->Unacquire()))
	{
		g_plog->Out("clsKeyboard::Unacquire >> cant free control over keyboard", enLCRed);
		return mmFail;
	}
	m_acquired = mmfalse;
	return mmOk;
}

// Snapshot: gets keyboard state
//==========================================================================================
mmRes clsKeyboard::Snapshot()
{
	mmUint i,i2;

	if (!m_acquired)
		return mmOk;
	
	DIDEVICEOBJECTDATA	didod[MHM_KEYBOARD_SAMPLER_SIZE];
	DWORD				numitems = MHM_KEYBOARD_SAMPLER_SIZE;
	
	mmZeroMem(didod, sizeof(DIDEVICEOBJECTDATA) * MHM_KEYBOARD_SAMPLER_SIZE);
	if (FAILED(m_pdevice->GetDeviceData(sizeof(DIDEVICEOBJECTDATA), 
										didod, &numitems, 0)))
	{
		Acquire();
		if (FAILED(m_pdevice->GetDeviceData(sizeof(DIDEVICEOBJECTDATA), 
											didod, &numitems, 0)))
		{
			g_plog->Out("clsKeyboard::Snapshot >> cant get device data", enLCRed);
			return mmFail;
		}
	}
	for ( i = 0; i < numitems; i++)
	{
		i2 = m_pstate->m_numkeys + i;

		m_pstate->m_pstate[i2].m_code = (mmByte)didod[i].dwOfs;
		m_pstate->m_pstate[i2].m_pressed = (didod[i].dwData & 0x80) ? mmtrue : mmfalse;
		if (m_pstate->m_pstate[i2].m_pressed)
			m_pstate->m_psnapshot[m_pstate->m_pstate[i2].m_code] = mmtrue;
		else
			m_pstate->m_psnapshot[m_pstate->m_pstate[i2].m_code] = mmfalse;
	}
	m_pstate->m_numkeys += numitems;
	return mmOk;
}

// IsAcquired: returns true if keyboard captured
//==========================================================================================
mmBool clsKeyboard::IsAcquired()
{
	return m_acquired;
}

// IsKeyPressed: returns true if key pressed
//==========================================================================================
mmBool clsKeyboard::IsKeyPressed(enKeyScanCode code)
{
	return m_pstate2->IsKeyPressed(code);
}

// IsKeyReleased: returns true if key released
//==========================================================================================
mmBool clsKeyboard::IsKeyReleased(enKeyScanCode code)
{
	return m_pstate2->IsKeyReleased(code);
}

// IsKeyAffected: returns true if key affected somehow
//==========================================================================================
mmBool clsKeyboard::IsKeyAffected(enKeyScanCode code)
{
	return m_pstate2->IsKeyAffected(code);
}

// SnapshotKeyPressed: returns true if snapshoted key pressed
//==========================================================================================
mmBool clsKeyboard::SnapshotKeyPressed(enKeyScanCode code)
{
	return m_pstate2->SnapshotKeyPressed(code);
}


// GetState: returns full keyboard state
//==========================================================================================
const clsKeyboardState* clsKeyboard::GetState()
{
	return m_pstate2;
}
