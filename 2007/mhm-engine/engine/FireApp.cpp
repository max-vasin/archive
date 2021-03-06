#include "mhmbase.h"
#include "FireApp.h"


mmBool SphereIntersects(clsVector3 & p1,clsVector3 & _p1, clsVector3 & p2,  clsVector3 & _p2,mmFloat r1, mmFloat r2, clsVector3 & out)
{
	clsVector3	vRelSpeed = (_p2-p2) - (p1 - _p1);
	clsVector3  vRelPos   = p2 - p1;
	mmFloat		fMinDist  = r1 + r2;
	mmFloat		a = vRelSpeed.LengthSq();
	mmFloat		b = 2 * vRelSpeed.Dot(vRelPos);
	mmFloat		c = vRelPos.LengthSq() - fMinDist*fMinDist;
	mmFloat	t1,t2;
	if(vRelSpeed.LengthSq() == 0.0f)
		return mmfalse;
	if (vRelPos.LengthSq() <= (fMinDist*fMinDist))
	{
		out = p1;
		return mmtrue;
	}

	mmFloat q = b*b - 4*a*c;
	if (q >= 0)
	{
		mmFloat sq = (mmFloat)mmSqrt(q);
		mmFloat d  = 1.0f / (2*a);
		t1 = (-b + sq) * d;
		t2 = (-b - sq) * d;
		t1 = min(t1,t2);
		if( (t1 > 1.0f)||(t1 < 0.0f) )
			return mmfalse;

		out = (p1-_p1) * t1 + p1;
		return mmtrue; // real roots (настоящие корни)
	}
	return mmfalse; // complex roots (комплексные корни == нафиг)
}


//*************************************************
mmRes clsFireApp::ShutApp()
{
	mmSafeDelete(m_s);
//	mmSafeDelete(m_sg);

	for(mmUint z=0;z < m_sounds_g.Count();z++)
		mmSafeDelete(m_sounds_g[z]);

	mmSafeDelete(m_s_up);
	mmSafeDelete(m_s_pr);
	mmSafeDelete(m_s_tush);
	mmSafeDelete(m_s_fdo);
	mmSafeDelete(m_s_jertva);
	mmSafeDelete(m_s_boom1);
	mmSafeDelete(m_s_key);
	mmSafeDelete(m_s_dver);
	mmSafeDelete(m_s_kar);
	mmSafeDelete(m_s_trrii);
	mmSafeDelete(m_s_din);



	if(m_pInput)
		return m_pInput->FreeMouse();
	return mmOk;
}
//------------------------------------------------------------------------------------------------------------
clsFireApp::clsFireApp(void)
{



#ifdef NDEBUG
	m_ExitOnEsc = mmfalse;
#endif

	m_Start = mmtrue;

	m_NoInit = mmfalse;

	m_SoundVoleme = 50;
	m_FxVoleme = 64;

	m_Curent_Resolutions = 0;

	m_mouse = mmtrue;

	m_mous_x = 0;
	m_mous_y = 0;

//	if(	OpenZipEnvironment("firedata.dat") == mmfalse)
//	{
		OpenZipEnvironment();
//	}

	m_curent = mmnull;

	m_df = 1.0f;

	m_s = mmnull;
	m_pInput = mmnull;

	m_Scene = mmnull;
	m_Renderer = mmnull;
//	m_Text     = mmnull;
//	m_Text1     = mmnull;
	m_cursor = mmnull;
	m_EngineINI = "\\engine.ini";





}
//------------------------------------------------------------------------------------------------------------
clsFireApp::~clsFireApp(void)
{
	CloseZipEnvironment();
}
//------------------------------------------------------------------------------------------------------------
mmRes clsFireApp::ClearScenes()
{

	mmSafeDelete(m_Scene);
	m_Scene = mmnull;
	return mmOk;
}
//------------------------------------------------------------------------------------------------------------
mmRes clsFireApp::InitStart(mmBool waitKey)
{
	m_df = 1.0f;
	m_waitKey = waitKey;
	m_win = 0;

//	m_scores = 0;
//	zast_state = 0;
	m_Timer->Initialization();
//	m_Zast->PlayAnimationRangeStart("zast_open", enAnimHold);

	return mmOk;

}
//------------------------------------------------------------------------------------------------------------
mmVoid clsFireApp::SetTextureZast(const mmString nam)
{
	((clsBaseMaterial*)((clsBaseMesh*)m_zast_text->Res)->GetMaterial(0))->m_map_diffuse.m_texture =
	(clsBaseTexture*)m_ResMan->GetResource(nam, MHMRES_TEXTURE_ID);

	
}
//---------------------------------------------------------------------------------
mmRes clsFireApp::InitApp()
{


	m_Pause = mmfalse;
	/*
	clsSound * m_ss;
	m_ss = m_pEngine->GetSound("Muz/test.ogg");	
	PlaySound(m_ss);
	mmFloat delta;
	for(mmFloat z = 1;z<65000.0f;z+=100)
	{
		delta = 20*log10( 1.0f/z );
		delta = (96.0f+delta)/96.0f;
		delta=((1.0f - z/65000.0f)+delta)*0.5f;
		m_ss->Volume( delta);
		Sleep(1);
	}

	return 0;
*/

//20*log10(1/65000)

/*
		clsVar v("zx");

	clsString sss;
	sss = "//zxczczxczx \n boo = False a=1 b=[1, [777,333],12,{a1=11,a2=22},\"xx\"] c = \"z\" d = {q=1 stru = {z1=1 z2=2} w=2 e=\"*\"} e=0";
	//sss = "arr = [12,\"xx\"]";
	v.log = mmtrue;
	v.ParseFromMem(sss.Data(),sss.Length());

	mmInt z;
	z=1;
	*/
	
	m_Renderer = m_pEngine->GetRenderer();
	m_ResMan   = m_Renderer->GetResourceManager();


	
	
	clsViewport* pViewPort = m_Renderer->GetDevice()->GetViewport();

	

	m_logo = (clsSprite*)m_ResMan->GetResource("$"+ clsString(pViewPort->m_nWidth)  + ":"+ clsString(pViewPort->m_nHeight) +".spr", MHMRES_MISC_ID);
	m_logo->SetTexture("inter/logo.jp2");
	
	m_logo->SetPosition(clsVector3((mmFloat)(pViewPort->m_nWidth >> 1)-0.5f,(mmFloat)(pViewPort->m_nHeight >> 1)-0.5f,0.5));
	m_logo->SetBlendType(enBlendAlpha);
	
mmUint co;
	for(mmUint z = 0;z<256;z+=2)
	{
		Sleep(1);
	m_Renderer->StartRender();
	m_Renderer->Clear(mmtrue);

	co = (z<<24)|0x00FFFFFF;

	m_logo->SetColor(co);

	m_logo->ReBuild(mmtrue);
	m_logo->Render();
	m_Renderer->StopRender();

	m_Renderer->Present();
	}


/*
	clsText * ttt;
	ttt = (clsText*)(m_ResMan->GetResource("font_01.mhmFont2D", MHMRES_MISC_ID));
	ttt->SetText("ZXC");
	ttt->SetRect(clsRect(320,240,600,600));

	ttt->Render();
*/

	


	//m_conf.log = mmtrue;
	m_conf.ParseFromFile("config.ini");




	
	

	m_FxVoleme = (mmInt)(m_Renderer->GetDevice()->m_pconfig->m_SoundVolume * 64.0f);
	m_SoundVoleme = (mmInt)(m_Renderer->GetDevice()->m_pconfig->m_StreamVolume * 64.0f);


	//m_s = m_pEngine->GetSound("Muz/choppi.xm");

	m_s =  m_pEngine->GetSound( *m_conf.FindFullVar("sound_menu/file") );
	m_pEngine->GetSM()->GlobalBoostXM(2.5f);

	for(mmInt z=0;z<(mmInt)m_conf.FindVar("sounds_game")->GetCount();z++)
	{
		m_sg = m_pEngine->GetSound(*(*m_conf.FindVar("sounds_game"))[z]->FindVar("file"));
		m_sounds_g.Add(m_sg);
	}
	m_sg = m_sounds_g[0];

	PlaySound(m_s);


	LoadHS();
	SaveHS();

	m_SceneTMP = mmnull;

//	SZ = enStart;
	
	m_level = 1;

	m_waitKey = mmfalse;

	
	m_Timer = m_pEngine->GetTimer();
/*
	m_Text = (clsText*)m_ResMan->GetResource("font_01.mhmFont2D", MHMRES_MISC_ID);
	m_Text->SetAlignX(enCenterRect);
	m_Text->SetAlignY(enBottomRect);
	clsViewport* pViewPort = m_Renderer->GetDevice()->GetViewport();
	m_Text->SetRect(clsRect(0,0,(mmFloat)pViewPort->m_nWidth,(mmFloat)pViewPort->m_nHeight));
	m_Text->SetSize((mmFloat)pViewPort->m_nWidth * 0.32f);


	m_TextState = (clsText*)m_ResMan->GetResource("state.mhmFont2D", MHMRES_MISC_ID);
	//m_TextState->SetAlignX(enCenterRect);
	m_TextState->SetAlignY(enBottomRect);
	clsViewport* pViewPort = m_Renderer->GetDevice()->GetViewport();
	m_TextState->SetRect(clsRect(0,0,(mmFloat)pViewPort->m_nWidth,(mmFloat)pViewPort->m_nHeight));
	m_TextState->SetSize((mmFloat)pViewPort->m_nWidth * 0.32f);

	m_Text1 = (clsText*)m_ResMan->GetResource("font_01.mhmFont2D", MHMRES_MISC_ID);
	m_Text1->SetSize((mmFloat)pViewPort->m_nWidth * 0.32f);
	m_Text1->SetX( (mmFloat)pViewPort->m_nWidth * 0.02f );
	m_Text1->SetY( (mmFloat)pViewPort->m_nWidth * 0.01f );
*/

	m_cursor = (clsText*)m_ResMan->GetResource("cursor.mhmFont2D", MHMRES_MISC_ID);
	m_cursor->SetText("V");
	m_cursor->SetSize(32);
	m_cursor->SetColor(0xFFFFFFFF);

	//m_Text->SetFiltering(mmtrue);
	//m_Text1->SetFiltering(mmtrue);


	m_pInput = m_pEngine->GetInput();
    m_pInput->CaptureMouse();
//	m_pInput->CaptureKeyboard();
    m_pMouse = m_pInput->GetMouse();
	//m_pMouse->SetPositionCenter();

	m_pMouse->SetPositionCenter();
	clsPoint2D pm = m_pMouse->GetPosition();
	pm.y = (mmInt)((mmFloat)pm.y / 2.5f);
	m_pMouse->SetPosition(pm);

	m_Light    = (clsBaseLight*)m_ResMan->GetResource("default light", enResBaseLight);

	m_Zast = (clsFireEntity*)m_pEngine->CreateScene("zast_01");	
	m_Zast->LoadMJ("Meshes/zast_01.mj");
	m_Zast->LoadAnimationKeys("anim_01.mhmAnim");
	m_Zast->EnableBoundSphereTest(mmfalse,mmtrue);

	//m_Zast->m_curent = m_main;

	m_zast_text = ((clsEntity*)m_Zast->FindChild("z_text"));
//	m_d2 = ((clsEntity*)m_Zast->FindChild("dver2"));

	
	SetTextureZast("inter/fire_starter.mtj");
	SetTextureZast("inter/game_over.mtj");
	SetTextureZast("inter/level_complete.mtj");
	SetTextureZast("inter/loser.mtj");
	SetTextureZast("inter/pause.mtj");
	SetTextureZast("inter/options.mtj");
	SetTextureZast("inter/info.mtj");
	SetTextureZast("inter/exit.mtj");
	SetTextureZast("inter/main_menu.mtj");
	SetTextureZast("inter/hiscores.mtj");

	SetTextureZast("inter/none.mtj");
	

	
	//m_pEngine->SetActiveScene(m_Zast);

	m_Camera.UseRH();
	m_Camera.UsePerspective();
	m_Camera.At(115.481f, 0.0f, 120.927f);
	m_Camera.Eye(115.481f, -531.163f, 120.927f);
	m_Camera.Up(0.0f, 0.0f, 1.0f);
	m_Camera.Far(2000.0f);
	m_Camera.Fov(0.6024178446184f);
	m_Renderer->SetCamera(&m_Camera);

	m_CameraZ = m_Camera;

	m_CameraZ.SetViewMatrix( ((clsFireEntity*)m_Zast->FindChild("Camera01"))->m_mxFull );

	m_ZastPlan = ((clsFireEntity*)m_Zast->FindChild("Rectangle01"));
	m_ZastPlan->SetOwner(mmnull);

//	m_s = m_pEngine->GetSound("Muz/choppi.xm");
	//m_s->Play((mmDword)this);

//	m_pEngine->GetSM()->GlobalBoostXM(2.5f);
	
//	PlaySound(m_s);

	//m_sg = m_pEngine->GetSound("Muz/game.xm");


	//level = level % (mmInt)m_conf.FindVar("level_index")->GetCount();

//	level = *(*m_conf.FindVar("level_index"))[level];

	

	m_s_up = m_pEngine->GetSample("Muz/fx/up.ogg");
	m_s_pr = m_pEngine->GetSample("Muz/fx/priz.ogg");
	m_s_tush = m_pEngine->GetSample("Muz/fx/tush.ogg");
	m_s_fdo = m_pEngine->GetSample("Muz/fx/fdo.ogg");
	m_s_jertva = m_pEngine->GetSample("Muz/fx/jertva.ogg");
	m_s_boom1 = m_pEngine->GetSample("Muz/fx/boom1.ogg");
	m_s_key = m_pEngine->GetSample("Muz/fx/keys.ogg");
	m_s_dver = m_pEngine->GetSample("Muz/fx/dver.ogg");
	m_s_kar = m_pEngine->GetSample("Muz/fx/kar.ogg");
	m_s_trrii = m_pEngine->GetSample("Muz/fx/trrii.ogg");
	m_s_din = m_pEngine->GetSample("Muz/fx/din.ogg");

	


	m_rand.init((mmUint)m_Timer->GetTicks());

	




	m_main = new cls_Main(this);
	m_exit = new cls_Exit(this);
	m_info = new cls_Info(this);
	m_game = new cls_Game(this);
	m_end_game = new cls_EndGame(this);
	m_next_level = new cls_NextLevel(this);
	m_hi_scores = new cls_HiScores(this);
	m_options = new cls_Options(this);
	m_pause = new cls_Pause(this);

	m_curent = m_main;
	//m_curent = m_next_level;

	SetTextureZast("inter/main_menu.mtj");
	m_Timer->Initialization();
	ZClose(m_main);
	//app->ZClose(app->m_return);

	InitStart(mmtrue);

	
	

	return mmOk;
}
//------------------------------------------------------------------------------------------------------------
mmRes clsFireApp::IdleLoop()
{
	PlayMuza();

	m_sizeF = 0;

	/*
	if((m_curent != m_game))
		m_Timer->SetDeltaFactor(0.0f);
	else
		m_Timer->SetDeltaFactor(1.0f);
	*/
	
	m_pEngine->Update();
	UpdateObjects();

	m_Renderer->Present();
	m_Renderer->StartRender();
	m_Renderer->Clear();

	RenderObjects();

	

	m_Renderer->StopRender();

	
	
	

	return mmOk;
}
//------------------------------------------------------------------------------------------------------------
mmRes clsFireApp::UpdateObjects()
{
	
//	for(int z=0;z<1;z++)
//	{
//	m_pMouse->Snapshot();
//	}
	m_mous_x = (mmFloat)m_pMouse->GetPosition().x;
	m_mous_y = (mmFloat)m_pMouse->GetPosition().y;


//	static clsWinMouse mm;	
//	mm.Update();
	//m_mous_x = mm.x;
	//m_mous_y = mm.y;


	if(m_curent)
		m_curent->UpdateObjects();


	m_Zast->AnimationRangeBuild(m_Timer->GetDeltaMiSecs());
	m_Zast->Animate();

	m_cursor->SetX(m_mous_x);
	m_cursor->SetY(m_mous_y);
	mmUint c = (mmUint)(((sinf( m_Timer->GetMiSecs() * 0.0005f ) + 1.0f)*0.5f)*255.0f);
	m_cursor->SetColor(0xFFFF0000 | c | ((255 -c)<<8)  );

	return mmOk;
}
//------------------------------------------------------------------------------------------------------------
mmRes clsFireApp::RenderObjects()
{
	if(m_curent )
		m_curent->RenderObjects();

	
	
	
	
	if(m_mouse && (!m_Start))
	{
	m_Renderer->GetDevice()->DepthWrite(mmfalse);
	m_cursor->Render();
	m_Renderer->GetDevice()->DepthWrite(mmtrue);
	}


	m_Renderer->SetCamera(&m_CameraZ);
	if(m_Zast->m_bRenderable)
		m_pEngine->RenderScene(m_Zast);

	return mmOk;
}

















//------------------------------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------------------------------
/*
mmRes clsFireApp::IdleLoop()
{
	if(m_waitKey == mmtrue)
	{
		//m_pEngine->Update();
		m_pMouse->Snapshot();
		if((m_pMouse->IsButtonReleased(enMouseButton0) == mmtrue))
		{
			m_s_key->Play((mmDword)this);
			m_waitKey = mmfalse;
			m_pMouse->Snapshot();
			m_Timer->Update();
		}
	}else{

	m_sizeF = 0;
	m_pEngine->Update();
	UpdateObjects();

	}


	RenderObjects();

	return mmOk;
}
*/
//------------------------------------------------------------------------------------------------------------

