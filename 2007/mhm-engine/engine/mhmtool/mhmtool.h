//==========================================================================================
// Mayhem engine 3DS Max tools plugin header file
// Coded:	MozG
// Created:	1 june 2006
//==========================================================================================
#ifndef _MHMTOOL_INCLUDED
#define _MHMTOOL_INCLUDED

#include <stdio.h>
#include <shlobj.h>
#include "max.h"
#include "resource.h"
#include "iparamm.h"
#include "iparamb2.h"
#include "iparamm2.h"
#include "stdmat.h"
#include "bmmlib.h"
#include "utilapi.h"
#include "CustAttrib.h"
#include "ICustAttribContainer.h"

#include "../mhmtypes.h"	// mayhem types definitions
#include "../mhmException.h"
#include "../mhmmacro.h"	// mayhem macros definitions
#include "../mhmlist.h"	// mayhem list template definition
#include "../mhmstring.h"	// mayhem string class
#ifdef MHM_ZIPUSE
#undef MHM_ZIPUSE
#endif
#include "../mhmfile.h"	// mayhem file class
#include "../mhmmath.h"	// mayhem macth classes
#include "../mhmini.h"
#include "mtlexport.h"
#include "meshexport.h"

TCHAR* GetString(int id);

class MayhemTool: public UtilityObj
{
public:
	Interface*			m_iface;
	IUtil*				m_iutil;
	HWND				m_hprefpanel;
	HWND				m_hmainpanel;
	HWND				m_hmeshpanel;
	tmpPtrList<INode*>	m_nodes;
	tmpPtrList<Mtl*>	m_materials;
	clsMaterialExporter	m_mtlexp;
	clsMeshExporter		m_meshexp;
	clsINIFile*			m_inifile;
	
	clsString			m_texroot;
	clsString			m_mtlroot;
	clsString			m_meshroot;
	
	mmBool				m_show_meshexporter;
	mmBool				m_show_materialexporter;
	
	// Panels open status
	mmBool				m_prefs_opened;
	mmBool				m_meshes_opened;
	mmBool				m_materials_opened;
	
	
public:
	MayhemTool();
   ~MayhemTool();
    
    // UtilityObj interface support
    void BeginEditParams(Interface *ip,IUtil *iu);
    void EndEditParams(Interface *ip,IUtil *iu);
    void SelectionSetChanged(Interface *ip,IUtil *iu);
    void DeleteThis();
    
    void Init(HWND hdlg);
    void Shut(HWND hdlg);
    
    void InitMeshDlg(HWND hdlg);
    void ShutMeshDlg(HWND hdlg);
    
    void InitPrefsDlg(HWND hdlg);
    void ShutPrefsDlg(HWND hdlg);
    
    void LoadConfig();
    void SaveConfig();
    
    void DoMaterialExport();
    void DoMeshExport();
    void DoPreferencesDlg();
    
    void EnableMeshExporter();
    void EnableMaterialExporter();
};

#define MHM_MAP_CUSTOM_ATTRIB_CLASS_ID		Class_ID(0x6a014fe0, 0x279d670c)

#endif // _MHMTOOL_INCLUDED