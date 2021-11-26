//==========================================================================================
// Mayhem engine resource manager class implementation
// Coded:	MozG
// Created:	9 may 2006
//==========================================================================================
#include "mhmbase.h"

//==========================================================================================
// Resource statistics
//==========================================================================================
clsResourceStats::clsResourceStats()
{
	m_nFrameTrianges		= 0;
	m_nFrameStreamChanges	= 0;
	m_nFrameDIP				= 0;
	m_nFrameDP				= 0;
}
//==========================================================================================
clsResourceStats::~clsResourceStats()
{

}
//==========================================================================================
mmVoid clsResourceStats::AddFrameRenderedTriangles(mmUint nCount)
{
	m_nFrameTrianges += nCount;
}
//==========================================================================================
mmVoid clsResourceStats::AddStreamChange()
{
	m_nFrameStreamChanges++;
}
//==========================================================================================
mmVoid clsResourceStats::AddDIPCall()
{
	m_nFrameDIP++;
}
//==========================================================================================
mmVoid clsResourceStats::AddDPCall()
{
	m_nFrameDP++;
}
//==========================================================================================
mmVoid clsResourceStats::Clear()
{
	m_nFrameTrianges		= 0;
	m_nFrameStreamChanges	= 0;
	m_nFrameDIP				= 0;
	m_nFrameDP				= 0;
}
//==========================================================================================
//==========================================================================================
clsString clsResourceStats::FormatStatistic()
{
	clsString sFloat, sOut;
	sOut = "Resource usage statistics\n";
	sOut += "Triangles drawed: " + clsString(m_nFrameTrianges) + "\n";
	sOut += "DrawIP calls: " + clsString(m_nFrameDIP) + "\n";
	sOut += "DrawP calls: " + clsString(m_nFrameDP) + "\n";
	sOut += "Vertex stream changes: " + clsString(m_nFrameStreamChanges) + "\n";
	
	return sOut;
}






// Resource manager constructor
//==========================================================================================
clsResourceManager::clsResourceManager(clsDevice *pdev):m_list_textures(mmtrue, 0),
														m_list_meshes(mmtrue, 1),
														m_list_materials(mmtrue, 2),
														m_list_shaders(mmtrue, 3),
														m_list_misc(mmtrue, 4)
{
	m_pdev = pdev;

	// for Particles
	PEDLL_Open(pdev);
}

// Resource manager destructor
//==========================================================================================
clsResourceManager::~clsResourceManager()
{
	ReleaseReferences();

	// for Particles
	PEDLL_Close();
	
	m_pdev = mmnull;
}

