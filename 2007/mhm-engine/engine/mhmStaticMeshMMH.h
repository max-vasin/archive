//==========================================================================================
// Mayhem engine static mesh MMH format file
// Coded:	MozG
// Created:	7 june 2006
//==========================================================================================

#ifndef _MHMSTATICMESHMMH_INCLUDED
#define _MHMSTATICMESHMMH_INCLUDED

//==============================================================
// Mesh file header
//==============================================================
struct stMMHMeshFileHeader
{
	mmDword		m_numsubs;
	mmDword		m_numverts;
	mmDword		m_numfaces;
	mmBool		m_chnl_normals;
	mmBool		m_chnl_mapping;
};

//==============================================================
// Mesh subset file header
//==============================================================
struct stMMHMeshSubset
{
	mmDword	m_numfaces;
	mmDword m_mtlindex;
};



//==============================================================
// Static MMH mesh class
//==============================================================
class clsStaticMeshMMH: public clsBaseMesh
{
public:
	clsStaticMeshMMH(clsResourceManager* presman, clsString name);
   ~clsStaticMeshMMH();
    
    mmRes  OnCreateMesh(clsResourceParams* params);
//    mmVoid ClearReferences();
};

#endif // _MHMSTATICMESHMMH_INCLUDED