/*
mmRes clsFireApp::UpdateObjects()
{
	
	if(zast_state < 2)
	{
		m_Timer->SetDeltaFactor(1.0f);
		m_Zast->AnimationRangeBuild(m_Timer->GetDeltaMiSecs());
		m_Zast->Animate();
		//if((m_pMouse->SnapshotButton(enMouseButton0) == enMouseButtonDown)&&(zast_state < 1))
		if((m_pMouse->IsButtonReleased(enMouseButton0) == mmtrue)&&(zast_state < 1))
		{
			m_s_key->Play((mmDword)this);
			zast_state = 1;
			SetTextureZast("inter/fire_starter.jpg");
			m_Zast->PlayAnimationRangeStart("zast_close", enAnimHold);
		}
		m_Timer->SetDeltaFactor(m_df);
		return mmOk;
	}

	if((zast_state == 2)||(zast_state == 5)||(zast_state == 6)  )
	{
		m_Timer->SetDeltaFactor(1.0f);
		m_Zast->AnimationRangeBuild(m_Timer->GetDeltaMiSecs());
		m_Zast->Animate();
		m_Timer->SetDeltaFactor(m_df);
	}
	

	SetLight();

		m_Text->SetText(
		
		"\brFire man: \bw" + clsString().FromFormatNum(m_lives,"") + 
		"\by ... \brMan in fire: \bw"+clsString().FromFormatNum(m_count,"") +
		"\by ... \brFire factor: \bw"+clsString().FromFormatNum(m_sizeF,"05.2")	);


	//m_Text->SetText(str);

	m_Text1->SetText("\brScores: \bw" + clsString().FromFormatNum(m_scores,"")
			+"-"+clsString().FromFormatNum((mmInt)m_Timer->GetFPS(),"")	
		);

	m_Scene->AnimationRangeBuild(m_Timer->GetDeltaMiSecs());
	m_Scene->Animate();
	


	if(LevelState == 0)
	{
		return mmOk;
	}
	if(LevelState == 1)
	{
		//if(m_pMouse->SnapshotButton(enMouseButton0) == enMouseButtonUp)
		//{
			LevelState = 2;
			m_pMan->SetAnimationPos("start_2");
			m_pMan->PlayAnimationRange("start_run_2", enAnimHold);
			m_pDoska->SetAnimationPos("start_2");
			m_pDoska->PlayAnimationRange("start_run_2", enAnimHold);
		//}
		return mmOk;
	}
	if(LevelState == 2)
	{
		return mmOk;
	}

	if(LevelState == 4)
	{
		m_pAAng->m_mxLocal._43 += m_Timer->GetDeltaMiSecs()*0.07f;

		if((m_pAAng->m_mxLocal._43 >= 200.0f)&&(zast_state != 5))
		{
			if(m_lives == 0)
			{
				SetTextureZast("inter/game_over.jpg");
			}else{
				SetTextureZast("inter/loser.jpg");
			}
			m_Zast->PlayAnimationRangeStart("zast_close", enAnimHold);
			zast_state = 5;
		}

		if(m_pAAng->m_mxLocal._43 >= 350.0f)
		{
			if(m_SceneTMP != mmnull)
				m_SceneTMP->m_bKillMe = mmtrue;
			m_pAAng->m_mxLocal._43 = 0;
			m_pDoska->Move(0);

			if(m_lives == 0)
			{
				InitStart(mmtrue);
				return mmOk;
			}

			InitScene();
			
			m_Zast->PlayAnimationRangeStart("zast_open", enAnimHold);
			return mmOk;
		}
		m_pDoska->Move((mmFloat)m_pMouse->SnapshotAxisDelta(enMouseX));
		return mmOk;
	}

	if( ((m_sizeF == 0)||(m_count == 0))&&(m_win == 0) )
	{
		m_win = 1;
	}

	if(m_win == 1)
	{
		if(m_pMan->m_mxLocal._43 <= 0.0f)
		{
			//m_df = max(0.0f,m_df - 0.2f);
			m_win = 2;
		}
	}
	if((m_win == 2)||(m_win == 3))
	{
		m_df = max(0.0f,m_df - m_Timer->GetDeltaMiSecs()*0.0005f);
		m_Timer->SetDeltaFactor(m_df);

			if((m_df < 0.1f)&&(m_win == 2))
			{
				SetTextureZast("inter/level_complete.jpg");
				m_Zast->PlayAnimationRangeStart("zast_close", enAnimHold);
				m_win = 3;
				zast_state = 6;
			}
	}

	m_pDoska->Move((mmFloat)m_pMouse->SnapshotAxisDelta(enMouseX));

	m_pMan->Move(m_Timer->GetDeltaMiSecs()* 0.12f);


	SetCam();


	
	return mmOk;
}
*/
//----------------------------
mmVoid clsFireApp::SetCam()
{
	clsVector3 at,eye;//,v;
	mmFloat z;

	m_pMan->m_mxFull._43 = max(m_pMan->m_mxFull._43 , 0.0f);

	//v = (m_pMan->m_mxFull.GetTranslation() + m_pDoska->m_mxFull.GetTranslation()) * 0.5f;
	z = m_pMan->m_mxFull._43 / (908.0f) * 10000.0f;

	if((m_pMan->m_state != enManPrigaet)&&(m_pMan->m_state != enManDed)&&(m_pMan->m_state != enAngel)&&(m_pMan->m_state != enManZakonchil))
		z = 0;

	z = max(z,0.0f);

	at = m_pMan->m_mxFull.GetTranslation();
	at.z = max(at.z,120.927f);
	m_at->PlayAnimationRangeMiSecStart(z,z,enAnimHold);
	at.z = m_at->m_mxFull._43;


	//eye.x = v.x;
	eye.y = m_Camera.Eye().y;
	m_eye->PlayAnimationRangeMiSecStart(z,z,enAnimHold);
	eye.z = m_eye->m_mxFull._43;

	z = mmAbs(m_pMan->m_mxFull._41) / (1000.0f) * 10000.0f;

	if((m_pMan->m_state != enManPrigaet)&&(m_pMan->m_state != enManDed)&&(m_pMan->m_state != enAngel)&&(m_pMan->m_state != enManZakonchil))
		z = 0;

	z = max(z,0.0f);

	m_eye_x->PlayAnimationRangeMiSecStart(z,z,enAnimHold);
	m_at_x->PlayAnimationRangeMiSecStart(z,z,enAnimHold);

	at.x = m_at_x->m_mxFull._41;
	eye.x = m_eye_x->m_mxFull._41;

	if(m_pMan->m_mxFull._41 < 0.0f)
	{
		at.x = -at.x;
		eye.x = -eye.x;
	}



	m_Camera.At(at);
	m_Camera.Eye(eye);

	m_Camera.Up(0.0f, 0.0f, 1.0f);



	return;


	//mmFloat c_x = (sRect.right - sRect.left)*0.5f + sRect.left;

//	mmFloat cX = (m_pMan->m_mxLocal._42 * 0.5f ) ;
//	mmFloat cX =  m_pMan->m_mxLocal._42 * ( (sRect.Width()-360.0f) / sRect.Width() );
//	mmFloat cY = (m_pMan->m_mxLocal._43 * 0.5f + 120.927f) ;

//	m_Camera.Eye(cX,  m_Camera.Eye().y , cY);
//	m_Camera.At(cX,  m_Camera.At().y , cY);
/*
	mmFloat cX =  -m_pMan->m_mxLocal._42 * ( (sRect.Width()-180.0f) / sRect.Width() );
	mmFloat cY = (m_pMan->m_mxLocal._43 * 0.4f + 120.927f);
	m_Camera.At(cX,  m_Camera.At().y , cY);

	cX =  -m_pMan->m_mxLocal._42 * ( (sRect.Width()-360.0f) / sRect.Width() );
	cY = (m_pMan->m_mxLocal._43 * 1.35f + 120.927f);
	  m_Camera.Eye(cX ,  m_Camera.Eye().y , cY);

	m_Camera.Up(0.0f, 0.0f, 1.0f);
*/
/*
	clsPlane p(0,-1,0,0);
	clsVector3 v1,v2,v,ve;
//	mmFloat a;

	ve.x =  m_pMan->m_mxFull._41 * ( (sRect.Width()-360.0f) / sRect.Width() );
	ve.y = m_Camera.Eye().y;

	//ve.z = max(m_pMan->m_mxFull._43 * 1.0f , 120.927f);

	 

//	a = (m_pMan->m_mxFull.GetTranslation() - ve).Normalize().Dot((m_pDoska->m_mxFull.GetTranslation() - ve).Normalize());
/*
	while(a < 0.89f)
	{
		ve.z++;
		a = (m_pMan->m_mxFull.GetTranslation() - ve).Normalize().Dot((m_pDoska->m_mxFull.GetTranslation() - ve).Normalize());
	}
	*/
/*
	mmFloat a,b,c,d;

	ve.z = 0;

	a = 1;
	b = -m_pMan->m_mxFull._43;
	c = (ve.y ) * (ve.y ) - 0.89f;
	d = b*b - 4 * a * c;
	if(d >= 0)
	{

		ve.z = (-b + sqrtf(d)) / (2 * a);

	}

	ve.z = max(ve.z , 120.927f);

	*/


	//ve.z = (m_pMan->m_mxFull._43 - 2 * (ve.y - m_pDoska->m_mxFull._42) - 0.89f) * 0.5f;

/*	mmFloat ax,ay,by,n, k ;
	ax = ve.y;
	by = m_pMan->m_mxFull._43;
	n = 0.89f;

	k = sqrtf(n*n*n*n*by*by-2.0f*n*n*by*by+by*by-4.0f*ax*ax*n*n*n*n+8.0f*ax*ax*n*n+4.0f*n*n*ax*
	sqrtf(-n*n*n*n*by*by+n*n*by*by)
	-4.0f*ax*ax-4.0f*ax*
	sqrt(-n*n*n*n*by*by+n*n*by*by)
	);

	if(k <= 0 )
	{
		ve.z = max(m_pMan->m_mxFull._43  ,120.927f);

	//	ve.z =  m_pMan->m_mxFull._43 / 247.0f * 156 + 120;
	}else{

		ay = (1.0f/(2.0f*(n*n-1)))*
			(n*n*by-by-
			k
			);

		ve.z = max(ay , 120.927f);
	}

	m_CamV = (ve - m_Camera.Eye()) * 0.5f ;

//	if( m_CamV.LengthSq() > (ve - m_Camera.Eye()).LengthSq() )
//	{
	//m_Camera.Eye(ve.x,  ve.y , ve.z);
//	}else{
		//m_Camera.Eye(m_Camera.Eye() + m_CamV);
		ve = m_Camera.Eye() + m_CamV;
//	}


	/*if(m_pMan->m_mxFull._43 <= 247.0f)
	{
		ve.z =  m_pMan->m_mxFull._43 / 247.0f * 156 + 120;
	}*/

//	ve = m_Camera.Eye() + m_CamV;


/*		ve.x = (m_pMan->m_mxFull._41 + m_pDoska->m_mxFull._41) * 0.5f;

		m_Camera.Eye(ve.x,  ve.y , ve.z);


//	m_CamV.z = max(m_CamV.z,m_pMan->m_mxFull._43 );
	
	/*
	v1 = m_pMan->m_mxFull.GetTranslation() - m_Camera.Eye();
	v2 = m_pDoska->m_mxFull.GetTranslation() - m_Camera.Eye();
	v1.Normalize();
	v1 *= v1.Dot(v2);
	v1.Lerp(v2,0.5f);
	p.IntersectRay(v,m_Camera.Eye(),v1);
	v.z = max(v.z,120.927f);
	//v1 += m_Camera.Eye();
	*/

/*	v1 = m_pMan->m_mxFull.GetTranslation() - m_Camera.Eye();
	v2 = m_pDoska->m_mxFull.GetTranslation() - m_Camera.Eye();
	v2.Normalize();
	v2 *= v1.Length();
	v1.Lerp(v2,0.5f);
	p.IntersectRay(v,m_Camera.Eye(),v1);
	v.z = max(v.z,120.927f);

	v.x = (m_pMan->m_mxFull._41 + m_pDoska->m_mxFull._41) * 0.5f;

	m_Camera.At(v);
	v.y = -600;
	m_Camera.Eye(v);

	m_Camera.Up(0.0f, 0.0f, 1.0f);*/
}
//------------------------------------------------------------------------------------------------------------
/*
mmRes clsFireApp::RenderObjects()
{//m_Renderer->Clear(mmtrue);
	if((zast_state == 0)||(zast_state == 1))
	{

		m_Renderer->SetCamera(&m_CameraZ);
		m_Renderer->StartRender();
		m_Renderer->Clear();

		m_pEngine->RenderScene(m_Zast);

		m_Renderer->StopRender();
		m_Renderer->Present();
		return mmOk;
	}

	m_Renderer->SetCamera(&m_Camera);
	m_Renderer->StartRender();
	m_Renderer->Clear();

	m_pEngine->RenderScene(m_Scene);

	m_Text->Render();
	m_Text1->Render();

	
	


	if((zast_state == 2)||(zast_state == 5)||(zast_state == 6) )
	{

		m_Renderer->SetCamera(&m_CameraZ);
		//m_Renderer->StartRender();
		m_Renderer->Clear();

		m_ZastPlan->EnableRendering(mmfalse);
		m_pEngine->RenderScene(m_Zast);
		m_ZastPlan->EnableRendering(mmtrue);

		m_Renderer->StopRender();
		m_Renderer->Present();
		return mmOk;
	}


	m_Renderer->StopRender();
	m_Renderer->Present();

	return mmOk;
}
*/
//------------------------------------------------------------------------------------------------------------
mmVoid clsFireApp::LoadScene(mmInt level)
{
	level--;
	level = level % (mmInt)m_conf.FindVar("level_index")->GetCount();

	level = *(*m_conf.FindVar("level_index"))[level];


	clsString level_name;
	level_name.FromFormatNum(level,"02");
	level_name = clsString("fire_")+level_name;

	m_win = 0;

	m_count = 0;
	m_sizeF = 0;

//	m_lives = 5;

	ClearScenes();

	m_Scene = (clsFireEntity*)m_pEngine->CreateScene(level_name);
	if(	m_Scene->LoadMJ(clsString("Meshes/")+level_name+".mj") == mmErr)
	{
		m_level = 1;
		LoadScene(m_level);
		return;

	}
/*
	for(int zzz=0;zzz<100;zzz++)
	{

		ClearScenes();

	m_Scene = (clsFireEntity*)m_pEngine->CreateScene(level_name);
	m_Scene->LoadMJ(clsString("Meshes/")+level_name+".mj");
	}
*/



	m_pEngine->SetActiveScene(m_Scene);
//	m_Scene->EnableBoundSphereTest(mmfalse,mmfalse);

	m_Renderer->SetCamera(&m_Camera);

	SetLight();

	m_Scene->AnimationFrame(0,0);
	m_Scene->AnimationRangeStop(mmtrue);

	m_Timer->Initialization();

	InitScene();

	m_Timer->Initialization();
}
//------------------------------------------------------------------------------------------------------------
mmVoid clsFireApp::SetLight()
{
	m_Light->SetType(enLightPoint);
	m_Light->SetRange(100000.0f);
	m_Light->SetAttenuation(enAttenuation1, 0.2f);
//	m_Light->SetAttenuation(enAttenuation1, 1.2f);
	m_Light->SetDiffuse(clsColorRGBA(1.0f, 1.0f, 1.0f, 1.0f));
	m_Light->SetDiffuse(clsColorRGBA(1.0f, 1.0f, 1.0f, 1.0f));
	m_Light->SetSpecular(clsColorRGBA(1.0f, 1.0f, 1.0f, 1.0f));
	m_Light->SetTheta(1.0f);
	m_Light->SetPhi(2.2f);
	m_Light->SetFalloff(1.0f);
	m_Light->SetPosition(m_Camera.Eye());
	m_Light->Activate();
}
//------------------------------------------------------------------------------------------------------------
mmVoid clsFireApp::InitScene()
{
	InitScene(m_Scene);


	m_CamV = 0.0f;

//	m_SceneTMP = mmnull;

//	LevelState = 0;

	m_eye = (clsEntity*)m_Scene->FindChild("eye");
	m_at = (clsEntity*)m_Scene->FindChild("at");

	m_eye_x = (clsEntity*)m_Scene->FindChild("eye_x");
	m_at_x = (clsEntity*)m_Scene->FindChild("at_x");

	m_pDoska = (clsFireDoska*)m_Scene->FindChild("Dummy_doska");
	m_pMan = (clsFireMan*)m_Scene->FindChild("Dummy_man");
	m_pBatut = (clsFireEntity*)m_Scene->FindChild("Bone_Box52");
	m_pAAng = (clsFireEntity*)m_Scene->FindChild("aangel");

	m_pMan->m_auragn = m_pMan->FindChild("auraagn");
	m_pMan->m_auragn->EnableRendering(mmfalse);

	m_priz1 = (clsFirePriz*)m_Scene->FindChild("priz_01");
	m_priz1->LoadAnimationKeys("anim_01.mhmAnim");
	

	m_FireDoska = (clsFireParticle*)m_Scene->FindChild("particles_flame2.doska");
	m_FireDoska->m_scale = 0;
	m_FireDoska->m_size = 0;
	m_FireDoska->m_bNeedUpdateEvent = mmtrue;
	m_FireDoska->OnInitialize(mmfalse);

	m_FireTush = (clsFireParticle*)m_Scene->FindChild("particles_tush.00");
	m_FireTush->m_scale = 0;
	m_FireTush->m_size = 0;
	m_FireTush->m_bNeedUpdateEvent = mmtrue;
	m_FireTush->OnInitialize(mmfalse);

	m_pMan_01 = (clsFireJertva*)m_Scene->FindChild("man_01");
	m_pMan_01->EnableRendering(mmfalse,mmtrue);

	m_pDoska->LoadAnimationKeys("anim_01.mhmAnim");
	m_pMan->LoadAnimationKeys("anim_01.mhmAnim");
	m_pBatut->LoadAnimationKeys("anim_01.mhmAnim");
	m_pAAng->LoadAnimationKeys("anim_01.mhmAnim");
	m_pMan_01->LoadAnimationKeys("anim_01.mhmAnim");

	m_pDoska->AnimationFrame(0,0);
	m_pDoska->AnimationRangeStop(mmtrue);
	m_pMan->AnimationFrame(0,0);
	m_pMan->AnimationRangeStop(mmtrue);



//	m_pDoska->PlayAnimationRange("start_run", enAnimHold);

	m_pMan->m_mxLocal =  m_pBatut->m_mxLocal =  m_pAAng->m_mxLocal =  m_pDoska->m_mxLocal.Identity();


	
	m_pDoska->OnInitialize(mmfalse);
	m_pDoska->FindChild("pd1")->m_mxLocal._41 = 0;
	m_pDoska->FindChild("pd2")->m_mxLocal._41 = 0;

	((clsBaseMaterial*)((clsBaseMesh*)m_pDoska->FindChild("batut")->Res)->GetMaterial(0))->m_map_diffuse.m_texture
				=
			(clsBaseTexture*)m_ResMan->GetResource("wall/gbrk_sch4_2.jpg", MHMRES_TEXTURE_ID);
	m_pDoska->m_dyrka = mmfalse;

	//m_pDoska->l = -(m_pDoska->w*0.5f)+m_pDoska->m_mxLocal._41;
	//m_pDoska->r = (m_pDoska->w*0.5f)+m_pDoska->m_mxLocal._41;

	m_pMan->Init();

	/*
	sRect.right = 180;
	sRect.left = -sRect.right;
	*/
	

	m_Camera.At(0.0f , 0.0f, 120.927f);
	m_Camera.Eye(0.0f , -467.0f, 120.927f);
	m_Camera.Up(0.0f , 0.0f, 1.f);


	clsString nam;
	for(mmInt z = 1;z<(N_PRIZ+1);z++)
	{
		nam = "priz/pr_";
		nam += clsString().FromFormatNum(z,"02")+".jpg";
		m_ResMan->GetResource(nam, MHMRES_TEXTURE_ID);
	}

	m_Timer->Update();

	m_RandPriz.SetCount(N_PRIZ);
	
	m_RandPriz.SetItem(3,3,100);//
	m_RandPriz.SetItem(0,4,100);//

	m_RandPriz.SetItem(1,8,80);//
	m_RandPriz.SetItem(2,9,80);//

	m_RandPriz.SetItem(8,14,50);//
	m_RandPriz.SetItem(9,15,50);//

	m_RandPriz.SetItem(5,14,40);//
	m_RandPriz.SetItem(6,15,40);//
	
	m_RandPriz.SetItem(4,20,30);
	m_RandPriz.SetItem(7,17,25);

	


	//SetCam();
/* //last
	m_RandPriz.SetCount(8);
	
	m_RandPriz.SetItem(3,3,1);//
	m_RandPriz.SetItem(0,4,2);//

	m_RandPriz.SetItem(1,8,3);//
	m_RandPriz.SetItem(2,9,4);//

	m_RandPriz.SetItem(5,14,5);//
	m_RandPriz.SetItem(6,15,6);//
	
	m_RandPriz.SetItem(4,20,7);
	m_RandPriz.SetItem(7,17,8);
	*/
	

/*
	m_RandPriz.SetItem(0,2,5000);
	m_RandPriz.SetItem(3,3,0);

	m_RandPriz.SetItem(1,5,5000);
	m_RandPriz.SetItem(2,4,0);

	m_RandPriz.SetItem(5,5,0);
	m_RandPriz.SetItem(6,7,0);
	
	m_RandPriz.SetItem(7,10,0);

	m_RandPriz.SetItem(4,11,0);
*/
/*	
	tmpRefArr <mmInt> aaa;

	aaa.Size(9);
	for(mmInt z = 0; z<9;z++)
		aaa[z] = 0;

	

	mmInt r;
	for(mmInt z = 0; z<40;z++)
	{
		m_RandPriz.Update(mmnull,mmtrue,5000);
		r = m_RandPriz.GetMax();
		//g_plog->Out(clsString(r), enLCRed);
		aaa[r+1] ++;

	}
	for(mmInt z = 0; z<9;z++)
		g_plog->Out(clsString(aaa[z]), enLCRed);
	r=r;

*/




	

}
//------------------------------------------------------------------------------------------------------------
mmRes clsFireApp::WindowSize()
{
	if(m_Renderer != mmnull)
	{
		clsViewport* pViewPort = m_Renderer->GetDevice()->GetViewport();
		m_Camera.Aspect(pViewPort->GetAspect());
/*

		m_Text->SetRect(clsRect(0,0,(mmFloat)pViewPort->m_nWidth,(mmFloat)pViewPort->m_nHeight));
		m_Text->SetSize((mmFloat)pViewPort->m_nWidth * 0.32f);

		m_TextState->SetRect(clsRect(0,0,(mmFloat)pViewPort->m_nWidth,(mmFloat)pViewPort->m_nHeight));
		m_TextState->SetSize((mmFloat)pViewPort->m_nWidth * 0.32f);


		m_Text1->SetSize((mmFloat)pViewPort->m_nWidth * 0.32f);
		m_Text1->SetX( (mmFloat)pViewPort->m_nWidth * 0.02f );
		m_Text1->SetY( (mmFloat)pViewPort->m_nWidth * 0.01f );

		*/
	}
	return mmOk;
}

