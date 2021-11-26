#ifndef FIREAPP_INCLUDED
#define FIREAPP_INCLUDED

#define N_PRIZ (10)
#define N_MANS (5)
#define SL (1)
#define RADPAT (40.0f)

#define LIVES (5)

#define M_SPID_0 ((1.0f))
#define M_SPID_1 ((1.6f))
#define M_SPID_2 ((1.7f))

class clsFireEntity;
class clsFireMan;
class clsFireDoska;
class clsFireJertva;
class clsFireParticle;
class clsFirePriz;
class clsFireVarona;
class clsFireApp;

enum enStateGame {enNoneGam = 0, enDoskaVybygaet, enManVybygaet, enGame , enManDed ,enManPrigaet,enManZakonchil,
enJrtvaUbigaet , enJrtvaUpalaMan,enManBejit,
enBatutVybygaet , enDoskaOk,
enAngel,
enStartFireDownVilet, enStartFireDown, enFireDownSlavil,
enPrizVilet, enPrizDown,
enJertvaStart,enJertvaVilezaet, enJertvaMahaet,enJertvaOk,enJertvaHvataet,enJertvaKamikadzeUpal,
enJertvaAndMan,enJrtvaUbigaetStart,enJrtvaUbigaetStartproc,enJrtvaBejit,enJrtvaUpalaManProc,enJertvaShvatil,
enJertvaPadaet,enJertvaKamikadzeRazbilsa,enJertvaKamikadzeIschezaet,enJertvinet
};

class clsTextButton
{
	protected:
	public:

		clsText * Text;

		clsTextButton();
		mmVoid Update(clsFireApp * app);

		mmUint Color;
		mmUint ColorOver;
		mmUint ColorDown;

		mmInt state;

		mmFloat x;
		mmFloat y;
		mmFloat s;

		mmBool procent;

		mmBool enable;

};

class cls_Scene
{
	protected:
	public:
	clsFireApp * app;
	virtual mmVoid UpdateObjects(){};
	virtual mmVoid RenderObjects(){};
	virtual mmVoid Init(){};

	enStateGame state;
};

class cls_Pause :public cls_Scene
{
	protected:
	public:
	cls_Pause(clsFireApp * app);
	mmVoid UpdateObjects();
	mmVoid RenderObjects();
	mmVoid Init();

	clsTextButton m_continue;
	clsTextButton m_options;
	clsTextButton m_hiscores;
	clsTextButton m_info;
	clsTextButton m_exit;

};

class cls_Main :public cls_Scene
{
	protected:
	public:
	cls_Main(clsFireApp * app);
	mmVoid UpdateObjects();
	mmVoid RenderObjects();
	mmVoid Init();

	clsTextButton m_start_game;
	clsTextButton m_options;
	clsTextButton m_hiscores;
	clsTextButton m_info;
	clsTextButton m_exit;

};
class cls_Exit :public cls_Scene
{
	protected:
	public:
	cls_Exit(clsFireApp * app);
	mmVoid UpdateObjects();
	mmVoid RenderObjects();
	mmVoid Init();

	clsTextButton m_exit_game;
	clsTextButton m_yes;
	clsTextButton m_no;
};
class cls_Info :public cls_Scene
{
	protected:
	public:
	cls_Info(clsFireApp * app);
	mmVoid UpdateObjects();
	mmVoid RenderObjects();
	mmVoid Init();

	clsTextButton m_text;
	clsTextButton m_return;
};

class cls_Game :public cls_Scene
{
	protected:
	public:
	cls_Game(clsFireApp * app);
	mmVoid UpdateObjects();
	mmVoid RenderObjects();
	mmVoid Init();

	mmBool m_reload;

	mmInt LastLevel;

	clsTextButton m_text_up;
	clsTextButton m_text_down;
	clsTextButton m_text_state;

	clsTextButton m_scor;
	clsTextButton m_liv;
	clsTextButton m_jert;
	clsTextButton m_fir;
};
class cls_EndGame :public cls_Scene
{
	protected:
	public:
	cls_EndGame(clsFireApp * app);
	mmVoid UpdateObjects();
	mmVoid RenderObjects();
	mmVoid Init();

	clsTextButton m_game_over;
	clsTextButton m_text_scores;
	clsTextButton m_scores;
	clsTextButton m_ok;
	clsTextButton m_name;

