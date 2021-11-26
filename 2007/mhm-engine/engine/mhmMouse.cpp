//==========================================================================================
// Mayhem engine input mouse device class
// Coded:	MozG
// Created:	5 july 2006
//==========================================================================================
#include "mhmbase.h"

//==========================================================================================
// Mouse state class
//==========================================================================================

// Mouse state construction
//==========================================================================================
clsMouseState::clsMouseState(clsMouse* pmouse)
{
	m_pmouse    = pmouse;
	m_pevents   = new clsMouseEvent[MHM_MOUSE_SAMPLER_SIZE];
	m_numevents = 0;
	FlushEvents();
}

// Mouse state destruction
//==========================================================================================
clsMouseState::~clsMouseState()
{
	m_pmouse = mmnull;
	mmSafeDeleteAr(m_pevents);
	m_numevents = 0;
}

// Copy: copy to new state
//==========================================================================================
mmVoid clsMouseState::Copy(clsMouseState *ns)
{
	ns->m_numevents = m_numevents;
	if(m_numevents>0)
	{
		mmCopyMem(ns->m_pevents, m_pevents, sizeof(clsMouseEvent) * m_numevents);
	}
	mmCopyMem(&ns->m_snapshot, &m_snapshot, sizeof(clsMouseEvent));
	mmCopyMem(&ns->m_clippos, &m_clippos, sizeof(clsPoint2D));
}

// FlushEvents: clear all event data for current number of events
//==========================================================================================
mmVoid clsMouseState::FlushEvents()
{
	mmZeroMem(m_pevents, sizeof(clsMouseEvent) * m_numevents);
	m_numevents = 0;
	//mmZeroVar(m_clippos);
	mmZeroVar(m_snapshot);
}

// GetNumEvents: returns number of snapshoted events
//==========================================================================================
mmUint clsMouseState::GetNumEvents() const
{
	return m_numevents;
}

// GetEventsPointer: returns events pointer directly
//==========================================================================================
const clsMouseEvent* clsMouseState::GetEventsPointer() const
{
	return m_pevents;
}

// GetSnapshot: returns events snapshot
//==========================================================================================
const clsMouseEvent* clsMouseState::GetSnapshot() const
{
	return &m_snapshot;
}

// GetEvent: returns event by its index
//==========================================================================================
const clsMouseEvent* clsMouseState::GetEvent(mmUint index) const
{
	if (index >= m_numevents)
		return mmnull;
	return &m_pevents[index];
}

// SnapshotAxisDelta: returns axis delta with buffer collapsing
//==========================================================================================
mmInt clsMouseState::SnapshotAxisDelta(enMouseAxis axis) const
{
	return m_snapshot.m_axis[axis];
}

// SnapshotButton: returns snapshoted button state
//==========================================================================================
enMouseButtonState clsMouseState::SnapshotButton(enMouseButton button) const
{
	return m_snapshot.m_buttons[button];
}

// ClipPosition: clips current mouse position by clip rect
//==========================================================================================
mmVoid clsMouseState::ClipPosition()
{
	m_pmouse->Resize();
	if (m_clippos.x < m_cliprect.left)
		m_clippos.x = m_cliprect.left;
	if (m_clippos.y < m_cliprect.top)
		m_clippos.y = m_cliprect.top;
	if (m_clippos.x > m_cliprect.right)
		m_clippos.x = m_cliprect.right;
	if (m_clippos.y > m_cliprect.bottom)
		m_clippos.y = m_cliprect.bottom;
}

// GetPosition: returns mouse cliped position
//==========================================================================================
const clsPoint2D* clsMouseState::GetPosition() const
{
	return &m_clippos;
}

mmVoid clsMouseState::SetPoint2D(const clsPoint2D & p_point)
{
	m_clippos = p_point;
}

// IsButtonPressed: returns true if button was pressed during last snapshot
//==========================================================================================
const mmBool clsMouseState::IsButtonPressed(enMouseButton button) const
{
	for (mmUint i = 0; i < m_numevents; i++)
		if (m_pevents[i].m_buttons[button] == enMouseButtonDown)
			return mmtrue;
	return mmfalse;
}
// IsButtonReleased: returns true if button was released during last snapshot
//==========================================================================================
const mmBool clsMouseState::IsButtonReleased(enMouseButton button) const
{
	for (mmUint i = 0; i < m_numevents; i++)
		if (m_pevents[i].m_buttons[button] == enMouseButtonUp)
			return mmtrue;
	return mmfalse;
}

