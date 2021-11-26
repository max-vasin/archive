//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// Brick journey shareware project source code
// (c) Mayhem Projex 2006
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

#include "mhmbase.h"
#include "BJApp.h"
#include "BJEFactory.h"

// Application constructor
//--------------------------------------------------
clsBJApp::clsBJApp()
{
	m_EngineINI = "engine.ini";	
	m_pGameScene = mmnull;
}


// Application destructor
//--------------------------------------------------
clsBJApp::~clsBJApp()
{

}

// Initialize application
//--------------------------------------------------
mmRes clsBJApp::InitApp()
{
	m_pGameScene = m_pEngine->CreateScene("Game");
	m_pGameScene->AddChildNode("GameMatrix", "GameMatrix", mmtrue, mmtrue);
	m_pGameScene->AddChildNode("Cursor", "Cursor", mmtrue, mmtrue);
	m_pEngine->SetActiveScene(m_pGameScene);
	m_pEngine->GetRenderer()->SetAmbientLight(0xFFFFFFFF);
	m_Camera.Eye(0.0f, 66.0f, 1529.264f);
	m_Camera.UsePerspective();
	m_Camera.Up(0.0f, 1.0f, 0.0f);
	m_Camera.At(0.0f, 66.0f, 0.0f);
	m_Camera.Aspect(m_pEngine->GetRenderer()->GetViewport()->GetAspect());
	m_Camera.Near(0.01f);
	m_Camera.Far(10000.0f);
	m_Camera.Fov(mmToRadian(35));
	m_Camera.UseRH();
	m_Camera.BuildMatrixs();
	m_pText =   (clsText*)m_pEngine->GetRenderer()->GetResourceManager()->GetResource("myfont.mhmFont2D", enResText);
	m_pSprite = (clsSprite*)m_pEngine->GetRenderer()->GetResourceManager()->GetResource("BigImage.spr", enResSprite);
	m_pSprite->SetPosition(clsVector3(0.0f, 0.0f, 1.0f));
	m_pSprite->ReBuild();
	//m_pText = (clsFont2D*)m_pEngine->GetRenderer()->GetResourceManager()->GetResource("sysfont.tga", enResFont2D);
	stFontRect rc;
	rc.m_bottom = 100;
	rc.m_top    = 0;
	rc.m_left   = 0;
	rc.m_right  = 500;
	//m_pText->SetRect(&rc);
	m_pEngine->GetInput()->CaptureMouse();
	m_pEngine->GetRenderer()->Clear(mmtrue);
	return mmOk;
}

// Shut application
//--------------------------------------------------
mmRes clsBJApp::ShutApp()
{
	return mmOk;
}

// Idle loop
//--------------------------------------------------
mmRes clsBJApp::IdleLoop()
{
	//m_pEngine->GetTimer()->Update();
	m_pGameScene->Animate();
	clsString sFps;
	sFps.FromFloat(m_pEngine->GetTimer()->GetFPS());
	sFps += "\n" + m_pEngine->GetRenderer()->GetResourceManager()->m_Stats.FormatStatistic();
	
	m_pText->SetText(sFps);
	
	m_pEngine->Update();
	
	m_pEngine->GetRenderer()->SetCamera(&m_Camera);
	m_pEngine->GetRenderer()->StartRender();
	//m_pEngine->GetRenderer()->Clear(mmfalse);
	m_pSprite->Activate(mmnull);
	m_pSprite->Render();
//	m_pSprite->SetPosition(clsVector3(0.0f, 0.0f, 0.3f));
//	m_pSprite->ReBuild();
/*	m_pSprite->Render();
	m_pSprite->SetPosition(clsVector3(0.0f, 0.0f, 0.29f));
	m_pSprite->ReBuild();
	m_pSprite->Render();
	m_pSprite->SetPosition(clsVector3(0.0f, 0.0f, 0.28f));
	m_pSprite->ReBuild();
	m_pSprite->Render();
	m_pSprite->SetPosition(clsVector3(0.0f, 0.0f, 0.27f));
	m_pSprite->ReBuild();
	m_pSprite->Render();
	m_pSprite->SetPosition(clsVector3(0.0f, 0.0f, 0.26f));
	m_pSprite->ReBuild();
	m_pSprite->Render();
	m_pSprite->SetPosition(clsVector3(0.0f, 0.0f, 0.25f));
	m_pSprite->ReBuild();
	m_pSprite->Render();
	m_pSprite->SetPosition(clsVector3(0.0f, 0.0f, 0.24f));
	m_pSprite->ReBuild();
	m_pSprite->Render();
	m_pSprite->SetPosition(clsVector3(0.0f, 0.0f, 0.23f));
	m_pSprite->ReBuild();
	m_pSprite->Render();
	m_pSprite->SetPosition(clsVector3(0.0f, 0.0f, 0.22f));
	m_pSprite->ReBuild();
	m_pSprite->Render();
	m_pSprite->SetPosition(clsVector3(0.0f, 0.0f, 0.21f));
	m_pSprite->ReBuild();
	m_pSprite->Render();
	m_pSprite->SetPosition(clsVector3(0.0f, 0.0f, 0.20f));
	m_pSprite->ReBuild();
	m_pSprite->Render();*/
	
	m_pEngine->RenderScene();
	
	m_pText->Render();

	m_pEngine->GetRenderer()->StopRender();
	m_pEngine->GetRenderer()->Present();
	
//	Sleep(100);
	
	return mmOk;
}

// Window size
//--------------------------------------------------
mmRes clsBJApp::WindowSize()
{
	return mmOk;
}

// Modifig engine window style
//--------------------------------------------------
mmRes clsBJApp::ModifyWindowStyle(stAppWindowStyle* pStyle)
{
	pStyle->m_sWindowName = "Brick Journey 3D (Mayhem Projex 2006)";
	return mmOk;
}

// Creating brick journey entity factory
//--------------------------------------------------
clsEntityFactory* clsBJApp::CreateEntityFactory(clsEngine* pengine)
{
	return new clsBJEntityFactory(pengine);
}







