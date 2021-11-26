//==========================================================================================
// Mayhem texture map custom attributes class
// Coded:	MozG
// Created:	20 june 2006
//==========================================================================================
#include "mhmca.h"

extern HINSTANCE g_hinst;


//============================================================
// Map custom attributes class
//============================================================
class clsMayhemMapCA: public CustAttrib
{
protected:
	IParamBlock2*	m_params;
	HWND			m_hpanel;
	
public:
	clsMayhemMapCA();
   ~clsMayhemMapCA();
    
    RefResult NotifyRefChanged(Interval changeInt,
							   RefTargetHandle hTarget,
							   PartID& partID,
							   RefMessage message);
	ParamDlg* CreateParamDlg(HWND hwMtlEdit, IMtlParams* imp);
	
	// Parameters blocks
	int NumParamBlocks();
	IParamBlock2* GetParamBlock(int i);
	IParamBlock2* GetParamBlockByID(short id);
	
	// References
	int NumRefs();
	RefTargetHandle GetReference(int i);
	void SetReference(int i, RefTargetHandle rtarg);
	
	// Subs
	int NumSubs();
	Animatable* SubAnim(int i);
	TSTR SubAnimName(int i);
	
	void BeginEditParams(IObjParam* ip, ULONG flags, Animatable* prev);
	void EndEditParams(IObjParam* ip, ULONG flags, Animatable* next);
	SClass_ID	SuperClassID();
	Class_ID	ClassID();
	ReferenceTarget* Clone(RemapDir& remap);
	bool CheckCopyAttribTo(ICustAttribContainer* to);
	TCHAR* GetName();
	void DeleteThis();
	
	// GUP
	DWORD Start();
	void  Stop();
	DWORD Control(DWORD parameter);
};

//============================================================
// Map custom attributes class description
//============================================================
class clsMayhemMapCADesc: public ClassDesc2
{
public:
	int 			IsPublic()				{return 1;}
	void*			Create(BOOL loading)	{return new clsMayhemMapCA;}
	const TCHAR*	ClassName()				{return GetString(IDS_MAYHEM_MAP_CLASS_NAME);}
	SClass_ID		SuperClassID()			{return CUST_ATTRIB_CLASS_ID;}
	Class_ID 		ClassID()				{return MHM_MAP_CUSTOM_ATTRIB_CLASS_ID;}
	const TCHAR* 	Category()				{return _T("");}
	const TCHAR*	InternalName()			{return _T("MayhemMapCustomAttrib");}
	HINSTANCE		HInstance()				{return g_hinst;}
};

static clsMayhemMapCADesc cdesc;

ClassDesc2* GetMayhemMapAttributesDesc()	{ return &cdesc; }


//const MIN_SPIN = 0;
//const MAX_SPIN = 100;

enum { simple_params};
enum { map_speed_u, map_speed_v, map_speed_rot, map_miplevel,
	   map_colorkey_enable, map_colorkey_low, map_colorkey_hi,
	   map_colorkey_alpha, map_alphatest_value, map_alphablend_enable,
	   map_additive_enable};

