//==========================================================================================
// Mayhem engine DLL entry point
// Coded:	MozG
// Created:	7 may 2006
//==========================================================================================
#include "mhmbase.h"


// Win32 based application entry point
#ifdef WIN32

clsRenderer* pren = mmnull;
clsDevice* pdev = mmnull;
clsResource* pmesh = mmnull;
clsResource* pclone = mmnull;
clsResource* ptex[10];//  = mmnull;
clsResource* pmtl[10];
clsVertexBuffer* pvb = mmnull;
clsFont2D*	pfont = mmnull;
clsResource* pmmh = mmnull;
//clsMatrix mxproj, mxview;
D3DXMATRIX mxproj, mxview;
HWND g_hwnd;
clsEngine* pengine;
clsInput*  pinput = mmnull;

mmDword lasttime;
mmDword ticks;

clsText * txt;


LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wpar, LPARAM lpar)
{
	switch(msg)
	{
	case WM_CREATE:
		return 0;
	case WM_KEYDOWN:
		if(wpar== VK_ESCAPE)
		{
			PostQuitMessage(0);
			return 0;
		}
		break;

		
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	case WM_SIZE:
		if (pdev)
			pdev->Reset();
		return 0;
	}
	return DefWindowProc(hwnd, msg, wpar, lpar);
}

HWND CreateMainWindow(HINSTANCE hinst)
{
	WNDCLASS wc;
	wc.cbClsExtra    = wc.cbWndExtra = 0;
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
	wc.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
	wc.hInstance	 = hinst;
	wc.lpfnWndProc	 = WndProc;
	wc.lpszClassName = "MHM";
	wc.lpszMenuName  = NULL;
	wc.style         = CS_HREDRAW|CS_VREDRAW|CS_OWNDC; 	

	RegisterClass(&wc);
	
	return CreateWindow("MHM", "Engine test", WS_OVERLAPPEDWINDOW,
						  0, 0,
						  800, 600,
						  HWND_DESKTOP, NULL, hinst, NULL);
}


void Idle()
{

//	pinput->GetKeyboard()->Snapshot();
//	pinput->GetMouse()->Snapshot();
	
	if (pinput->GetKeyboard()->IsKeyPressed(enKeyEscape))
	{
		DestroyWindow(g_hwnd);
		return;
	}
		
	static clsCamera cam;

	
	static mmBool _1_ = mmtrue;

	if(_1_)
	{
		cam.UseRH();
		cam.Near(0.5f);
		cam.Far(1000.0f);
		cam.Eye(0,0,-60);
		cam.At(0,0,0);
		cam.Up(0,1,0);
		cam.Fov(3.145f/2.0f);
		_1_ = mmfalse;
	}
	RECT rc;
	GetClientRect(g_hwnd, &rc);
	cam.Aspect((mmFloat)(rc.right - rc.left) / (mmFloat)(rc.bottom - rc.top));


/*	static float pt = 0;
	pt += 0.0002f;

	POINT ptCursor;
	static mmFloat m_x=0;
	static mmFloat m_y=0;
	mmFloat mx,my;
	GetCursorPos( &ptCursor );
	mx = ptCursor.x - m_x;
	my = ptCursor.y - m_y;
	m_x = (mmFloat)ptCursor.x;
	m_y = (mmFloat)ptCursor.y;*/

	
	cam.RotationCameraToTargetUp(pinput->GetMouse()->SnapshotAxisDelta(enMouseX)/100.0f);
	cam.RotationCameraToTargetHorizon(pinput->GetMouse()->SnapshotAxisDelta(enMouseY)/100.0f);
	static mmFloat rot = 0.0f;
	if (pinput->GetKeyboard()->SnapshotKeyPressed(enKeyUp))
		cam.MoovCameraLook(1.0f);
	if (pinput->GetKeyboard()->SnapshotKeyPressed(enKeyDown))
		cam.MoovCameraLook(-1.0f);
	if (pinput->GetKeyboard()->SnapshotKeyPressed(enKeyLeft))
		cam.RotationCameraToTargetZ(0.01f);
	if (pinput->GetKeyboard()->SnapshotKeyPressed(enKeyRight))
		cam.RotationCameraToTargetZ(-0.01f);
		
	static mmFloat spd = 0.0f;
//	if (pinput->GetMouse()->SnapshotAxisDelta(enMouseWheel) != 0)
//		spd = pinput->GetMouse()->SnapshotAxisDelta(enMouseWheel) / 10.0f;
		
	cam.MoovCameraLook(pinput->GetMouse()->SnapshotAxisDelta(enMouseWheel) / 100.0f);
	
/*	if (spd < 0.0f)
		spd += 0.01f;
	else
	   	spd -= 0.01f;
	
	if ((spd > -0.01f) && (spd < 0.01f))
		spd = 0;*/
	

	static float rotx = 0;
	static float roty = 0;
	
	rotx += 0.02f;
	roty += 0.03f;
	
	clsMatrix mxview1, mxproj1, mxrotx1, mxroty1, mxrot1, mxrot2, ident;
	ident.Identity();
	clsVector3 axis(-1.0f, -1.0f, -1.0f);
	mxrot1.RotationAxis(axis, rotx);
	axis.x = 1.0f;
	mxrot2.RotationAxis(axis, -rotx);
	mxrotx1.Translation(-60.0f, 0.0f, 0.0f);
	mxroty1.Translation(0.0f, 15.0f, 0.0f);
	
	
	MHMDirect3DDevice* pddev = *pdev;
	pdev->Clear();
	pddev->BeginScene();


	pddev->SetRenderState(D3DRS_LIGHTING, TRUE);

	pddev->SetRenderState(D3DRS_DIFFUSEMATERIALSOURCE, D3DMCS_COLOR1 );
	pddev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);


	
	
	pren->SetCamera(&cam);

	pdev->SetModelTM(&ident);

	//pdev->SetModelTM(&mxrot1);
	

	mxrot1 *= mxrotx1;
	pdev->SetModelTM(&mxrot1);

	mxrotx1.Translation(60.0f, 0.0f, 0.0f);
	mxrot2 *= mxrotx1;
	pdev->SetModelTM(&mxrot2);

	
	clsMatrix lightpos;
	lightpos.Translation(cosf(pengine->GetTimer()->GetSecs() / 2.0f) * 40.0f,
						 sinf(pengine->GetTimer()->GetSecs() / 2.0f) * 40.0f,
						 0.0f);
	pdev->SetModelTM(&lightpos);
	pmmh->Render();
	
	pdev->SetModelTM(&ident);
	pddev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pddev->SetRenderState(D3DRS_ALPHAREF, 0x00000009);
	pclone->Render();
	


	
	
	static mmInt ticks = 0;
	static mmFloat lastfps = pengine->GetTimer()->GetFPS();
	clsString fps;
	//fps.FromFloat(lastfps);
	fps.FromFormatNum(lastfps,"07.2");