mmVoid clsFireApp::PrizOk(mmInt tip)
{
	if(tip == 0)
	{
		return;
	}

	if(tip == 2)
	{
		m_pDoska->n_w += 50.0f;
		m_pDoska->n_w = min(m_pDoska->n_w, 250);
	}
	if(tip == 3)
	{
		m_pDoska->n_w -= 50.0f;
		m_pDoska->n_w = max(m_pDoska->n_w, 70);
	}
	if(tip == 4)
	{
		m_FireDoska->m_size = 0.0f;
	}
	if(tip == 5)
	{
		m_lives ++;
		m_game->m_liv.Text->UpdateEffect(0,mmtrue);
	}
	if(tip == 6)
	{
		SetFireFactor(m_Scene,1);
	}
	if(tip == 7)
	{
		SetFireFactor(m_Scene,0);
	}
	if(tip == 8)
	{
		m_pMan->m_priz = 1;
		m_FireDoska->m_size = 0.0f;
	}

	if(tip == 9)
	{
		m_pMan->m_man_spid = M_SPID_2;
	}

	if(tip == 10)
	{
		m_pMan->m_man_spid = M_SPID_0;
	}

}
//------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
clsEntityFactory* clsFireApp::CreateEntityFactory(clsEngine* pengine)
{
	return new clsFireEntityFactory(pengine);
}
//******************************************************************************
//==========================================================================================
clsFireEntityFactory::clsFireEntityFactory(clsEngine *pEngine):
						 clsEntityFactory(pEngine)
{
}

// Entity factory destructor
//==========================================================================================
clsFireEntityFactory::~clsFireEntityFactory()
{
}

// CreateEntity: creates new entity objects
//==========================================================================================
clsEntity* clsFireEntityFactory::CreateEntity(const clsString &type,
												 const clsString &name)
{
	clsString entname(name);
	//entname.UpperCase();
	clsEntity* pEntity = mmnull;


	if((entname.Left(5) == "priz_") && (entname[entname.Length()-1] != '_'))
		pEntity = new clsFirePriz(this, type, name);
	else
	if(entname.Left(12) == "particles_d_")
		pEntity = new clsFireFireDouw(this, type, name);
	else
	if(entname.Left(12) == "particles_f_")
		//pEntity = new clsFireFire(this, type, name);
		pEntity = new clsFireFire(this, "fire" , name);
	else
		if(entname.Left(10) == "particles_")
			pEntity = new clsFireParticle(this, type, name);
		else
			if(entname.Left(6) == "jertva")
				pEntity = new clsFireJertvaImiter(this, type, name);
			else
				if(entname.Left(4) == "man_")
		pEntity = new clsFireJertva(this, type, name);
	else
	if(entname == "Dummy_man")
		pEntity = new clsFireMan(this, type, name);
	else
	if(entname == "Dummy_doska")
		pEntity = new clsFireDoska(this, type, name);
	else
	if(entname == "varona")
		pEntity = new clsFireVarona(this, type, name);
	else
	if(type == "Mesh")
		pEntity = new clsFireEntity(this, type, name);
	else
	if(type == "Helper")
		pEntity = new clsFireEntity(this, type, name);
	else
	if (pEntity == mmnull)
		pEntity = new clsFireEntity(this, type, name);
	
	return pEntity;
}
//****************************************************************************
clsFireEntity::clsFireEntity(clsEntityFactory *pFactory, 
					 const clsString &type,
					 const clsString &name):
		   clsEntity(pFactory, type, name)
{
	m_scene_state = enNoneGam;
	m_state = enNoneGam;
	m_next_state = enNoneGam;

	m_sel_tex = mmnull;

	m_curent = mmnull;

	app = (clsFireApp*)(g_pEnvironment->m_pMyApp);

	m_anim = "";
	m_anim_r = "";
	m_anim_loop = enNull;

	m_alpha = -1.0f;

	//m_scene_name = "";

}
mmRes clsFireEntity::OnRender()
{
	if(m_alpha != -1)
	{

	D3DCOLORVALUE c;
		c.a = m_alpha;
		c.r = 1.0f;
		c.g = 1.0f;
		c.b = 1.0f;

		((clsBaseMaterial*)((clsBaseMesh*)this->Res)->GetMaterial(0))->m_dxmtl.Diffuse = c;

	//	((clsBaseMaterial*)((clsBaseMesh*)this->Res)->GetMaterial(0))->m_dxmtl.Ambient = c;

	//	((clsBaseMaterial*)((clsBaseMesh*)this->Res)->GetMaterial(0))->m_dxmtl.Emissive = c;
		
	//	((clsBaseMaterial*)((clsBaseMesh*)this->Res)->GetMaterial(0))->m_dxmtl.Power = m_alpha;

	//	((clsBaseMaterial*)((clsBaseMesh*)this->Res)->GetMaterial(0))->m_dxmtl.Specular = c;


		if(c.a == 1.0f)
		{
			((clsBaseMaterial*)((clsBaseMesh*)this->Res)->GetMaterial(0))->SetBlendType(enBlendNone);
			((clsBaseMesh*)this->Res)->SetBlendType(enBlendNone);
		//	((clsBaseMesh*)this->Res)->SetBlendType(enBlendAlpha);//????????????????????????????????
		}
		else
		{
			((clsBaseMaterial*)((clsBaseMesh*)this->Res)->GetMaterial(0))->SetBlendType(enBlendAlpha);
			((clsBaseMesh*)this->Res)->SetBlendType(enBlendAlpha);
		//	((clsBaseMesh*)this->Res)->SetBlendType(enBlendNone);
		}


		//((clsBaseMaterial*)((clsBaseMesh*)this->Res)->GetMaterial(0))->SetBlendType(enBlendAlpha);

		//((clsBaseMesh*)this->Res)->SetBlendType(enBlendAlpha);
	}

		/*

	if(state == 14)
	{
		size = max(0.0f, size - 0.01f);
		c.a = size;
		((clsBaseMaterial*)((clsBaseMesh*)FindChild("Box38")->Res)->GetMaterial(0))->SetBlendType(enBlendAlpha);		
	}else{
		((clsBaseMaterial*)((clsBaseMesh*)FindChild("Box38")->Res)->GetMaterial(0))->SetBlendType(enBlendNone);
	}
	((clsBaseMaterial*)((clsBaseMesh*)FindChild("Box38")->Res)->GetMaterial(0))->m_dxmtl.Diffuse = c;
	*/

	/*}else{
		((clsBaseMaterial*)((clsBaseMesh*)this->Res)->GetMaterial(0))->SetBlendType(enBlendNone);
	}*/



	if(m_sel_tex == mmnull)
		return mmOk;

	((clsBaseMaterial*)((clsBaseMesh*)this->Res)->GetMaterial(0))->m_map_diffuse.m_texture = m_sel_tex;

	return mmOk;
}
mmVoid clsFireEntity::OnInit()
{
	if(m_anim != "")
		LoadAnimationKeys(m_anim);
	if(m_anim_loop != enNull)
		PlayAnimationRangeStart(m_anim_r, m_anim_loop);	
}
//==========================================================================================
mmVoid clsFireEntity::OnLoadPropsPost(clsVarList * vars)
{
	clsVariable* var;
	var = vars->FindVariable("scene_right");
	if (var != mmnull)
	{	
		app->sRect.right = *var;
	}
	var = vars->FindVariable("scene_left");
	if (var != mmnull)
	{	
		app->sRect.left = *var;
	}

	var = vars->FindVariable("scene_height");
	if (var != mmnull)
	{	
		app->sRect.top = *var;
		app->sRect.bottom = 0.0f;
	}


	var = vars->FindVariable("anim");
	if (var != mmnull)
	{	
		//LoadAnimationKeys((mmString)*var);
		m_anim = ((mmString)*var);
	}
	var = vars->FindVariable("play");
	if (var != mmnull)
	{
		clsString str,loop;
	//	str = (mmString)(*var);
		m_anim_r = (mmString)(*var);
		var = vars->FindVariable("loop");
		if (var != mmnull)
		{
			loop = (mmString)*var;
			if(loop == "loop")
				m_anim_loop = enAnimLoop;
				//PlayAnimationRangeStart(str, enAnimLoop);
			else if(loop == "hold")
			m_anim_loop = enAnimHold;
				//PlayAnimationRangeStart(str, enAnimHold);
			else if(loop == "pingpong")
			m_anim_loop = enAnimPingPong;
				//PlayAnimationRangeStart(str, enAnimPingPong);
			else
			m_anim_loop = enAnimLoop;
			//	PlayAnimationRangeStart(str, enAnimLoop);
		}else{
			m_anim_loop = enAnimLoop;
			//PlayAnimationRangeStart(str, enAnimLoop);
		}
	}


	var = vars->FindVariable("poz_x");
	if (var != mmnull)
	{	
		m_mxWorld._41 = *var;
	}
	var = vars->FindVariable("poz_y");
	if (var != mmnull)
	{	
		m_mxWorld._42 = *var;
	}
	var = vars->FindVariable("poz_z");
	if (var != mmnull)
	{	
		m_mxWorld._43 = *var;
	}

	var = vars->FindVariable("load_scene");
	if (var != mmnull)
	{	
//		m_scene_name = clsString("Meshes/") +(mmString)(*var);
//		clsEntity * scen;
//		scen = app->m_pEngine->CreateScene("fire_01");
		this->LoadMJ(clsString("Meshes/") +(mmString)(*var));
	//	scen->LoadMJ(clsString("Meshes/") +(mmString)(*var));
	//	scen->SetOwner(this);
	}
	
}
//==========================================================================================
clsFireEntity::~clsFireEntity()
{
	
}
//---------------------------------
mmVoid clsFireEntity::OnAnimationPlay()
{
	if(this == app->m_Zast)
		if(app->m_Zast->m_AnimationRageController)
			if(!app->m_Zast->m_AnimationRageController->IsPlaying)
				app->PlayFX(app->m_s_dver);
}
mmVoid clsFireEntity::OnAnimationKey(mmInt key)
{

	app->m_Start = mmfalse;

	if((key == 24)&&(this == app->m_Zast))
	{
		app->m_Pause = mmfalse;
		if( app->m_Zast->m_bRenderable == mmtrue)
			 app->m_Zast->m_bRenderable = mmfalse;
	}

	if(this->m_curent != mmnull)
	{
		app->m_curent = this->m_curent;	
		app->m_curent->Init();
	}
	this->m_curent = mmnull;

	if(this->m_scene_state !=	enNoneGam)
	{
		app->m_curent->state = this->m_scene_state;
	}
	this->m_scene_state = enNoneGam;

	if(this->m_next_state !=	enNoneGam)
	{
		this->m_state = this->m_next_state;
	}
	this->m_next_state = enNoneGam;	

return;

/*
	if((key == 25)&&(app->m_win == 3))
	{
			if(app->m_SceneTMP != mmnull)
				app->m_SceneTMP->m_bKillMe = mmtrue;
			app->m_pDoska->Move(0);
			app->InitStart(mmtrue);
			return;
	}


	if((key == 25)&&(app->zast_state == 1))
	{
		app->LoadScene(1);
		app->zast_state = 2;
		app->m_Zast->PlayAnimationRangeStart("zast_open", enAnimHold);
	}
	if((key == 24)&&((app->zast_state == 2)||(app->zast_state == 5)))
	{
		app->zast_state = 3;
	}

	

	if(key == 27)
	{
		this->PlayAnimationRange("pr_2");
		state = 1;
	}

	if(key == 1)
	{
		app->LevelState =1;
	}
	if(key == 3)
	{
		app->LevelState =3;
		app->m_pDoska->SetAnimationPos("run_man_end");
		app->m_pMan->SetAnimationPos("a_r");
	}

	if(key == 13)
	{
		((clsFireEntity*)this->Owner)->state = 2;
	}

	if(key == 16)
	{
		((clsFireEntity*)this->Owner)->state = 5;
	}

	if(key == 18)
	{
		((clsFireJertva*)this)->state = 4;
	}

	if(key == 30)
	{
//		((clsFireEntity*)this->Owner)->state = 7;
	}

	if(key == 32)
	{
		((clsFireJertva*)this)->state = 14;
		((clsFireJertva*)this)->PlayAnimationRangeStart("j_polz_c", enAnimLoop);
	}

	*/
			
}
//**************************************************************************
clsFireMan::clsFireMan(clsEntityFactory *pFactory, 
					 const clsString &type,
					 const clsString &name):
		   clsFireEntity(pFactory, type, name)
{
	Init();
}
//-----------------------
mmVoid clsFireMan::Init()
{
	m_man_spid = M_SPID_0;


	pos = 0;
	_pos = 0;
	v = 0;
	v.y = 0.0f;
	vUp = 3.0f;
	v.z = vUp;
	vDo = 0.02f;//0.02f;
	t = 0.0f;

	m_scene_state = enNoneGam;
	m_next_state = enNoneGam;
	m_state = enNoneGam;

	jertva = mmnull;

	m_speed = 0.007f;
	m_speed_f = 1.0f;

	m_priz = 0;
}