	clsString name;

	mmBool papal;
};
class cls_NextLevel :public cls_Scene
{
	protected:
	public:
	cls_NextLevel(clsFireApp * app);
	mmVoid UpdateObjects();
	mmVoid RenderObjects();
	mmVoid Init();

	mmInt proc;

	mmInt m_score_out;
	mmInt m_proc_out;

	mmFloat tim;

	clsTextButton m_proc;
	clsTextButton m_scores;
	clsTextButton m_next;
	clsTextButton m_replay;
	clsTextButton m_exit;
};
class cls_HiScores :public cls_Scene
{
	protected:
	public:
	cls_HiScores(clsFireApp * app);
	mmVoid UpdateObjects();
	mmVoid RenderObjects();
	mmVoid Init();

	clsTextButton m_names;
	clsTextButton m_scores;
	clsTextButton m_text;
	clsTextButton m_exit;
};

class clsHSItem
{
protected:
public:

	clsString name;
	mmInt score;
	inline mmBool operator > (clsHSItem & ptr) const { return score < ptr.score;};
	inline mmBool operator < (clsHSItem & ptr) const { return score > ptr.score;};

};


class cls_Options :public cls_Scene
{
	protected:
	public:
	cls_Options(clsFireApp * app);
	mmVoid UpdateObjects();
	mmVoid RenderObjects();
	mmVoid Init();
	mmVoid InitVol();
	mmVoid InitOp();

	clsTextButton m_text;
	clsTextButton m_resolut;
	clsTextButton m_r_add;
	clsTextButton m_r_sub;
	clsTextButton m_exit;
	clsTextButton m_aply;

	clsTextButton m_text1;

	clsTextButton m_vol;
	clsTextButton m_v_add;
	clsTextButton m_v_sub;

	clsTextButton m_fx;
	clsTextButton m_f_add;
	clsTextButton m_f_sub;


	clsVideoModeList * vm;
	mmBool FS;


	mmInt resol;
};

class clsFireApp :public clsApp
{
protected:
	
	public:

		mmVoid PlayMuza();

		mmInt m_alpha_pause;

		clsSprite*		m_logo;
		mmBool m_Start;

		clsVar m_conf;

		mmVoid PlaySound(clsSound* s);
		mmVoid PlayFX(clsSound* s);

		mmInt m_SoundVoleme;
		mmInt m_FxVoleme;

		mmInt m_Curent_Resolutions;

		mmBool m_NoInit;

		cls_Scene * m_curent;
		cls_Scene * m_return;

		cls_Main * m_main;
		cls_Exit * m_exit;
		cls_Info * m_info;
		cls_Game * m_game;
		cls_EndGame * m_end_game;
		cls_NextLevel * m_next_level;
		cls_HiScores * m_hi_scores;
		cls_Options * m_options;
		cls_Pause * m_pause;

		mmBool m_Pause;

		tmpRefArr <clsHSItem> m_HS;

		mmVoid ZOpen(cls_Scene * next = mmnull);
		mmVoid ZClose(cls_Scene * next = mmnull);

		mmVoid SaveHS();
		mmVoid LoadHS();


		mmInt m_count_start_jertv;
		mmInt m_count_dead_jertv;

		mmBool m_win;

		//mmBool m_pause;

		mmVoid SceneClouse();

		mmInt m_level;

		mmBool m_mouse;

		mmFloat m_mous_x;
		mmFloat m_mous_y;

		


		clsFireEntity*			m_Scene;
		clsFireEntity*			m_Zast;
		clsFireEntity*			m_ZastPlan;

//		enStateZast SZ;

		clsSound*	m_s;
		clsSound*	m_sg;

		tmpRefArr <clsSound*> m_sounds_g;

		clsSound*	m_s_up;
		clsSound*	m_s_pr;
		clsSound*	m_s_tush;
		clsSound*	m_s_fdo;
		clsSound*	m_s_jertva;
		clsSound*	m_s_boom1;
		clsSound*	m_s_key;
		clsSound*	m_s_dver;
		clsSound*	m_s_kar;
		clsSound*	m_s_trrii;
		clsSound*	m_s_din;


		clsRandRange m_RandPriz;

	clsFirePriz*	m_priz1;


	clsText*			m_cursor;

