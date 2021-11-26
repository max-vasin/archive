//==========================================================================================
// Mayhem engine application base class interface
// Coded:	MozG
// Created:	7 july 2006
//==========================================================================================
#ifndef _MHMAPP_INCLUDED
#define _MHMAPP_INCLUDED

//============================================
// This structure used to create window
//============================================
struct stAppWindowStyle
{
	mmUlong		m_dwExStyle;
	clsString	m_sClassName;
	clsString	m_sWindowName;
	mmUlong		m_dwStyle;
	mmInt		m_nPosx;
	mmInt		m_nPosy;
	mmInt		m_nWidth;
	mmInt		m_nHeight;
	HWND		m_hParentWnd;
	HMENU		m_hMenu;
	HINSTANCE	m_hInst;
	mmVoid*		m_pParam;
};

//============================================
// Application class
//============================================
class clsApp
{
friend LRESULT CALLBACK AppWndProc(HWND,UINT,WPARAM,LPARAM);
friend DWORD WINAPI DoIterate(LPVOID);
private:
	WNDCLASSEX	m_WndClass;
	
protected:
	HINSTANCE	m_hInst;
	clsString	m_CmdLine;
	clsLog*		m_pLog;
	//clsEntityFactory* m_pEFactory;
	mmDword		m_dwIdleThread;

	mmBool m_ExitOnEsc;
	
	//============================================
	// This attribs must be defined in
	// constructor of your app class
	//============================================
	clsString	m_EngineINI;	// specify engine INI file, which will contain engine config
	
public:
	clsApp();
    virtual ~clsApp();
    
    mmRes Build(HINSTANCE hinst, LPSTR lpcmd);
    mmRes Run();
	mmRes Close();

	mmVoid Quit();
	
	HWND		m_hWnd;
	clsEngine*	m_pEngine;

	mmChar m_char_key_up;

	clsEntityFactory* m_pEFactory;
	
protected:
	//============================================
	// Application overrides (events)
	//============================================    
    virtual mmRes InitEngine();		// must initialize engine object
    virtual mmRes InitApp();		// your initializtion here
    virtual mmRes IdleLoop();		// your app cycle
    virtual mmRes ShutApp();		// your deinitization here
    virtual mmRes ShutEngine();		// must deinitialize engine object
    virtual mmRes WindowSize();		// use it to catch window size changings
    
    virtual mmRes ModifyWindowClass(WNDCLASSEX* pWndClass);
    virtual mmRes ModifyWindowStyle(stAppWindowStyle* pStyle);
    virtual clsLog* CreateAppLog();
    virtual clsEntityFactory* CreateEntityFactory(clsEngine* pengine);
    

private:
	mmRes CreateAppWindow();
	mmRes HandleWindowCreation(HWND hwnd, WPARAM wpar, LPARAM lpar);
	mmRes HandleWindowDestruction(HWND hwnd, WPARAM wpar, LPARAM lpar);
	mmRes HandleWindowSize(HWND hwnd, WPARAM wpar, LPARAM lpar);
};

// Make function, define this instead WinMain
//================================================
clsApp* MakeApp();

#define USE_APPLICATION(classname) clsApp* MakeApp() { return new classname; }

#endif // _MHMAPP_INCLUDED