// IsButtonAffected: returns true if button was pressed or released during last snapshot
//==========================================================================================
const mmBool clsMouseState::IsButtonAffected(enMouseButton button) const
{
	for (mmUint i = 0; i < m_numevents; i++)
		if (m_pevents[i].m_buttons[button] != enMouseButtonUnaffected)
			return mmtrue;
	return mmfalse;
}

//==========================================================================================
// Mouse device class implementation
//==========================================================================================

// Mouse input device construction
//==========================================================================================
clsMouse::clsMouse(clsInput* pinput)
{
	m_pdevice  = mmnull;
	m_pinput   = pinput;
	m_acquired = mmnull;
	m_pstate   = mmnull;
	m_pstate2  = mmnull;
}

// Mouse device destruction
//==========================================================================================
clsMouse::~clsMouse()
{
	mmSafeDelete(m_pstate2);
	mmSafeDelete(m_pstate);
	mmSafeRelease(m_pdevice);
	m_pinput = mmnull;
}


// IsAcquired: returns true if mouse acquired
//==========================================================================================
mmBool clsMouse::IsAcquired()
{
	return m_acquired;
}

// Create: creates mouse device
//==========================================================================================
mmRes clsMouse::Create()
{
	g_plog->Out("clsMouse::Create >> begin creating mouse device", enLCWhite);
	IDirectInput8* pdi = *m_pinput;
	if (pdi == mmnull)
	{
		g_plog->Out("clsMouse::Create >> no IDirectInput8 interface", enLCRed);
		return mmFail;
	}
	
	if (FAILED(pdi->CreateDevice(GUID_SysMouse, &m_pdevice, NULL)))
	{
		g_plog->Out("clsMouse::Create >> cant create mouse device interface", enLCRed);
		return mmFail;
	}
	
	if (FAILED(m_pdevice->SetCooperativeLevel(m_pinput->GetWindow(), 
											  DISCL_FOREGROUND|DISCL_EXCLUSIVE)))
	{
		g_plog->Out("clsMouse::Create >> cant set mouse behaivor", enLCRed);
		return mmFail;
	}
	
	if (FAILED(m_pdevice->SetDataFormat(&c_dfDIMouse2)))
	{
		g_plog->Out("clsMouse::Create >> cant set mouse device data format", enLCRed);
		return mmFail;
	}
	
	DIPROPDWORD dipdw;
	
	dipdw.diph.dwSize       = sizeof(DIPROPDWORD);
	dipdw.diph.dwHeaderSize = sizeof(DIPROPHEADER);
	dipdw.diph.dwObj        = 0;
	dipdw.diph.dwHow        = DIPH_DEVICE;
	dipdw.dwData            = MHM_MOUSE_SAMPLER_SIZE; 
	if (FAILED(m_pdevice->SetProperty(DIPROP_BUFFERSIZE, &dipdw.diph)))
	{
		g_plog->Out("clsMouse::Create >> cant set mouse device sampler buffer size", enLCRed);
		return mmFail;	
	}
		
	m_pstate = new clsMouseState(this);
	if (m_pstate == mmnull)
	{
		g_plog->Out("clsMouse::Create >> out of memory while alloc mouse state", enLCRed);
		return mmFail;	
	}
	if (MHMFAIL(Resize()))
	{
		g_plog->Out("clsMouse::Create >> cannot resize mouse params", enLCRed);
		return mmFail;	
	}

	m_pstate2 = new clsMouseState(this);
	if (m_pstate2 == mmnull)
	{
		g_plog->Out("clsMouse::Create >> out of memory while alloc mouse state #2", enLCRed);
		return mmFail;	
	}

	g_plog->Out("clsMouse::Create >> mouse device created sucessfully", enLCWhite);

	return mmOk;
}

// Acquire: captures mouse device
//==========================================================================================
mmRes clsMouse::Acquire()
{
	HRESULT hr = m_pdevice->Acquire();
	if (FAILED(hr))
	{
		if (hr == DIERR_INPUTLOST)
			while(hr == DIERR_INPUTLOST)
				hr = m_pdevice->Acquire();
		if (hr == DIERR_OTHERAPPHASPRIO || 
            hr == DIERR_NOTACQUIRED)
		{
			g_plog->Out("clsMouse::Acquire >> mouse capturing failed", enLCRed);
			return mmFail;
		}
	}
	m_acquired = mmtrue;
	return mmOk;
}

