//==========================================================================================
// Mayhem engine 3DS Max tool plugin
// Coded:	MozG
// Created:	1 june 2006
//==========================================================================================
#include "mhmtool.h"

extern HINSTANCE g_hinst;

#define MHMTOOL_CLASS_A		0x60b30cd7
#define MHMTOOL_CLASS_B		0x747240ba

INT_PTR CALLBACK DlgProc		(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK MeshDlgProc	(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK ConfigDlgProc	(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK PrefDlgProc	(HWND, UINT, WPARAM, LPARAM);

// Opens directory choose dialog
//==========================================================================================
clsString ChooseDir(clsString initial, HWND owner)
{
	MessageBox(owner, "Не работает. Спасибо некрософту скажите.", "Oops", MB_OK);
	return clsString();
}

// Mayhem tool constructor
//==========================================================================================
MayhemTool::MayhemTool():m_nodes(mmfalse), m_materials(mmfalse)
{
	m_iface      = NULL;
	m_iutil      = NULL;
	m_hmainpanel = NULL;
	m_hmeshpanel = NULL;
	m_hprefpanel = NULL;
	m_inifile    = NULL;
	m_show_meshexporter		= mmfalse;
	m_show_materialexporter	= mmfalse;
	m_prefs_opened			= mmfalse;
	m_meshes_opened			= mmfalse;
	m_materials_opened		= mmfalse;
}

// Mayhem tool destructor
//==========================================================================================
MayhemTool::~MayhemTool()
{
	
}

// BeginEditParameters: utility object interface method
//==========================================================================================
void MayhemTool::BeginEditParams(Interface *ip, IUtil *iu)
{
	m_iface = ip;
	m_iutil = iu;
	
	m_inifile = new clsINIFile(m_iface->GetDir(APP_PLUGCFG_DIR) + clsString("\\mhmtool.ini"));
	LoadConfig();
	// Preferences panel (show always)
	m_hprefpanel = m_iface->AddRollupPage(g_hinst, MAKEINTRESOURCE(IDD_PREFERENCES_PANEL), 
										  ConfigDlgProc, GetString(IDS_PREFPANEL_TITLE), 0,
										  m_prefs_opened ? 0 : APPENDROLL_CLOSED); 
	// Material export panel (vary)
	if (m_show_materialexporter)
		m_hmainpanel = m_iface->AddRollupPage(g_hinst, MAKEINTRESOURCE(IDD_MAYHEM_PANEL), 
											  DlgProc, GetString(IDS_PANEL_TITLE), 0,
											  m_materials_opened ? 0 : APPENDROLL_CLOSED);
	// Mesh export panel (vary)
	if (m_show_meshexporter)
		m_hmeshpanel = m_iface->AddRollupPage(g_hinst, MAKEINTRESOURCE(IDD_MESH_PANEL), 
											  MeshDlgProc, GetString(IDS_PANEL_MESH), 0,
											  m_meshes_opened ? 0 : APPENDROLL_CLOSED);
	//iu->CloseUtility();
}

// EndEditParameters: utility object interface method
//==========================================================================================
void MayhemTool::EndEditParams(Interface *ip, IUtil *iu)
{
	SaveConfig();
	if (m_hprefpanel)
		m_iface->DeleteRollupPage(m_hprefpanel);
	if (m_hmainpanel)
		m_iface->DeleteRollupPage(m_hmainpanel);
	if (m_hmeshpanel)
		m_iface->DeleteRollupPage(m_hmeshpanel);
	m_iutil      = iu;
	m_hmainpanel = NULL;
	m_hprefpanel = NULL;
	m_hmeshpanel = NULL;
	mmSafeDelete(m_inifile);
}

// SelectionSetChanged: utility object event
//==========================================================================================
void MayhemTool::SelectionSetChanged(Interface* ip, IUtil* iu)
{
	m_nodes.Clear();
	m_materials.Clear();
	SendDlgItemMessage(m_hmainpanel, IDC_MATERIAL_LIST, LB_RESETCONTENT, 0, 0);
	int numnodes = m_iface->GetSelNodeCount();
	mmUint counter = 0;
	for (int i = 0; i < numnodes; i++)
	{
		INode* pnode = m_iface->GetSelNode(i);
		Mtl* pmtl = pnode->GetMtl();
		m_nodes.Add(pnode);
		if (pmtl == mmnull)
			continue;
		
		if (pmtl->IsMultiMtl())
			for (int m = 0; m < pmtl->NumSubMtls(); m++)
			{
				SendDlgItemMessage(m_hmainpanel, IDC_MATERIAL_LIST, LB_ADDSTRING, 0, 
								   (LPARAM)pmtl->GetSubMtl(m)->GetName().data());						
				SendDlgItemMessage(m_hmainpanel, IDC_MATERIAL_LIST, LB_SETITEMDATA, counter, 
								   (LPARAM)pmtl->GetSubMtl(m));										
				m_materials.Add(pmtl->GetSubMtl(m));
				counter++;
			}
		else
		{
			SendDlgItemMessage(m_hmainpanel, IDC_MATERIAL_LIST, LB_ADDSTRING, 0,
							   (LPARAM)pmtl->GetName().data());
			SendDlgItemMessage(m_hmainpanel, IDC_MATERIAL_LIST, LB_SETITEMDATA, counter, 
							   (LPARAM)pmtl);										
			m_materials.Add(pmtl);
			counter++;
		}
	}
	if (m_nodes.Count() == 0)
	{
		SetDlgItemText(m_hmeshpanel, IDC_SOURCE_NODE, "no selection");
	}
	else
	{
		if (m_nodes.Count() > 1)
			SetDlgItemText(m_hmeshpanel, IDC_SOURCE_NODE, "multiple selected");
		else
			SetDlgItemText(m_hmeshpanel, IDC_SOURCE_NODE, m_nodes[0]->GetName());
	}
}

// DeleteThis: self-delete
//==========================================================================================
void MayhemTool::DeleteThis()
{

}

// Init: inits rollup panel
//==========================================================================================
void MayhemTool::Init(HWND hdlg)
{
	bool flag;
	m_inifile->ReadBool("MaterialExporter", "Overwrite", &flag);
	SendDlgItemMessage(hdlg, IDC_CHECK_MTLEXPORT_OVERWRITE, BM_SETCHECK,
					   flag ? BST_CHECKED : BST_UNCHECKED, 0);
	m_inifile->ReadBool("MaterialExporter", "OffsetAsOffset", &flag);
	SendDlgItemMessage(hdlg, IDC_MATERIALEXPORT_OFFSET_AS_OFFSET, BM_SETCHECK,
					   flag ? BST_CHECKED : BST_UNCHECKED, 0);
	SendDlgItemMessage(hdlg, IDC_MATERIALEXPORT_OFFSET_AS_ANIMATION, BM_SETCHECK,
					   !flag ? BST_CHECKED : BST_UNCHECKED, 0);

}

// Shut: deinits rollup panel
//==========================================================================================
void MayhemTool::Shut(HWND hdlg)
{
	int check = SendDlgItemMessage(hdlg, IDC_CHECK_MTLEXPORT_OVERWRITE, BM_GETCHECK, 0, 0);
	m_inifile->WriteBool("MaterialExporter", "Overwrite", check == BST_CHECKED ? true : false);
	check = SendDlgItemMessage(hdlg, IDC_MATERIALEXPORT_OFFSET_AS_OFFSET, BM_GETCHECK, 0, 0);
	m_inifile->WriteBool("MaterialExporter", "OffsetAsOffset", check == BST_CHECKED ? true : false);
}

// InitMehsDlg: inits mesh rollup panel
//==========================================================================================
void MayhemTool::InitMeshDlg(HWND hdlg)
{
	bool flag;
	m_inifile->ReadBool("MeshExporter", "Overwrite", &flag);
	SendDlgItemMessage(hdlg, IDC_MESHEXPORT_OVERWRITE, BM_SETCHECK,
					   flag ? BST_CHECKED : BST_UNCHECKED, 0);
	m_inifile->ReadBool("MeshExporter", "NormalsChannel", &flag);
	SendDlgItemMessage(hdlg, IDC_CHECK_NORMALS, BM_SETCHECK,
					   flag ? BST_CHECKED : BST_UNCHECKED, 0);
	m_inifile->ReadBool("MeshExporter", "MappingChannel", &flag);
	SendDlgItemMessage(hdlg, IDC_CHECK_MAPPING, BM_SETCHECK,
					   flag ? BST_CHECKED : BST_UNCHECKED, 0);
	m_inifile->ReadBool("MeshExporter", "VertexColorChannel", &flag);
	SendDlgItemMessage(hdlg, IDC_CHECK_VCOLORS, BM_SETCHECK,
					   flag ? BST_CHECKED : BST_UNCHECKED, 0);
	m_inifile->ReadBool("MeshExporter", "Packed", &flag);
	SendDlgItemMessage(hdlg, IDC_CHECK_PACKED_MESH, BM_SETCHECK,
					   flag ? BST_CHECKED : BST_UNCHECKED, 0);
	
}

// ShutMeshDlg: shuts mesh rollup panel
//==========================================================================================
void MayhemTool::ShutMeshDlg(HWND hdlg)
{
	int check = SendDlgItemMessage(hdlg, IDC_MESHEXPORT_OVERWRITE, BM_GETCHECK, 0, 0);
	m_inifile->WriteBool("MeshExporter", "Overwrite", check == BST_CHECKED ? true : false);
	check = SendDlgItemMessage(hdlg, IDC_CHECK_NORMALS, BM_GETCHECK, 0, 0);
	m_inifile->WriteBool("MeshExporter", "NormalsChannel", check == BST_CHECKED ? true : false);
	check = SendDlgItemMessage(hdlg, IDC_CHECK_MAPPING, BM_GETCHECK, 0, 0);
	m_inifile->WriteBool("MeshExporter", "MappingChannel", check == BST_CHECKED ? true : false);
	check = SendDlgItemMessage(hdlg, IDC_CHECK_VCOLORS, BM_GETCHECK, 0, 0);
	m_inifile->WriteBool("MeshExporter", "VertexColorChannel", check == BST_CHECKED ? true : false);
	check = SendDlgItemMessage(hdlg, IDC_CHECK_PACKED_MESH, BM_GETCHECK, 0, 0);
	m_inifile->WriteBool("MeshExporter", "Packed", check == BST_CHECKED ? true : false);
}

// ShutPrefsDlg: inits preferences rollup panel
//==========================================================================================
void MayhemTool::InitPrefsDlg(HWND hdlg)
{
	
	SendDlgItemMessage(hdlg, IDC_PREFERENCES_MESHEXPORTER,
					   BM_SETCHECK, m_show_meshexporter ? BST_CHECKED : BST_UNCHECKED, 0);
	SendDlgItemMessage(hdlg, IDC_PREFERENCES_MATERIALEXPORTER,
					   BM_SETCHECK, m_show_materialexporter ? BST_CHECKED : BST_UNCHECKED, 0);
			
}

// ShutPrefsDlg: shuts preferences rollup panel
//==========================================================================================
void MayhemTool::ShutPrefsDlg(HWND hdlg)
{

}







// LoadConfig: loads plugin configuration
//==========================================================================================
void MayhemTool::LoadConfig()
{
	m_inifile->ReadBool("PanelsStatus", "PreferencesOpened", &m_prefs_opened); 
	m_inifile->ReadBool("PanelsStatus", "MaterialsOpened",	 &m_materials_opened);
	m_inifile->ReadBool("PanelsStatus", "MeshesOpened",		 &m_meshes_opened); 
	
	m_inifile->ReadBool("MayhemExporter", "ShowMeshExporter",		&m_show_meshexporter); 
	m_inifile->ReadBool("MayhemExporter", "ShowMaterialExporter",	&m_show_materialexporter);
	m_inifile->ReadString("MayhemExporter", "TexturesRoot",			m_texroot);
	m_inifile->ReadString("MayhemExporter", "MaterialsRoot",		m_mtlroot);
	m_inifile->ReadString("MayhemExporter", "MeshesRoot",			m_meshroot);
	
}

// SaveConfig: saves plugin configuration
//==========================================================================================
void MayhemTool::SaveConfig()
{
	IRollupWindow* pwnd = m_iface->GetCommandPanelRollup();
#pragma warning(disable:4800)
	bool isopen;
	if (m_hprefpanel)
	{
		isopen = (mmBool)pwnd->IsPanelOpen(pwnd->GetPanelIndex(m_hprefpanel));
		m_inifile->WriteBool("PanelsStatus", "PreferencesOpened", isopen); 		
	}
	else
		m_inifile->WriteBool("PanelsStatus", "PreferencesOpened", false); 		
	
	if (m_hmainpanel)
	{
		isopen = (mmBool)pwnd->IsPanelOpen(pwnd->GetPanelIndex(m_hmainpanel));
		m_inifile->WriteBool("PanelsStatus", "MaterialsOpened", isopen); 		
	}
	else
		m_inifile->WriteBool("PanelsStatus", "MaterialsOpened", false); 		
	
	if (m_hmeshpanel)
	{
		isopen = (mmBool)pwnd->IsPanelOpen(pwnd->GetPanelIndex(m_hmeshpanel));
		m_inifile->WriteBool("PanelsStatus", "MeshesOpened", isopen); 		
	}
	else
		m_inifile->WriteBool("PanelsStatus", "MeshesOpened", false); 		
#pragma warning(default:4800)
	
	m_inifile->WriteBool("MayhemExporter", "ShowMeshExporter",		m_show_meshexporter); 
	m_inifile->WriteBool("MayhemExporter", "ShowMaterialExporter",	m_show_materialexporter);
	m_inifile->WriteString("MayhemExporter", "TexturesRoot",		m_texroot);
	m_inifile->WriteString("MayhemExporter", "MaterialsRoot",		m_mtlroot);
	m_inifile->WriteString("MayhemExporter", "MeshesRoot",			m_meshroot);
	
}


// DoMaterialExport: exports materials
//==========================================================================================
void MayhemTool::DoMaterialExport()
{
	m_mtlexp.SetRootPath(m_mtlroot);
	m_mtlexp.SetTexturesRoot(m_texroot);
	m_mtlexp.AllowOverwrite(SendDlgItemMessage(m_hmainpanel, 
							IDC_CHECK_MTLEXPORT_OVERWRITE, BM_GETCHECK, 0, 0)
							== BST_CHECKED);
	for (mmUint i = 0; i < m_materials.Count(); i++)
		m_mtlexp.ExportMaterial(m_materials[i]);
}

// DoMeshExport: exports meshes
//==========================================================================================
void MayhemTool::DoMeshExport()
{
	m_meshexp.SetRoot(m_meshroot);
//	m_mtlexp.AllowOverwrite(SendDlgItemMessage(m_hmainpanel, 
//							IDC_CHECK_MTLEXPORT_OVERWRITE, BM_GETCHECK, 0, 0)
//							== BST_CHECKED);
	m_meshexp.EnableChannels(SendDlgItemMessage(m_hmeshpanel, IDC_CHECK_NORMALS, BM_GETCHECK, 0, 0) == BST_CHECKED,
							 SendDlgItemMessage(m_hmeshpanel, IDC_CHECK_MAPPING, BM_GETCHECK, 0, 0) == BST_CHECKED,
							 SendDlgItemMessage(m_hmeshpanel, IDC_CHECK_VCOLORS, BM_GETCHECK, 0, 0) == BST_CHECKED);
	for (mmUint i = 0; i < m_nodes.Count(); i++)
	{
		if (MHMFAIL(m_meshexp.ExportMesh(m_nodes[i])))
			MessageBox(m_iface->GetMAXHWnd(), clsString("Node ") + m_nodes[i]->GetName() + " export failed",
					   "Error" , MB_OK);
	}
}


// DoPreferencesDlg: show plugin preferences dialog
//==========================================================================================
void MayhemTool::DoPreferencesDlg()
{
	mmInt res = DialogBox(g_hinst, MAKEINTRESOURCE(IDD_PREFERENCES), 
						  m_iface->GetMAXHWnd(), PrefDlgProc);
	if (res == mmOk)
		SaveConfig();
}

// EnableMaterialExporter: enables (disables) material exporter
//==========================================================================================
void MayhemTool::EnableMaterialExporter()
{
	if (m_hmainpanel)
	{
		m_iface->DeleteRollupPage(m_hmainpanel);
		m_show_materialexporter = false;
		m_hmainpanel			= mmnull;
	}
	else
	{
		m_hmainpanel = m_iface->AddRollupPage(g_hinst, MAKEINTRESOURCE(IDD_MAYHEM_PANEL), 
											  DlgProc, GetString(IDS_PANEL_TITLE), 0,
											  m_materials_opened ? 0 : APPENDROLL_CLOSED);
		m_show_materialexporter = true;
		
	}
}

// EnableMeshExporter: enables (disables) mesh exporter
//==========================================================================================
void MayhemTool::EnableMeshExporter()
{
	if (m_hmeshpanel)
	{
		m_iface->DeleteRollupPage(m_hmeshpanel);
		m_show_meshexporter = false;
		m_hmeshpanel        = mmnull;
	}
	else
	{
		m_hmeshpanel = m_iface->AddRollupPage(g_hinst, MAKEINTRESOURCE(IDD_MESH_PANEL), 
											  MeshDlgProc, GetString(IDS_PANEL_MESH), 0,
											  m_meshes_opened ? 0 : APPENDROLL_CLOSED);
		m_show_meshexporter = true;
	}
}


//==========================================================================================
// The one and only mayhem tool
//==========================================================================================
MayhemTool	theTOOL;

//==========================================================================================
// Rollup page dialog proc
//==========================================================================================
INT_PTR CALLBACK DlgProc(HWND hdlg, UINT msg, WPARAM wpar, LPARAM lpar)
{
	switch (msg) 
	{
	case WM_INITDIALOG:
		theTOOL.Init(hdlg);
		break;	
	case WM_DESTROY:
		theTOOL.Shut(hdlg);
		break;
	case WM_MOUSEMOVE:
		theTOOL.m_iface->RollupMouseMessage(hdlg, msg, wpar, lpar);
		break;
	case WM_COMMAND:
		switch(LOWORD(wpar))
		{
		case IDC_BUTTON_EXPORT:
			theTOOL.DoMaterialExport();
			break;
		}
		break;
	default:
		return FALSE;
	}
	return TRUE;
}

//==========================================================================================
// Mesh rollup dlg proc
//==========================================================================================
INT_PTR CALLBACK MeshDlgProc(HWND hdlg, UINT msg, WPARAM wpar, LPARAM lpar)
{
	switch(msg)
	{
	case WM_INITDIALOG:
		theTOOL.InitMeshDlg(hdlg);
		break;
	case WM_DESTROY:
		theTOOL.ShutMeshDlg(hdlg);
		break;
	case WM_MOUSEMOVE:
		theTOOL.m_iface->RollupMouseMessage(hdlg, msg, wpar, lpar);
		break;
	case WM_COMMAND:
		switch(LOWORD(wpar))
		{
		case IDC_BUTTON_MESH_EXPORT:
			theTOOL.DoMeshExport();
			break;
		}
		break;
	default:
		return FALSE;
	}
	return TRUE;
}

//==========================================================================================
// Config panel dlg proc
//==========================================================================================
INT_PTR CALLBACK ConfigDlgProc(HWND hdlg, UINT msg, WPARAM wpar, LPARAM lpar)
{
	switch(msg)
	{
	case WM_INITDIALOG:
		theTOOL.InitPrefsDlg(hdlg);
		break;
	case WM_DESTROY:
		theTOOL.ShutPrefsDlg(hdlg);
		break;
	case WM_MOUSEMOVE:
		theTOOL.m_iface->RollupMouseMessage(hdlg, msg, wpar, lpar);
		break;
	case WM_COMMAND:
		switch(LOWORD(wpar))
		{
		case IDC_BUTTON_PREFERENCES:
			theTOOL.DoPreferencesDlg();
			break;
		case IDC_PREFERENCES_MESHEXPORTER:
			theTOOL.EnableMeshExporter();
			break;
		case IDC_PREFERENCES_MATERIALEXPORTER:
			theTOOL.EnableMaterialExporter();
			break;
		}
		break;
	default:
		return FALSE;
	}
	return TRUE;
}

//==========================================================================================
// Preferences dialog proc
//==========================================================================================
INT_PTR CALLBACK PrefDlgProc(HWND hdlg, UINT msg, WPARAM wpar, LPARAM lpar)
{
	clsString path;
	switch(msg)
	{
	case WM_INITDIALOG:
		SetDlgItemText(hdlg, IDC_MTLEXPORT_TEXTURES_ROOT, theTOOL.m_texroot);
		SetDlgItemText(hdlg, IDC_MTLEXPORT_MATERIALS_ROOT, theTOOL.m_mtlroot);
		SetDlgItemText(hdlg, IDC_MESHEXPORT_ROOT, theTOOL.m_meshroot);
		break;
	case WM_COMMAND:
		switch(LOWORD(wpar))
		{
		case IDCANCEL:
			EndDialog(hdlg, mmFail);
			break;
		case IDOK:
			path.Alloc(MAX_PATH);
			GetDlgItemText(hdlg, IDC_MTLEXPORT_TEXTURES_ROOT, path, MAX_PATH);
			theTOOL.m_texroot = path;
			GetDlgItemText(hdlg, IDC_MTLEXPORT_MATERIALS_ROOT, path, MAX_PATH);
			theTOOL.m_mtlroot = path;
			GetDlgItemText(hdlg, IDC_MESHEXPORT_ROOT, path, MAX_PATH);
			theTOOL.m_meshroot = path;
			EndDialog(hdlg, mmOk);
			break;
		case IDC_BUTTON_SELECT_TEXTURES_ROOT:
			path.Alloc(MAX_PATH);
			GetDlgItemText(hdlg, IDC_MTLEXPORT_TEXTURES_ROOT, path, MAX_PATH);
			path = ChooseDir(path, hdlg);
			if (!path.IsEmpty())
				SetDlgItemText(hdlg, IDC_MTLEXPORT_TEXTURES_ROOT, path);
			break;
		case IDC_BUTTON_SELECT_MATERIALS_ROOT:
			path.Alloc(MAX_PATH);
			GetDlgItemText(hdlg, IDC_MTLEXPORT_MATERIALS_ROOT, path, MAX_PATH);
			path = ChooseDir(path, hdlg);
			if (!path.IsEmpty())
				SetDlgItemText(hdlg, IDC_MTLEXPORT_MATERIALS_ROOT, path);
			break;
		}
		break;
	case WM_CLOSE:
		EndDialog(hdlg, mmFail);
		break;
	default:
		return FALSE;
	}
	return TRUE;
}


//==========================================================================================
// Class description
//==========================================================================================
class MayhemToolClassDesc: public ClassDesc
{
public:
	int			 IsPublic()		{return 1;}
	const TCHAR* ClassName()	{return _T(GetString(IDS_MHMTOOL_CLASS_NAME)); }
	SClass_ID	 SuperClassID() {return UTILITY_CLASS_ID;}
	Class_ID	 ClassID()		{return Class_ID(MHMTOOL_CLASS_A, MHMTOOL_CLASS_B);}
	const TCHAR* Category()		{return _T("");}
	void*		 Create(BOOL loading = false)
								{return &theTOOL;}
}Desc;

ClassDesc* GetMayhemToolDesc()
{
	return &Desc;
}