mmVoid clsFireApp::SceneClouse()
{
	/*
	m_count
	m_sizeF
	m_lives
	*/

	/*
			if(app->m_lives == 0)
			{
				app->SetTextureZast("inter/game_over.jpg");
			}else{
				app->SetTextureZast("inter/loser.jpg");
			}
			app->m_Zast->m_curent = app->m_game;
			app->m_Zast->PlayAnimationRange("zast_close", enAnimHold);
			*/

	if(m_lives == 0)
	{
		m_Zast->m_bRenderable = mmtrue;
		m_Zast->m_curent = m_end_game;
		SetTextureZast("inter/game_over.mtj");
		m_Zast->PlayAnimationRange("zast_close", enAnimHold);
		//ZClose(m_end_game);
		return;
	}else{

		if(m_win == mmtrue)
		{
			SetTextureZast("inter/level_complete.mtj");
			m_Zast->m_bRenderable = mmtrue;
			m_Zast->m_curent = m_next_level;
			m_Zast->PlayAnimationRange("zast_close", enAnimHold);
			//ZClose(m_next_level);
		}else{
			SetTextureZast("inter/loser.mtj");
			m_Zast->m_bRenderable = mmtrue;
			m_Zast->m_curent = m_game;
			m_Zast->PlayAnimationRange("zast_close", enAnimHold);
			//ZClose(m_game);
		}

	}

}
//==========================================================================================
clsFireMan::~clsFireMan()
{

}
//---------------------------------
mmVoid clsFireMan::Move(mmFloat k)
{
	if(m_man_spid < M_SPID_1)
		m_man_spid = min(m_man_spid + k * 0.000025f , M_SPID_1);

	k *= m_man_spid;



	if(m_priz == 1)
		m_auragn->EnableRendering(mmtrue);
	else
		m_auragn->EnableRendering(mmfalse);


	if(m_state == enManZakonchil)
	{
		if(m_mxFull._42 < -20)
		{

		}else{
			m_mxLocal._41 -= k*0.5f;
		}

		if(m_mxLocal._43 > -5)
		{

		if(m_speed_f < 1.0f)
			m_speed_f = min(1.0f, m_speed_f + k*0.01f);

		k *= 0.02f;
		v.z = (1 - (t-1)*(t-1)) * 50 + 0.00001f;
		t += k;
		m_mxLocal._43 = v.z;

		}else{
			m_mxLocal._43 = -5;
			app->SceneClouse();
		}

	}


	if(m_state == enManPrigaet)
	{

		_pos = m_mxFull.GetTranslation();
		pos.y += v.y * k;

		if(m_speed_f < 1.0f)
			m_speed_f = min(1.0f, m_speed_f + k*0.01f);

		k *= m_speed * (m_speed_f  *(220.0f/app->sRect.top));
		v.z = (1 - (t-1)*(t-1)) * app->sRect.top + 0.00001f;
		t += k;
		pos.z = v.z;

		if(pos.y >= app->sRect.right)
		{
			v.y = -v.y;
			pos.y = app->sRect.right - (pos.y - app->sRect.right);
			app->m_FireTush->m_scale = 1;
//pp			app->m_s_tush->Play((mmDword)this);

			app->PlayFX(app->m_s_tush);
		}
		if(pos.y <= app->sRect.left)
		{
			v.y = -v.y;
			pos.y = app->sRect.left + (app->sRect.left - pos.y);
			app->m_FireTush->m_scale = 1;
//pp			app->m_s_tush->Play((mmDword)this);
			app->PlayFX(app->m_s_tush);
		}

		if(pos.z < 0)
		{
			if( ( !app->m_pDoska->m_dyrka ) & (((pos.y >= app->m_pDoska->l)&&(pos.y <= app->m_pDoska->r)) ))
			{
				


				t = 0;

	//pp			app->m_s_up->Play((mmDword)this);
				app->PlayFX(app->m_s_up);

				v.z = vUp;
				pos.z = 0 ;//-pos.z;
				app->m_pBatut->SetAnimationPos("batut_1");
				app->m_pBatut->PlayAnimationRange("batut_up_do", enAnimHold);

				if(app->m_win == mmtrue)
				{
					m_state = enManZakonchil;
					return;
				}


				mmFloat kx;
				if(v.y < 0)
					kx = -1.0f;
				else
					kx = 1.0f;

				if( pos.y > app->m_pDoska->m_mxLocal._41 )
				{
					v.y = kx*(pos.y - app->m_pDoska->m_mxLocal._41)/   app->m_pDoska->w * 4.0f;	// -> x
					if(v.y < 0.0f)
						v.y = -v.y;
				}else{
					v.y = kx*(app->m_pDoska->m_mxLocal._41 - pos.y)/   app->m_pDoska->w * 4.0f ;
					if(v.y > 0.0f)
						v.y = -v.y;
				}

				if(jertva != mmnull)
				{
					jertva->m_mxWorld._43 = 33.557f;
					jertva->m_state = enJrtvaUbigaetStart;
					jertva = mmnull;
				//	m_state = enManDed;
				}

			}else{ //smert

				app->m_curent->state = enManDed;

				app->m_lives --;
				app->m_game->m_liv.Text->UpdateEffect(0,mmtrue);

	//pp			app->m_s_boom1->Play((mmDword)this);
				app->PlayFX(app->m_s_boom1);
				

				m_mxLocal._43 = 0.0f;

				app->m_pDoska->Move(0);
				m_state = enManDed;
				app->m_pMan->m_next_state = enAngel;
				app->m_pMan->SetAnimationPos("over1");
				app->m_pMan->PlayAnimationRange("over_1", enAnimHold);

				app->m_pAAng->m_mxLocal._41 = -m_mxLocal._42;
				app->m_pAAng->EnableRendering(mmtrue,mmtrue);
				app->m_pAAng->PlayAnimationRange("aang", enAnimLoop);

				if(jertva != mmnull)
				{
					app->m_count--;
					app->m_game->m_jert.Text->UpdateEffect(0,mmtrue);
					jertva->m_mxWorld._43 = 33.557f;
					jertva->m_state = enJrtvaUpalaMan;
					jertva = mmnull;
					app->m_count_dead_jertv++;
				}
				return;
			}

		}

		mmFloat klook = (1.0f - ((pos.y - app->sRect.left) / (app->sRect.right - app->sRect.left)));
		mmFloat plook = (app->m_pMan->m_AnimKeys->GetKey(9).pos - app->m_pMan->m_AnimKeys->GetKey(8).pos)*klook +  app->m_pMan->m_AnimKeys->GetKey(8).pos  ;
		app->m_pMan->PlayAnimationToMiSec(plook);

		m_mxLocal._43 = pos.z;
		m_mxLocal._42 = -pos.y;


		return;
	}

	if(m_state == enAngel)
	{
		app->m_pAAng->m_mxLocal._43 += app->m_Timer->GetDeltaMiSecs()*0.07f;
		if(app->m_pAAng->m_mxLocal._43 >= 200.0f)
		{
			app->SceneClouse();

		}
		return;
	}


return;
	/*
	if(m_state == enManVybygaet)
	{
	app->m_pMan->m_next_state = enManPrigaet;
	app->m_pMan->PlayAnimationRangeStart("start_run_2", enAnimHold);
	return;
	}*/

/*
	if(app->m_curent->state == enManDed)
	{
		app->m_pAAng->m_mxLocal._43 += app->m_Timer->GetDeltaMiSecs()*0.07f;
		if(app->m_pAAng->m_mxLocal._43 >= 200.0f)
		{
			if(app->m_lives == 0)
			{
				app->SetTextureZast("inter/game_over.jpg");
			}else{
				app->SetTextureZast("inter/loser.jpg");
			}
			app->m_Zast->m_curent = app->m_game;
			app->m_Zast->PlayAnimationRange("zast_close", enAnimHold);
		}

		*/
		/*
		if(app->m_pAAng->m_mxLocal._43 >= 350.0f)
		{
			if(app->m_SceneTMP != mmnull)
				app->m_SceneTMP->m_bKillMe = mmtrue;
			app->m_pAAng->m_mxLocal._43 = 0;
			app->m_pDoska->Move(0);

			if(app->m_lives == 0)
			{
				app->InitStart(mmtrue);
				return ;
			}

			app->InitScene();
			app->m_curent = app->m_game;
			app->m_curent->Init();
			app->m_Zast->PlayAnimationRangeStart("zast_open", enAnimHold);
			app->m_curent->state=enGame;
			return;
		}
		*/
		//return;
	//}
/*
	if(m_priz == 1)
	{
		m_auragn->EnableRendering(mmtrue);
	}else{
		m_auragn->EnableRendering(mmfalse);
	}

	_pos = m_mxFull.GetTranslation();
	pos.y += v.y * k;

	if(m_speed_f < 1.0f)
	{
		m_speed_f = min(1.0f, m_speed_f + k*0.01f);
	}

	k *= m_speed * (m_speed_f  *(220.0f/app->sRect.top));
	v.z = (1 - (t-1)*(t-1)) * app->sRect.top + 0.00001f;

	t += k;
	pos.z = v.z;


	if(pos.y >= app->sRect.right)
	{
		v.y = -v.y;
		pos.y = app->sRect.right - (pos.y - app->sRect.right);

		app->m_FireTush->m_scale = 1;
		app->m_s_tush->Play((mmDword)this);
	}
	if(pos.y <= app->sRect.left)
	{
		v.y = -v.y;
		pos.y = app->sRect.left + (app->sRect.left - pos.y);

		app->m_FireTush->m_scale = 1;
		app->m_s_tush->Play((mmDword)this);
	}
	if(pos.z < 0)
	{
//		if( ( !app->m_pDoska->m_dyrka ) & (((pos.y >= app->m_pDoska->l)&&(pos.y <= app->m_pDoska->r))&&(app->LevelState != 4) ))
		if( ( !app->m_pDoska->m_dyrka ) & (((pos.y >= app->m_pDoska->l)&&(pos.y <= app->m_pDoska->r)) ))
		{
			t = 0;

			app->m_s_up->Play((mmDword)this);

			v.z = vUp;
			pos.z = 0 ;//-pos.z;
			app->m_pBatut->SetAnimationPos("batut_1");
			app->m_pBatut->PlayAnimationRange("batut_up_do", enAnimHold);

			mmFloat kx;
			if(v.y < 0)
				kx = -1.0f;
			else
				kx = 1.0f;
			
			if( pos.y > app->m_pDoska->m_mxLocal._41 )
			{
				v.y = kx*(pos.y - app->m_pDoska->m_mxLocal._41)/   app->m_pDoska->w * 4.0f;	// -> x
				if(v.y < 0.0f)
					v.y = -v.y;
			}else{
				v.y = kx*(app->m_pDoska->m_mxLocal._41 - pos.y)/   app->m_pDoska->w * 4.0f ;
				if(v.y > 0.0f)
					v.y = -v.y;
			}

			if(jertva != mmnull)
			{
				jertva->m_mxWorld._43 = 33.557f;
				jertva->m_state = enJrtvaUbigaet;
				jertva = mmnull;
				m_state = enManDed;
			}

		}else{ //smert

			app->m_curent->state = enManDed;

			app->m_lives --;

			app->m_s_boom1->Play((mmDword)this);

			m_mxLocal._43 = 0.0f;

			//app->m_pDoska->Move(0);
//			app->LevelState = 4;
			app->m_pMan->SetAnimationPos("over1");
			app->m_pMan->PlayAnimationRange("over_1", enAnimHold);

			app->m_pAAng->m_mxLocal._41 = -m_mxLocal._42;
			app->m_pAAng->PlayAnimationRange("aang", enAnimLoop);

			if(jertva != mmnull)
			{
				app->m_count--;
				jertva->m_mxWorld._43 = 33.557f;
				jertva->m_state = enJrtvaUpalaMan;
				jertva = mmnull;
			}
			return;
		}

	}

//	if((app->LevelState != 4))
//	{
		mmFloat klook = (1.0f - ((pos.y - app->sRect.left) / (app->sRect.right - app->sRect.left)));
		mmFloat plook = (app->m_pMan->m_AnimKeys->GetKey(9).pos - app->m_pMan->m_AnimKeys->GetKey(8).pos)*klook +  app->m_pMan->m_AnimKeys->GetKey(8).pos  ;
		app->m_pMan->PlayAnimationToMiSec(plook);
//	}

	
	m_mxLocal._43 = pos.z;
	m_mxLocal._42 = -pos.y;

//	Sleep(50);

//	m_mxFull = m_mxLocal * m_mxWorld;

*/

}
//**************************************************************************
clsFireDoska::clsFireDoska(clsEntityFactory *pFactory, 
					 const clsString &type,
					 const clsString &name):
		   clsFireEntity(pFactory, type, name)
{
	OnInitialize(mmfalse);		
}
mmVoid clsFireDoska::OnInitialize(mmBool bClone)
{
	w = 110;
	n_w = w;
	l = -(w*0.5f);
	r = (w*0.5f);
	m_dyrka = mmfalse;
	
}
//==========================================================================================
clsFireDoska::~clsFireDoska()
{
}
//--------------------------------
mmVoid clsFireDoska::Move(mmFloat k)
{
	if(m_state == enManVybygaet)
	{
		m_state = enManBejit;
		app->m_pMan->m_next_state = enManPrigaet;
		app->m_pMan->PlayAnimationRangeStart("start_run_2", enAnimHold);
		app->m_pDoska->m_next_state = enDoskaOk;
		app->m_pDoska->PlayAnimationRangeStart("start_run_2", enAnimHold);
		return;
	}


	if(m_state == enDoskaOk)
	{


		if( k == 0 )
		{
			PlayAnimationRange("run_man", enAnimHold);
		}else{
			PlayAnimationRange("run_man", enAnimLoop);
		}

		if(w != n_w)
		{
			PlayAnimationRange("run_man", enAnimLoop);
			w += (n_w-w)* (app->m_Timer->GetDeltaMiSecs())*0.001f;
			if(mmAbs(n_w-w) < 1.0f)
				w = n_w;
		}


		//m_pDoska = (clsFireDoska*)
		this->FindChild("pd1")->m_mxLocal._41 = -(w-110.f)*0.5f;
		this->FindChild("pd2")->m_mxLocal._41 = (w-110.f)*0.5f;



		m_mxLocal._41 += k;

		if(m_mxLocal._41 < app->sRect.left)
			m_mxLocal._41 = app->sRect.left;
		if(m_mxLocal._41 > app->sRect.right)
			m_mxLocal._41 = app->sRect.right;

		l = -(w*0.5f)+m_mxLocal._41;
		r = (w*0.5f)+m_mxLocal._41;

	}
}
//**************************************************************************
clsFireJertvaImiter::clsFireJertvaImiter(clsEntityFactory *pFactory, 
					 const clsString &type,
					 const clsString &name):
		   clsFireEntity(pFactory, type, name)
{
	m_bNeedUpdateEvent = mmtrue;
	count = 1;
	tim = 0;
	m_state = enJertvaStart;


	app->m_count += count;

	m_kamikadze = mmfalse;
}
//----------------------
mmVoid clsFireJertvaImiter::OnLoadProps(clsVarList * vars)
{
	clsFireEntity::OnLoadProps(vars);

	clsVariable* var = vars->FindVariable("count");
	if (var != mmnull)
	{
		count = *var;

		app->m_count -= 1;
		app->m_count += count;

		app->m_count_start_jertv = app->m_count;
	}
}
//==========================================================================================
clsFireJertvaImiter::~clsFireJertvaImiter()
{
}
//--------------------------------
mmVoid clsFireJertvaImiter::OnUpdate(const clsTimer* pTimer)
{
	
	if(count == 0)
		((clsFireFire*)(this->Child))->m_inman = mmfalse;
	//		((clsFireFire*)(this->Child->Next))->m_inman = mmfalse;

	tim += pTimer->GetDeltaMiSecs();
	if(count > 0)
	{
		if(m_state == enJertvaStart)
		{

			clsEntity * clon = app->m_pMan_01->Clone("man_x",mmtrue);
			clon->m_mxWorld.Identity();// = this->m_mxFull;
			clon->m_mxFull.Identity();
			clon->m_mxLocal.Identity();
			clon->EnableRendering(mmtrue,mmtrue);
			clon->SetOwner(this);
			tim = 0;
			m_state = enJertvaVilezaet;
			clon->PlayAnimationRange("j_1",enAnimHold);
		}
		if(m_state == enJertvaVilezaet)
		{
			if(!this->Child->m_AnimationRageController->IsPlaying)
				m_state = enJertvaMahaet;
		}
		if(m_state == enJertvaMahaet)
		{
			m_state = enJertvaOk; // mahaet
			tim = 0;

			mmFloat s,e;
			s = this->Child->m_AnimKeys->GetKey(this->Child->m_AnimKeys->FindKey("j_2")).pos;
			e = this->Child->m_AnimKeys->GetKey(this->Child->m_AnimKeys->FindKey("j_3")).pos;

			this->Child->SetAnimationPosMiSec((e-s) * app->m_rand.frand(1.0f) +s)  ;
			this->Child->PlayAnimationRange("j_2");
		}
		if(m_state == enJertvaOk)
		{	
			if( app->m_pMan->m_state == enManPrigaet )
			{
				mmFloat rast;
				clsVector3 pj,pm;
				pj = app->m_Camera.ProjectHomo(m_mxFull.GetTranslation());
				pm = app->m_Camera.ProjectHomo(app->m_pMan->m_mxFull.GetTranslation());

				pj.z = pm.z = 0.0f;

				rast = (pm - pj).Length();
				if( (rast <= 0.05f)&&(app->m_pMan->jertva == mmnull))
				{

					//state = 4;
					this->Child->SetAnimationPos("j_4")  ;
					this->Child->PlayAnimationRange("j_3",enAnimHold);

//p					app->m_pMan->state = 1;

	 				m_state = enJertvaHvataet;

					//app->m_pMan->v = -app->m_pMan->v;
				
				}
			}
		}
	//	if(state == 7)
	//	{

	//	}

	
		if((((clsFireEntity*)(this->Child))->m_state == 	enJertvaKamikadzeUpal)&&(m_kamikadze == mmtrue))
		{
			((clsFireJertva*)(this->Child))->m_state = enJertvaPadaet;//12

			this->Child->m_mxWorld = this->Child->m_mxFull;

			this->Child->PlayAnimationRangeStart("j_do",enAnimHold);

//			m_state = enJertvaStart;
			//count--;
			m_kamikadze = mmfalse;
//			if(count == 0)
//				((clsFireFire*)(this->Child->Next))->m_inman = mmfalse;

			this->Child->SetOwner(app->m_Scene);
			tim = 0;

			m_state = enJertvinet;
			app->m_count_dead_jertv++;
		}


		if(m_state == enJertvaHvataet)
		{
			if(m_kamikadze == mmtrue)
			{
//p				((clsFireJertva*)(this->Child))->state = 12;

		
/*

				this->Child->m_mxWorld = this->Child->m_mxFull;

				this->Child->PlayAnimationRangeStart("j_do",enAnimHold);

				m_state = enJertvaStart;
				count--;
				m_kamikadze = mmfalse;
				if(count == 0)
					((clsFireFire*)(this->Child->Next))->m_inman = mmfalse;

				this->Child->SetOwner(app->m_Scene);
*/

			}else{

		//pp		app->m_s_jertva->Play((mmDword)this);
				app->PlayFX(app->m_s_jertva);

				app->m_pMan->jertva = (clsFireJertva*)(this->Child);

				this->Child->m_mxWorld = this->Child->m_mxFull;
				((clsFireEntity*)(this->Child))->m_state = enJertvaAndMan;
				this->Child->SetOwner(app->m_Scene);
				m_state = enJertvaShvatil;//enJertvaKamikadzeUpal;
			}

			tim = 0;

		}
		if((m_state == enJertvinet)||(m_state == enJertvaShvatil))
		{
			if( tim >= 2000.0f)
			{
				m_state = enJertvaStart;
				tim=0;
				count--;
			}
		}

	}

}
//**************************************************************************
clsFireJertva::clsFireJertva(clsEntityFactory *pFactory, 
					 const clsString &type,
					 const clsString &name):
		   clsFireEntity(pFactory, type, name)
{
	m_bNeedUpdateEvent = mmtrue;

	size = 1.0f;
//	((clsFireEntity*)FindChild("Box38"))->m_alpha = size;

	m_state = enNoneGam;
	time = 0;

	nap_rot = 0.0f;

	nap_polz = 1.0f;
	if((app->m_rand.rand()%2)==0)
		nap_polz = -nap_polz;
}
//==========================================================================================
clsFireJertva::~clsFireJertva()
{
}
mmVoid clsFireJertva::OnInitialize(mmBool bClone)
{
	clsString nam;

	if(bClone == mmfalse)
	{
		for(mmInt z = 2;z < (N_MANS+1) ;z++)
		{
			nam = "mans/man_" + clsString().FromFormatNum(z,"02")+".mtj";
			(clsBaseTexture*)app->m_ResMan->GetResource(nam, MHMRES_TEXTURE_ID);
		}
	}

	size = 1.0f;
	((clsFireEntity*)FindChild("Box38"))->m_alpha = size;

	mmInt tip;
	tip = app->m_rand.rand()%(N_MANS-1)+2;
	
	nam = "mans/man_" + clsString().FromFormatNum(tip,"02")+".mtj";
	((clsFireEntity*)this->FindChild("Box38"))->m_sel_tex = (clsBaseTexture*)app->m_ResMan->GetResource(nam, MHMRES_TEXTURE_ID);

}
//--------------------------------
mmVoid clsFireJertva::OnUpdate(const clsTimer* pTimer)
{
	

	if(m_state == enJertvaAndMan)
	{
		clsVector3 v,p;
		v = this->m_mxWorld.GetTranslation();
		p = app->m_pMan->m_mxFull.GetTranslation();
		p.y += 30;
		v.MoveToVector(p, pTimer->GetDeltaMiSecs()*5.0f);
		this->m_mxWorld.SetTranslation(v);
		//this->m_mxWorld = app->m_pMan->m_mxFull;
	}
	
	if(m_state == enJrtvaUbigaetStart)
	{
		m_state = enJrtvaUbigaetStartproc;
		m_next_state = enJrtvaUbigaet;
		SetAnimationPos("j_p1");
		PlayAnimationRange("j_p1", enAnimHold);

		app->m_scores += 100;
		app->m_game->m_scor.Text->UpdateEffect(0,mmtrue);
		app->m_count--;
		app->m_game->m_jert.Text->UpdateEffect(0,mmtrue);
	}
	
	if(m_state == enJrtvaUbigaet)
	{
		m_state = enJrtvaBejit;
		PlayAnimationRange("j_r1");
	}

	if(m_state == enJrtvaBejit)
	{
		this->m_mxLocal._41 -= pTimer->GetDeltaMiSecs()*0.09f;
		if(this->m_mxLocal._41 <= -(app->sRect.Width()+200))
		{
			this->m_bKillMe = mmtrue;	
		}
	}

	if(m_state == enJrtvaUpalaMan)
	{
		m_state = enJrtvaUpalaManProc;
		PlayAnimationRangeStart("j_boom", enAnimHold);
		app->m_SceneTMP = this;

	}

	if(m_state == enJertvaPadaet)
	{
		mmFloat k;
		k = pTimer->GetDeltaMiSecs() * 0.013f;
		m_mxLocal._43 = -((time)*(time));

		m_mxLocal._42 -= pTimer->GetDeltaMiSecs() * 0.06f;

		if(m_mxLocal._42 < -43.9f)
			m_mxLocal._42 = -43.9f;

		time += k;

		if( m_mxFull._43 <= 35.0f)
		{
			m_mxFull._43 = 35.0f;
			m_mxWorld = m_mxFull;
			m_mxLocal.Identity();
			m_state = enJertvaKamikadzeRazbilsa;//13
			PlayAnimationRangeStart("j_paden", enAnimHold);
			app->m_count --;
			app->m_game->m_jert.Text->UpdateEffect(0,mmtrue);

		}
	}

	if(m_state == enJertvaKamikadzeRazbilsa)
	{
				app->PlayFX(app->m_s_boom1);
		m_state = enJertvaKamikadzeIschezaet;

/*
		size -= pTimer->GetDeltaMiSecs() * 0.001f;
		size = 0.5f;
		if(size < 0.0f)
		{
			size = 0.0f;
			this->m_bKillMe = mmtrue;
		}
		((clsFireEntity*)FindChild("Box38"))->m_alpha = size;
		*/	

	}
	
	if(m_state == enJertvaKamikadzeIschezaet)
	{
		
/*
		nap_rot += nap_polz * pTimer->GetDeltaMiSecs() * 0.001f;
		if(mmAbs(nap_rot) > (MHM_PI * 0.5f ))
			nap_rot = MHM_PI * 0.5f * nap_polz;
		else
			m_mxLocal._42 -= pTimer->GetDeltaMiSecs() * 0.01f;

		m_mxLocal = clsMatrix().RotationZ(nap_rot).SetTranslation(m_mxLocal.GetTranslation());

		m_mxLocal._41 += nap_polz * pTimer->GetDeltaMiSecs() * 0.02f;

		if( mmAbs(m_mxLocal._41) > (app->sRect.Width()+100.0f) )
			this->m_bKillMe = mmtrue;
*/

		size -= pTimer->GetDeltaMiSecs() * 0.001f;
		//size = 0.5f;
		if(size < 0.0f)
		{
			size = 0.0f;
			this->m_bKillMe = mmtrue;
		}
		((clsFireEntity*)FindChild("Box38"))->m_alpha = size;
	
	}

}
mmVoid clsFireJertva::OnInit()
{
	size = 1.0f;
	((clsFireEntity*)FindChild("Box38"))->m_alpha = size;
//	if(state >= 13)
//	this->m_bKillMe = mmtrue;


}
///////////////////////
//**************************************************************************
clsFireParticle::clsFireParticle(clsEntityFactory *pFactory, 
					 const clsString &type,
					 const clsString &name):
		   clsFireEntity(pFactory, type, name)
{
	//m_bNeedUpdateEvent = mmtrue;

//	state = 0;
	m_scale = 0;

//	this->EnableBoundSphereTest(mmfalse);

	//this->m_cBoundSphere.m_fRadius = 80.0f;
	//this->m_fBoundScale = 80.0f;
	
}
mmVoid clsFireParticle::OnLoadProps(clsVarList * vars)
{
	clsVariable* var = vars->FindVariable("size");
	if (var != mmnull)
	{	
		m_scale = *var;
	}
}
mmVoid clsFireParticle::OnNoSphereTest()
{
	if( ((clsBaseParticles*)Res)->LoopEmitter() == mmfalse )
		{	
			//((clsBaseParticles*)Res)->Render();
			((clsBaseParticles*)Res)->ClearEmitter();
			((clsBaseParticles*)Res)->StopEmitter();
		}
}
mmVoid clsFireParticle::OnInitialize(mmBool bClone)
{
	this->m_cBoundSphere.m_fRadius = RADPAT;
	//this->m_fBoundScale = 80.0f;

	//if(bClone)
	//	return;
	if(!bClone)
	{
		clsString nam;
		nam = Name.Right(Name.Length() - 10);
		nam = nam.CutBy('.');
		nam = nam+".apd";
		mmSafeRelease(Res);
		Res = Renderer->GetResourceManager()->GetResource(nam,enResBaseParticles);	
	}
		
		((clsBaseParticles*)Res)->SetEmitterScale(m_scale);

		((clsBaseParticles*)Res)->ResetEmitter();

		if( ((clsBaseParticles*)Res)->LoopEmitter() == mmfalse )
		{	
			((clsBaseParticles*)Res)->Render();
			((clsBaseParticles*)Res)->ClearEmitter();
			((clsBaseParticles*)Res)->StopEmitter();
		}
		
}
mmVoid clsFireParticle::OnClone(clsEntity* pInstance)
{
	m_scale = ((clsFireParticle*)pInstance)->m_scale;
}
mmVoid clsFireParticle::OnUpdate(const clsTimer* pTimer)
{
	mmFloat kst;
	kst = pTimer->GetDeltaMiSecs() * 0.0005f;
	if( m_size > m_scale)
	{
		m_scale += kst;
		if(m_size < m_scale)
			m_scale = m_size;
		((clsBaseParticles*)Res)->SetEmitterScale(m_scale);
	}else
	if( m_size < m_scale)
	{
		m_scale -= kst;
		if(m_size > m_scale)
			m_scale = m_size;
		((clsBaseParticles*)Res)->SetEmitterScale(m_scale);
	}else
	if(m_size > 0)
	{
		m_size -= pTimer->GetDeltaMiSecs() * 0.00005f;
		if(m_size < 0)
			m_size = 0;
	}
}
//==========================================================================================
clsFireParticle::~clsFireParticle()
{
}
//**************************************************************************
clsFireFire::clsFireFire(clsEntityFactory *pFactory, 
					 const clsString &type,
					 const clsString &name):
		   clsFireParticle(pFactory, type, name)
{
	m_bNeedUpdateEvent = mmtrue;

//	state = 0;
	m_scale = 0;
	m_size = 0;
	m_inman = mmfalse;
	
}
mmVoid clsFireFire::OnLoadProps(clsVarList * vars)
{
	clsVariable* var = vars->FindVariable("size");
	if (var != mmnull)
	{
		m_scale = *var;
		m_size = m_scale;
		m_max = m_scale;
	}
	var = vars->FindVariable("max");
	if (var != mmnull)
	{
		m_max = *var;
	}

	var = vars->FindVariable("tim_doun");
	if (var != mmnull)
	{
		m_j_doun = *var;
	}else{
		m_j_doun = 30000;
	}
}
mmVoid clsFireFire::OnInitialize(mmBool bClone)
{
	this->m_cBoundSphere.m_fRadius = RADPAT;

	TimFire = (mmFloat)(app->m_rand.rand()%5000 + 27500);
	if(m_j_doun == 0)
		TimFire_j = 0;
	else
		TimFire_j = (mmFloat)(app->m_rand.rand()%20000 + m_j_doun);
//	TimFire_j = 3000;


	//time = 0;
	time = (mmFloat)(app->m_rand.rand()%((mmInt)(TimFire-1000)));



	if(bClone)
		return;

	//	m_scale = 1;//p
	//	m_size = m_scale;

		clsString nam;
		nam = Name.Right(Name.Length() - 12);
		nam = nam.CutBy('.');
		nam = nam+".apd";
		mmSafeRelease(Res);
		Res = Renderer->GetResourceManager()->GetResource(nam,enResBaseParticles);
		((clsBaseParticles*)Res)->SetEmitterScale(m_scale);

		

		app->m_sizeF += m_scale;


		if(this->Owner != mmnull)
		{
			if( this->Owner->Name.Left(7) == "jertva_" )
			{
				m_inman = mmtrue;
			}
		}

		//app->m_sizeF += m_size;

/*
		if(this->Child != mmnull)
		{
			((clsBaseParticles*)this->Child->Res)->SetEmitterScale(m_scale*0.5f);
			if(this->Child->Next != mmnull)
			((clsBaseParticles*)this->Child->Next->Res)->SetEmitterScale(m_scale*0.5f);
		}*/
}
//==========================================================================================
clsFireFire::~clsFireFire()
{
}
//--------------------------------
mmVoid clsFireFire::OnUpdate(const clsTimer* pTimer)
{

	if(m_scale < 0)
		m_scale = 0;

	app->m_sizeF += m_scale;

	mmFloat kst;
	time += pTimer->GetDeltaMiSecs();
	kst = pTimer->GetDeltaMiSecs() * 0.0005f;
	if(app->m_win == mmtrue)
		kst = 0;
	if( m_size > m_scale)
	{	
		if(app->m_game->m_fir.Text->ok == mmfalse)
			app->m_game->m_fir.Text->UpdateEffect(0,mmtrue);

		m_scale += kst;
		if(m_size < m_scale)
			m_scale = m_size;
		((clsBaseParticles*)Res)->SetEmitterScale(m_scale);
		if(this->Child != mmnull)
		{
			((clsBaseParticles*)this->Child->Res)->SetEmitterScale(m_scale*0.5f);
			if(this->Child->Next != mmnull)
				((clsBaseParticles*)this->Child->Next->Res)->SetEmitterScale(m_scale*0.5f);
		}
	}
	if( m_size < m_scale)
	{
		if(app->m_game->m_fir.Text->ok == mmfalse)
			app->m_game->m_fir.Text->UpdateEffect(0,mmtrue);
		m_scale -= kst;
		if(m_size > m_scale)
			m_scale = m_size;
		((clsBaseParticles*)Res)->SetEmitterScale(m_scale);
		if(this->Child != mmnull)
		{
			((clsBaseParticles*)this->Child->Res)->SetEmitterScale(m_scale*0.5f);
			if(this->Child->Next != mmnull)
				((clsBaseParticles*)this->Child->Next->Res)->SetEmitterScale(m_scale*0.5f);
		}
	}

	clsVector3 pf,pm, _pm, p;
	pf = app->m_Camera.ProjectHomo(m_mxFull.GetTranslation());
	pm = app->m_Camera.ProjectHomo((app->m_pMan->m_mxLocal * app->m_pMan->m_mxWorld).GetTranslation());
	_pm = app->m_Camera.ProjectHomo(app->m_pMan->_pos);
	pf.z = pm.z = _pm.z = 0.0f;
	pf.y -= 0.05f;


	if((m_size == m_scale)&&(m_size > 0))
	{
//		if(app->LevelState == 3)
//		{
		if( (app->m_pMan->_pos.LengthSq() != 0.0f) && (SphereIntersects(pm,_pm, pf, pf, 0.05f , 0.05f , p)) )
			{
				//** Priz

				app->m_RandPriz.Update(pTimer);
				mmInt priz;
				priz = app->m_RandPriz.GetMax();
				if(priz >= 0)
				{

					clsFirePriz * pr = (clsFirePriz*)(app->m_priz1->Clone("priz_x",mmtrue));
					pr->OnInitialize(mmfalse);
					pr->InitPr(priz);
					pr->m_mxWorld.Identity();
					pr->m_mxWorld.SetTranslation(this->m_mxFull.GetTranslation());
					pr->m_next_state = enPrizDown;
					pr->PlayAnimationRangeStart("pr_1",enAnimHold);
					pr->state = 0;

				}
				//**


	//pp			app->m_s_tush->Play((mmDword)this);
				app->PlayFX(app->m_s_tush);


				app->m_scores += 25;
				app->m_game->m_scor.Text->UpdateEffect(0,mmtrue);

				app->m_FireTush->m_scale = 1;


				if(app->m_pMan->m_priz == 0)
				{
					if(m_inman != mmtrue)
					{
						p = (p-pf).NormalizeFast();

						if( (p.x < 0.5f) && (p.x > -0.5f) )
						{
							app->m_pMan->v.y = - app->m_pMan->v.y;
						}else if(app->m_pMan->t < 1.0f)
						{
							app->m_pMan->t = 2.0f - app->m_pMan->t;
							app->m_pMan->m_speed_f = 0.0f;
						}
					}
				}else if(app->m_pMan->m_priz == 1)
				{
					m_size = 0;
				}

				

				m_size -= 0.5f;
				if(m_size < 0)
					m_size = 0;

				time = 0.0f;

				// fire down
				clsFireFireDouw * fap = (clsFireFireDouw *)(app->m_Scene->AddChildNode("particles_d_flame2.007","particles_d_flame2.007"));
				fap->m_scale = 0.5f;
				fap->m_size = fap->m_scale;
				fap->OnInitialize(mmfalse);
				fap->m_mxWorld.SetTranslation(m_mxFull.GetTranslation());
				//fap->m_mxWorld._42 -= 55;
			}
	//	}
	}

	if(m_inman == mmtrue)
	{
		if((time > TimFire_j)&&(TimFire_j != 0))
		{
			m_size = min(m_size + 0.5f, m_max);
			time = 0.0f;
	//pp		app->m_s_fdo->Play((mmDword)this);
			app->PlayFX(app->m_s_fdo);


			if( m_size == m_max)
			{
				m_size *= 0.5f;

	//			((clsFireJertvaImiter*)(Owner))->state = 40;
				((clsFireJertvaImiter*)(Owner->Child))->m_next_state = enJertvaKamikadzeUpal;
				((clsFireJertvaImiter*)(Owner))->Child->PlayAnimationRangeStart("j_3", enAnimHold);

				((clsFireJertvaImiter*)(Owner))->m_kamikadze = mmtrue;

			}


				// fire down
				clsFireFireDouw * fap = (clsFireFireDouw *)(app->m_Scene->AddChildNode("particles_d_flame2.007","particles_d_flame2.007"));
				fap->m_scale = 0.5f;
				fap->m_size = fap->m_scale;
				fap->OnInitialize(mmfalse);
				fap->m_mxWorld.SetTranslation(m_mxFull.GetTranslation());
				//fap->m_mxWorld._42 -= 55;


		}
	}else{
		if(time > TimFire)
		{
			m_size = min(m_size + 0.5f, m_max);
			time = 0.0f;
	//pp		app->m_s_fdo->Play((mmDword)this);
			app->PlayFX(app->m_s_fdo);


				// fire down
				clsFireFireDouw * fap = (clsFireFireDouw *)(app->m_Scene->AddChildNode("particles_d_flame2.007","particles_d_flame2.007"));
				fap->m_scale = 0.5f;
				fap->m_size = fap->m_scale;
				fap->OnInitialize(mmfalse);
				fap->m_mxWorld.SetTranslation(m_mxFull.GetTranslation());
				//fap->m_mxWorld._42 -= 55;
		}
	}

}