/*
	pfont->SetText("Frames per second: " + fps);
	pddev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	
	pfont->Render();
*/
	
	//txt->SetRect(clsRect(10,10,200,500));
	txt->SetWordWrap(mmtrue);
	txt->SetPos(0,10,0.9f);

	static mmFloat _y=-100;
	_y += 0.05f;

	txt->SetPos(0,0,0.9f);
	txt->SetText("Frames per second: \a(0xFFFF0000)" + fps);


//	txt->SetText("Frames \a(0xFFFF0000)per\a(def) second ABC zx 12345 67890 12345 67890 zzzzz xxxxxxXTYU ccccc");

	txt->SetFiltering(mmtrue);

	//txt->SetAlignX(enCenter);
	//txt->SetAlignX(enFill);
	txt->SetSize(256);
	txt->SetColor(0xFFFFFFFF);
	txt->Render();
	
	static clsString strkeys = "Keys pressed: ";
	
	if (ticks >= 100)
	{
		
		strkeys = "Keys pressed: ";
		const clsKeyboardState* pstate =  pinput->GetKeyboard()->GetState();
		for (mmUint i = 0; i < pstate->GetNumAffectedKeys(); i++)
			if (pstate->GetKeyState(i)->m_pressed)
				strkeys += "[" + clsString(pstate->GetKeyState(i)->m_code) + "]";
		ticks =0;
	}
	
//	txt->SetPos(0,30,0.0f);
//	txt->SetText(strkeys);
//	txt->Render();
	stFontRect frc;
	frc.m_left = 0;
	frc.m_right = 1024;
	frc.m_top = 30;
	frc.m_bottom = 0;
	//pfont->SetRect(&frc);
	//pfont->SetText(strkeys);
	//pfont->Render();


	pddev->EndScene();
	pdev->Present();
	
	pengine->Update();
	
//	if (ticks == 25)
//	{
		lastfps       = pengine->GetTimer()->GetFPS();
//		ticks         = 0;
//	}
	ticks++;
//	Sleep(100);
}

struct SpriteVtx
{
	mmFloat x, y, z, w;
	mmDword color;
	mmFloat u, v;
};