// Init: initializes manager enviropment
//==========================================================================================
mmRes clsResourceManager::Init(const mmString inifile)
{
	clsINIFile ini(inifile);
	
	// Textures path reading
	if (MHMFAIL(ini.ReadString(MHMRESMAN_SECTION, 
							   MHMRESMAN_KEY_PATH_TEXTURES,
							   m_path_textures)))
	{
		if (MHMFAIL(ini.WriteString(MHMRESMAN_SECTION,
									MHMRESMAN_KEY_PATH_TEXTURES,
									MHMRESMAN_DEFAULT_PATH_TEXTURES)))
		{
			g_plog->Out("clsResourceManager::Init >> cant write default value for textures path", enLCRed);
			return mmFail;
		}
		m_path_textures = MHMRESMAN_DEFAULT_PATH_TEXTURES;
	}
	
	// Materials path reading
	if (MHMFAIL(ini.ReadString(MHMRESMAN_SECTION, 
							   MHMRESMAN_KEY_PATH_MATERIALS,
							   m_path_materials)))
	{
		if (MHMFAIL(ini.WriteString(MHMRESMAN_SECTION,
									MHMRESMAN_KEY_PATH_MATERIALS,
									MHMRESMAN_DEFAULT_PATH_MATERIALS)))
		{
			g_plog->Out("clsResourceManager::Init >> cant write default value for materials path", enLCRed);
			return mmFail;
		}
		m_path_materials = MHMRESMAN_DEFAULT_PATH_MATERIALS;
	}
	
	// Meshes path reading
	if (MHMFAIL(ini.ReadString(MHMRESMAN_SECTION, 
							   MHMRESMAN_KEY_PATH_MESHES,
							   m_path_meshes)))
	{
		if (MHMFAIL(ini.WriteString(MHMRESMAN_SECTION,
									MHMRESMAN_KEY_PATH_MESHES,
									MHMRESMAN_DEFAULT_PATH_MESHES)))
		{
			g_plog->Out("clsResourceManager::Init >> cant write default value for materials path", enLCRed);
			return mmFail;
		}
		m_path_meshes = MHMRESMAN_DEFAULT_PATH_MESHES;
	}
	
	// Sahders path reading
	if (MHMFAIL(ini.ReadString(MHMRESMAN_SECTION, 
							   MHMRESMAN_KEY_PATH_SHADERS,
							   m_path_shaders)))
	{
		if (MHMFAIL(ini.WriteString(MHMRESMAN_SECTION,
									MHMRESMAN_KEY_PATH_SHADERS,
									MHMRESMAN_DEFAULT_PATH_SHADERS)))
		{
			g_plog->Out("clsResourceManager::Init >> cant write default value for shaders path", enLCRed);
			return mmFail;
		}
		m_path_shaders = MHMRESMAN_DEFAULT_PATH_SHADERS;
	}
	// Sahders path reading
	if (MHMFAIL(ini.ReadString(MHMRESMAN_SECTION, 
							   MHMRESMAN_KEY_PATH_MISC,
							   m_path_misc)))
	{
		if (MHMFAIL(ini.WriteString(MHMRESMAN_SECTION,
									MHMRESMAN_KEY_PATH_MISC,
									MHMRESMAN_DEFAULT_PATH_MISC)))
		{
			g_plog->Out("clsResourceManager::Init >> cant write default value for shaders path", enLCRed);
			return mmFail;
		}
		m_path_misc = MHMRESMAN_DEFAULT_PATH_MISC;
	}
// TODO: Make full paths based on executable location

	return mmOk;
}



// GetResource: returns resource by name, type and params
//==========================================================================================
clsResource* clsResourceManager::GetResource(clsString name,
											 enResType type,
											 clsResourceParams* params,
											 mmBool* fail)
{
	mmInt nLight = 0;
	clsBaseLight *l; 
	clsResource *r; 
	mmResList* plist = GetListByType(type);

	if (plist == mmnull)
	{
		g_plog->Out("clsResourceManager::GetResource >> unknown resource class id [" + clsString(mmHLID(type)) + "]", enLCRed);
		if (fail != mmnull)
			*fail = mmtrue;	
		return mmnull;
	}
	
	if (!name.IsEmpty())
		for (mmUint i = 0; i < plist->Count(); i++)
			if (((*plist)[i]->GetName() == name) &&
				((*plist)[i]->GetType() == type))
			{
				if (fail != mmnull)
					*fail = mmfalse;
				return (*plist)[i]->AddRef();
			}

	clsResource* pres = NewResource(name, type);
	if (pres == mmnull)
	{
		if (fail != mmnull)
			*fail = mmtrue;
		return mmnull;
	}


	if(pres->GetType() == enResBaseLight)
	{
		for (mmUint i = 0; i < plist->Count(); i++)
		{				
				r = ((*plist)[i]);
				if(r->GetType() != enResBaseLight) continue;
				l = (clsBaseLight *)r;
				if(nLight <  (l->m_Number))
					nLight = (l->m_Number);
		}
		((clsBaseLight*)(pres))->m_Number = nLight+1;
	}

	if (!name.IsEmpty())
	{	
		MakeFullName(pres);
		if (MHMFAIL(pres->Create(params)))
		{
			g_plog->Out("clsResourceManager::GetResource[" + name + "] >> resource Create method failed", enLCRed);
			if (fail != mmnull)
				*fail = mmtrue;
			plist->Add(pres);
			return pres->AddRef();
		}
	}
	
	plist->Add(pres);
	if (fail != mmnull)
		*fail = mmfalse;
	return pres->AddRef();
}

