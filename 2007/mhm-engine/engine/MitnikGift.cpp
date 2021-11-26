//==========================================================================================
// Biocrisis application
// Coded:	MozG
// Created:	7 july 2006
//==========================================================================================
#include "mhmbase.h"
#include "MitnikGift.h"
//==========================================================================================
// BIOKRIZ entity factory class implementation
// Coded:	MozG
// Created:	29 july 2006
//==========================================================================================
#include "mhmbase.h"
#include "BKGameCamera.h"
#include "BKCrosshair.h"
#include "BKHero.h"
#include "BKCreature.h"
#include "BKFlyer.h"
#include "BKSucker.h"
#include "BKOrc.h"
#include "BKBug.h"
#include "BKCreatureInstance.h"
#include "BKCreatureEmitter.h"
#include "BKDirector.h"
#include "BKBaseDecal.h"
#include "BKParticle.h"
#include "BKParticleEmitter.h"
#include "BKAllParticles.h"
#include "BKWeapon.h"
#include "BKSpriteParticles.h"
#include "BKBonus.h"
#include "BKBonusManager.h"

#include "BKEntityFactory.h"

// Entity factory constructor
//==========================================================================================
clsMitnikEntityFactory::clsMitnikEntityFactory(clsEngine *pEngine):
						 clsEntityFactory(pEngine)
{
}

// Entity factory destructor
//==========================================================================================
clsMitnikEntityFactory::~clsMitnikEntityFactory()
{
}

// CreateEntity: creates new entity objects
//==========================================================================================
clsEntity* clsMitnikEntityFactory::CreateEntity(const clsString &type,
												 const clsString &name)
{
	clsString sType(type);
	sType.UpperCase();
	clsString entname(name);
	entname.UpperCase();
	clsEntity* pEntity = mmnull;
	if (pEntity == mmnull)
		pEntity = new clsEntity(this, type, name);
	return pEntity;
}




// Application constructor
//===============================================================
clsMitnikApp::clsMitnikApp()
{
	m_EngineINI		= "engine.ini";
}

// Application destructor
//===============================================================
clsMitnikApp::~clsMitnikApp()
{
}

// InitApp: initializes application
//===============================================================
mmRes clsMitnikApp::InitApp()
{
	m_pText = (clsText*)m_pEngine->GetRenderer()->GetResourceManager()->GetResource("font.mhmFont2D", MHMRES_MISC_ID);
	m_pBarrel = (clsBaseMesh*)m_pEngine->GetRenderer()->GetResourceManager()->GetResource("barrel.mmh", MHMRES_MESH_ID);
	m_pBarrelScene = m_pEngine->CreateScene("BarrelScene");
	m_pBarrelScene->LoadMJ("meshes\\barrel.mj");
	m_pAutoCamera = m_pBarrelScene->FindChild("AutoCamera");
	
	m_pStairsScene = m_pEngine->CreateScene("StairsScene");
	m_pStairsScene->LoadMJ("meshes\\stairs.mj");
	m_pStairsScene->LoadAnimationKeys("stairs.mhmAnim");
	m_pStairsScene->PlayAnimationRange("play");
	m_pAutoCamera = m_pStairsScene->FindChild("Camera02");

	m_eState			= enWords;
	m_nWordCount		= 0;
	m_fWordTime			= 0.0f;
	m_fWordAppearTime	= 3.0f;
	
	m_pText->SetText("\bgцксанйнсбюфюелши\n\brлхръи\n");
	clsRect rect(0.0f, 0.0f, (mmFloat)m_pEngine->GetRenderer()->GetWidth(), (mmFloat)m_pEngine->GetRenderer()->GetHeight());
	m_pText->SetRect(rect);
	m_pText->SetAlignX(enCenterRect);
	m_pText->SetAlignY(enCenterRect);
	
	m_Cam.UsePerspective();
	m_Cam.Aspect((mmFloat)m_pEngine->GetRenderer()->GetWidth() / (mmFloat)m_pEngine->GetRenderer()->GetHeight());
	m_Cam.Fov(MHM_PI / 2.0f);
	m_Cam.Eye(200.0f, 0.0f, 0.0f);
	m_Cam.At(0.0f, 0.0f, 0.0f);
	m_Cam.Up(0.0f, 0.0f, 1.0f);
	//m_Cam.SetViewMatrix(m_pAutoCamera->m_mxWorld);
	//clsMatrix mxTemp;
	//mxTemp.LookAtRH(clsVector3(0.0f, 400.0f, 0.0f), clsVector3(0.0f, 0.0f, 0.0f), clsVector3(0.0f, 0.0f, 1.0f));
	m_Cam.UseRH();
	m_Cam.RotationCameraToTargetHorizon(mmToRadian(25.0f));
	m_Cam.BuildMatrixs();
	
	m_pMusic = m_pEngine->GetStream("sounds\\trance.ogg");
	
	
	
	m_mxProj.PerspectiveFovRH(MHM_PI / 2.0f, (mmFloat)m_pEngine->GetRenderer()->GetWidth() / (mmFloat)m_pEngine->GetRenderer()->GetHeight(), 0.1f, 10000.0f);
	
	//m_pEngine->SetActiveScene(m_pBarrelScene);
	//m_pBarrelScene->Animate();
	m_pEngine->SetActiveScene(m_pStairsScene);
	m_pStairsScene->Animate();
	m_pEngine->Update();
	
	return mmOk;
}

