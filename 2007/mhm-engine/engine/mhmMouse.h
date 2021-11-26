//==========================================================================================
// Mayhem engine input mouse device class
// Coded:	MozG
// Created:	5 july 2006
//==========================================================================================

#ifndef MHMMOUSE_INCLUDED
#define MHMMOUSE_INCLUDED

#define MHM_MOUSE_SAMPLER_SIZE	1024

//====================================================
// Mouse button state enum
//====================================================
enum enMouseButtonState
{
	enMouseButtonUnaffected = 0,
	enMouseButtonUp			= 1,
	enMouseButtonDown		= 2,

};

//====================================================
// Mouse buttons
//====================================================
enum enMouseButton
{
	enMouseButton0		= 0,
	enMouseButton1		= 1,
	enMouseButton2		= 2,
	enMouseButton3		= 3,
	enMouseButton4		= 4,
	enMouseButton5		= 5,
	enMouseButton6		= 6,
	enMouseButton7		= 7,
	
	// do not remove or modify string below
	enMouseButtonCount
};

//====================================================
// Mouse axis
//====================================================
enum enMouseAxis
{
	enMouseX		= 0,
	enMouseY		= 1,
	enMouseWheel	= 2,
	
	// do not remove or modify string below
	enMouseAxisCount
};


//====================================================
// Mouse state class
//====================================================
class clsMouseEvent
{
public:
	mmInt				m_axis[enMouseAxisCount];
	enMouseButtonState	m_buttons[enMouseButtonCount];
	clsMouseEvent()
	{
		mmZeroVar(m_axis);
		mmZeroVar(m_buttons);
	}
};

//====================================================
// Mouse states class
//====================================================
class clsMouseState
{
friend class clsMouse;
protected:
	clsMouse*		m_pmouse;
	mmUint			m_numevents;
	clsMouseEvent*	m_pevents;
	clsMouseEvent	m_snapshot;
	clsRectInt		m_cliprect;
	clsPoint2D		m_clippos;

public:
	clsMouseState(clsMouse* pmouse);
   ~clsMouseState();
    
    mmUint GetNumEvents() const;
    const clsMouseEvent* GetEvent(mmUint index) const;
    const clsMouseEvent* GetSnapshot() const;
    const clsMouseEvent* GetEventsPointer() const;
    mmInt SnapshotAxisDelta(enMouseAxis axis) const;
    const clsPoint2D* GetPosition() const;
	mmVoid SetPoint2D(const clsPoint2D & p_point);
    enMouseButtonState SnapshotButton(enMouseButton button) const;
    
    const mmBool IsButtonPressed(enMouseButton button) const;
    const mmBool IsButtonReleased(enMouseButton button) const;
    const mmBool IsButtonAffected(enMouseButton button) const;

protected:
	mmVoid FlushEvents();
	mmVoid ClipPosition();
	mmVoid Copy(clsMouseState *ns);
};


//====================================================
// Mouse device class
//====================================================
class clsInput;
class clsMouse
{
	friend class clsMouseState;
	friend DWORD WINAPI InputProc( LPVOID lpParameter );
	friend class clsInput;

protected:
	clsInput*				m_pinput;
	IDirectInputDevice8*	m_pdevice;
	clsMouseState*			m_pstate;
	clsMouseState*			m_pstate2;
	mmBool					m_acquired;

    mmRes Create();
    mmRes Snapshot();
    mmRes Resize();
	mmRes CopyState();

public:
	clsMouse(clsInput* pinput);
   ~clsMouse();
    
    const clsMouseState* GetMouseState();
    mmInt SnapshotAxisDelta(enMouseAxis axis);
    enMouseButtonState SnapshotButton(enMouseButton button);
    clsPoint2D GetPosition();
	mmVoid SetPosition(const clsPoint2D & p_point);
	mmVoid SetPositionCenter();
    mmBool IsAcquired();
    mmRes Acquire();
    mmRes Unacquire();
    
    const mmBool IsButtonPressed(enMouseButton button) const;
    const mmBool IsButtonReleased(enMouseButton button) const;
    const mmBool IsButtonAffected(enMouseButton button) const;
};


#endif // MHMMOUSE_INCLUDED