// Unacquire: frees mouse control
//==========================================================================================
mmRes clsMouse::Unacquire()
{
	if (FAILED(m_pdevice->Unacquire()))
	{
		g_plog->Out("clsMouse::Unacquire >> device method failed", enLCRed);
		return mmFail;
	}
	m_acquired = mmfalse;
	return mmOk;
}

// Snapshot: snapshots mouse state
//==========================================================================================
mmRes clsMouse::Snapshot()
{
	mmUint	i,i2;
    mmUlong            numitems;

	if (!m_acquired)
		return mmOk;
	
	DIDEVICEOBJECTDATA didod[256];

	while(1)
	{
		numitems = 256;
		HRESULT hr = m_pdevice->GetDeviceData(sizeof(DIDEVICEOBJECTDATA), didod, &numitems, 0);

		if (FAILED(hr))
		{
			return mmFail; // pinkmous

			if (MHMFAIL(Acquire()))
			{
				g_plog->Out("clsMouse::Snapshot >> cant re-acquire mouse device", enLCRed);
				return mmFail;
			}
			hr = m_pdevice->GetDeviceData(sizeof(DIDEVICEOBJECTDATA), didod, &numitems, 0);
			if (FAILED(hr))
			{
				g_plog->Out("clsMouse::Snapshot >> strange thing happend with mouse", enLCRed);
				return mmFail;
			}		
		}
		//m_pstate->FlushEvents();

		if(numitems == 0) break;

	for (i = 0; i < numitems; i++)
	{
		i2 = i + m_pstate->m_numevents;

		switch(didod[i].dwOfs)
		{
			case DIMOFS_BUTTON0:
				m_pstate->m_pevents[i2].m_buttons[enMouseButton0] =
					(didod[i].dwData & 0x80) ? enMouseButtonDown : enMouseButtonUp;
				m_pstate->m_snapshot.m_buttons[enMouseButton0] =
					m_pstate->m_pevents[i2].m_buttons[enMouseButton0];
				break;
			case DIMOFS_BUTTON1:
				m_pstate->m_pevents[i2].m_buttons[enMouseButton1] =
					(didod[i].dwData & 0x80) ? enMouseButtonDown : enMouseButtonUp;
				m_pstate->m_snapshot.m_buttons[enMouseButton1] =
					m_pstate->m_pevents[i2].m_buttons[enMouseButton1];
				break;
			case DIMOFS_BUTTON2:
				m_pstate->m_pevents[i2].m_buttons[enMouseButton2] =
					(didod[i].dwData & 0x80) ? enMouseButtonDown : enMouseButtonUp;
				m_pstate->m_snapshot.m_buttons[enMouseButton2] =
					m_pstate->m_pevents[i2].m_buttons[enMouseButton2];
				break;
			case DIMOFS_BUTTON3:
				m_pstate->m_pevents[i2].m_buttons[enMouseButton3] =
					(didod[i].dwData & 0x80) ? enMouseButtonDown : enMouseButtonUp;
				m_pstate->m_snapshot.m_buttons[enMouseButton3] =
					m_pstate->m_pevents[i2].m_buttons[enMouseButton3];
				break;
            case DIMOFS_BUTTON4:
				m_pstate->m_pevents[i2].m_buttons[enMouseButton4] =
					(didod[i].dwData & 0x80) ? enMouseButtonDown : enMouseButtonUp;
				m_pstate->m_snapshot.m_buttons[enMouseButton4] =
					m_pstate->m_pevents[i2].m_buttons[enMouseButton4];
				break;
            case DIMOFS_BUTTON5:
				m_pstate->m_pevents[i2].m_buttons[enMouseButton5] =
					(didod[i].dwData & 0x80) ? enMouseButtonDown : enMouseButtonUp;
				m_pstate->m_snapshot.m_buttons[enMouseButton5] =
					m_pstate->m_pevents[i2].m_buttons[enMouseButton5];
				break;
            case DIMOFS_BUTTON6:
				m_pstate->m_pevents[i2].m_buttons[enMouseButton6] =
					(didod[i].dwData & 0x80) ? enMouseButtonDown : enMouseButtonUp;
				m_pstate->m_snapshot.m_buttons[enMouseButton6] =
					m_pstate->m_pevents[i2].m_buttons[enMouseButton6];
				break;
            case DIMOFS_BUTTON7:
				m_pstate->m_pevents[i2].m_buttons[enMouseButton7] =
					(didod[i].dwData & 0x80) ? enMouseButtonDown : enMouseButtonUp;
				m_pstate->m_snapshot.m_buttons[enMouseButton7] =
					m_pstate->m_pevents[i2].m_buttons[enMouseButton7];
				break;
            case DIMOFS_X:
            	m_pstate->m_pevents[i2].m_axis[enMouseX] = (mmInt)didod[i].dwData;
				m_pstate->m_snapshot.m_axis[enMouseX] += (mmInt)didod[i].dwData;
				m_pstate->m_clippos.x += (mmInt)didod[i].dwData;
				break;			
            case DIMOFS_Y:
               	m_pstate->m_pevents[i2].m_axis[enMouseY] = (mmInt)didod[i].dwData;
				m_pstate->m_snapshot.m_axis[enMouseY] += (mmInt)didod[i].dwData;
				m_pstate->m_clippos.y += (mmInt)didod[i].dwData;
				break;			
			case DIMOFS_Z:
            	m_pstate->m_pevents[i2].m_axis[enMouseWheel] = (mmInt)didod[i].dwData;
				m_pstate->m_snapshot.m_axis[enMouseWheel] += (mmInt)didod[i].dwData;
				break;			
		}
	}
		m_pstate->m_numevents += numitems;
	//	break;
	}
	return mmOk;
}



