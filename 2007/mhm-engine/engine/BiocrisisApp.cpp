//==========================================================================================
// Biocrisis application
// Coded:	MozG
// Created:	7 july 2006
//==========================================================================================
#include "mhmbase.h"
#include "BiocrisisApp.h"

clsBiocrisisApp::clsBiocrisisApp():m_fAngles(0.0f, 0.0f, 0.0f)
{
	m_pText     = mmnull;
	m_pHero     = mmnull;
	m_EngineINI = "engine.ini";
//	OpenZipEnvironment();
//	AddZipEnvironment("data.dat");
	m_SceneHeroRun = mmnull;
}

clsBiocrisisApp::~clsBiocrisisApp()
{
//	CloseZipEnvironment();
}

mmRes clsBiocrisisApp::InitApp()
{
	m_pEngine->GetInput()->CaptureMouse();
	m_pEngine->GetInput()->CaptureKeyboard();
	m_pRenderer = m_pEngine->GetRenderer();
	m_pResMan   = m_pRenderer->GetResourceManager();
	m_pText = (clsText*)m_pResMan->GetResource("myfont.mhmFont2D", MHMRES_MISC_ID);
	m_pTextStatus = (clsText*)m_pResMan->GetResource("myfont.mhmFont2D", MHMRES_MISC_ID);
	m_pTextMouse = (clsText*)m_pResMan->GetResource("myfont.mhmFont2D", MHMRES_MISC_ID);
	m_pHero     = (clsBaseMesh*)m_pResMan->GetResource("hero.mmh", MHMRES_MESH_ID);
	m_pLight    = (clsBaseLight*)m_pResMan->GetResource("default light", enResBaseLight);
	
	RECT rc;
	GetClientRect(m_hWnd, &rc);
	clsRect rect(mmFloat(rc.left) + 100, mmFloat(rc.top), mmFloat(rc.right) - 100, mmFloat(rc.bottom));
	m_pText->SetRect(rect);
	m_pText->SetText("В 2245 году человечество столкнулось лицом к лицу с глобальной энергетической проблемой. Запасы природного газа, нефти и угля иссякли более 100 лет назад, а запасы топлива ядерных реакторов уменьшались с каждым годом. Постоянные междоусобные войны поставили человечество на грань полного исчезновения, но вместе с тем значительно продвинулся прогресс в сфере военных технологий. Когда казалось человечеству уже ничто не поможет, один ученый-исследователь юпитера открыл сверхтяжелый элемент, названный в последствии эндориум. Залежи руды, содержащей эндориум буквально излучали громадное количество энергии. Энергетическая проблема казалось была решена или отодвинута на большое время. На базе эндориума военные ученые разработали межзвездные двигатели поразительной мощности. Началась крупномасштабная колонизации галактики. Однако биологи открыли интересное факт -  излучение эндориума вызывало сильные мутации биологических форм жизни, особенно их нервной системы…\n\nС планетой X786Y98Z870 сектора Альфа-Круцис внезапно прекратилась связь. Дежуривший в этом секторе бот-разведчик зафиксировал следы разрушений колонии и отсутствие биологической активности. Еще более непонятным являлся тот факт, что сканирование планеты перед колонизацией не выявило никаких форм жизни, способных причинить серьезный вред человеку в защитном костюме. Для выяснение причин происшествия и поиска возможных выживших колонизаторов на планету был отправлен военный киборг, способный при необходимости уничтожить всю жизнь на планете…");
	//m_pText->SetText("С е");
	m_pText->SetColor(0xFF00FF00);
	m_pText->SetAlignX(enFillRect);
	m_pText->SetWordWrap(mmtrue);
	m_pText->SetAlignY(enBottom);
	
	m_pTextMouse->SetAlignX(enCenter);
	m_pTextMouse->SetAlignY(enCenter);
	m_pTextMouse->SetColor(0xFFFF33FF);
	
	
	
	m_cCamera.UseRH();
	m_cCamera.UsePerspective();
	//m_cCamera.SetPosCamera(0.0f, 600.0f, 0.0f);
	//m_cCamera.SetPosTarget(0.0f, 0.0f, 0.0f);
	//m_cCamera.RotationCameraToTargetHorizon(1.5f);
	m_cCamera.Far(100000.0f);
	m_cCamera.Fov(3.145f / 4.0f);
	m_pRenderer->SetCamera(&m_cCamera);
	
	m_pLight->SetType(enLightPoint);
	m_pLight->SetRange(100000.0f);
	m_pLight->SetAttenuation(enAttenuation1, 1.0f);
	m_pLight->SetDiffuse(clsColorRGBA(1.0f, 1.0f, 1.0f, 1.0f));
	//m_pLight->SetAmbient(clsColorRGBA(1.0f, 1.0f, 1.0f, 1.0f));
	m_pLight->SetSpecular(clsColorRGBA(1.0f, 1.0f, 1.0f, 1.0f));
	clsVector3 dir(9000.0f, 9000.0f, 9000.0f);//1000.0f);
	m_pLight->SetPosition(dir);
	
	m_pLight->SetTheta(1.0f);
	m_pLight->SetPhi(2.2f);
	m_pLight->SetFalloff(1.0f);
	
	m_pLight->Activate();
		
	m_SceneHeroRun = m_pEngine->CreateScene("dfd");
	m_SceneHeroRun->LoadMJ("meshes\\trees.mj");
	//m_SceneHeroRun->AssignController(&m_cLinearCtrl, mmtrue);
//	m_SceneHeroRun->FindList("Ammo_pack")->AssignController(&m_cConstRotCtrl);
	
	//m_cConstRotCtrl.SetSpeedZ(0.004f);
	
	
	clsEntity* pent = m_SceneHeroRun->FindList("mini_gun");
	pent->AssignController(&m_cConstRotCtrl);
	pent = m_SceneHeroRun->FindList("mini_gun_trunk");
	pent->AssignController(&m_cMiniGunRot);
	
	
	m_fMoveSpeed = 0.0f;
	m_fStrafeSpeed = 0.0f;
//	UpdateObjects();
	return mmOk;
}