void Init()
{


//	clsText * txt;
	txt = (clsText*)pdev->GetResourceManager()->GetResource("myfont.mhmFont2D", enResText);
	//txt->SetText("123 zxc\nqwe");
	





	pinput->CaptureKeyboard();
	pinput->CaptureMouse();
	
	lasttime = GetTickCount();
	ticks    = 0;
	clsString numstr("000100");
	mmInt myvar = numstr.ToInt();
	MHMDirect3DDevice* pddev = *pdev;
	pddev->SetRenderState(D3DRS_LIGHTING, FALSE);
	MHMLight light;
	MHMMaterial mtl;
	mmZeroVar(mtl);
	mtl.Diffuse.r = mtl.Diffuse.g = mtl.Diffuse.b = 1.0f;
	mtl.Power = 1.0f;
	mmZeroVar(light);
	light.Type = D3DLIGHT_POINT;
	light.Range = 10000.0f;
	light.Diffuse.r = light.Diffuse.g = light.Diffuse.b = 1.0f;
	light.Specular.r = light.Specular.g = light.Specular.b = 1.0f;
	light.Attenuation0 = 1.0f;
	light.Attenuation1 = 0.0f;	
	light.Attenuation2 = 0.0f;
	light.Position.x = 200.0f; light.Position.y = 200.0f; light.Position.z = 200.0f;

	light.Theta = 1.0f;
	light.Phi = 2.2f;
	light.Falloff=1.0f;
	
	pddev->SetMaterial(&mtl);
	pddev->SetLight(0, &light);
	pddev->LightEnable(0, TRUE);
	pddev->SetRenderState(D3DRS_AMBIENT, 0x66666666);

	
	pddev->SetRenderState(D3DRS_ALPHAREF, 0x00000009);
	pddev->SetRenderState(D3DRS_SPECULARENABLE, TRUE);
	pddev->SetRenderState(D3DRS_DIFFUSEMATERIALSOURCE, D3DMCS_COLOR1 );

	pddev->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE);


/*
	pddev->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	pddev->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	pddev->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
*/
	// box
	clsSimpleBoxParams box;
	box.m_size = 10.0f;
	box.m_vclr = 0x55FFFFFF;
	//pmesh = pdev->GetResourceManager()->GetResource("mybox", enResSimpleBox, &box);
//	pmesh = pdev->GetResourceManager()->GetResource("MultiSphere.mmh", MHMRES_MESH_ID);
	pclone = pdev->GetResourceManager()->GetResource("hero.mmh", MHMRES_MESH_ID);
	pmmh = pdev->GetResourceManager()->GetResource("my_light.mmh", MHMRES_MESH_ID);
	clsFont2DParams fp;
	fp.m_key_hi = clsColorRGB(100, 100, 100);
	fp.m_key_enable  = mmtrue;
	fp.m_key_alpha   = 0;
	fp.m_maxchars    = 1024;
	
	pfont  = (clsFont2D*)pdev->GetResourceManager()->GetResource("sysfont.tga", enResFont2D, &fp);
	stFontRect frc;
//	RECT rc;
	frc.m_left = 0;
	frc.m_right = 1000;
	frc.m_top = 0;
	frc.m_bottom = 800;
	pfont->SetRect(&frc, 0xFFFFFFFF);
	//pfont->SetText("Sample text 2D");
	//pfont->SetColor(0xFF00FF00);
	
	
	//((clsBaseMesh*)pmesh)->Clone(pclone);
	//((clsSimpleBox*)pclone)->FlipNormals();
	//((clsSimpleBox*)pclone)->FlipVertexOrder();
	
	// texture
//	ptex[0]  = pdev->GetResourceManager()->GetResource("perlin.tga", enResTextureTGA);
//	ptex[1]  = pdev->GetResourceManager()->GetResource("perlin.tga", enResTextureTGA);
//	ptex[2]  = pdev->GetResourceManager()->GetResource("perlin.tga", enResTextureTGA);
	
	// material
	//pmtl[0] = pdev->GetResourceManager()->GetResource("manymaps.mtl", enResBaseMaterial);
	
//<<<<<<< mhmentry.cpp
//	pmmh = pdev->GetResourceManager()->GetResource("Pyramid.mmh", enResStaticMeshMMH);
//=======
	
//>>>>>>> 1.18
		
}
/*

INT CALLBACK WinMain(HINSTANCE hinst, HINSTANCE hpinst, LPSTR lpcmd, int show)
{
	clsLog log("log.html");
	log.SetAsCommon();
	g_plog->Create();
	g_plog->Out("Resource manager test log");
	g_plog->OutBreakLine();
	clsString curdir;
	curdir.Alloc(MAX_PATH);
	GetCurrentDirectory(MAX_PATH, curdir);
	curdir.Truncate();
	
	clsEngine engine(hinst);
	pengine = &engine;
	//pren = new clsRenderer();	
	
	HWND hwnd = CreateMainWindow(hinst);
	ShowWindow(hwnd, SW_SHOW);
	UpdateWindow(hwnd);
	engine.Init(hwnd, curdir + "\\engine.ini");
	pren = engine.GetRenderer();
	pdev = pren->GetDevice();
	pinput = engine.GetInput();
	
	Init();
	g_hwnd = hwnd;
	
	MSG		 msg;
	BOOL	 bGotMsg;
	msg.message = WM_NULL;
	PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE);
	while(WM_QUIT != msg.message)
	{
		bGotMsg = PeekMessage(&msg, NULL, 0, 0, PM_REMOVE);
		if (bGotMsg)
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else 	
			Idle();
	}
	g_plog->Out("Shutdown", enLCBlue);
	g_plog->OutBreakLine();
		
	
	//mmSafeDelete(pren);
	//mmSafeRelease(pd3d);
	return 0;
}
*/
#ifdef MHM_EXPORT




#endif // MHM_EXPORT

#endif // WIN32