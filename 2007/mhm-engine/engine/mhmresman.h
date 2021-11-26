//==========================================================================================
// Mayhem engine resource manager class
// Coded:	MozG
// Created:	9 may 2006
//==========================================================================================

#ifndef _MHMRESMAN_INCLUDED_
#define _MHMRESMAN_INCLUDED_

#include "mhmresource.h"

// Resource manager config
#define MHMRESMAN_SECTION					"Resource manager"
#define MHMRESMAN_KEY_PATH_TEXTURES			"Textures path"
#define MHMRESMAN_KEY_PATH_MATERIALS		"Materials path"
#define MHMRESMAN_KEY_PATH_MESHES			"Meshes path"
#define MHMRESMAN_KEY_PATH_SHADERS			"Shaders path"
#define MHMRESMAN_KEY_PATH_MISC				"Misc path"

// Defaults
#define MHMRESMAN_DEFAULT_PATH_TEXTURES		"Textures/"
#define MHMRESMAN_DEFAULT_PATH_MESHES		"Meshes/"
#define MHMRESMAN_DEFAULT_PATH_MATERIALS	"Materials/"
#define MHMRESMAN_DEFAULT_PATH_SHADERS		"Shaders/"
#define MHMRESMAN_DEFAULT_PATH_MISC			"Misc/"


// Resources prothotypes
class clsVertexBuffer;
class clsIndexBuffer;

typedef tmpPtrList<clsResource*>	mmResList;
//typedef tmpRefArr<clsResource*> mmResList;

class clsResourceStats
{
public:
	mmUint	m_nFrameTrianges;
	mmUint	m_nFrameStreamChanges;
	mmUint	m_nFrameDIP;
	mmUint	m_nFrameDP;

public:
	clsResourceStats();
   ~clsResourceStats();
    
    
    mmVoid AddFrameRenderedTriangles(mmUint nCount);
    mmVoid AddStreamChange();
    mmVoid AddDIPCall();
    mmVoid AddDPCall();
    mmVoid Clear();
    
    clsString FormatStatistic();
};

class clsResourceManager
{
friend clsResource;
public:
	clsResourceStats m_Stats;
	
protected:
	clsDevice* m_pdev;
	
	mmResList	m_list_textures;
	mmResList	m_list_meshes;
	mmResList	m_list_materials;
	mmResList	m_list_shaders;
	mmResList	m_list_misc;
	
	// Paths
	clsString	m_path_textures;
	clsString	m_path_materials;
	clsString	m_path_meshes;
	clsString	m_path_shaders;
	clsString	m_path_misc;
	
public:
	clsResourceManager(clsDevice* pdev);
   ~clsResourceManager();
    
    
    mmRes				Init(const mmString inifile);
    clsResource*		GetResource(clsString name, 
									enResType type, 
									clsResourceParams* params=mmnull,
									mmBool* fail = mmnull);
    clsResource*		GetResource(clsString name, 
									mmWord hlid,
									clsResourceParams* params=mmnull,
									mmBool* fail = mmnull);
	clsResource*		GetResource(clsResource* pRes, 
									clsResourceParams* params = mmnull,
									mmBool* fail = mmnull);
    clsVertexBuffer*	CreateVertexBuffer();
    clsIndexBuffer*		CreateIndexBuffer();
    clsVertexDecl*		CreateVertexDeclaration(mmUint numitems);
    clsDevice*			GetDevice();
    clsResourceParams*	CreateResourceParams(clsString name, enResType type);
    clsString			GetTexturesPath();
    clsString			GetMaterialsPath();
    clsString			GetMeshesPath();
	clsString			GetMiscPath();

    mmVoid				DestroyVertexBuffer(clsVertexBuffer*);
    mmVoid				DestroyIndexBuffer(clsIndexBuffer*);
	
	mmRes				Update();

protected:
	clsResource*		NewResource(clsString name, enResType type);
	mmRes				MakeFullName(clsResource* pres);
	mmRes				MakeFullName(mmWord hlid, clsString name, clsString& sOut);
	mmRes				ReleaseResource(clsResource* pres);
	mmRes				BeforeReset();
	mmRes				AfterReset();
	mmRes				SaveDefaultConfig();

private:
	mmResList*			GetListByType(enResType type);
	mmVoid				ReleaseReferences();
};


#endif // _MHMRESMAN_INCLUDED_