// GetResource: automatic resource type detection
//==========================================================================================
clsResource* clsResourceManager::GetResource(clsString name, 
											 mmWord hlid,
											 clsResourceParams* params,
											 mmBool* fail)
{
	clsString ext = name.CutBy('.', mmfalse);
	ext.UpperCase();
	clsString sFullName;
	MakeFullName(hlid, name, sFullName);
	sFullName = sFullName.CutBy('.', mmtrue);
	clsFile* pFile;
	switch(hlid)
	{
	case MHMRES_TEXTURE_ID:
		//fyrex 16 apr 2007
		if(name[0] == '$')
		{
			
			return GetResource(name, enResBaseTexture, params, fail);
		}
		//

		if(ext != "MJT")
		{
			// check files
			pFile = new clsFile(sFullName + "." + ext);
			if (!pFile->IsExist())
			{
				ext = "JP2";
				mmSafeDelete(pFile);
				pFile = new clsFile(sFullName + "." + ext);
				if (!pFile->IsExist())
				{
					ext = "JPG";
					mmSafeDelete(pFile);
					pFile = new clsFile(sFullName + "." + ext);
					if (!pFile->IsExist())
					{
						ext = "TGA";
						mmSafeDelete(pFile);
						pFile = new clsFile(sFullName + "." + ext);
						if (!pFile->IsExist())
						{
							g_plog->Out("Texture loading error: no appropriate texture to [" + name + "]", enLCRed);
							mmSafeDelete(pFile);
							return mmnull;
						}
					}
				}
			}

			
			mmSafeDelete(pFile);
		}else{
			ext = "JP2";	
		}

		name = name.CutBy('.') + "." + ext;
		

		if (ext == "TGA")
			return GetResource(name, enResTextureTGA, params, fail);
		else if (ext == "JPG")
			return GetResource(name, enResTextureJPG, params, fail);
		else if (ext == "JP2")
			return GetResource(name, enResTextureJP2, params, fail);
		break;
	case MHMRES_MESH_ID:
		if (ext == "MMH")
			return GetResource(name, enResStaticMeshMMH, params, fail);
		break;
	case MHMRES_MATERIAL_ID:
		if (ext == "MTL")
			return GetResource(name, enResBaseMaterial, params, fail);
		break;
	case MHMRES_SHADER_ID:
		if ((ext == "VSH") || (ext == "VSO"))
			return GetResource(name, enResVertexShader, params, fail);
		else if ((ext == "PSH") || (ext == "PSO"))
			return GetResource(name, enResPixelShader, params, fail);
		break;
	case MHMRES_MISC_ID:
		if (ext == "MHMFONT2D")
			return GetResource(name, enResText, params, fail);
		if (ext == "SPR")
			return GetResource(name, enResSprite, params, fail);
		break;
	}
	g_plog->Out("clsResourceManager::GetResource[" + name + "] >> cant detect resource type automatically", enLCRed);
	return mmnull;
};

// GetResource: returns resource reference
//==========================================================================================
clsResource* clsResourceManager::GetResource(clsResource *pRes, 
											 clsResourceParams* params,
											 mmBool *fail)
{
	return GetResource(pRes->GetName(), pRes->GetType(), params, fail);
}


// CreateVertexBuffer: creates new vertex buffer
//==========================================================================================
clsVertexBuffer* clsResourceManager::CreateVertexBuffer()
{
	return new clsVertexBuffer(this);
}

// CreateIndexBuffer: creates new index buffer
//==========================================================================================
clsIndexBuffer* clsResourceManager::CreateIndexBuffer()
{
	return new clsIndexBuffer(this);
}

// CreateVertexDeclaration: creates vertex declaration
//==========================================================================================
clsVertexDecl* clsResourceManager::CreateVertexDeclaration(mmUint numitems)
{
	return new clsVertexDecl(m_pdev, numitems);
}

// GetDevice: returns resource manager device
//==========================================================================================
clsDevice* clsResourceManager::GetDevice()
{
	return m_pdev;
}