	/*
	clsText*			m_Text;
	clsText*			m_TextState;
	clsText*			m_Text1;
	*/
	clsRenderer*		m_Renderer;
	clsResourceManager*	m_ResMan;
	clsTimer*			m_Timer;

	clsFireParticle* m_FireDoska;
	clsFireParticle* m_FireTush;

	clsCamera			m_Camera;
	clsCamera			m_CameraZ;
	clsBaseLight*		m_Light;

	
//	clsEntity*			m_d1;
//	clsEntity*			m_d2;
	clsEntity*			m_zast_text;

	clsEntity*			m_SceneTMP;

	clsEntity*			m_eye;
	clsEntity*			m_at;

	clsEntity*			m_eye_x;
	clsEntity*			m_at_x;

	//clsFireVarona*		m_Varona;


	clsInput*			m_pInput;
	clsMouse*			m_pMouse;

	mmFloat m_df;

	mmInt m_count;
	mmFloat m_sizeF;

	mmInt m_lives;




	mmInt m_scores;

//	mmInt zast_state;

	mmBool m_waitKey;
//	mmInt m_win;


	clsEntityFactory* CreateEntityFactory(clsEngine* pengine);



	clsRand m_rand;

	clsRect sRect; 

	clsFireDoska*		m_pDoska;
	clsFireMan *		m_pMan;
	clsFireEntity*		m_pBatut;
	clsFireEntity*		m_pAAng;

	clsFireJertva*		m_pMan_01;

	clsVector3 m_CamV;

//	mmInt LevelState;


	clsFireApp(void);
	~clsFireApp(void);

	mmRes InitApp();
    mmRes IdleLoop();
	mmRes WindowSize();
	mmRes ShutApp();

	mmRes InitStart(mmBool waitKey = mmfalse);
	mmRes ClearScenes();
    
    mmRes UpdateObjects();
    mmRes RenderObjects();

	mmVoid LoadScene(mmInt level);
	mmVoid InitScene();

	mmVoid SetCam();
	mmVoid SetLight();

	mmVoid SetTextureZast(const mmString nam);

	mmVoid SetFireFactor(clsEntity * en,mmInt f);
	mmVoid InitScene(clsEntity* en);

	mmVoid PrizOk(mmInt tip);
};
//****************************************
class clsFireEntityFactory: public clsEntityFactory
{
public:
	clsFireEntityFactory(clsEngine* pEngine);
   ~clsFireEntityFactory();
	
	clsEntity* CreateEntity(const clsString& type,
							const clsString& name);
};
//*********************************************************
class clsFireEntity: public clsEntity
{
public:

	clsString m_anim;
   enAnimLooping m_anim_loop;
   clsString m_anim_r;

public:
	clsFireEntity(clsEntityFactory* pFactory,
			  const clsString& type,
			  const clsString& name);
   ~clsFireEntity();

   clsFireApp * app;

   cls_Scene * m_curent;

 //  clsString m_scene_name;

	enStateGame m_scene_state;
	enStateGame m_next_state;
	enStateGame m_state;

	  clsBaseTexture * m_sel_tex;

   mmVoid OnAnimationKey(mmInt key);
   mmVoid OnAnimationPlay();

   mmRes  OnRender();

   mmVoid OnLoadPropsPost(clsVarList * vars);

   virtual mmVoid OnInit();

   	mmFloat m_alpha;

   /* 
    mmVoid OnInitialize();
    mmVoid OnRender();
    mmVoid OnBufferedKeyboardEvent(const clsKeyState* pEvent, mmBool bFirst);
    mmVoid OnEndBufferedEvents(mmUint nNumMouseEvents, mmUint nNumKeyboardEvents);
    mmVoid OnUpdate(const clsTimer* pTimer);
    mmVoid OnDepencyUpdate(clsEntity* pEntity);
	*/
};
//***********************************************
class clsFireMan: public clsFireEntity
{
public:

public:
	mmInt m_priz;

	mmFloat m_man_spid;

	clsEntity *	m_auragn;

	clsVector3 pos;
	clsVector3 _pos;
	clsVector3 v;
	mmFloat	vUp;
	mmFloat vDo;

	mmFloat t;

	clsFireMan(clsEntityFactory* pFactory,
			  const clsString& type,
			  const clsString& name);
   ~clsFireMan();

   mmVoid Move(mmFloat k);

   mmVoid Init();

   clsFireJertva * jertva;


