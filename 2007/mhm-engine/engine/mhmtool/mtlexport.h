//==========================================================================================
// Mayhem engine 3DS Max tool - material exporter class
// Coded:	MozG
// Created: 2 june 2006
//==========================================================================================

#ifndef _MTLEXPORT_INCLUDED
#define _MTLEXPORT_INCLUDED


//=====================================================
// Material exporter class
//=====================================================
class clsMaterialExporter
{
public:
	clsString	m_root;
	clsString	m_texroot;
	mmBool		m_boverw;
	mmBool		m_asanim;
	
public:
	clsMaterialExporter();
   ~clsMaterialExporter();
    
    mmRes SetRootPath(clsString root);
    mmRes SetTexturesRoot(clsString root);
    mmRes AllowOverwrite(mmBool allow);
    mmRes UVOffsetAsAnimation(mmBool allow);
    mmRes ExportMaterial(Mtl* pmtl);

protected:
	mmRes WriteParam(clsFile& file, clsString name, mmFloat val);
	mmRes WriteParam(clsFile& file, clsString name, Color val);
	mmRes WriteParam(clsFile& file, clsString name, clsString val, mmBool bNoSemicolons = mmfalse);
	//mmRes WriteParam(clsFile& file, clsString name, clsString val, clsString valadd);
	mmRes ArrayOpen(clsFile& file, clsString name);
	mmRes ArrayAdd(clsFile& file, clsString val, mmBool last);
	mmRes ArrayAdd(clsFile& file, mmBool val, mmBool last);
	mmRes ArrayAdd(clsFile& file, mmFloat val, mmBool last);
	mmRes ArrayAdd(clsFile& file, mmInt val, mmBool last);
	
	mmRes ArrayClose(clsFile& file);
	clsString GetMapName(clsString slotname);
	clsString RootRelative(clsString path);
};

#endif // _MTLEXPORT_INCLUDED