// Update: updates resource manager depencies
//==========================================================================================
mmRes clsResourceManager::Update()
{
	mmUint i;
	for (i = 0; i < m_list_textures.Count(); i++)
		m_list_textures[i]->Update();
	for (i = 0; i < m_list_meshes.Count(); i++)
		m_list_meshes[i]->Update();
	for (i = 0; i < m_list_materials.Count(); i++)
		m_list_materials[i]->Update();
	for (i = 0; i < m_list_shaders.Count(); i++)
		m_list_shaders[i]->Update();
	for (i = 0; i < m_list_misc.Count(); i++)
		m_list_misc[i]->Update();
	
	m_Stats.Clear();
	return mmOk;
}

// NewResource: makes new resource
//==========================================================================================
clsResource* clsResourceManager::NewResource(clsString name, enResType type)
{
#define NER(a,b) 	case a:\
		pres = new b(this, name);\
		break;			

#define NR(x) 	case enRes##x:\
		pres = new cls##x(this, name);\
		break;			

	clsResource* pres = mmnull;

	switch(type)
	{
		NR(BaseTexture);

	//case enResBaseTexture:
		//pres = new clsBaseTexture(this, name);
		//break;			
	case enResTextureTGA:
		pres = new clsTextureTGA(this, name);
		break;
	case enResBaseMesh:
		pres = new clsBaseMesh(this, name);
		break;
	case enResStaticMesh:
		pres = new clsStaticMesh(this, name);
		break;
	case enResSimpleBox:
		pres = new clsSimpleBox(this, name);
		break;
	case enResBaseMaterial:
		pres = new clsBaseMaterial(this, name);
		break;
	case enResFont2D:
		pres = new clsFont2D(this, name);
		break;
	case enResStaticMeshMMH:
		pres = new clsStaticMeshMMH(this, name);
		break;
	case enResTextureJPG:
		pres = new clsTextureJPG(this, name);
		break;
	case enResTextureJP2:
		pres = new clsTextureJP2(this, name);
		break;
	case enResVertexShader:
		pres = new clsVertexShader(this, name);
		break;
	case enResPixelShader:
		pres = new clsPixelShader(this, name);
		break;
	case enResText:
		pres = new clsText(this, name);
		break;
	case enResAnimKeys:
		pres = new clsAnimKeys(this, name);
		break;
	case enResFont:
		pres = new clsFont(this, name);
		break;
	case enResBaseLight:
		pres = new clsBaseLight(this, name);
		break;
	case enResBaseParticles:
		pres = new clsBaseParticles(this, name);
		break;
	case enResSprite:
		pres = new clsSprite(this, name);
		break;
	case enResRectangle:
		pres = new clsRectangle(this, name);
		break;
	default:
		g_plog->Out("clsResourceManager::NewResource >> unsupported resource type", enLCRed);
		break;
	}
	return pres;
}

// MakeFullName: makes resource fullname
//==========================================================================================
mmRes clsResourceManager::MakeFullName(clsResource *pres)
{
	return MakeFullName(mmHLID(pres->GetType()), pres->m_name, pres->m_fullname);
}

mmRes clsResourceManager::MakeFullName(mmWord hlid, 
									   clsString name,
									   clsString& sOut)
{
	switch(hlid)
	{
	case MHMRES_TEXTURE_ID: 	sOut = m_path_textures  + name; break;
	case MHMRES_MATERIAL_ID:	sOut = m_path_materials + name; break;
	case MHMRES_MESH_ID:		sOut = m_path_meshes    + name; break;
	case MHMRES_SHADER_ID:		sOut = m_path_shaders   + name; break;
	case MHMRES_MISC_ID:		sOut = m_path_misc      + name; break;
	}
	return mmOk;
}

// ReleaseResource: called by resource when before it self-destruct
//==========================================================================================
mmRes clsResourceManager::ReleaseResource(clsResource* pres)
{
	if (pres == mmnull)
	{
		g_plog->Out("clsResourceManager::ReleaseResource >> null resource pointer", enLCRed);
		return mmFail;
	}
	mmResList* plist = GetListByType(pres->GetType());
	if (plist == mmnull)
	{
		g_plog->Out("clsResourceManager::ReleaseResource >> cant find any list for resource type", enLCRed);
		return mmFail;
	}
	plist->Remove(pres);
	return mmOk;
}