mmRes clsBiocrisisApp::IdleLoop()
{
	m_pEngine->Update();
	UpdateObjects();
	RenderObjects();
	return mmOk;
}

mmRes clsBiocrisisApp::UpdateObjects()
{
	if (m_pEngine->GetInput()->GetKeyboard()->SnapshotKeyPressed(enKeyEscape))
	{
		Quit();
		return mmOk;
	}
	
	RECT rc;
	GetClientRect(m_hWnd, &rc);
	clsRect rect(mmFloat(rc.left) + 100, mmFloat(rc.top), mmFloat(rc.right) - 100, mmFloat(rc.bottom));
	
	static mmFloat inc = 0.0f;
	static mmInt   cols = 0;
	//m_pText->Build();
	m_pText->SetPos(0.0f /*rect.Width() / 2.0f*/, rect.Height() + 800 - inc, 0.0f);
	m_mxHeroRotation.RotationZ(inc / 30.0f);
	inc += 0.2f;
	static int ticks = 0;
//	if (ticks > 25)
//	{
		clsString status("FPS: ");
		clsString sfps;
		sfps.FromFormatNum(m_pEngine->GetTimer()->GetFPS(), "04.0");
		sfps.TrimL('0');
		status += sfps;
		status += "\nPoints: ";
		status += clsString(cols);
		m_pTextStatus->SetText(status);
//		clsPoint2D mpos = m_pEngine->GetInput()->GetMouse()->GetPosition();
//		sfps.FromFormatNum(mpos.x, "04");
//		sfps.TrimL('0');
//		status = "X:" + sfps;
//		sfps.FromFormatNum(mpos.y, "04");
//		sfps.TrimL('0');
//		status += "\nY:" + sfps;
		
		mmFloat offsx = (mmFloat)m_pEngine->GetInput()->GetMouse()->SnapshotAxisDelta(enMouseX);
		mmFloat offsy = (mmFloat)m_pEngine->GetInput()->GetMouse()->SnapshotAxisDelta(enMouseY);
		
		
		
		ticks = 0;
//	}
	ticks++;
	clsEntity* pent = m_SceneHeroRun->FindList("Camera02");
	m_fAngles.y += -(mmFloat)offsx / 500.0f;
	m_fAngles.x += -(mmFloat)offsy / 500.0f;
	clsMatrix mxRot, mxMove, mxTrans;
	mxMove.RotationX(m_fAngles.x);
	mxRot.RotationY(m_fAngles.y);
	mxMove *= mxRot;
	
	clsKeyboard* pkb = m_pEngine->GetInput()->GetKeyboard();
	const clsKeyboardState* pstate = pkb->GetState();
	
	clsVector3 ts = pent->m_mxLocal.GetTranslation();
	for (mmUint i = 0; i < pstate->GetNumAffectedKeys(); i++)
	{
		const clsKeyState* pks = pstate->GetKeyState(i);
		if (pks->m_code == enKeyW)
		{
			if (pks->m_pressed)
				m_fMoveSpeed = -1.5f;
			else
				if (m_fMoveSpeed < 0.0f)
					m_fMoveSpeed = 0.0f;
		}
		if (pks->m_code == enKeyS)
		{
			if (pks->m_pressed)
				m_fMoveSpeed = 1.5f;
			else
				if (m_fMoveSpeed > 0.0f)
					m_fMoveSpeed = 0.0f;
		}
		if (pks->m_code == enKeyA)
		{
			if (pks->m_pressed)
				m_fStrafeSpeed = -1.5f;
			else
				if (m_fStrafeSpeed < 0.0f)
					m_fStrafeSpeed = 0.0f;
		}
		if (pks->m_code == enKeyD)
		{
			if (pks->m_pressed)
				m_fStrafeSpeed = 1.5f;
			else
				if (m_fStrafeSpeed > 0.0f)
					m_fStrafeSpeed = 0.0f;
		}
		if ((m_fMoveSpeed != 0.0f) || (m_fStrafeSpeed != 0.0f))
			m_cConstRotCtrl.SetAmpSinX(0.1f);
		else
			m_cConstRotCtrl.SetAmpSinX(0.0f);
		
	}
	clsVector3 speed(m_fStrafeSpeed, 0.0f, m_fMoveSpeed);
	speed.Transform(mxMove);
	ts += speed;
	mxTrans.Translation(ts);
	pent->m_mxLocal = mxMove * mxTrans;//.Translation(ts.x, ts.y, ts.z);
	m_SceneHeroRun->AnimationFrame(m_pEngine->GetTimer()->GetMiSecs(),
								   m_pEngine->GetTimer()->GetDeltaMiSecs());
	m_SceneHeroRun->Animate();
	m_cCamera.SetViewMatrix(pent->m_mxFull);
	m_cCamera.BuildMatrixs();
	
	static mmBool firing = mmfalse;
	
	if (m_pEngine->GetInput()->GetMouse()->SnapshotButton(enMouseButton0) == enMouseButtonDown)
	{
		m_cMiniGunRot.SetSpeedX(0.01f);
		firing = mmtrue;
	}
	else
	{
		m_cMiniGunRot.SetSpeedX(0.0f);
		firing = mmfalse;
	}
	
	
	// Collision detection
	
	clsPoint2D chair((mmInt)((mmFloat)m_pEngine->GetRenderer()->GetViewport()->m_nWidth / 2.0f),
					 (mmInt)((mmFloat)m_pEngine->GetRenderer()->GetViewport()->m_nHeight / 2.0f));
	clsRay lookray = m_pEngine->GetRenderer()->UnProject(chair);

	m_pTextMouse->SetPos((mmFloat)chair.x, (mmFloat)chair.y);
	
	// SCORL005
	clsEntity* pa = m_SceneHeroRun->FindList("Camera02");
	clsEntity* pb = m_SceneHeroRun->FindList("korpus");
	
	clsBaseMesh* pmesh = (clsBaseMesh*)pb->Res;
	clsBaseMaterial* pmtl = (clsBaseMaterial*)pmesh->GetMaterial(0);
	
	static mmBool stroke = mmfalse;
	if (!firing)
		//pmtl->m_map_diffuse.m_enabled = mmtrue;
		pmtl->m_dxmtl.Diffuse.r = 0.0f;
	
	stroke = !stroke;
	
//	clsBoundSphere sa = *((clsBaseMesh*)pa->Res)->GetBoundSphere();
	clsBoundSphere sb = *((clsBaseMesh*)pb->Res)->GetBoundSphere();
	
	//sa.m_cCenter.Transform(pa->m_mxFull);
	sb.m_cCenter.Transform(pb->m_mxFull);
	//sb.m_fRadius *= (pb->m_mxFull._11 + pb->m_mxFull._22 + pb->m_mxFull._33) / 3.0f;
	//lookray.Normalize();
	//lookray *= clsVector3(10000.0f, 10000.0f, 10000.0f);
	if (sb.RayIntersects(lookray) && firing)
	{
		if (pmesh->RayIntersect(lookray, &pb->m_mxFull))
		{
			if (stroke)
				//pmtl->m_map_diffuse.m_enabled = mmfalse;
				pmtl->m_dxmtl.Diffuse.r = 1.0f;
			else
				pmtl->m_dxmtl.Diffuse.r = 0.0f;
			cols++;
		}
	}
	
/*	clsString flt, or;
	flt.FromFloat(lookray.x);
	or = flt;
	flt.FromFloat(lookray.y);
	or += "\n" + flt;
	flt.FromFloat(lookray.z);
	or += "\n" + flt;*/
	
	m_pTextMouse->SetText("*");
	//if (sa.SphereIntersects(sb))
	//	cols++;
	
	
	
	return mmOk;
}