   mmFloat m_speed;
   mmFloat m_speed_f;
   
};
//***********************************************
class clsFireDoska: public clsFireEntity
{
public:

public:
	mmFloat n_w;
	mmFloat w;
	mmFloat l;
	mmFloat r;

	mmBool m_dyrka;

	clsFireDoska(clsEntityFactory* pFactory,
			  const clsString& type,
			  const clsString& name);
   ~clsFireDoska();
   mmVoid Move(mmFloat k);
   mmVoid OnInitialize(mmBool bClone);
  
};
//*********************************************************
class clsFireJertvaImiter: public clsFireEntity
{
public:

public:
	clsFireJertvaImiter(clsEntityFactory* pFactory,
			  const clsString& type,
			  const clsString& name);
   ~clsFireJertvaImiter(); 
   mmVoid OnLoadProps(clsVarList * vars);

   mmBool m_kamikadze;

   mmInt count;
   mmFloat tim;

   mmVoid OnUpdate(const clsTimer* pTimer);
};
//*********************************************************
class clsFireJertva: public clsFireEntity
{
public:

public:
	clsFireJertva(clsEntityFactory* pFactory,
			  const clsString& type,
			  const clsString& name);
   ~clsFireJertva(); 
   mmVoid OnUpdate(const clsTimer* pTimer);
   mmVoid OnInitialize(mmBool bClone);

   mmFloat time;

   mmFloat nap_polz;
   mmFloat nap_rot;

   mmVoid OnInit();

   mmFloat size;
};
//*********************************************************
class clsFireParticle: public clsFireEntity
{
public:

public:
	clsFireParticle(clsEntityFactory* pFactory,
			  const clsString& type,
			  const clsString& name);
   ~clsFireParticle(); 
	mmFloat m_scale;
	mmFloat m_size;
   mmVoid OnUpdate(const clsTimer* pTimer);
   mmVoid OnInitialize(mmBool bClone);
   mmVoid OnLoadProps(clsVarList * vars);

   mmVoid OnClone(clsEntity* pInstance);

   mmVoid OnNoSphereTest();
};
//*********************************************************
class clsFireFire: public clsFireParticle
{
public:

public:
	clsFireFire(clsEntityFactory* pFactory,
		const clsString& type,
		const clsString& name);
	~clsFireFire(); 
	mmFloat m_scale;
	mmVoid OnUpdate(const clsTimer* pTimer);
	mmVoid OnInitialize(mmBool bClone);
	mmVoid OnLoadProps(clsVarList * vars);
	mmFloat time;
	mmFloat m_max;
	mmFloat m_j_doun;
	
	mmBool m_inman;

	mmFloat TimFire;
	mmFloat TimFire_j;

};
//*********************************************************

class clsFireFireDouw: public clsFireParticle
{
public:

public:
	clsFireFireDouw(clsEntityFactory* pFactory,
		const clsString& type,
		const clsString& name);
	~clsFireFireDouw(); 
	mmFloat m_scale;
	mmVoid OnUpdate(const clsTimer* pTimer);
	mmVoid OnInitialize(mmBool bClone);
	mmFloat time;
};
//*********************************************************

class clsFirePriz: public clsFireEntity
{
public:

public:
	clsFirePriz(clsEntityFactory* pFactory,
		const clsString& type,
		const clsString& name);
	~clsFirePriz(); 
	mmFloat m_scale;
	mmVoid OnUpdate(const clsTimer* pTimer);
	mmVoid OnInitialize(mmBool bClone);
	mmFloat time;

	clsEntity*			m_ecol;
	clsMatrix mxs;

	mmVoid InitPr(mmInt pr);

	mmInt tip;

	mmInt state;
};
//*********************************************************

class clsFireVarona: public clsFireEntity
{
public:

public:
	clsFireVarona(clsEntityFactory* pFactory,
		const clsString& type,
		const clsString& name);
	~clsFireVarona(); 

	mmVoid OnUpdate(const clsTimer* pTimer);

	mmVoid OnInit();

	mmFloat RangeTime;
	mmFloat Time;
	mmFloat Time2;
	mmFloat nap;

	clsRand rand;

	mmInt n_priz;

	clsFireEntity * m_priz;


};
//*********************************************************
//*********************************************************

USE_APPLICATION(clsFireApp)

#endif // FIREAPP_INCLUDED