// BeforeReset: called by device when its being reset
//==========================================================================================
mmRes clsResourceManager::BeforeReset()
{
	mmUint i;
	for (i = 0; i < m_list_textures.Count(); i++)
		m_list_textures[i]->Invalidate(mmnull);
	for (i = 0; i < m_list_meshes.Count(); i++)
		m_list_meshes[i]->Invalidate(mmnull);
	for (i = 0; i < m_list_materials.Count(); i++)
		m_list_materials[i]->Invalidate(mmnull);
	for (i = 0; i < m_list_shaders.Count(); i++)
		m_list_shaders[i]->Invalidate(mmnull);
	for (i = 0; i < m_list_misc.Count(); i++)
		m_list_misc[i]->Invalidate(mmnull);
	return mmOk;
}

// AfterReset: called by device after reset
//==========================================================================================
mmRes clsResourceManager::AfterReset()
{
	mmUint i;
	for (i = 0; i < m_list_textures.Count(); i++)
		m_list_textures[i]->Restore(mmnull);
	for (i = 0; i < m_list_meshes.Count(); i++)
		m_list_meshes[i]->Restore(mmnull);
	for (i = 0; i < m_list_materials.Count(); i++)
		m_list_materials[i]->Restore(mmnull);
	for (i = 0; i < m_list_shaders.Count(); i++)
		m_list_shaders[i]->Restore(mmnull);
	for (i = 0; i < m_list_misc.Count(); i++)
		m_list_misc[i]->Restore(mmnull);
	return mmOk;
}

// GetListByType: resturns resource's appropriate list
//==========================================================================================
mmResList* clsResourceManager::GetListByType(enResType type)
{
	switch(mmHLID(type))
	{
	case MHMRES_TEXTURE_ID: 	return &m_list_textures;
	case MHMRES_MATERIAL_ID:	return &m_list_materials;
	case MHMRES_MESH_ID:		return &m_list_meshes;
	case MHMRES_SHADER_ID:		return &m_list_shaders;
	case MHMRES_MISC_ID:		return &m_list_misc;	
	}
	return mmnull;
}

// GetMaterialsPath: resturns materials resource path
//==========================================================================================
clsString clsResourceManager::GetMaterialsPath()
{
	return m_path_materials;
}

// GetTexturesPath: resturns textures resource path
//==========================================================================================
clsString clsResourceManager::GetTexturesPath()
{
	return m_path_textures;
}

// GetMeshesPath: resturns meshes resource path
//==========================================================================================
clsString clsResourceManager::GetMeshesPath()
{
	return m_path_meshes;
}

// GetMiscPath: resturns miscellanous resource path
//==========================================================================================
clsString clsResourceManager::GetMiscPath()
{
	return m_path_misc;
}

// ReleaseReferences: releases all references between resources
//==========================================================================================
mmVoid clsResourceManager::ReleaseReferences()
{
	mmUint i;
	for (i = 0; i < m_list_textures.Count(); i++)
		m_list_textures[i]->~clsResource();// ClearReferences();
	for (i = 0; i < m_list_meshes.Count(); i++)
		m_list_meshes[i]->~clsResource();
	for (i = 0; i < m_list_materials.Count(); i++)
		m_list_materials[i]->~clsResource();//ClearReferences();
	for (i = 0; i < m_list_shaders.Count(); i++)
		m_list_shaders[i]->~clsResource();//ClearReferences();
	for (i = 0; i < m_list_misc.Count(); i++)
		m_list_misc[i]->~clsResource();//ClearReferences();	
}



mmVoid clsResourceManager::DestroyVertexBuffer(clsVertexBuffer* x)
{
	mmSafeDelete(x);
}

mmVoid clsResourceManager::DestroyIndexBuffer(clsIndexBuffer* x)
{
	mmSafeDelete(x);
}