//**************************************************************************


//**************************************************************************
clsFireFireDouw::clsFireFireDouw(clsEntityFactory *pFactory, 
					 const clsString &type,
					 const clsString &name):
		   clsFireParticle(pFactory, type, name)
{
	m_bNeedUpdateEvent = mmtrue;

//	state = 0;
	m_scale = 0;
	m_size = 0;	
}
mmVoid clsFireFireDouw::OnInitialize(mmBool bClone)
{
	m_state = enStartFireDownVilet;
	if(bClone)
		return;

		clsString nam;
		nam = Name.Right(Name.Length() - 12);
		nam = nam.CutBy('.');
		nam = nam+".apd";
		mmSafeRelease(Res);
		Res = Renderer->GetResourceManager()->GetResource(nam,enResBaseParticles);
		((clsBaseParticles*)Res)->SetEmitterScale(m_scale);
			time = 0;
}
//==========================================================================================
clsFireFireDouw::~clsFireFireDouw()
{
}
//--------------------------------
mmVoid clsFireFireDouw::OnUpdate(const clsTimer* pTimer)
{
#define SPIDD ((0.005f))
	
	if(m_mxFull._42 > -0.408f)
	{
		m_mxLocal._42 -= pTimer->GetDeltaMiSecs() * 0.05f;//0.03f;
	}else{
		m_mxFull._42 = 0.408f;
	}

	if(m_state == enStartFireDownVilet)
	{
		mmFloat k;
		k = pTimer->GetDeltaMiSecs() * SPIDD;
		m_mxLocal._43 = -((time)*(time));

		time += k;

		if(m_mxFull._43 <= 3.636f)
		{
			m_state = enStartFireDown;	
			app->PlayFX(app->m_s_fdo);
		}

		if( ( !app->m_pDoska->m_dyrka ) & ((m_mxFull._43 <= 15.85f)&&( (m_mxFull._41 >= (app->m_pDoska->m_mxFull._41 - app->m_pDoska->w*0.5f ) )&&(m_mxFull._41 <= (app->m_pDoska->m_mxFull._41 + app->m_pDoska->w*0.5f)) ) ))
		{
			m_state = enFireDownSlavil;
			app->m_FireDoska->m_size += 0.3f;
	//pp		app->m_s_fdo->Play((mmDword)this);
			app->PlayFX(app->m_s_fdo);

			//------ dyrka
			if(app->m_FireDoska->m_size >= 1.0f)
			{
				app->m_FireDoska->m_size = 0;

				((clsBaseMaterial*)((clsBaseMesh*)app->m_pDoska->FindChild("batut")->Res)->GetMaterial(0))->m_map_diffuse.m_texture
					=
					(clsBaseTexture*)app->m_ResMan->GetResource("mans/dyrka.mtj", MHMRES_TEXTURE_ID);
				((clsBaseMaterial*)((clsBaseMesh*)app->m_pDoska->FindChild("batut")->Res)->GetMaterial(0))->SetBlendType(enBlendAlpha); //m_map_diffuse.m_alphablend = mmtrue;
				app->m_pDoska->m_dyrka = mmtrue;



				((clsBaseParticles*)(app->m_pDoska->FindChild("particles_smok.doska01")->Res))->ResetEmitter();

			}
		}
	}

	if(m_state == enStartFireDown)
	{
		m_scale -= pTimer->GetDeltaMiSecs() * 0.0003f;
		if(m_scale <= 0)
		{
			this->m_bKillMe = mmtrue;
			m_scale = 0;
		}

		((clsBaseParticles*)Res)->SetEmitterScale(m_scale);
	}

	if(m_state == enFireDownSlavil)
	{
		m_scale -= pTimer->GetDeltaMiSecs() * 0.0005f;
		if(m_scale <= 0)
		{
			this->m_bKillMe = mmtrue;
			m_scale = 0;
		}

		((clsBaseParticles*)Res)->SetEmitterScale(m_scale);
	}

	

}
//***********************************************************************






//**************************************************************************
clsFirePriz::clsFirePriz(clsEntityFactory *pFactory, 
					 const clsString &type,
					 const clsString &name):
		   clsFireEntity(pFactory, type, name)
{
	m_bNeedUpdateEvent = mmtrue;

	state = 0;
}
mmVoid clsFirePriz::OnInitialize(mmBool bClone)
{
	/*

	tip = (app->m_rand.rand()%N_PRIZ)+1;

	clsString nam = "priz/pr_";
	nam += clsString().FromFormatNum(tip,"02")+".jpg";
	((clsFireEntity*)this->Child->Child)->m_sel_tex = (clsBaseTexture*)app->m_ResMan->GetResource(nam, MHMRES_TEXTURE_ID);
*/
	if(bClone)
		return;
	time = 0;
	state = 0;
	m_scale = 1.0f;

	m_ecol = this->FindChild("priz_01_");

	m_ecol->Child->m_bRenderable = mmfalse;

	

	//((clsBaseParticles*)(m_ecol->Child->Res))->ResetEmitter();
}
mmVoid clsFirePriz::InitPr(mmInt pr)
{
	m_state = enPrizVilet;
	tip = pr+1;//(app->m_rand.rand()%N_PRIZ)+1;

	clsString nam = "priz/pr_";
	nam += clsString().FromFormatNum(tip,"02")+".jpg";
	((clsFireEntity*)this->Child->Child)->m_sel_tex = (clsBaseTexture*)app->m_ResMan->GetResource(nam, MHMRES_TEXTURE_ID);
/*
	if(bClone)
		return;
	time = 0;
	state = 0;
	m_scale = 1.0f;

	m_ecol = this->FindChild("priz_01_");

	m_ecol->Child->m_bRenderable = mmfalse;
*/	
}
//==========================================================================================
clsFirePriz::~clsFirePriz()
{
}
//--------------------------------
mmVoid clsFirePriz::OnUpdate(const clsTimer* pTimer)
{
	if(m_state == enPrizDown)
	{
		m_state = enPrizVilet;
		state = 1;
		PlayAnimationRangeStart("pr_2");
	}

	if(state == 1)
	{
		mmFloat k;
		k = pTimer->GetDeltaMiSecs() * 0.003f;
		m_mxLocal._43 = -((time)*(time));


		if(m_ecol->m_mxFull._42 > -0.408f)
		{
			m_mxLocal._42 -= pTimer->GetDeltaMiSecs() * 0.03f;
		}

		time += k;
									//3.636f
		if(m_ecol->m_mxFull._43 <= 10.0f)
		{
			state = 3;	
			this->AnimationRangeStop(mmtrue);
			mxs = this->Child->m_mxLocal;

			app->PlayFX(app->m_s_trrii);
		}
									//15.85f
		if( (!app->m_pDoska->m_dyrka ) & ((m_ecol->m_mxFull._43 <= 25.85f)&&( (m_ecol->m_mxFull._41 >= (app->m_pDoska->m_mxFull._41 - app->m_pDoska->w*0.5f ) )&&(m_ecol->m_mxFull._41 <= (app->m_pDoska->m_mxFull._41 + app->m_pDoska->w*0.5f)) )) )
		{
			state = 4;
			this->AnimationRangeStop(mmtrue);
			mxs = this->Child->m_mxLocal;
			m_ecol->Child->m_bRenderable = mmtrue;
			((clsBaseParticles*)(m_ecol->Child->Res))->ResetEmitter();
			app->m_scores += 100;

			app->m_game->m_scor.Text->UpdateEffect(0,mmtrue);

	//pp		app->m_s_pr->Play((mmDword)this);
			app->PlayFX(app->m_s_pr);


			app->PrizOk(tip);


		}
	}

	if((state == 3)||(state == 4))
	{
		m_scale -= pTimer->GetDeltaMiSecs() * 0.003f;
		if(m_scale < -10.0f)
		{
			m_scale = 0;
			this->m_bKillMe = mmtrue;
		}
		this->Child->m_mxLocal  = mxs * clsMatrix().Scale(max(0.0f,m_scale),max(0.0f,m_scale),max(0.0f,m_scale));
	}

}
//-------------------------------------
mmVoid clsFireApp::SetFireFactor(clsEntity* en,mmInt f)
{
	clsEntity	*Childs = en;
	while(Childs)
	{
		if(Childs->m_sType == "fire")
		{
			if(((clsFireFire*)Childs)->m_size > 0)
			{
				if(f == 0)

					((clsFireFire*)Childs)->m_size = 0.5f;
				else
					((clsFireFire*)Childs)->m_size =	((clsFireFire*)Childs)->m_max;
			}else{
				if(f == 0)
				{}
				else
				{
					((clsFireFire*)Childs)->m_size =	0.5f;
				}
			}
		}
		SetFireFactor(Childs->Child, f);
		Childs = Childs->Next;
	}

}
//-------------------------------------
mmVoid clsFireApp::InitScene(clsEntity* en)
{
	clsEntity	*Childs = en;
	while(Childs)
	{
		((clsFireEntity*)Childs)->OnInit();
		InitScene(Childs->Child);
		Childs = Childs->Next;
	}

}