ParamBlockDesc2 simpleblk( simple_params, _T("Mayhem map attributes"),  0, &cdesc, P_AUTO_CONSTRUCT+ P_AUTO_UI, 0,
		//rollout
		IDD_MAP_CUSTATTRIB, IDS_MAP_CUSTATTRIB, 0, 0, NULL,
		// Speed vector U
		map_speed_u, _T("mapspeedU"), TYPE_FLOAT, P_ANIMATABLE, IDS_MAPATTRIB_SPEEDU,
		p_default,  0.0f,
		p_range,	-9999.0f, 9999.0f,
		p_ui, TYPE_SPINNER, EDITTYPE_UNIVERSE, IDC_MAPATTRIB_SPEEDU, IDC_MAPATTRIB_SPEEDU_SPIN, 0.01f,
		end,
		// Speed vector V
		map_speed_v, _T("mapspeedV"), TYPE_FLOAT, P_ANIMATABLE, IDS_MAPATTRIB_SPEEDV,
		p_default,  0.0f,
		p_range,	-9999.0f, 9999.0f,
		p_ui, TYPE_SPINNER, EDITTYPE_UNIVERSE, IDC_MAPATTRIB_SPEEDV, IDC_MAPATTRIB_SPEEDV_SPIN, 0.01f,
		end,
		// Speed vector rotation
		map_speed_rot, _T("mapspeedRot"), TYPE_FLOAT, P_ANIMATABLE, IDS_MAPATTRIB_SPEEDROT,
		p_default,  0.0f,
		p_range,	-9999.0f, 9999.0f,
		p_ui, TYPE_SPINNER, EDITTYPE_UNIVERSE, IDC_MAPATTRIB_SPEEDROT, IDC_MAPATTRIB_SPEEDROT_SPIN, 0.01f,
		end,
		
		// Map mipmap level
		map_miplevel, _T("mapMipLevel"), TYPE_INT, P_ANIMATABLE, IDS_MAPATTRIB_MIPLEVEL,
		p_default,  0,
		p_range,	0, 255,
		p_ui, TYPE_SPINNER, EDITTYPE_INT, IDC_MAPATTRIB_MIPLEVEL, IDC_MAPATTRIB_MIPLEVEL_SPIN, 1.0f,
		end,
		
		// Color key controls
		// Enable color key
		map_colorkey_enable, _T("mapColorKeyEnable"), TYPE_BOOL, P_ANIMATABLE, IDS_MAPATTRIB_COLORKEY_ENABLE,
		p_default, 		FALSE, 
		p_ui, 			TYPE_SINGLECHEKBOX, IDC_MAPATTRIB_COLORKEY_ENABLE, 
		end,
		
		// Color key low range value
		map_colorkey_low, _T("mapColorKeyLow"),	TYPE_RGBA, P_ANIMATABLE, IDS_MAPATTRIB_COLORKEY_LOW,	
		p_default,		Color(1.0,1.0,1.0), 
		p_ui,			TYPE_COLORSWATCH, IDC_MAPATTRIB_COLORKEY_LOW, 
		end,

		// Color key hi range value
		map_colorkey_hi, _T("mapColorKeyHi"),	TYPE_RGBA, P_ANIMATABLE, IDS_MAPATTRIB_COLORKEY_HI,	
		p_default,		Color(1.0,1.0,1.0), 
		p_ui,			TYPE_COLORSWATCH, IDC_MAPATTRIB_COLORKEY_HI, 
		end,
		
		// Color key alpha spinner
		map_colorkey_alpha, _T("mapColorKeyAlpha"), TYPE_INT, P_ANIMATABLE, IDS_MAPATTRIB_COLORKEY_ALPHA,
		p_default,  0,
		p_range,	0, 255,
		p_ui, TYPE_SPINNER, EDITTYPE_INT, IDC_MAPATTRIB_COLORKEY_ALPHA, IDC_MAPATTRIB_COLORKEY_ALPHA_SPIN, 1.0f,
		end,
		
		// Alpha test value
		map_alphatest_value, _T("mapAlphatestValue"), TYPE_INT, P_ANIMATABLE, IDS_MAPATTRIB_ALPHATEST_VALUE,
		p_default,  10,
		p_range,	0, 255,
		p_ui, TYPE_SPINNER, EDITTYPE_INT, IDC_MAPATTRIB_ALPHATEST_VALUE, IDC_MAPATTRIB_ALPHATEST_VALUE_SPIN, 1.0f,
		end,	
		
		// Alpha blending flag
		map_alphablend_enable, _T("mapAlphablendEnable"), TYPE_BOOL, P_ANIMATABLE, IDS_MAPATTRIB_ALPHABLEND_ENABLE,
		p_default,  FALSE,
		p_ui, TYPE_SINGLECHEKBOX, IDC_MAPATTRIB_ALPHABLEND_ENABLE,
		end,	
		
		// Additive blending flag
		map_additive_enable, _T("mapAdditiveEnable"), TYPE_BOOL, P_ANIMATABLE, IDS_MAPATTRIB_ADDITIVE_ENABLE,
		p_default,  FALSE,
		p_ui, TYPE_SINGLECHEKBOX, IDC_MAPATTRIB_ADDITIVE_ENABLE,
		end,	
		end
);

//============================================================
// Map custom attributes class implementation
//============================================================

// Constructor
//==========================================================================================
clsMayhemMapCA::clsMayhemMapCA()
{
	m_hpanel = NULL;
	m_params = NULL;
	cdesc.MakeAutoParamBlocks(this);
}