// ShutApp: deinitializes application
//===============================================================
mmRes clsMitnikApp::ShutApp()
{
	return mmOk;
}

// IdleLoop: game idle loop
//===============================================================
mmRes clsMitnikApp::IdleLoop()
{
	clsMatrix mxTemp;
	
	
	switch(m_eState)
	{
	case enWords:
		BuildWords();
		break;
	case enBegining:
		m_mxWorld.RotationZ(m_pEngine->GetTimer()->GetSecs());
		m_pBarrelScene->FindChild("barrel")->m_mxLocal = m_mxWorld;
		mxTemp.RotationZ(m_pEngine->GetTimer()->GetSecs() / 10.0f);
		m_mxBSphere = mxTemp;
		mxTemp.RotationY(m_pEngine->GetTimer()->GetSecs() / 8.0f);
		m_mxBSphere *= mxTemp;
		mxTemp.RotationX(m_pEngine->GetTimer()->GetSecs() / 7.0f);
		m_mxBSphere *= mxTemp;
		m_pBarrelScene->FindChild("bsphere")->m_mxLocal = m_mxBSphere;
		m_pBarrelScene->Animate();
		
		m_pStairsScene->AnimationRangeBuild(m_pEngine->GetTimer()->GetDeltaMiSecs() / 10.0f);
		m_pStairsScene->Animate();
		break;
	}
	
	m_pEngine->Update();
		
	
	clsRenderer* pRenderer = m_pEngine->GetRenderer();
	m_Cam.Eye(m_pAutoCamera->m_mxFull.GetTranslation());
	pRenderer->SetCamera(&m_Cam);
	//pRenderer->GetDevice()->SetModelTM(&m_mxWorld);
	//pRenderer->GetDevice()->SetViewTM(&m_pAutoCamera->m_mxFull);
	//pRenderer->GetDevice()->SetProjectionTM(&m_mxProj);
	
	pRenderer->StartRender();
	pRenderer->GetDevice()->SetAmbientLight(0x00000000);
	switch(m_eState)
	{
	case enWords:
		pRenderer->Clear(mmtrue);
		m_pText->Render();	
		break;
	case enBegining:
		pRenderer->Clear(mmtrue);
		pRenderer->GetDevice()->EnableLighting(mmtrue);
		pRenderer->GetDevice()->EnableSpecular(mmtrue);
		//m_pEngine->RenderScene(m_pBarrelScene);
		m_pEngine->RenderScene(m_pStairsScene);
		//m_pBarrel->Render();
		break;
	}
	//m_pBarrel->Render();
	
	pRenderer->StopRender();
	pRenderer->Present();
	return mmOk;
}

// ModifyWindowStyle: modifies default window style
//===============================================================
mmRes clsMitnikApp::ModifyWindowStyle(stAppWindowStyle* pStyle)
{
	pStyle->m_nPosx = 0;
	pStyle->m_nPosy = 0;
	pStyle->m_nWidth = 800;
	pStyle->m_nHeight = 600;
	pStyle->m_sWindowName = "Happy birthday mitnik (c) Mayhem projex 2006";
	return mmOk;
}

// WindowSize: event
//===============================================================
mmRes clsMitnikApp::WindowSize()
{
	return mmOk;
}

// CreateEntityFactory: creates game entity factory
//===============================================================
clsEntityFactory* clsMitnikApp::CreateEntityFactory(clsEngine* pengine)
{
	return new clsMitnikEntityFactory(pengine);
}
//===============================================================
mmVoid clsMitnikApp::BuildWords()
{
	m_fWordTime += m_pEngine->GetTimer()->GetDeltaSecs();
	if (m_fWordTime >= m_fWordAppearTime)
	{
		m_fWordTime -= m_fWordAppearTime;
		switch(m_nWordCount)
		{
		case 0:
			m_pText->SetText("\bgцпсоою юкйнцнкэмшу нкхцюпунб\n\bbMAYHEM PROJEX");
			m_nWordCount++;
			break;
		case 1:
			m_pText->SetText("\bgкхвмн ъ\n\brMozG");
			m_nWordCount++;
			break;
		case 2:
			m_pText->SetText("\bgунрекх аш бшпюгхрэ бюл\n\baRESPECT\n\bgх\n\byсбюфсус");
			m_nWordCount++;
			break;
		case 3:
			m_pText->SetText("\bgонгдпюбхб HAPPY BIRTHDAY");
			m_nWordCount++;
			break;
		case 4:
			m_pText->SetText("\bgщрни лецюмюбюпнвеммни делни");
			m_nWordCount++;
			break;
		case 5:
			m_pText->SetText("\bgонеуюкх...(c) цЮЦЮПХМ");
			m_nWordCount++;
			break;
		case 6:
			m_pText->SetText("\bgыюонцндэ\nяюсмд ондцпсгхл...");
			m_nWordCount++;
			m_pMusic->Play();
			m_fWordAppearTime = 6.75f;
			break;
		case 7:
			m_pText->SetText("\bgюоърэ ыюонцндэ\nLOADING GRAPHIC NODES...");
			m_nWordCount++;
			m_fWordAppearTime = 6.75f;
			break;
		case 8:
			m_eState = enBegining;
			break;
		}
	}
}