//**************************************************************************
clsFireVarona::clsFireVarona(clsEntityFactory *pFactory, 
					 const clsString &type,
					 const clsString &name):
		   clsFireEntity(pFactory, type, name)
{
	m_bNeedUpdateEvent = mmtrue;
//	state = 0;
	Time2 = 0;
}
clsFireVarona::~clsFireVarona()
{

}
mmVoid clsFireVarona::OnUpdate(const clsTimer* pTimer)
{
	if(!this->m_bRenderable)
	{
		Time += pTimer->GetDeltaMiSecs();
		if(Time < RangeTime)
			return;
		Time = 0.0f;
		Time2 = 0;
		RangeTime = max(0.0f, RangeTime - 1000.0f);
		if((app->m_rand.rand()%2) != 0)
			return;
		nap = -nap;
		
		if(nap > 0.0f)
		{
			m_mxLocal.RotationZ(MHM_PI);
			m_mxWorld._41 = app->sRect.left - 150;
		}
		else
		{
			m_mxLocal.Identity();
			m_mxWorld._41 = app->sRect.right + 150;
		}
		this->EnableRendering(mmtrue,mmtrue);



		//---- pr
		if((app->m_rand.rand()%2) == 0)
		{
			n_priz = (app->m_rand.rand()%N_PRIZ)+1;

			clsString nam = "priz/pr_";
			nam += clsString().FromFormatNum(n_priz,"02")+".jpg";
			((clsBaseMaterial*)((clsBaseMesh*)m_priz->Res)->GetMaterial(0))->m_map_diffuse.m_texture =
				(clsBaseTexture*)app->m_ResMan->GetResource(nam, MHMRES_TEXTURE_ID);
			m_priz->EnableRendering(mmtrue,mmtrue);
		}else{
			n_priz = 0;
			m_priz->EnableRendering(mmfalse,mmtrue);
		}



	}else{

		Time2 += pTimer->GetDeltaMiSecs();

		m_mxWorld._41 += nap * pTimer->GetDeltaMiSecs() * 0.03f;

		if((nap > 0.0f) && (m_mxWorld._41 > (app->sRect.right + 150) ) )
		{
			this->EnableRendering(mmfalse,mmtrue);
		}
		if((nap < 0.0f) && (m_mxWorld._41 < (app->sRect.left - 150) ) )
		{
			this->EnableRendering(mmfalse,mmtrue);
		}

		m_mxWorld._43 = rand.lerpfrand(pTimer->GetMiSecs()*0.00025f)*(mmAbs(app->sRect.Height()) - 50.0f) + 100.0f;


		if(Time2 > 3000.0f)
		{
		if(  ( app->m_pMan->m_mxFull.GetTranslation() - m_mxFull.GetTranslation() ).LengthSq() < 400.0f )
		{
			app->PlayFX(app->m_s_kar);
			Time2 = 0;
			app->m_scores += 13;
			app->m_game->m_scor.Text->UpdateEffect(0,mmtrue);
			if(app->m_pMan->t < 1.0f)
			{
				app->m_pMan->t = 2.0f - app->m_pMan->t;
				app->m_pMan->m_speed_f = 0.0f;


				if(n_priz != 0)
				{
					FindChild("particles_pinto_power_punch.varona")->m_bRenderable = mmtrue;
					((clsBaseParticles*)(FindChild("particles_pinto_power_punch.varona")->Res))->ResetEmitter();
					m_priz->EnableRendering(mmfalse,mmtrue);
		//pp			app->m_s_pr->Play((mmDword)this);
					app->PlayFX(app->m_s_pr);
					app->m_scores += 100;
					app->m_game->m_scor.Text->UpdateEffect(0,mmtrue);

					app->PrizOk(n_priz);
					n_priz = 0;
				}
			}else{
				app->m_pMan->t = 2.0f - app->m_pMan->t ;
			}
		}
		}

	}

}
mmVoid clsFireVarona::OnInit()
{
	clsFireEntity::OnInit();
	m_mxWorld._41 = app->sRect.right + 150;
	this->EnableRendering(mmfalse,mmtrue);
	Time = 0;
	Time2 = 0;
	nap = 1.0f;
	if((app->m_rand.rand()%2) != 0)
		nap = -nap;
	RangeTime = 1;//10000.0f;


	n_priz = 0;
	m_priz = (clsFireEntity*)(app->m_Scene->FindChild("varona_priz"));
	m_priz->EnableRendering(mmfalse,mmtrue);
}
//******************************************
cls_HiScores::cls_HiScores(clsFireApp * papp)
{
	app = papp;

	mmFloat xx,yy,dy;
	xx = 0.5f;
	yy = 0.305f;
	dy = 0.085f;

	m_text.Text = (clsText*)(app->m_ResMan->GetResource("font_01.mhmFont2D", MHMRES_MISC_ID));
	m_text.Text->SetText("\bgHISCORES");
	m_text.Text->SetAlignX(enCenter);
	m_text.x = xx;
	m_text.y = yy-0.03f;
	m_text.Text->SetFiltering(mmtrue);
	m_text.enable = mmfalse;
	m_text.s = 0.5f;

	m_exit.Text = (clsText*)(app->m_ResMan->GetResource("font_01.mhmFont2D", MHMRES_MISC_ID));
	m_exit.Text->SetText("RETURN");//EXIT TO MAIN MENU");
	m_exit.Text->SetAlignX(enCenter);
	m_exit.x = xx;
	m_exit.y = 0.71f;
	m_exit.s = 0.42f;

	m_names.Text = (clsText*)(app->m_ResMan->GetResource("font_02.mhmFont2D", MHMRES_MISC_ID));
	m_names.Text->SetText("");
	m_names.Text->SetAlignX(enLeft);
	m_names.Text->SetAlignY(enCenter);
	m_names.x = 0.35f;
	m_names.y = 0.51f;
	m_names.enable = mmfalse;
	m_names.Text->SetFiltering(mmtrue);
	m_names.s = 0.35f;
	m_names.Color = 0xFFFFFF00;

	m_scores.Text = (clsText*)(app->m_ResMan->GetResource("font_02.mhmFont2D", MHMRES_MISC_ID));
	m_scores.Text->SetText("");
	m_scores.Text->SetAlignX(enRight);
	m_scores.Text->SetAlignY(enCenter);
	m_scores.x = 0.65f;
	m_scores.y = 0.51f;
	m_scores.enable = mmfalse;
	m_scores.s = 0.35f;
	m_scores.Color = 0xFFFFFF00;


}
mmVoid cls_HiScores::UpdateObjects()
{

	m_text.Update(app);
	m_exit.Update(app);
	m_names.Update(app);
	m_scores.Update(app);

	if(m_exit.state == 2)
	{
		//app->m_Zast->PlayAnimationRangeStart("zast_close", enAnimHold);
		//app->m_Zast->m_curent = app->m_main;
		app->SetTextureZast("inter/main_menu.mtj");
		//app->ZClose(app->m_main);
		app->ZClose(app->m_return);
	}


}
mmVoid cls_HiScores::RenderObjects()
{
	app->m_Renderer->SetCamera(&app->m_CameraZ);
	app->m_pEngine->RenderScene(app->m_ZastPlan);
	
	app->m_Renderer->GetDevice()->DepthWrite(mmfalse);
	m_text.Text->Render();
	m_exit.Text->Render();
	m_names.Text->Render();
	m_scores.Text->Render();
	app->m_Renderer->GetDevice()->DepthWrite(mmtrue);
}
mmVoid cls_HiScores::Init()
{
	app->m_Zast->m_curent = mmnull;
	app->m_Zast->PlayAnimationRangeStart("zast_open", enAnimHold);
	app->m_mouse = mmtrue;


	clsString n,s;
	n="";s="";

	for(mmUint z=0;z<app->m_HS.Size();z++)
	{
		if(z == 0)
		{
			n += "\b(FFFFE000)";
			s += "\b(FFFFE000)";
		}
		if(z == 3)
		{
			n += "\b(FFFFFF00)";
			s += "\b(FFFFFF00)";
		}
		n += app->m_HS[z].name + "\n";
		s += clsString().FromFormatNum( app->m_HS[z].score,"")  + "\n";
	}
	m_names.Text->SetText(n);
	m_scores.Text->SetText(s);




}
//*****************************************************************************************
//******************************************
cls_Pause::cls_Pause(clsFireApp * papp)
{
	app = papp;

	mmFloat xx,yy,dy,s;
	xx = 0.5f;
	yy = 0.305f;
	//dy = 0.085f;

	dy = 0.078f;
	s = 0.85f;

	m_continue.Text = (clsText*)(app->m_ResMan->GetResource("font_01.mhmFont2D", MHMRES_MISC_ID));
	m_continue.Text->SetText("CONTINUE");
	m_continue.Text->SetAlignX(enCenter);
	m_continue.x = xx;
	m_continue.y = yy;
	m_continue.Text->SetFiltering(mmtrue);
	m_continue.s=s;

	m_options.Text = (clsText*)(app->m_ResMan->GetResource("font_01.mhmFont2D", MHMRES_MISC_ID));
	m_options.Text->SetText("OPTIONS");
	m_options.Text->SetAlignX(enCenter);
	m_options.x = xx;
	m_options.y = yy+dy;
	m_options.s=s;

	m_hiscores.Text = (clsText*)(app->m_ResMan->GetResource("font_01.mhmFont2D", MHMRES_MISC_ID));
	m_hiscores.Text->SetText("HISCORES");
	m_hiscores.Text->SetAlignX(enCenter);
	m_hiscores.x = xx;
	m_hiscores.y = yy+dy*2;
	m_hiscores.s=s;

	m_info.Text = (clsText*)(app->m_ResMan->GetResource("font_01.mhmFont2D", MHMRES_MISC_ID));
	m_info.Text->SetText("INFO");
	m_info.Text->SetAlignX(enCenter);
	m_info.x = xx;
	m_info.y = yy+dy*3;
	m_info.s=s;

	m_exit.Text = (clsText*)(app->m_ResMan->GetResource("font_01.mhmFont2D", MHMRES_MISC_ID));
	m_exit.Text->SetText("MAIN MENU");
	m_exit.Text->SetAlignX(enCenter);
	m_exit.x = xx;
	m_exit.y = yy+dy*4;
	m_exit.s=s;

}
mmVoid cls_Pause::UpdateObjects()
{

	m_continue.Update(app);
	m_options.Update(app);
	m_hiscores.Update(app);
	m_info.Update(app);
	m_exit.Update(app);


	if(m_continue.state == 2)
	{
		app->m_NoInit = mmtrue;
		app->SetTextureZast("inter/fire_starter.mtj");
		app->ZClose(app->m_game);
		app->m_rand.init((mmUint)app->m_Timer->GetTicks());
		app->m_Pause = mmtrue;
	}

	if(m_exit.state == 2)
	{
		app->SetTextureZast("inter/main_menu.mtj");
		app->ZClose(app->m_main);
	}

	if(m_options.state == 2)
	{
		app->SetTextureZast("inter/options.mtj");
		app->ZClose(app->m_options);
	}

	if(m_info.state == 2)
	{
		//app->m_Zast->PlayAnimationRangeStart("zast_close", enAnimHold);
		//app->m_Zast->m_curent = app->m_info;
		app->SetTextureZast("inter/info.mtj");
		app->ZClose(app->m_info);
	}

	if(m_hiscores.state == 2)
	{
		//app->m_Zast->PlayAnimationRangeStart("zast_close", enAnimHold);
		//app->m_Zast->m_curent = app->m_hi_scores;
		app->SetTextureZast("inter/hiscores.mtj");
		app->ZClose(app->m_hi_scores);
	}



}
mmVoid cls_Pause::RenderObjects()
{
	app->m_Renderer->SetCamera(&app->m_CameraZ);
	app->m_pEngine->RenderScene(app->m_ZastPlan);


	
	app->m_Renderer->GetDevice()->DepthWrite(mmfalse);
	m_continue.Text->Render();
	m_options.Text->Render();
	m_hiscores.Text->Render();
	m_info.Text->Render();
	m_exit.Text->Render();
	app->m_Renderer->GetDevice()->DepthWrite(mmtrue);
}
mmVoid cls_Pause::Init()
{
	app->m_return = this;
	app->m_pEngine->SetActiveScene(app->m_Zast);
	app->m_Zast->m_curent = mmnull;
	app->m_Zast->PlayAnimationRangeStart("zast_open", enAnimHold);

	app->m_mouse = mmtrue;
/*
	app->m_pMouse->SetPositionCenter();
	clsPoint2D pm = app->m_pMouse->GetPosition();
	pm.y = (mmInt)((mmFloat)pm.y / 2.5f);
	app->m_pMouse->SetPosition(pm);
	*/
}
//*****************************************************************************************
//******************************************
//*****************************************************************************************
//******************************************
cls_Main::cls_Main(clsFireApp * papp)
{
	app = papp;
	app->m_return = this;



	mmFloat xx,yy,dy,s;
	xx = 0.495f;
	yy = 0.36f;
	dy = 0.078f;
	s = 0.85f;

	m_start_game.Text = (clsText*)(app->m_ResMan->GetResource("font_01.mhmFont2D", MHMRES_MISC_ID));
	m_start_game.Text->SetText("START GAME");
	m_start_game.Text->SetAlignX(enCenter);
	m_start_game.x = xx;
	m_start_game.y = yy;
	m_start_game.Text->SetFiltering(mmtrue);
	m_start_game.s = s;

	m_options.Text = (clsText*)(app->m_ResMan->GetResource("font_01.mhmFont2D", MHMRES_MISC_ID));
	m_options.Text->SetText("OPTIONS");
	m_options.Text->SetAlignX(enCenter);
	m_options.x = xx;
	m_options.y = yy+dy;
	m_options.s = s;

	m_hiscores.Text = (clsText*)(app->m_ResMan->GetResource("font_01.mhmFont2D", MHMRES_MISC_ID));
	m_hiscores.Text->SetText("HISCORES");
	m_hiscores.Text->SetAlignX(enCenter);
	m_hiscores.x = xx;
	m_hiscores.y = yy+dy*2;
	m_hiscores.s = s;

	m_info.Text = (clsText*)(app->m_ResMan->GetResource("font_01.mhmFont2D", MHMRES_MISC_ID));
	m_info.Text->SetText("INFO");
	m_info.Text->SetAlignX(enCenter);
	m_info.x = xx;
	m_info.y = yy+dy*3;
	m_info.s = s;

	m_exit.Text = (clsText*)(app->m_ResMan->GetResource("font_01.mhmFont2D", MHMRES_MISC_ID));
	m_exit.Text->SetText("EXIT");
	m_exit.Text->SetAlignX(enCenter);
	m_exit.x = xx;
	m_exit.y = yy+dy*4;
	m_exit.s = s;

}
mmVoid cls_Main::UpdateObjects()
{

	m_start_game.Update(app);
	m_options.Update(app);
	m_hiscores.Update(app);
	m_info.Update(app);
	m_exit.Update(app);


	if(m_start_game.state == 2)
	{
		//app->m_Zast->PlayAnimationRangeStart("zast_close", enAnimHold);
		//app->m_Zast->m_curent = app->m_game;
		app->SetTextureZast("inter/fire_starter.mtj");
		app->m_level = SL;
		app->ZClose(app->m_game);
		app->m_scores = 0;
		app->m_count_start_jertv = 0;
		app->m_count_dead_jertv = 0;
		app->m_lives = LIVES;
		app->m_rand.init((mmUint)app->m_Timer->GetTicks());
		app->m_game->m_reload = mmtrue;

	//	app->PlayFX(app->m_s_dver);
	}

	if(m_exit.state == 2)
	{
		app->SetTextureZast("inter/exit.mtj");
		app->ZClose(app->m_exit);
	}

	if(m_options.state == 2)
	{
		app->SetTextureZast("inter/options.mtj");
		app->ZClose(app->m_options);
	}

	if(m_info.state == 2)
	{
		app->SetTextureZast("inter/info.mtj");
		app->ZClose(app->m_info);
	}

	if(m_hiscores.state == 2)
	{
		app->SetTextureZast("inter/hiscores.mtj");
		app->ZClose(app->m_hi_scores);
	}



}
mmVoid cls_Main::RenderObjects()
{
	app->m_Renderer->SetCamera(&app->m_CameraZ);

	if(app->m_Start)
	{
	app->m_Renderer->GetDevice()->DepthWrite(mmfalse);
	clsViewport* pViewPort = app->m_Renderer->GetDevice()->GetViewport();
	app->m_logo->SetPosition(clsVector3((mmFloat)(pViewPort->m_nWidth >> 1)-0.5f,(mmFloat)(pViewPort->m_nHeight >> 1)-0.5f,0.5f));
	app->m_logo->Render();
	app->m_Renderer->GetDevice()->DepthWrite(mmtrue);
	}
	else{

	app->m_pEngine->RenderScene(app->m_ZastPlan);
	
	app->m_Renderer->GetDevice()->DepthWrite(mmfalse);
	m_start_game.Text->Render();
	m_options.Text->Render();
	m_hiscores.Text->Render();
	m_info.Text->Render();
	m_exit.Text->Render();
	app->m_Renderer->GetDevice()->DepthWrite(mmtrue);

	}

	
}
mmVoid cls_Main::Init()
{

	app->m_alpha_pause = 0;

	app->m_return = this;
	app->m_pEngine->SetActiveScene(app->m_Zast);
	app->m_Zast->m_curent = mmnull;
	app->m_Zast->PlayAnimationRangeStart("zast_open", enAnimHold);

	app->m_mouse = mmtrue;



	app->m_Pause = mmfalse;
}
//*****************************************************************************************
//******************************************
cls_EndGame::cls_EndGame(clsFireApp * papp)
{
	mmFloat dy;
	dy = 0.05f;
	papal = mmfalse;

	app = papp;
/*
	mmFloat xx,yy,dy;
	xx = 0.5f;
	yy = 0.305f;
	dy = 0.085f;*/

	m_game_over.Text = (clsText*)(app->m_ResMan->GetResource("font_01.mhmFont2D", MHMRES_MISC_ID));
	m_game_over.Text->SetText("\brGAME OVER");
	m_game_over.Text->SetAlignX(enCenter);
	m_game_over.x = 0.5f;
	m_game_over.y = 0.305f +dy;
	m_game_over.Text->SetFiltering(mmtrue);
	m_game_over.enable = false;

	m_text_scores.Text = (clsText*)(app->m_ResMan->GetResource("font_01.mhmFont2D", MHMRES_MISC_ID));
	m_text_scores.Text->SetText("\byYOU SCORES:");
	m_text_scores.Text->SetAlignX(enCenter);
	m_text_scores.x = 0.5f;
	m_text_scores.y = 0.4f +dy;
	m_text_scores.enable = false;
	m_text_scores.s = 0.8f;

	m_scores.Text = (clsText*)(app->m_ResMan->GetResource("font_01.mhmFont2D", MHMRES_MISC_ID));
//	m_scores.Text->SetText("\by"+clsString().FromFormatNum(app->m_scores,""));
	m_scores.Text->SetAlignX(enCenter);
	m_scores.x = 0.5f;
	m_scores.y = 0.46f +dy;
	m_scores.enable = false;
	m_scores.s = 0.8f;

	m_ok.Text = (clsText*)(app->m_ResMan->GetResource("font_01.mhmFont2D", MHMRES_MISC_ID));
	m_ok.Text->SetText("OK");
	m_ok.Text->SetAlignX(enCenter);
	m_ok.x = 0.5f;
	m_ok.y = 0.64f +dy;


	m_name.Text = (clsText*)(app->m_ResMan->GetResource("font_01.mhmFont2D", MHMRES_MISC_ID));
	m_name.Text->SetText("\bbENTER YOU NAME");
	m_name.Text->SetAlignX(enCenter);
	m_name.x = 0.5f;
	m_name.y = 0.55f +dy;
	m_name.enable = false;
	m_name.s = 0.8f;

	name = "";

}
mmVoid cls_EndGame::UpdateObjects()
{
	clsString CH;

	m_game_over.Update(app);
	m_text_scores.Update(app);
	m_scores.Update(app);
	m_ok.Update(app);
	m_name.Update(app);

//	mmByte kod = app->m_pInput->GetKeyboard()->GetState()->GetKeysStatePointer()->m_code;

//	if(kod != 0)
//		name = (char)(app->m_pInput->GetKeyboard()->GetState()->GetKeysStatePointer()->m_code  );

	if( ((app->m_char_key_up >= '0')&&(app->m_char_key_up <= '9')) || ((app->m_char_key_up >= 'A')&&(app->m_char_key_up <= 'Z'))||(app->m_char_key_up == ' ') )
	{
		if(name.Length() < 9)
		{
		CH = app->m_char_key_up;
		app->m_char_key_up = 0;
		name += CH;
		}
	}
	if( (app->m_char_key_up == 8)&&(name.Length()!=0))
	{
		name = name.Left(name.Length()-1);
		app->m_char_key_up = 0;
		if(name.Length()==0)
			name = "";
	}

	if(name == "")
	{
		m_name.Text->SetText("\bbENTER YOU NAME");
	}else{
		m_name.Text->SetText("\bg"+name);
	}

	if(app->m_char_key_up == 13)
	{
		app->m_char_key_up = 0;
		m_ok.state = 2;
	}

	if(m_ok.state == 2)
	{
		/*
		static mmBool raz = mmtrue;
		if((papal && raz)&&(name != ""))
		{
			app->m_HS[app->m_HS.Size()-1].score = app->m_scores;
			//if(name == "")
			//	name = "FIRE MAN";
			app->m_HS[app->m_HS.Size()-1].name = name;
			app->m_HS.Sort();
			app->SaveHS();
			raz = mmfalse;
		}
		*/

		if((papal )&&(name != ""))
		{
			app->m_HS[app->m_HS.Size()-1].score = app->m_scores;
			//if(name == "")
			//	name = "FIRE MAN";
			app->m_HS[app->m_HS.Size()-1].name = name;
			app->m_HS.Sort();
			app->SaveHS();
			m_ok.enable = mmfalse;
		}



		if(((papal)&&(name != ""))||(!papal))
		{
		app->SetTextureZast("inter/hiscores.mtj");
		app->ZClose(app->m_hi_scores);
		}
	}

}
mmVoid cls_EndGame::RenderObjects()
{
	app->m_Renderer->SetCamera(&app->m_CameraZ);
	app->m_pEngine->RenderScene(app->m_ZastPlan);
	
	app->m_Renderer->GetDevice()->DepthWrite(mmfalse);
	m_game_over.Text->Render();
	m_text_scores.Text->Render();
	m_scores.Text->Render();

	if(((papal)&&(name != ""))||(!papal))
	{
		m_ok.Text->Render();
	}

	if(papal)
		m_name.Text->Render();
	app->m_Renderer->GetDevice()->DepthWrite(mmtrue);
}
mmVoid cls_EndGame::Init()
{
	app->m_return = app->m_main;

	app->m_pEngine->SetActiveScene(app->m_Zast);
	m_scores.Text->SetText("\by"+clsString().FromFormatNum(app->m_scores,""));
	app->m_Zast->m_curent = mmnull;
	app->m_Zast->PlayAnimationRangeStart("zast_open", enAnimHold);
	app->m_mouse = mmtrue;

	for(mmUint z = 0;z<app->m_HS.Size();z++)
	{
		if(app->m_scores > app->m_HS[z].score)
		{
			papal = mmtrue;
			break;
		}
	}
	m_ok.enable = mmtrue;



	app->m_pMouse->SetPositionCenter();
	clsPoint2D pm = app->m_pMouse->GetPosition();
	pm.y = (mmInt)((mmFloat)pm.y / 2.5f);
	app->m_pMouse->SetPosition(pm);


	//papal = mmtrue;
}
//*****************************************************************************************
//******************************************
cls_NextLevel::cls_NextLevel(clsFireApp * papp)
{
	app = papp;
/*
	mmFloat xx,yy,dy;
	xx = 0.5f;
	yy = 0.305f;
	dy = 0.085f;*/

	m_proc.Text = (clsText*)(app->m_ResMan->GetResource("font_01.mhmFont2D", MHMRES_MISC_ID));
//	m_proc.Text->SetText("\bySAVED LIVES: \ba100%");
	m_proc.Text->SetAlignX(enCenter);
	m_proc.x = 0.5f;
	m_proc.y = 0.355f;
	m_proc.Text->SetFiltering(mmtrue);
	m_proc.enable = false;
	m_proc.s = 0.6f;

	m_scores.Text = (clsText*)(app->m_ResMan->GetResource("font_01.mhmFont2D", MHMRES_MISC_ID));
//	m_scores.Text->SetText("\byYOU SCORES: \ba123456");
	m_scores.Text->SetAlignX(enCenter);
	m_scores.x = 0.5f;
	m_scores.y = 0.41f;
	m_scores.enable = false;
	m_scores.s = 0.6f;

	m_next.Text = (clsText*)(app->m_ResMan->GetResource("font_01.mhmFont2D", MHMRES_MISC_ID));
	m_next.Text->SetText("NEXT LEVEL");
	m_next.Text->SetAlignX(enCenter);
	m_next.x = 0.5f;
	m_next.y = 0.54f;
	m_next.s = 0.6f;

	m_replay.Text = (clsText*)(app->m_ResMan->GetResource("font_01.mhmFont2D", MHMRES_MISC_ID));
	m_replay.Text->SetText("REPLAY");
	m_replay.Text->SetAlignX(enCenter);
	m_replay.x = 0.5f;
	m_replay.y = 0.6f;
	m_replay.s = 0.6f;


	m_exit.Text = (clsText*)(app->m_ResMan->GetResource("font_01.mhmFont2D", MHMRES_MISC_ID));
	m_exit.Text->SetText("EXIT TO MAIN MENU");
	m_exit.Text->SetAlignX(enCenter);
	m_exit.x = 0.5f;
	m_exit.y = 0.66f;
	m_exit.s = 0.6f;

}
mmVoid cls_NextLevel::UpdateObjects()
{
	static mmBool chet_s = mmfalse;

	if( (app->m_Timer->GetMiSecs() - tim) > 10 )
	{
		tim = app->m_Timer->GetMiSecs();

		if(app->m_scores > m_score_out)
		{
			m_score_out = min(app->m_scores , m_score_out + max(1,app->m_scores / 150) );
			
			chet_s = !chet_s;

			if(chet_s)
				app->PlayFX(app->m_s_din);

		}else{
			m_score_out = app->m_scores;
		}


		if(proc > m_proc_out)
		{
			m_proc_out = min(proc , m_proc_out + max(1,proc / 100) );

			

		}else{
			m_proc_out = proc;
		}
	}

	


	m_proc.Text->SetText("\bySAVED LIVES: \ba"+clsString().FromFormatNum(m_proc_out,"")+"%");
	m_scores.Text->SetText("\byYOU SCORES: \ba"+clsString().FromFormatNum(m_score_out,""));


	m_proc.Update(app);
	m_scores.Update(app);
	m_next.Update(app);
	m_replay.Update(app);
	m_exit.Update(app);



	if(m_replay.state == 2)
	{
		//app->m_Zast->PlayAnimationRangeStart("zast_close", enAnimHold);
		//app->m_Zast->m_curent = app->m_game;
		app->SetTextureZast("inter/fire_starter.mtj");
		app->ZClose(app->m_game);
		app->m_game->m_reload = mmtrue;
	//	app->m_scores = 0;
		app->m_count_start_jertv = 0;
		app->m_count_dead_jertv = 0;
	}

	if((m_next.state == 2)&&(proc >= 50))
	{
		app->m_level ++;
		app->SetTextureZast("inter/fire_starter.mtj");
		app->ZClose(app->m_game);
	//	app->m_scores = 0;
		app->m_count_start_jertv = 0;
		app->m_count_dead_jertv = 0;
	}

	if(m_exit.state == 2)
	{
		app->SetTextureZast("inter/main_menu.mtj");
		app->ZClose(app->m_main);
	}



}
mmVoid cls_NextLevel::RenderObjects()
{
	app->m_Renderer->SetCamera(&app->m_CameraZ);
	app->m_pEngine->RenderScene(app->m_ZastPlan);
	
	app->m_Renderer->GetDevice()->DepthWrite(mmfalse);
	m_proc.Text->Render();
	m_scores.Text->Render();
	if(proc >= 50)
		m_next.Text->Render();
	m_replay.Text->Render();
	m_exit.Text->Render();
	app->m_Renderer->GetDevice()->DepthWrite(mmtrue);
}
mmVoid cls_NextLevel::Init()
{
	app->m_return = app->m_main;

	tim = app->m_Timer->GetMiSecs();
	m_score_out = 0;
	m_proc_out = 0;

	app->m_pEngine->SetActiveScene(app->m_Zast);
	
	proc = (mmInt)(100.0f - (mmFloat)app->m_count_dead_jertv / ((mmFloat)app->m_count_start_jertv * 0.01f));

	//app->m_scores += proc*10; //pppppppp

	m_proc.Text->SetText("\bySAVED LIVES: \ba"+clsString().FromFormatNum(proc,"")+"%");
	m_scores.Text->SetText("\byYOU SCORES: \ba"+clsString().FromFormatNum(app->m_scores,""));


	app->m_Zast->m_curent = mmnull;
	app->m_Zast->PlayAnimationRangeStart("zast_open", enAnimHold);
	app->m_mouse = mmtrue;

	app->m_pMouse->SetPositionCenter();
	clsPoint2D pm = app->m_pMouse->GetPosition();
	pm.y = (mmInt)((mmFloat)pm.y / 2.5f);
	app->m_pMouse->SetPosition(pm);
}
//*****************************************************************************************
//******************************************
cls_Exit::cls_Exit(clsFireApp * papp)
{
	app = papp;


	m_exit_game.Text = (clsText*)(app->m_ResMan->GetResource("font_01.mhmFont2D", MHMRES_MISC_ID));
	m_exit_game.Text->SetText("EXIT GAME ?");
	m_exit_game.Text->SetAlignX(enCenter);
	m_exit_game.enable = mmfalse;
	m_exit_game.x = 0.5f;
	m_exit_game.y = 0.4f;
	m_exit_game.Color = 0xFFFFFFA0;
	m_exit_game.Text->SetFiltering(mmtrue);

	m_yes.Text = (clsText*)(app->m_ResMan->GetResource("font_01.mhmFont2D", MHMRES_MISC_ID));
	m_yes.Text->SetText("YES");
	m_yes.Text->SetAlignX(enLeft);
	m_yes.x = 0.3f;
	m_yes.y = 0.5f;
	m_yes.s = 1.2f;

	m_no.Text = (clsText*)(app->m_ResMan->GetResource("font_01.mhmFont2D", MHMRES_MISC_ID));
	m_no.Text->SetText("NO");
	m_no.Text->SetAlignX(enRight);
	m_no.x = 0.7f;
	m_no.y = 0.5f;
	m_no.s = 1.2f;


}
mmVoid cls_Exit::UpdateObjects()
{

	m_exit_game.Update(app);
	m_yes.Update(app);
	m_no.Update(app);

	if(m_no.state == 2)
	{
		//app->m_Zast->PlayAnimationRangeStart("zast_close", enAnimHold);
		//app->m_Zast->m_curent = app->m_main;
		app->SetTextureZast("inter/main_menu.mtj");
		//app->ZClose(app->m_main);
		app->ZClose(app->m_return);
	}
	if(m_yes.state == 2)
		app->Quit();
}
mmVoid cls_Exit::RenderObjects()
{
	app->m_Renderer->SetCamera(&app->m_CameraZ);
	app->m_pEngine->RenderScene(app->m_ZastPlan);

	app->m_Renderer->GetDevice()->DepthWrite(mmfalse);
	m_exit_game.Text->Render();
	m_yes.Text->Render();
	m_no.Text->Render();
	app->m_Renderer->GetDevice()->DepthWrite(mmtrue);
}
mmVoid cls_Exit::Init()
{
	app->m_Zast->m_curent = mmnull;
	app->m_Zast->PlayAnimationRangeStart("zast_open", enAnimHold);		
}
//*****************************************************************************************
//******************************************
cls_Info::cls_Info(clsFireApp * papp)
{
	app = papp;


	m_text.Text = (clsText*)(app->m_ResMan->GetResource("font_02.mhmFont2D", MHMRES_MISC_ID));
	m_text.Text->SetText("\byMAYHEM PROJEX GAME\n\brPINK FIRE\n\by(C) 2006-2007\n\bfBETA VERSION 0.6\n\bg*****************");
	//m_text.Text->SetText("\byMEGA MAYHEM GAME\n\n\brPINK FIRE\n\n\by(C) 2006-2007\n\n\bg*****************");
	m_text.Text->SetAlignX(enCenter);
	m_text.enable = mmfalse;
	m_text.x = 0.5f;
	m_text.y = 0.35f;
	m_text.s = 0.6f;
	m_text.Color = 0xFFFFFFA0;
	m_text.Text->SetFiltering(mmtrue);

	m_return.Text = (clsText*)(app->m_ResMan->GetResource("font_01.mhmFont2D", MHMRES_MISC_ID));
	m_return.Text->SetText("RETURN");
	m_return.Text->SetAlignX(enCenter);
	m_return.x = 0.5f;
	m_return.y = 0.66f;
	m_return.s = 0.7f;

}
mmVoid cls_Info::UpdateObjects()
{

	m_text.Update(app);
	m_return.Update(app);

	if(m_return.state == 2)
	{
		//app->m_Zast->PlayAnimationRangeStart("zast_close", enAnimHold);
		//app->m_Zast->m_curent = app->m_main;
		app->SetTextureZast("inter/main_menu.mtj");
		//app->ZClose(app->m_main);
		app->ZClose(app->m_return);
	}

}
mmVoid cls_Info::RenderObjects()
{
	app->m_Renderer->SetCamera(&app->m_CameraZ);
	app->m_pEngine->RenderScene(app->m_ZastPlan);

	app->m_Renderer->GetDevice()->DepthWrite(mmfalse);
	m_text.Text->Render();
	m_return.Text->Render();
	app->m_Renderer->GetDevice()->DepthWrite(mmtrue);
}
mmVoid cls_Info::Init()
{
	app->m_Zast->m_curent = mmnull;
	app->m_Zast->PlayAnimationRangeStart("zast_open", enAnimHold);		
}
//*****************************************************************************************
//******************************************
cls_Game::cls_Game(clsFireApp * papp)
{
	m_reload = mmfalse;
	LastLevel = 0;
	app = papp;


	m_text_up.Text = (clsText*)(app->m_ResMan->GetResource("font_01.mhmFont2D", MHMRES_MISC_ID));
	m_text_up.Text->SetText("SCORES ");
	m_text_up.Text->SetAlignX(enLeft);
	m_text_up.enable = mmfalse;
	m_text_up.x = 0.01f;
	m_text_up.y = 0.01f;
	m_text_up.s = 0.6f;
	m_text_up.Color = 0xFFFFFF00;
	m_text_up.Text->SetFiltering(mmtrue);
	

	/*
	m_text_down.Text = (clsText*)(app->m_ResMan->GetResource("font_01.mhmFont2D", MHMRES_MISC_ID));
	m_text_down.Text->SetText("MAN");
	m_text_down.Text->SetAlignX(enLeft);
	m_text_down.enable = mmfalse;
	m_text_down.Text->SetFiltering(mmtrue);
	m_text_down.x = 0.1f;
	m_text_down.y = 0.95f;
	m_text_down.s = 0.6f;
	*/

mmFloat x = 0.055f;

	m_scor.Text = (clsText*)(app->m_ResMan->GetResource("font_01.mhmFont2D", MHMRES_MISC_ID));
	m_scor.Text->SetText("0");
	m_scor.Text->SetAlignX(enLeft);
	m_scor.enable = mmfalse;
	m_scor.Text->SetFiltering(mmtrue);
	m_scor.x = 0.076f +x;
	m_scor.y = 0.945f;
	m_scor.s = 0.6f;

	m_liv.Text = (clsText*)(app->m_ResMan->GetResource("font_01.mhmFont2D", MHMRES_MISC_ID));
	m_liv.Text->SetText("0");
	m_liv.Text->SetAlignX(enLeft);
	m_liv.enable = mmfalse;
	m_liv.Text->SetFiltering(mmtrue);
	m_liv.x = 0.3f +x;
	m_liv.y = 0.945f;
	m_liv.s = 0.6f;

	m_jert.Text = (clsText*)(app->m_ResMan->GetResource("font_01.mhmFont2D", MHMRES_MISC_ID));
	m_jert.Text->SetText("0");
	m_jert.Text->SetAlignX(enLeft);
	m_jert.enable = mmfalse;
	m_jert.Text->SetFiltering(mmtrue);
	m_jert.x = 0.53f +x;
	m_jert.y = 0.945f;
	m_jert.s = 0.6f;

	m_fir.Text = (clsText*)(app->m_ResMan->GetResource("font_01.mhmFont2D", MHMRES_MISC_ID));
	m_fir.Text->SetText("0");
	m_fir.Text->SetAlignX(enLeft);
	m_fir.enable = mmfalse;
	m_fir.Text->SetFiltering(mmtrue);
	m_fir.x = 0.754f +x;
	m_fir.y = 0.945f;
	m_fir.s = 0.6f;






	m_text_state.Text = (clsText*)(app->m_ResMan->GetResource("states.mhmFont2D", MHMRES_MISC_ID));
	m_text_state.Text->SetText("MAN");
	m_text_state.Text->SetAlignX(enLeft);
	m_text_state.enable = mmfalse;
	m_text_state.x = 0.01f +x;
	m_text_state.y = 0.91f;
	m_text_state.s = 0.6f;
	m_text_state.Text->SetFiltering(mmtrue);

	
}
mmVoid cls_Game::UpdateObjects()
{
#ifdef NDEBUG
	if(app->m_char_key_up == 27)
#else
	if(app->m_char_key_up == 'P')
#endif
	{
		if(!app->m_Pause)
		{
			app->m_Pause = mmfalse;
			if( app->m_Zast->m_bRenderable == mmfalse)
			{

				app->SetTextureZast("inter/pause.mtj");
				app->ZClose(app->m_pause);
				app->m_char_key_up = 0;
				app->m_Pause = mmtrue;

				app->m_pMouse->SetPositionCenter();
				clsPoint2D pm = app->m_pMouse->GetPosition();
				pm.y = (mmInt)((mmFloat)pm.y / 2.5f);
				app->m_pMouse->SetPosition(pm);

				return;
			}
			else{
				app->m_char_key_up = 0;
			}
		}
	}

	if(app->m_Pause)
	{
		m_text_up.Update(app);

		m_scor.Update(app);
		m_liv.Update(app);
		m_jert.Update(app);
		m_fir.Update(app);
		m_text_state.Update(app);
		return;
	}

	if((app->m_count == 0)||(app->m_sizeF==0))
	{
		app->m_win = mmtrue;
	}else{
		app->m_win = mmfalse;
	}

	static mmInt score_out = 0;
	if(app->m_scores > score_out)
	{
		score_out = min(app->m_scores , score_out + 1);

	}else{
		score_out = app->m_scores;
	}

	m_scor.Text->SetText(clsString().FromFormatNum(score_out,""));
	m_liv.Text->SetText(clsString().FromFormatNum(app->m_lives,"03"));
	m_jert.Text->SetText(clsString().FromFormatNum(app->m_count,"03"));
	m_fir.Text->SetText(clsString().FromFormatNum(app->m_sizeF,"05.2"));

	/*
	m_text_down.Text->SetText(
		clsString().FromFormatNum(app->m_scores,"06") + "    " +
		clsString().FromFormatNum(app->m_lives,"03")	+ "    " +
		clsString().FromFormatNum(app->m_count,"03")	+ "    " +
		clsString().FromFormatNum(app->m_sizeF,"05.2")
		);
	*/

/*
	m_text_down.Text->SetText(
		
		"\brFIRE MAN: \bw" + clsString().FromFormatNum(app->m_lives,"") + 
		"\by ... \brMEN IN FIRE: \bw"+clsString().FromFormatNum(app->m_count,"") +
		"\by ... \brFIRE FACTOR: \bw"+clsString().FromFormatNum(app->m_sizeF,"05.2")	);
		*/

	m_text_up.Text->SetText("PINK_FIRE_V0.6      \bwLEVEL: "+clsString().FromFormatNum((mmInt)app->m_level,"")+"     FPS: "+clsString().FromFormatNum(((mmInt)app->m_Timer->GetFPS()),"03"))	;

	m_text_state.Text->SetText("D   A   B   C");

	


	m_text_up.Update(app);

	m_scor.Update(app);
	m_liv.Update(app);
	m_jert.Update(app);
	m_fir.Update(app);
	m_text_state.Update(app);

	//m_text_state.Text->UpdateEffect(0.01f);
	m_scor.Text->UpdateEffect(app->m_Timer->GetDeltaMiSecs() * 0.001f);
	m_liv.Text->UpdateEffect(app->m_Timer->GetDeltaMiSecs() * 0.001f);
	m_jert.Text->UpdateEffect(app->m_Timer->GetDeltaMiSecs() * 0.001f);
	m_fir.Text->UpdateEffect(app->m_Timer->GetDeltaMiSecs() * 0.001f);


	app->m_Scene->AnimationRangeBuild(app->m_Timer->GetDeltaMiSecs());
	app->m_Scene->Animate();


	app->m_pDoska->Move((mmFloat)app->m_pMouse->SnapshotAxisDelta(enMouseX));
	app->m_pMan->Move(app->m_Timer->GetDeltaMiSecs()* 0.12f);


	
	


}
mmVoid cls_Game::RenderObjects()
{
	

	app->SetLight();
	app->SetCam();
	app->m_Renderer->SetCamera(&app->m_Camera);
	app->m_pEngine->RenderScene(app->m_Scene);

	mmInt addAL;
	addAL = (mmInt)(app->m_Timer->GetDeltaMiSecs() * 0.5f); 

	if(app->m_Pause)
	{
		app->m_alpha_pause = min(app->m_alpha_pause + addAL , 0x80);
	}else{
		app->m_alpha_pause = max(app->m_alpha_pause - addAL , 0);
	}

app->m_Renderer->GetDevice()->DepthWrite(mmfalse);

	if(app->m_alpha_pause != 0)
	{
		clsViewport* pViewPort = app->m_Renderer->GetDevice()->GetViewport();
		app->m_logo->SetPosition(clsVector3((mmFloat)(pViewPort->m_nWidth >> 1)-0.5f,(mmFloat)(pViewPort->m_nHeight >> 1)-0.5f,0.5f));

		mmDword col = app->m_alpha_pause;
		col = (col<<24)|0x00FFFFFF;
		app->m_logo->SetColor(col);
		app->m_logo->SetBlendType(enBlendAlpha);
		app->m_logo->ReBuild(mmtrue);
		app->m_logo->Render();
	}



//	app->m_Renderer->GetDevice()->DepthWrite(mmfalse);
	m_text_up.Text->Render();
//	m_text_down.Text->Render();

	m_scor.Text->Render();
	m_liv.Text->Render();
	m_jert.Text->Render();
	m_fir.Text->Render();

	m_text_state.Text->Render();
	app->m_Renderer->GetDevice()->DepthWrite(mmtrue);
}
mmVoid cls_Game::Init()
{


	app->m_char_key_up = 0;

	app->m_pEngine->SetActiveScene(app->m_Scene);
	app->m_mouse = mmfalse;
	

	if(app->m_NoInit == mmtrue)
	{
		app->m_NoInit = mmfalse;
		app->m_Zast->m_curent = mmnull;
		app->m_Zast->PlayAnimationRangeStart("zast_open", enAnimHold);	
		m_reload = mmfalse;
		return;
	}


	if((app->m_level != LastLevel)||(m_reload == mmtrue))
	{
		app->LoadScene(app->m_level);
		LastLevel = app->m_level;
	}else{
		app->InitScene();
	}
	
	m_reload = mmfalse;

	if(app->m_SceneTMP != mmnull)
		app->m_SceneTMP->m_bKillMe = mmtrue;
	app->m_SceneTMP = mmnull;

	app->m_pAAng->m_mxLocal._43 = 50;

	app->m_pAAng->EnableRendering(mmfalse,mmtrue);

	/*
	state = enManVybygaet;
	app->m_pMan->m_next_state = enManVybygaet;
	app->m_pMan->PlayAnimationRangeStart("start_run_1", enAnimHold);
	*/

	app->m_pDoska->m_state = enBatutVybygaet;
	app->m_pDoska->m_next_state = enManVybygaet;
	app->m_pDoska->m_scene_state = enManVybygaet;
	app->m_pDoska->PlayAnimationRangeStart("start_run", enAnimHold);
	
	

	app->m_Zast->m_curent = mmnull;
	app->m_Zast->PlayAnimationRangeStart("zast_open", enAnimHold);	

	

}


