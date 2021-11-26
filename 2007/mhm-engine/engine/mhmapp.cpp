#include "mhmbase.h"

static clsApp*  g_pApp		   = mmnull;
clsEnvironment* g_pEnvironment = mmnull;

#ifdef	_PINK_FIRE
	#define MHMAPP_CLASS_NAME		"Mayhem engine Pink Fire"
	#define MHMAPP_WINDOW_TITLE		"Pink Fire"
#else
	#define MHMAPP_CLASS_NAME		"Mayhem engine"
	#define MHMAPP_WINDOW_TITLE		"Mayhem engine application"
#endif


#define MHMAPP_WINDOW_TITLED		"Mayhem engine application (DEBUG)"


//==========================================================================================
// Application window message proc
// DO NOT MODIFY CODE BELOW!! (MozG)
//==========================================================================================
LRESULT CALLBACK AppWndProc(HWND hwnd, UINT msg, WPARAM wpar, LPARAM lpar)
{
	switch(msg)
	{
	case WM_CREATE:
		g_pApp->HandleWindowCreation(hwnd, wpar, lpar);
		return 0;

	case WM_KEYUP:
		g_pApp->m_char_key_up = (mmChar)wpar;

		break;

	case WM_LBUTTONDOWN:
		if(g_pApp->m_pEngine->GetInput())
			if(g_pApp->m_pEngine->GetInput()->GetMouse())
			{
				//if(g_pApp->m_pEngine->GetInput()->GetMouse()->IsAcquired() == mmfalse)
					g_pApp->m_pEngine->GetInput()->GetMouse()->Acquire();
			}
	break; 

	case WM_KEYDOWN:	
		if(g_pApp->m_ExitOnEsc)
		{
			if(wpar== VK_ESCAPE)
			{
				g_pApp->Quit();
				return 0;
			}
		}
		break;
		
	case WM_DESTROY:
		g_pApp->HandleWindowDestruction(hwnd, wpar, lpar);
		return 0;
	case WM_SIZE:
		g_pApp->HandleWindowSize(hwnd, wpar, lpar);
     //   PostMessage( hwnd, WM_PAINT, 0, 0 );
		return 0;
	case WM_PAINT:
		ValidateRect(hwnd, NULL);
		return 1;
	}
	return DefWindowProc(hwnd, msg, wpar, lpar);
}

// Application constructor
//==========================================================================================
clsApp::clsApp()
{
	m_char_key_up = 0;
	m_hInst   = mmnull;
	m_pEngine = mmnull;
	m_pLog    = mmnull;
	m_pEFactory = mmnull;
	g_pEnvironment = new clsEnvironment;
	g_pEnvironment->m_cRand.init(GetTickCount());
	//g_pEnvironment->m_pStringPool = new clsMemoryPool();
	g_pEnvironment->m_pMyApp = this;
	m_ExitOnEsc = mmtrue;
}

// Application destructor
//==========================================================================================
clsApp::~clsApp()
{
	mmSafeDelete(m_pEFactory);
	mmSafeDelete(m_pEngine);
	mmSafeDelete(m_pLog);
	mmSafeDelete(g_pEnvironment);
	m_hInst	= mmnull;
}

// Application building
//==========================================================================================
mmRes clsApp::Build(HINSTANCE hinst, LPSTR lpcmd)
{
	m_hInst   = hinst;
	m_CmdLine = (mmString)lpcmd;
	m_pLog    = CreateAppLog();
	if (m_pLog == mmnull)
		mmThrow(enExceptObjectMissed, clsApp, Build, Missed log object);

	m_pEngine = new clsEngine(hinst);
	if (m_pEngine == mmnull)
		mmThrow(enExceptOutOfMemory, clsApp, Build, Engine object alloc failed);

	mmRes res = CreateAppWindow();
	if (MHMFAIL(res))
		return res;

	g_plog->Out("Engine initialization started", enLCWhite);
	res = InitEngine();
	if (MHMFAIL(res))
	{
		g_plog->Out("Engine initialization failed", enLCRed);
		Quit();
		return res;
	}
	g_plog->Out("Retrieving entity factory", enLCWhite);
	m_pEFactory = CreateEntityFactory(m_pEngine);
	if (m_pEFactory == mmnull)
	{
		g_plog->Out("No entity factory constructed!", enLCRed);
		Quit();
		return res;
	}
	m_pEngine->SetEntityFactory(m_pEFactory);
	g_plog->Out("Engine initialization completed successfully", enLCWhite);

	ShowWindow(m_hWnd, SW_SHOW);
	UpdateWindow(m_hWnd);

	return mmOk;
}

