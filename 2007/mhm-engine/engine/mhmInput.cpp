//==========================================================================================
// Mayhem engine input class implementation
// Coded:	MozG
// Created:	5 july 2006
//==========================================================================================
#include "mhmbase.h"



//STATIC SECTION
CRITICAL_SECTION CriticalSection; 
DWORD		NotifyThreadID;
HANDLE		NotifyThread;
clsInput    *m_this;

static DWORD WINAPI InputProc( LPVOID lpParameter )
{
    MSG     msg;
    BOOL    bDone = FALSE;

	//SetForegroundWindow(m_this->GetWindow());

	if(MHMSUCC(m_this->t_Init()))
    while( !bDone ) 
    { 
		Sleep(10);

		EnterCriticalSection(&CriticalSection); 

		m_this->GetMouse()->Snapshot();
		m_this->GetKeyboard()->Snapshot();

        // Messages are available
        while( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) ) 
        { 
          if( msg.message == WM_QUIT )
			bDone = TRUE;
		  else if( msg.message == WM_USER )
		  {
			  // CUSTOM
			  switch(msg.wParam)
			  {
			  case 1:   // mouse ack
				  m_this->GetMouse()->Acquire();
				  break;
			  case 2:   // keyb ack
				  m_this->GetKeyboard()->Acquire();
				  break;
			  case 3:   // mouse unack
				  m_this->GetMouse()->Unacquire();
				  break;
			  case 4:   // keyb unack
				  m_this->GetKeyboard()->Unacquire();
				  break;
			  case 5:   // resize
				  m_this->GetMouse()->Resize();
				  break;
			  }
		  }
        }
	    LeaveCriticalSection(&CriticalSection);
    }

	m_this->t_Shut();

	return 0;
}



// Input system constructor
//==========================================================================================
clsInput::clsInput(clsEngine* pengine)
{
	m_pengine     = pengine;
	m_pdi	      = mmnull;	
	m_pkeyboard   = mmnull;
	m_pmouse      = mmnull;
	m_initialized = mmfalse;
	m_autocapture = mmtrue;
	m_this = this;
}

// Input system destructor
//==========================================================================================
clsInput::~clsInput()
{
	Shut();
	m_pengine = mmnull;
}

// Init: initializes subsystem devices and objects
//==========================================================================================
mmRes clsInput::Init(HWND hwnd)
{
   InitializeCriticalSection(&CriticalSection);

	m_hwnd = hwnd;
	NotifyThread = CreateThread( NULL, 0, InputProc, NULL, 0, &NotifyThreadID );
    SetThreadPriority(NotifyThread, THREAD_PRIORITY_TIME_CRITICAL);//THREAD_PRIORITY_HIGHEST);
	Sleep(100);
	return mmOk;
}

mmRes clsInput::t_Init()
{
	if (FAILED(DirectInput8Create(m_pengine->GetHinstance(), DIRECTINPUT_VERSION, 
								  IID_IDirectInput8, (void**)&m_pdi, NULL)))
	{
		g_plog->Out("clsInput::Init >> cant create DirectInput object, check DirecX version, expected at least 8.0", enLCRed);
		return mmFail;
	}
	m_pkeyboard = new clsKeyboard(this);
	if (m_pkeyboard == mmnull)
	{
		g_plog->Out("clsInput::Init >> cant allocate keyboard object, out of memory", enLCRed);
		t_Shut();
		return mmFail;
	}
	if (MHMFAIL(m_pkeyboard->Create()))
	{
		g_plog->Out("clsInput::Init >> cant create keyboard object", enLCRed);
		t_Shut();
		return mmFail;
	}
	m_pmouse = new clsMouse(this);
	if (m_pmouse == mmnull)
	{
		g_plog->Out("clsInput::Init >> cant allocate mouse object, out of memory", enLCRed);
		t_Shut();
		return mmFail;
	}
	if (MHMFAIL(m_pmouse->Create()))
	{
		g_plog->Out("clsInput::Init >> cant create mouse object", enLCRed);
		t_Shut();
		return mmFail;
	}
	
	m_initialized = mmtrue;
	return mmOk;
}