//**********************
clsTextButton::clsTextButton()
{ // "font_01.mhmFont2D"
	//m_Text = (clsText*)m_ResMan->GetResource(font_name, MHMRES_MISC_ID);

	Color = 0xFFFFFFFF;
	ColorOver = 0xFFFFFF00;
	ColorDown = 0xFFFF0000;

	state = 0;

	procent = mmtrue;

	enable = mmtrue;

	s = 1.0f;
}
//-----------------------------------
mmVoid clsTextButton::Update(clsFireApp * app)
{
	clsViewport* pViewPort = app->m_Renderer->GetDevice()->GetViewport();
	if(procent)
	{
		Text->SetX( (mmFloat)pViewPort->m_nWidth * x );
		Text->SetY( (mmFloat)pViewPort->m_nHeight * y );

	}else{
		Text->SetX( x );
		Text->SetY( y );
	}

	Text->SetSize( (mmFloat)pViewPort->m_nWidth * s * 0.52f);

	Text->SetColor(Color);

	if(app->m_Zast->m_AnimationRageController)
		if(app->m_Zast->m_AnimationRageController->IsPlaying)
				if(state > 1)
				{
					state = 3;
					return;
				}



	if(  (app->m_mous_x > Text->TextRect.left)&&
		(app->m_mous_x < Text->TextRect.right)&&
		(app->m_mous_y > Text->TextRect.top)&&
		(app->m_mous_y < Text->TextRect.bottom) )
	{
		state = 1;

		//if(app->m_pMouse->SnapshotButton(enMouseButton0) == enMouseButtonDown)
		if(app->m_pMouse->IsButtonReleased(enMouseButton0) == mmtrue)
		{
			state = 2;
			app->PlayFX(app->m_s_key);
		}

	}else{
		state = 0;
	}

	if(!enable)
	{
		state = 0;
	}


	
	if(state == 1)
		Text->SetColor(ColorOver);
	if(state == 2)
		Text->SetColor(ColorDown);

	
}
mmVoid clsFireApp::ZOpen(cls_Scene * next)
{
	m_Zast->PlayAnimationRangeStart("zast_open", enAnimHold);
	m_Zast->m_curent = next;

	//PlayFX(m_s_dver);
}
mmVoid clsFireApp::ZClose(cls_Scene * next)
{
	m_Zast->m_bRenderable = mmtrue;
	m_Zast->PlayAnimationRangeStart("zast_close", enAnimHold);
	m_Zast->m_curent = next;

	//PlayFX(m_s_dver);
}
//----------------------------------------------------
mmVoid clsFireApp::SaveHS()
{
	clsString str;
	str = "_PinkFire_HiScores_\n";
	for(mmUint z=0; z<m_HS.Size() ; z++)
	{
		str += "name"+clsString(z)+" = \"" + m_HS[z].name+"\"\n";
		str += "scores"+clsString(z)+" = " + clsString(m_HS[z].score)+"\n";
	}

	mmByte * dat = new mmByte[str.Length()+1];
	memcpy(dat,str.Data(),str.Length()+1);
	for(mmUint z = 0; z<str.Length()+1; z++)
	{
		dat[z] ^= 0xAA;
	}
	
	clsFile f("hiscores.dat");
	f.Create();
	f.Open();
	f.Write(dat,str.Length()+1);
	f.Close();
	delete [] dat;

}
//----------------------------------------------------
mmVoid clsFireApp::LoadHS()
{
	m_HS.Size(10);
	m_HS[0].name = "PINK";
	m_HS[0].score = 9000;
	m_HS[1].name = "MOZG";
	m_HS[1].score = 8000;
	m_HS[2].name = "FYREX";
	m_HS[2].score = 7000;
	m_HS[3].name = "KAEL";
	m_HS[3].score = 6000;
	m_HS[4].name = "PUSHKIN";
	m_HS[4].score = 1000;
	m_HS[5].name = "SAND";
	m_HS[5].score = 5000;
	m_HS[6].name = "GAMER";
	m_HS[6].score = 50;
	m_HS[7].name = "SHTIRLIC";
	m_HS[7].score = 300;
	m_HS[8].name = "ZAPERHAL";
	m_HS[8].score = 200;
	m_HS[9].name = "FISKULTURNIK";
	m_HS[9].score = 100;
	m_HS.Sort();


	clsFile f("hiscores.dat");
	if(f.Open() != mmOk)
		return;

	mmUint siz = f.GetSize();
	mmByte * dat;
	mmVoid * dat1;
	f.ReadToMemory( &dat1 );
	f.Close();
	dat = (mmByte*)dat1;
	for(mmUint z = 0; z<siz; z++)
	{
		dat[z] ^= 0xAA;
	}
	clsString str;
	str = (mmString)dat;
	delete [] dat;

	if(str.Left(20) != "_PinkFire_HiScores_\n")
		return;

	clsVarParser parser;
	if (MHMFAIL(parser.ParseMem(str.Data(),str.Length())))
		return;
	clsVarList* plist = parser.GetVarList();
	clsVariable* var;
	clsString nam;
	for(mmUint z = 0; z<m_HS.Size(); z++)
	{
		var = plist->FindVariable("name"+clsString(z));
		if (var != mmnull)
		{
			nam = (mmString)*var;
			var = plist->FindVariable("scores"+clsString(z));
			if (var != mmnull)
			{
				m_HS[z].name = nam;
				m_HS[z].score = *var;
			}
		}
	}
	m_HS.Sort();
}