//==========================================================================================
mmRes clsApp::Run()
{
	MSG		 msg;
	msg.message = WM_NULL;

	InitApp();
	do
	{  
		if(PeekMessage(&msg,NULL,0,0,PM_REMOVE))
		{  
			if (msg.message == WM_QUIT)
				break;  
			DispatchMessage(&msg);  
		}
		else 	
			if (MHMFAIL(IdleLoop()))
				Quit();

		if(GetForegroundWindow() != g_pApp->m_hWnd)
			Sleep(50);

    }while(1);

	g_plog->Out("Application deinitialization started", enLCWhite);

	mmRes res = ShutApp();
	if (MHMFAIL(res))
	{
		g_plog->Out("Application deinitialization failed", enLCRed);
		return res;
	}
	g_plog->Out("Application deinitialization completed successfully", enLCWhite);

	return mmOk;
}

mmRes clsApp::Close()
{
	g_plog->Out("Engine deinitialization started", enLCWhite);
	mmRes res = g_pApp->ShutEngine();
	if (MHMFAIL(res))
	{
		g_plog->Out("Engine deinitialization failed", enLCRed);
		return res;
	}
	g_plog->Out("Engine deinitialization completed successfully", enLCWhite);
	return mmOk;
}

// InitEngine: initializes engine, can be overrided
//==========================================================================================
mmRes clsApp::InitEngine()
{
	return m_pEngine->Init(m_hWnd, m_EngineINI);
}

// InitApp: empty
//==========================================================================================
mmRes clsApp::InitApp()
{
	return mmOk;
}

// IdleLoop: empty
//==========================================================================================
mmRes clsApp::IdleLoop()
{
	return mmOk;
}

// ShutApp: empty
//==========================================================================================
mmRes clsApp::ShutApp()
{
	return mmOk;
}

// ShutEngine: empty
//==========================================================================================
mmRes clsApp::ShutEngine()
{
	return m_pEngine->Shut();
}

// WindowSize: empty
//==========================================================================================
mmRes clsApp::WindowSize()
{
	return mmOk;
}

// ModifyWindowClass: unaffected
//==========================================================================================
mmRes clsApp::ModifyWindowClass(WNDCLASSEX* pWndClass)
{
	return mmOk;
}


// ModifyWindowStyle: unaffected
//==========================================================================================
mmRes clsApp::ModifyWindowStyle(stAppWindowStyle* pStyle)
{
	return mmOk;
}

// CreateAppLog: creates application log object
//==========================================================================================
clsLog* clsApp::CreateAppLog()
{
	clsLog* plog = new clsLog("mayhem engine log.html");
	if (plog == mmnull)
		mmThrow(enExceptOutOfMemory, clsApp, CreateAppLog, Log object alloc failed);
	plog->Create();
	plog->Open();
	plog->Out("Mayhem engine log file", enLCWhite);
	plog->OutBreakLine();
	return plog;
}

// CreateEntityFactory: creates app entity factory
//==========================================================================================
clsEntityFactory* clsApp::CreateEntityFactory(clsEngine* pengine)
{
	return new clsEntityFactory(pengine);
}

// Quit: quits application
//==========================================================================================
mmVoid clsApp::Quit()
{
	DestroyWindow(m_hWnd);
//	ExitProcess(0);
}

//==========================================================================================
// Iternal application methods
//==========================================================================================