struct lv
{
	clsVector3 pos;
	mmDword	   clr;
};

mmRes clsBiocrisisApp::RenderObjects()
{
	m_pRenderer->SetCamera(&m_cCamera);
	m_pRenderer->StartRender();
	m_pRenderer->Clear();
//	m_pText->Render();
	m_pTextStatus->Render();
	m_pTextMouse->Render();
//	m_pRenderer->GetDevice()->SetModelTM(&m_mxHeroRotation);
//	m_pHero->Render();*/
	m_pEngine->RenderScene(m_SceneHeroRun);
	
	
	clsPoint2D chair((mmInt)((mmFloat)m_pEngine->GetRenderer()->GetViewport()->m_nWidth / 2.0f),
					 (mmInt)((mmFloat)m_pEngine->GetRenderer()->GetViewport()->m_nHeight / 2.0f));
	clsRay lookray = m_pEngine->GetRenderer()->UnProject(chair);
	//lookray *= clsVector3(0.001f, 0.001f, 0.001f);
	clsVector3 at = m_cCamera.m_view.GetTranslation();
//	lookray.Normalize();
//	lookray.x *= 50.0f;
//	lookray.y *= 50.0f;
//	lookray.z *= 50.0f;
//	clsMatrix mxTr, mxSc;
//	mxTr.Translation(lookray);
	//mxSc.Scale(20.0f, 20.0f, 20.0f);
	//mxTr = mxSc * mxTr
	
/*	m_pRenderer->GetDevice()->SetModelTM(mmnull);
	lv verts[2];
	clsVector3 dir = lookray - at;
	dir.Normalize();
	dir *= clsVector3(10.0f, 10.0f, 10.0f);
	verts[0].pos = dir;
	dir *= clsVector3(200.0f, 200.0f, 200.0f);
	verts[1].pos = lookray;
	verts[0].clr = 0xFF00FF00;
	verts[1].clr = 0xFFFF0000;
	IDirect3DDevice9* pdev = *m_pRenderer->GetDevice();
	pdev->SetFVF(D3DFVF_XYZ|D3DFVF_DIFFUSE);
	pdev->SetRenderState(D3DRS_ANTIALIASEDLINEENABLE, TRUE);
	mmFloat psize = 10.0f;
	pdev->SetRenderState(D3DRS_POINTSIZE, *(DWORD*)&psize);
	pdev->DrawPrimitiveUP(D3DPT_POINTLIST, 2, verts, 16);*/
	
	m_pRenderer->StopRender();
	
	//UpdateObjects();
	m_pRenderer->Present();
	return mmOk;
}

mmRes clsBiocrisisApp::ModifyWindowStyle(stAppWindowStyle* pStyle)
{
	pStyle->m_nPosx = 0;
	pStyle->m_nPosy = 0;
	pStyle->m_nWidth = 800;
	pStyle->m_nHeight = 600;
	return mmOk;
}

mmRes clsBiocrisisApp::WindowSize()
{
	m_cCamera.Aspect((mmFloat)m_pRenderer->GetWidth() / (mmFloat)m_pRenderer->GetHeight()); 
	return mmOk;
}