//==========================================================================================
// Biocrisis application
// Coded:	MozG
// Created:	7 july 2006
//==========================================================================================
#include "mhmbase.h"
#include "BKEntityFactory.h"
#include "BKGameCamera.h"
#include "BKSurvival.h"
#include "BKApp.h"

// Application constructor
//===============================================================
clsBiokrizApp::clsBiokrizApp()
{
	m_EngineINI		= "engine.ini";
	m_eMode			= enBKStartup;
	m_pSurvivalMode = mmnull;
}

// Application destructor
//===============================================================
clsBiokrizApp::~clsBiokrizApp()
{
}

// InitApp: initializes application
//===============================================================
mmRes clsBiokrizApp::InitApp()
{
	// currently only survival mode
	m_pSurvivalMode = new clsBiokrizSurvival(m_pEngine);
	if (m_pSurvivalMode == mmnull)
		mmThrow(enExceptOutOfMemory, clsBiokrizApp, InitApp, Survival mode alloc failed);
	m_eMode = enBKSurvival;
	
	if (MHMFAIL(m_pSurvivalMode->LoadObjects()))
	{
		g_plog->Out("clsBiokrizApp::InitApp >> survival mode objects loading failed", enLCRed);
		return mmFail;
	}
	
	if (MHMFAIL(m_pSurvivalMode->FirstTimeInit()))
	{
		g_plog->Out("clsBiokrizApp::InitApp >> survival mode objects initializing failed", enLCRed);
		return mmFail;
	}
	return mmOk;
}

// ShutApp: deinitializes application
//===============================================================
mmRes clsBiokrizApp::ShutApp()
{
	if (m_pSurvivalMode != mmnull)
	{
		m_pSurvivalMode->LeaveMode();
		mmSafeDelete(m_pSurvivalMode);
	}
	m_eMode = enBKStartup;
	return mmOk;
}

// IdleLoop: game idle loop
//===============================================================
mmRes clsBiokrizApp::IdleLoop()
{
	m_pEngine->Update();
	switch(m_eMode)
	{
	case enBKSurvival:
		if (MHMFAIL(m_pSurvivalMode->UpdateObjects()))
			return mmFail;
		break;
	}
	
	clsRenderer* pRenderer = m_pEngine->GetRenderer();
	pRenderer->StartRender();
	pRenderer->Clear(mmfalse);
	switch(m_eMode)
	{
	case enBKSurvival:
		if (MHMFAIL(m_pSurvivalMode->RenderObjects()))
			return mmFail;
		break;
	}
	pRenderer->StopRender();
	pRenderer->Present();
	//Sleep(100);
	return mmOk;
}

// ModifyWindowStyle: modifies default window style
//===============================================================
mmRes clsBiokrizApp::ModifyWindowStyle(stAppWindowStyle* pStyle)
{
	pStyle->m_nPosx = 0;
	pStyle->m_nPosy = 0;
	pStyle->m_nWidth = 800;
	pStyle->m_nHeight = 600;
	pStyle->m_sWindowName = "BIOKRIZ (Mayhem 2006)";
	return mmOk;
}

// WindowSize: event
//===============================================================
mmRes clsBiokrizApp::WindowSize()
{
	switch(m_eMode)
	{
	case enBKSurvival:
		if (MHMFAIL(m_pSurvivalMode->ResizeViewport()))
			return mmFail;
		break;
	}
	return mmOk;
}

// CreateEntityFactory: creates game entity factory
//===============================================================
clsEntityFactory* clsBiokrizApp::CreateEntityFactory(clsEngine* pengine)
{
	return new clsBiokrizEntityFactory(pengine);
}