void clsInput::t_Shut()
{
	mmSafeDelete(m_pkeyboard);
	mmSafeDelete(m_pmouse);
	mmSafeRelease(m_pdi);
	m_initialized = mmfalse;

}

// Shut: release subsystem objects
//==========================================================================================
mmRes clsInput::Shut()
{
    PostThreadMessage( NotifyThreadID, WM_QUIT, 0, 0 );
    WaitForSingleObject( NotifyThread, INFINITE );
    CloseHandle( NotifyThread );
	DeleteCriticalSection(&CriticalSection);
	return mmOk;
}

// GetWindow: returns input focus window
//==========================================================================================
HWND clsInput::GetWindow()
{
	return m_hwnd;
}


// GetEngine: returns engine
//==========================================================================================
clsEngine* clsInput::GetEngine()
{
	return m_pengine;
}

// Direct3D typecast operator
//==========================================================================================
clsInput::operator IDirectInput8*()
{
	return m_pdi;
}

// GetKeyboard: returns keyboard object
//==========================================================================================
clsKeyboard* clsInput::GetKeyboard()
{
	return m_pkeyboard;
}

// GetMouse: returns mouse object
//==========================================================================================
clsMouse* clsInput::GetMouse()
{
	return m_pmouse;
}

// CaptureKeyboard: captures keyboard
//==========================================================================================
mmRes clsInput::CaptureKeyboard()
{
	if (!m_initialized)
	{
		g_plog->Out("clsInput::CaptureKeyboard >> input subsystem not initialized", enLCRed);
		return mmFail;
	}
    PostThreadMessage( NotifyThreadID, WM_USER, 2, 0 );
	return mmOk;
}

// CaptureMouse: captures mouse
//==========================================================================================
mmRes clsInput::CaptureMouse()
{
	ShowCursor(FALSE);

	if (!m_initialized)
	{
		g_plog->Out("clsInput::CaptureMouse >> input subsystem not initialized", enLCRed);
		return mmFail;
	}
    PostThreadMessage( NotifyThreadID, WM_USER, 1, 0 );
	return mmOk;
}
// FreeMouse: free mouse control
//==========================================================================================
mmRes clsInput::FreeMouse()
{
	ShowCursor(TRUE);

	if (!m_initialized)
	{
		g_plog->Out("clsInput::FreeMouse >> input subsystem not initialized", enLCRed);
		return mmFail;
	}
    PostThreadMessage( NotifyThreadID, WM_USER, 3, 0 );
	return mmOk;
}

mmRes clsInput::AutoCaptureMouse()
{
	if(m_autocapture)
	{
		CaptureMouse();
	}
	return mmOk;
}

mmRes clsInput::AutoCapture(mmBool onoff)
{
	m_autocapture=onoff;
	return mmOk;
}

// FreeKeyboard: free keyboard control
//==========================================================================================
mmRes clsInput::FreeKeyboard()
{
	if (!m_initialized)
	{
		g_plog->Out("clsInput::FreeKeyboard >> input subsystem not initialized", enLCRed);
		return mmFail;
	}
    PostThreadMessage( NotifyThreadID, WM_USER, 4, 0 );
	return mmOk;
}

// Resize: resizes input objects, such as mouse
//==========================================================================================
mmRes clsInput::Resize()
{
    PostThreadMessage( NotifyThreadID, WM_USER, 5, 0 );
	return mmOk;
}

// Snapshot: snapshots current device
//==========================================================================================
mmRes clsInput::Snapshot()
{
	EnterCriticalSection(&CriticalSection); 

	  m_this->GetMouse()->CopyState();
	  m_this->GetKeyboard()->CopyState();

    LeaveCriticalSection(&CriticalSection);
	return mmOk;
}