// CopyState: copy thread state to real state
//==========================================================================================
mmRes clsMouse::CopyState()
{
	m_pstate->Copy(m_pstate2);
	m_pstate->FlushEvents();
	m_pstate2->ClipPosition();
	return mmOk;
}

// GetMouseState: returns mouse state
//==========================================================================================
const clsMouseState* clsMouse::GetMouseState()
{
	if (!m_pstate2) return mmnull;
	return m_pstate2;
}

// SnapshotAxisDelta: returns snapshoted axis delta value
//==========================================================================================
mmInt clsMouse::SnapshotAxisDelta(enMouseAxis axis)
{
	if (!m_pstate2) return 0;
	return m_pstate2->SnapshotAxisDelta(axis);
}

// SnapshotButton: returns snapshoted button state
//==========================================================================================
enMouseButtonState clsMouse::SnapshotButton(enMouseButton button)
{
	if (!m_pstate2) return enMouseButtonUnaffected;
	return m_pstate2->SnapshotButton(button);
}


// IsButtonPressed: returns true if button was pressed during last snapshot
//==========================================================================================
const mmBool clsMouse::IsButtonPressed(enMouseButton button) const
{
	if (!m_pstate2) return mmfalse;
	return m_pstate2->IsButtonPressed(button);
}
// IsButtonReleased: returns true if button was released during last snapshot
//==========================================================================================
const mmBool clsMouse::IsButtonReleased(enMouseButton button) const
{
	if (!m_pstate2) return mmfalse;
	return m_pstate2->IsButtonReleased(button);
}

// IsButtonAffected: returns true if button was pressed or released during last snapshot
//==========================================================================================
const mmBool clsMouse::IsButtonAffected(enMouseButton button) const
{
	if (!m_pstate2) return mmfalse;
	return m_pstate2->IsButtonAffected(button);
}

// GetPosition: returns mouse cliped position
//==========================================================================================
clsPoint2D clsMouse::GetPosition()
{
	return m_pstate2->m_clippos;
}

mmVoid clsMouse::SetPosition(const clsPoint2D & p_point)
{
	m_pstate2->m_clippos = p_point;
}

mmVoid clsMouse::SetPositionCenter()
{
	Resize();
	SetPosition(clsPoint2D(m_pstate2->m_cliprect.right / 2 , m_pstate2->m_cliprect.bottom / 2));
}

// Resize: resizes mouse clip region
//==========================================================================================
mmRes clsMouse::Resize()
{
	if(m_pstate2)
	{
	m_pstate2->m_cliprect.left   = 0;
	m_pstate2->m_cliprect.right  = m_pinput->GetEngine()->GetViewWidth();
	m_pstate2->m_cliprect.top    = 0;
	m_pstate2->m_cliprect.bottom = m_pinput->GetEngine()->GetViewHeight();
	}
	return mmOk;
}