// CreateAppWindow: creates application window
//==========================================================================================
mmRes clsApp::CreateAppWindow()
{
	mmZeroVar(m_WndClass);
	m_WndClass.cbSize			= sizeof(WNDCLASSEX);
	m_WndClass.hbrBackground	= (HBRUSH)GetStockObject(BLACK_BRUSH);
	m_WndClass.hCursor			= LoadCursor(NULL, IDC_ARROW);
	m_WndClass.hIcon			= LoadIcon(NULL, IDI_APPLICATION);
	m_WndClass.hIconSm			= LoadIcon(NULL, IDI_APPLICATION);
	m_WndClass.hInstance		= m_hInst;
	m_WndClass.lpfnWndProc		= AppWndProc;
	m_WndClass.lpszClassName	= MHMAPP_CLASS_NAME;
	m_WndClass.lpszMenuName		= NULL;
	m_WndClass.style			= CS_HREDRAW|CS_VREDRAW|CS_OWNDC;
	
	g_plog->Out("Modifying windows class (pre-create)...", enLCWhite);
	mmRes res = ModifyWindowClass(&m_WndClass);
	if (MHMFAIL(res))
	{
		g_plog->Out("Modifying windows class failed", enLCRed);
		return res;
	}
	g_plog->Out("Registering window class...", enLCWhite);
	if (!RegisterClassEx(&m_WndClass))
	{
		g_plog->Out("Registering window class failed", enLCRed);
		return mmFail;
	}
	
	stAppWindowStyle style;
	style.m_dwExStyle	= 0;
	style.m_dwStyle		=  WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU  | WS_MINIMIZEBOX ; // pink
	//WS_OVERLAPPEDWINDOW;

	style.m_hInst		= m_hInst;
	style.m_hMenu		= NULL;
	style.m_hParentWnd	= HWND_DESKTOP;
	style.m_nHeight		= CW_USEDEFAULT;
	style.m_nWidth		= CW_USEDEFAULT;
	style.m_nPosx		= CW_USEDEFAULT;
	style.m_nPosy		= CW_USEDEFAULT;
	style.m_pParam		= NULL;
	style.m_sClassName	= MHMAPP_CLASS_NAME;
#ifdef	NDEBUG
	style.m_sWindowName = MHMAPP_WINDOW_TITLE;
#else
	style.m_sWindowName = MHMAPP_WINDOW_TITLED;
#endif

	g_plog->Out("Modifying windows style (pre-create)...", enLCWhite);
	res = ModifyWindowStyle(&style);
	if (MHMFAIL(res))
	{
		g_plog->Out("Modifying windows style failed", enLCRed);
		return res;
	}

	/*
	style.m_nHeight		= 100;
	style.m_nWidth		= 100;
	style.m_nPosx		= 0;
	style.m_nPosy		= 0;
*/
	style.m_nPosx		= 99999;

	g_plog->Out("Creating application window...", enLCWhite);

	if ((m_hWnd = CreateWindowEx(style.m_dwExStyle, style.m_sClassName, style.m_sWindowName,
					   style.m_dwStyle, style.m_nPosx, style.m_nPosy,
					   style.m_nWidth, style.m_nHeight, style.m_hParentWnd,
					   style.m_hMenu,  style.m_hInst, style.m_pParam)) == NULL)
	{
		g_plog->Out("Creating application window failed", enLCRed);
		return mmFail;
	}


	return mmOk;
}


// HandleWindowCreation: handles WM_CREATE
//==========================================================================================
mmRes clsApp::HandleWindowCreation(HWND hwnd, WPARAM wpar, LPARAM lpar)
{
	return mmOk;
}

// HandleWindowDestruction: handles WM_DESTROY
//==========================================================================================
mmRes clsApp::HandleWindowDestruction(HWND hwnd, WPARAM wpar, LPARAM lpar)
{
	if (m_hWnd != hwnd)
		return mmOk;
	
	PostQuitMessage(0);
	return mmOk;
}

// HandleWindowSize: handles WM_SIZE
//==========================================================================================
mmRes clsApp::HandleWindowSize(HWND hwnd, WPARAM wpar, LPARAM lpar)
{
	mmRes res;
	res = m_pEngine->GetRenderer()->GetDevice()->Reset();
	if (MHMFAIL(res))
		return res;
	return WindowSize();
}


//==========================================================================================
// DO NOT MODIFY CODE BELOW! (MozG)
//==========================================================================================
INT WINAPI WinMain(HINSTANCE hinst, HINSTANCE hPrevInst, LPSTR lpcmd, int show)
{
	try
	{
		g_pApp = MakeApp();
		if (g_pApp == mmnull)
			return 0;

		mmRes res = g_pApp->Build(hinst, lpcmd);

		if (MHMSUCC(res))
		{
			res = g_pApp->Run();
		}

		g_pApp->Close();

		mmSafeDelete(g_pApp);
		return MHMSUCC(res)?1:0;
	}
	catch (clsException& except)
	{
		clsString buffer;
		clsString header;
		header = "Exception raised - ";
		header += except.GetReasonDesc();
		buffer = clsString("Class name:\t[") + except.m_classname +
						   "]\nClass method:\t[" +
						   except.m_method + 
						   "]\nDescription:\t[" +
						   except.m_description + "]";
		MessageBox(HWND_DESKTOP, buffer, header, MB_OK|MB_ICONSTOP|MB_APPLMODAL);
	}
	return 0;
}