// Destructor
//==========================================================================================
clsMayhemMapCA::~clsMayhemMapCA()
{
	m_hpanel = NULL;
	m_params = NULL;
}

// NotifyRefChanged: ???
//==========================================================================================
RefResult clsMayhemMapCA::NotifyRefChanged(Interval changeInt, 
										   RefTargetHandle hTarget,
										   PartID &partID,
										   RefMessage message)
{
	return REF_SUCCEED;
}

// CreateParamsDlg: ???
//==========================================================================================
ParamDlg* clsMayhemMapCA::CreateParamDlg(HWND hwMtlEdit, IMtlParams *imp)
{
	return cdesc.CreateParamDlgs(hwMtlEdit, imp, this);
}


//============================================================
// Parameters block operations
//============================================================

// NumParamBlocks: must return number of parameters blocks used by this class
//==========================================================================================
int clsMayhemMapCA::NumParamBlocks()
{
	return 1;
}

// GetParamBlock: must return parameter block by specified index
//==========================================================================================
IParamBlock2* clsMayhemMapCA::GetParamBlock(int i)
{
	return m_params;
}

// GetParamBlockByID: must return parameter block by ID
//==========================================================================================
IParamBlock2* clsMayhemMapCA::GetParamBlockByID(short id)
{
	return (m_params->ID() == id) ? m_params : NULL;
}

//============================================================
// References operations
//============================================================

// NumRefs: must return number of references used by this class
//==========================================================================================
int clsMayhemMapCA::NumRefs()
{
	return 1;
}

// GetReference: must return reference target by specified index
//==========================================================================================
RefTargetHandle clsMayhemMapCA::GetReference(int i)
{
	return (i == 0) ? m_params : NULL;
}

// SetReference: set iternal reference
//==========================================================================================
void clsMayhemMapCA::SetReference(int i, RefTargetHandle rtarg)
{
	if (i == 0)
		m_params = (IParamBlock2*)rtarg;
}

//============================================================
// Subobjects operations
//============================================================

// NumSubs: must return number of subobjects used by this class
//==========================================================================================
int clsMayhemMapCA::NumSubs()
{
	return 1;
}

// SubAnim: must return animatable object by specified index
//==========================================================================================
Animatable* clsMayhemMapCA::SubAnim(int i)
{
	return m_params;
}

// SubAnimName: must return subanimatable name
//==========================================================================================
TSTR clsMayhemMapCA::SubAnimName(int i)
{
	return GetString(IDS_MAPCA);
}

//============================================================
// Plugin operations
//============================================================

// BeginEditParams: called to edit parameters
//==========================================================================================
void clsMayhemMapCA::BeginEditParams(IObjParam *ip, ULONG flags, Animatable *prev)
{
	cdesc.BeginEditParams(ip,this,flags,prev);
}

// EndEditParams: called to stop parameters editing
//==========================================================================================
void clsMayhemMapCA::EndEditParams(IObjParam *ip, ULONG flags, Animatable *next)
{
	cdesc.EndEditParams(ip,this,flags,next);
}

// SuperClassID: must return this superclass ID
//==========================================================================================
SClass_ID clsMayhemMapCA::SuperClassID()
{
	return CUST_ATTRIB_CLASS_ID;
}

// ClassID: must return this class ID
//==========================================================================================
Class_ID clsMayhemMapCA::ClassID()
{
	return MHM_MAP_CUSTOM_ATTRIB_CLASS_ID;
}

// Clone: ???
//==========================================================================================
ReferenceTarget* clsMayhemMapCA::Clone(RemapDir &remap)
{
	clsMayhemMapCA *pnew = new clsMayhemMapCA;
	pnew->MakeRefByID(FOREVER,0,remap.CloneRef(m_params));
	BaseClone(this, pnew, remap);
	return pnew;
}

// CheckCopyAttribTo: ???
//==========================================================================================
bool clsMayhemMapCA::CheckCopyAttribTo(ICustAttribContainer *to)
{
	return true;
}

// GetName: must return class name
//==========================================================================================
TCHAR* clsMayhemMapCA::GetName()
{
	return "Mayhem map attributes";
}

// DeleteThis: you really know this
//==========================================================================================
void clsMayhemMapCA::DeleteThis()
{
	delete this;
}