//*****************************************************************************************
//******************************************
cls_Options::cls_Options(clsFireApp * papp)
{
	app = papp;


	m_text.Text = (clsText*)(app->m_ResMan->GetResource("font_01.mhmFont2D", MHMRES_MISC_ID));
	m_text.Text->SetText("FULL SCREEN OFF");
	m_text.Text->SetAlignX(enCenter);
//	m_text.enable = mmfalse;
	m_text.x = 0.5f;
	m_text.y = 0.31f;
	m_text.s = 0.55f;
//	m_text.Color = 0xFFFFFFA0;
	m_text.Text->SetFiltering(mmtrue);

	m_text1.Text = (clsText*)(app->m_ResMan->GetResource("font_01.mhmFont2D", MHMRES_MISC_ID));
	m_text1.Text->SetText("RESOLUTIONS\n\n\n\nSOUND VOLUME\n\n\n\nFX VOLUME");
	m_text1.Text->SetAlignX(enCenter);
	m_text1.enable = mmfalse;
	m_text1.x = 0.5f;
	m_text1.y = 0.37f;
	m_text1.s = 0.4f;
	m_text1.Color = 0xFFE0FFA0;
	m_text1.Text->SetFiltering(mmtrue);

	m_r_add.Text = (clsText*)(app->m_ResMan->GetResource("font_01.mhmFont2D", MHMRES_MISC_ID));
	m_r_add.Text->SetText(">>");
	m_r_add.Text->SetAlignX(enCenter);
	m_r_add.x = 0.615f;
	m_r_add.y = 0.395f;
	m_r_add.s = 1.0f;

	m_r_sub.Text = (clsText*)(app->m_ResMan->GetResource("font_01.mhmFont2D", MHMRES_MISC_ID));
	m_r_sub.Text->SetText("<<");
	m_r_sub.Text->SetAlignX(enCenter);
	m_r_sub.x = 0.385f;
	m_r_sub.y = 0.395f;
	m_r_sub.s = 1.0f;

	m_resolut.Text = (clsText*)(app->m_ResMan->GetResource("font_01.mhmFont2D", MHMRES_MISC_ID));
	m_resolut.Text->SetText("640 X 480");
	m_resolut.Text->SetAlignX(enCenter);
	m_resolut.x = 0.5f;
	m_resolut.y = 0.41f;
	m_resolut.s = 0.55f;
	m_resolut.enable = mmfalse;



	m_v_add.Text = (clsText*)(app->m_ResMan->GetResource("font_01.mhmFont2D", MHMRES_MISC_ID));
	m_v_add.Text->SetText(">>");
	m_v_add.Text->SetAlignX(enCenter);
	m_v_add.x = 0.615f;
	m_v_add.y = 0.497f;
	m_v_add.s = 1.0f;

	m_v_sub.Text = (clsText*)(app->m_ResMan->GetResource("font_01.mhmFont2D", MHMRES_MISC_ID));
	m_v_sub.Text->SetText("<<");
	m_v_sub.Text->SetAlignX(enCenter);
	m_v_sub.x = 0.385f;
	m_v_sub.y = 0.497f;
	m_v_sub.s = 1.0f;

	m_vol.Text = (clsText*)(app->m_ResMan->GetResource("font_01.mhmFont2D", MHMRES_MISC_ID));
	m_vol.Text->SetText("||||||||||||||||||||||||||||||||||||||");
	m_vol.Text->SetAlignX(enCenter);
	m_vol.x = 0.5f;
	m_vol.y = 0.51f;
	m_vol.s = 0.55f;
	m_vol.enable = mmfalse;



	m_f_add.Text = (clsText*)(app->m_ResMan->GetResource("font_01.mhmFont2D", MHMRES_MISC_ID));
	m_f_add.Text->SetText(">>");
	m_f_add.Text->SetAlignX(enCenter);
	m_f_add.x = 0.615f;
	m_f_add.y = 0.597f;
	m_f_add.s = 1.0f;

	m_f_sub.Text = (clsText*)(app->m_ResMan->GetResource("font_01.mhmFont2D", MHMRES_MISC_ID));
	m_f_sub.Text->SetText("<<");
	m_f_sub.Text->SetAlignX(enCenter);
	m_f_sub.x = 0.385f;
	m_f_sub.y = 0.597f;
	m_f_sub.s = 1.0f;

	m_fx.Text = (clsText*)(app->m_ResMan->GetResource("font_01.mhmFont2D", MHMRES_MISC_ID));
	m_fx.Text->SetText("||||||||||||||||||||||||||||||||||||||");
	m_fx.Text->SetAlignX(enCenter);
	m_fx.x = 0.5f;
	m_fx.y = 0.61f;
	m_fx.s = 0.55f;
	m_fx.enable = mmfalse;




	m_exit.Text = (clsText*)(app->m_ResMan->GetResource("font_01.mhmFont2D", MHMRES_MISC_ID));
	m_exit.Text->SetText("RETURN");
	m_exit.Text->SetAlignX(enCenter);
	m_exit.x = 0.5f;
	m_exit.y = 0.76f;
	m_exit.s = 0.5f;

	m_aply.Text = (clsText*)(app->m_ResMan->GetResource("font_01.mhmFont2D", MHMRES_MISC_ID));
	m_aply.Text->SetText("APPLY");
	m_aply.Text->SetAlignX(enCenter);
	m_aply.x = 0.5f;
	m_aply.y = 0.7f;
	m_aply.s = 0.5f;

}
mmVoid cls_Options::UpdateObjects()
{

	m_text.Update(app);
	m_r_add.Update(app);
	m_r_sub.Update(app);
	m_resolut.Update(app);
	m_exit.Update(app);
	m_aply.Update(app);
	m_text1.Update(app);
	m_vol.Update(app);
	m_v_add.Update(app);
	m_v_sub.Update(app);
	m_fx.Update(app);
	m_f_add.Update(app);
	m_f_sub.Update(app);

	if(FS)
		m_text.Text->SetText("FULL SCREEN ON");
	else
		m_text.Text->SetText("FULL SCREEN OFF");

	if(m_exit.state == 2)
	{
		app->SetTextureZast("inter/main_menu.mtj");
		//app->ZClose(app->m_main);
		app->ZClose(app->m_return);

		app->m_Renderer->GetDevice()->m_pconfig->m_SoundVolume = (((mmFloat)app->m_FxVoleme)/64.0f);
		app->m_Renderer->GetDevice()->m_pconfig->m_StreamVolume =(((mmFloat)app->m_SoundVoleme)/64.0f);
	}
	mmInt add_res = 0;

	if(m_r_add.state == 2)
	{
		add_res = 1;
	}
	if(m_r_sub.state == 2)
	{
		add_res = -1;
	}

	if(m_text.state == 2)
	{
		FS = !FS;
	}

	if(add_res != 0)
	{
m1:;
		resol += add_res;

		if(resol >= (mmInt)(*vm).Count() )
		{
			resol = 0;
		}
		if(resol < 0)
		{
			resol = (mmInt)(*vm).Count()-1;
		}

		mmFloat w,h;
		w = (mmFloat)(*vm)[resol]->m_width;
		h = (mmFloat)(*vm)[resol]->m_height;
		if( ((h/w) != (3.0f/4.0f)) )
		{
			goto m1;
		}
		if( (w > 1024.0f) )
		{
			goto m1;
		}

		m_resolut.Text->SetText(clsString((mmInt)w)+
			" X "+
			clsString((mmInt)h)
			);

	}

	if(m_aply.state == 2)
	{
		if(	FS == app->m_Renderer->GetDevice()->m_pconfig->m_windowed)
			app->m_Renderer->GetDevice()->m_pconfig->m_windowed = !FS;

		//app->ZClose(app->m_main);
		app->m_Curent_Resolutions = resol;
		app->m_Renderer->GetDevice()->Reset(
			(*vm)[resol]->m_width ,
			(*vm)[resol]->m_height
			);
		app->m_pMouse->SetPositionCenter();

		InitOp();


		clsViewport* pViewPort = app->m_Renderer->GetDevice()->GetViewport();
		app->m_logo->SetWidthHeight(pViewPort->m_nWidth,pViewPort->m_nHeight);
		app->m_logo->ReBuild(mmtrue);
	}


	if(m_v_add.state == 2)
	{
		app->m_SoundVoleme = min(app->m_SoundVoleme + 2,64);
		InitVol();
		//app->m_s->Volume((mmDword)app->m_SoundVoleme);
	}
	if(m_v_sub.state == 2)
	{
		app->m_SoundVoleme = max(app->m_SoundVoleme - 2,0);
		InitVol();
		//app->m_s->Volume((mmDword)app->m_SoundVoleme);
	}
	if(m_f_add.state == 2)
	{
		app->m_FxVoleme = min(app->m_FxVoleme + 2,64);
		InitVol();
	}
	if(m_f_sub.state == 2)
	{
		app->m_FxVoleme = max(app->m_FxVoleme - 2,0);
		InitVol();
	}

}
mmVoid cls_Options::RenderObjects()
{
	app->m_Renderer->SetCamera(&app->m_CameraZ);
	app->m_pEngine->RenderScene(app->m_ZastPlan);

	app->m_Renderer->GetDevice()->DepthWrite(mmfalse);
	m_text.Text->Render();
	m_r_add.Text->Render();
	m_r_sub.Text->Render();
	m_resolut.Text->Render();
	m_exit.Text->Render();
	m_aply.Text->Render();
	m_text1.Text->Render();
	m_vol.Text->Render();
	m_v_add.Text->Render();
	m_v_sub.Text->Render();
	m_fx.Text->Render();
	m_f_add.Text->Render();
	m_f_sub.Text->Render();

	app->m_Renderer->GetDevice()->DepthWrite(mmtrue);
}
mmVoid cls_Options::Init()
{
	app->m_Zast->m_curent = mmnull;
	app->m_Zast->PlayAnimationRangeStart("zast_open", enAnimHold);

	InitOp();
}
mmVoid cls_Options::InitOp()
{
	if(
		(app->m_Renderer->GetDevice()->m_pconfig->m_bbfmt == D3DFMT_R5G6B5)||
		(app->m_Renderer->GetDevice()->m_pconfig->m_bbfmt == D3DFMT_X1R5G5B5)||
		(app->m_Renderer->GetDevice()->m_pconfig->m_bbfmt == D3DFMT_A1R5G5B5)||
		(app->m_Renderer->GetDevice()->m_pconfig->m_bbfmt == D3DFMT_A1R5G5B5)
		)
	{
	vm = &app->m_Renderer->GetDevice()->m_modes16;
	}else{
	vm = &app->m_Renderer->GetDevice()->m_modes32;
	}

	FS = !app->m_Renderer->GetDevice()->m_pconfig->m_windowed;

	for(mmUint z=0;z< (*vm).Count();z++)
	{
		if(
		(app->m_Renderer->GetDevice()->GetWidth() == (*vm)[z]->m_width)
			&&
			(app->m_Renderer->GetDevice()->GetHeight() == (*vm)[z]->m_height)
			)
		{
			app->m_Curent_Resolutions = z;
			break;
		}
	}

	resol = app->m_Curent_Resolutions;

	m_resolut.Text->SetText(clsString((mmInt)(*vm)[resol]->m_width)+
			" X "+
			clsString((mmInt)(*vm)[resol]->m_height)
			);

	InitVol();
}
mmVoid cls_Options::InitVol()
{
	clsString prog;
	mmInt si;
	si = (mmInt)(((mmFloat)app->m_SoundVoleme) * 38.0f / 64.0f);
	prog = "";
	for(mmInt z=0; z<si;z++)
		prog +="|";
	if(si == 0)
		prog = "OFF";
	m_vol.Text->SetText(prog);//38
	si = (mmInt)(((mmFloat)app->m_FxVoleme) * 38.0f / 64.0f);
	prog = "";
	for(mmInt z=0; z<si;z++)
		prog +="|";
	if(si == 0)
		prog = "OFF";
	m_fx.Text->SetText(prog);//38
}
//*****************************************************************************************
mmVoid clsFireApp::PlaySound(clsSound* s)
{
//	m_SoundVoleme =0;
	s->Play(0, (mmFloat)m_SoundVoleme / 65.0f);
}
mmVoid clsFireApp::PlayFX(clsSound* s)
{
	static mmDword id = 5;
	id = max(id,5);
//	m_FxVoleme = 0;
//	s->Play(10, ((mmFloat)m_FxVoleme / 65.0f)   );
//	s->Play((mmDword)s, ((mmFloat)m_FxVoleme / 65.0f)   );
	s->Play(id++, ((mmFloat)m_FxVoleme / 65.0f)   );
	
}


mmVoid clsFireApp::PlayMuza()
{
	static mmFloat sm = 1.0f;
	static mmFloat sg = 0.0f;
	static mmInt s_n = 0;
	static mmInt num_sg = 0;

	mmFloat add;
	add = min(m_Timer->GetDeltaMiSecs() * 0.0004f , 0.01f);


	/*
	m_s->Volume((mmDword)(((mmFloat)m_SoundVoleme) * sm)  );
	m_sg->Volume((mmDword)(((mmFloat)m_SoundVoleme) * sg) );
	*/

	if(m_curent == m_game)
	{
		if(s_n == 0)
		{
			m_sg = m_sounds_g[num_sg];
			num_sg++;
			if(num_sg >= (mmInt)m_sounds_g.Count())
				num_sg = 0;

			s_n = 1;
			PlaySound(m_sg);
		}
	}
	else
	{
		if(s_n == 1)
		{
			s_n = 0;
			PlaySound(m_s);
		}
	}

	if(s_n == 0)
	{
		sm = min(sm + add , 1.0f);
		sg = max(sg - add , 0.0f);
	}
	else
	{
		sg = min(sg + add , 1.0f);
		sm = max(sm - add , 0.0f);
	}

	

	if((sm == 0)&&(s_n == 1))
		m_s->Stop();
	if((sg == 0)&&(s_n == 0))
		m_sg->Stop();



	m_s->Volume((mmDword)(((mmFloat)m_SoundVoleme) * sm)  );
	m_sg->Volume((mmDword)(((mmFloat)m_SoundVoleme) * sg) );


	//app->PlaySound(app->m_s);

}