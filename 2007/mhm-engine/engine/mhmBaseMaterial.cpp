#include "mhmbase.h"

//====================================================
// Base material map class
//====================================================


static mmBool g_StageFilteringStates[] = {mmfalse, mmfalse, mmfalse};
static mmBool g_StageWrappingStates[]  = {mmfalse, mmfalse, mmfalse};

// Base map constructor
//==========================================================================================
clsBaseMap::clsBaseMap():m_speed(0.0f, 0.0f),
						 m_uvoffset(0.0f, 0.0f),
						 m_curpos(0.0f, 0.0f)
{
	m_animated = mmfalse;
	m_tform.Identity();
	m_texture = mmnull;
	m_speed_r = 0.0f;
	m_currot  = 0.0f;
	m_pvshader = mmnull;
	m_ppshader = mmnull;
	m_alphablend = mmfalse;
	m_filtering_enable = mmtrue;
	m_wrapping_enable = mmtrue;
	m_enabled = mmtrue;
	m_bAdditive = mmfalse;
}

// Base map destructor
//==========================================================================================
clsBaseMap::~clsBaseMap()
{
	mmSafeRelease(m_texture);
	mmSafeRelease(m_pvshader);
	mmSafeRelease(m_ppshader);
}

// Update transform
//==========================================================================================
mmVoid clsBaseMap::UpdateTransform(mmFloat k)
{
	if (!m_animated)
		return;
	
	m_curpos.x += m_speed.x * k;
	m_curpos.y += m_speed.y * k;
	m_currot   += m_speed_r * k;
	clsMatrix mxrot, mxoffset, mxzero, mxspeed;
	mxoffset.Identity();
	mxzero.Identity();
	mxspeed.Identity();
	mxrot.RotationZ(m_currot);
	mxoffset._31 = -0.5f;
	mxoffset._32 = -0.5f;
	mxzero._31   = 0.5f;
	mxzero._32   = 0.5f;
	mxspeed._31  = m_curpos.x;
	mxspeed._32  = m_curpos.y;
	m_tform = mxoffset * mxrot * mxzero * mxspeed;
}



clsBaseMaterial::clsBaseMaterial(clsResourceManager *presman, clsString name, enResType type):
						   clsResource(presman, name, type)
{

	m_vshader	 = mmnull;
	m_pshader	 = mmnull;
	m_eBlendType = enBlendNone;
	mmZeroVar(m_dxmtl);
	
	m_bTwoSided  = mmfalse;
	m_bWireframe = mmfalse;
	m_pDevice = m_presman->GetDevice();
}

clsBaseMaterial::~clsBaseMaterial(void)
{
	mmSafeRelease(m_vshader);
	mmSafeRelease(m_pshader);
}

mmVoid clsBaseMaterial::SetColorLong(D3DCOLORVALUE *c, mmDword d)
{
	float k = 1.0f/255.0f;
	c->r = k * ((float)(d>>24));
	c->g = k * ((float)((d>>16)&255));
	c->b = k * ((float)((d>>8)&255));
	c->a = k * ((float)((d)&255));
}

mmVoid clsBaseMaterial::SetColor(D3DCOLORVALUE *c, mmFloat *rgb, mmFloat a)
{
	c->r = rgb[0];
	c->g = rgb[1];
	c->b = rgb[2];
	c->a = a;
}


// Create: creates material
//==========================================================================================
mmRes clsBaseMaterial::Create(clsResourceParams* params)
{
	clsString filename = (m_fullname.IsEmpty()) ? m_name : m_fullname;
	clsFile file(filename);
	if (file.IsExist())
	{
		if (MHMFAIL(LoadBaseMaterial(filename)))
		{
			g_plog->Out("clsBaseMaterial::Create[" + m_name + "] >> file parsing error", enLCRed);
			return mmFail;	
		}
	}
	else
	{
		g_plog->Out("clsBaseMaterial::Create[" + m_name + "] >> material file not exist", enLCRed);
		return mmFail;
	}
	g_plog->Out("clsBaseMaterial::Create[" + m_name + "] >> material created", enLCGreen);
	return mmOk;
}

// Activate: activates material 
//==========================================================================================
mmRes clsBaseMaterial::Activate(clsResource* powner)
{
	clsDevice* pdev = m_presman->GetDevice();
	if (pdev == mmnull)
		return mmFail;
		
	MHMDirect3DDevice* pddev = *pdev;
	if (pddev == mmnull)
		return mmFail;

	if (FAILED(pddev->SetMaterial(&m_dxmtl)))
		return mmFail;

	mmUint stage = 0;
	
	if (MHMFAIL(pdev->EnableWireframe(m_bWireframe)))
		return mmFail;

	if (MHMFAIL(pdev->EnableCulling(!m_bTwoSided)))
		return mmFail;
	
	if (MHMFAIL(SetBlendMode()))
		return mmFail;
	
	if ((m_map_emission.m_texture != mmnull) &&
	    (m_map_emission.m_enabled))
	{
		SetStageMap(stage, &m_map_emission, mmtrue, mmfalse);
		stage++;
	}
	
	if ((m_map_diffuse.m_texture != mmnull) &&
	    (m_map_diffuse.m_enabled))
	{
		SetStageMap(stage, &m_map_diffuse, mmfalse, mmfalse);
		stage++;
	}

	if ((m_map_reflection.m_texture != mmnull) &&
	    (m_map_reflection.m_enabled))
	{
		SetStageMap(stage, &m_map_reflection, mmfalse, mmtrue);
		stage++;
	}
	
	for (mmUint i = stage; i < 3; i++)
	{
		pddev->SetTexture(i, NULL);
		pddev->SetTextureStageState(i, D3DTSS_TEXTURETRANSFORMFLAGS,
									   D3DTTFF_DISABLE);
	}
	return mmOk;
}

// Deactivate: deactivates resource
//==========================================================================================
mmRes clsBaseMaterial::Deactivate(clsResource* powner)
{
	clsDevice* pdev = m_presman->GetDevice();
	if (pdev == mmnull)
		return mmFail;
	MHMDirect3DDevice* pddev = *pdev;
	if (pddev == mmnull)
		return mmFail;
	
//	pddev->SetVertexShader(mmnull);
//	pddev->SetPixelShader(mmnull);
	return mmOk;
}


// Update: resource event
//==========================================================================================
mmRes clsBaseMaterial::Update()
{
	clsTimer* ptimer = m_presman->GetDevice()->GetRenderer()->GetEngine()->GetTimer();
	mmFloat delta = ptimer->GetDeltaSecs();
	
	if (m_map_diffuse.m_animated)
		m_map_diffuse.UpdateTransform(delta);
	if (m_map_emission.m_animated)
		m_map_emission.UpdateTransform(delta);
	if (m_map_reflection.m_animated)
		m_map_reflection.UpdateTransform(delta);
	if (m_map_bump.m_animated)
		m_map_bump.UpdateTransform(delta);
	return mmOk;
}

// LoadBaseMaterial: parse file and loads base material parameters
//==========================================================================================
mmRes clsBaseMaterial::LoadBaseMaterial(clsString &file)
{

	clsVarParser parser;
	if (MHMFAIL(parser.ParseFile(file)))
	{
		g_plog->Out("clsBaseMaterial::LoadBaseMaterial[" + m_name + "] >> file parsing failed", enLCRed);
		return mmFail;
	}
	clsVarList* plist = parser.GetVarList();
	clsVariable* var;
	clsVarList arr;

	
	// Whole opacity
	mmFloat opacity = 1.0f;
	var = plist->FindVariable("Opacity");
	if (var == mmnull)
		g_plog->Out("clsBaseMaterial::LoadBaseMaterial[" + m_name + "] >> opacity variable not founded", enLCYellow);
	else
		opacity = *var;
		
	// Strength
	mmFloat strength = 0.0f;
	var = plist->FindVariable("Strength");
	if (var == mmnull)
		g_plog->Out("clsBaseMaterial::LoadBaseMaterial[" + m_name + "] >> strength variable not founded", enLCYellow);
	else
		strength = *var;
	
	// Shininess
	var = plist->FindVariable("Shininess");
	if (var == mmnull)
	{
		g_plog->Out("clsBaseMaterial::LoadBaseMaterial[" + m_name + "] >> shininess variable not founded", enLCYellow);
		m_dxmtl.Power = 0.0f;
	}
	else
		m_dxmtl.Power = 100.0f * (mmFloat)(*var);
	
	
	// Diffuse color
	var = plist->FindVariable("DiffuseColor");
	if (var == mmnull)
		g_plog->Out("clsBaseMaterial::LoadBaseMaterial[" + m_name + "] >> diffuse color variable not founded", enLCYellow);
	else
	{
		arr  = *var;
		if (arr.Count() < 3)
			g_plog->Out("clsBaseMaterial::LoadBaseMaterial[" + m_name + "] >> invalid diffuse color", enLCYellow);		
		else
		{
			if (arr[0])
				m_dxmtl.Diffuse.r = *arr[0];
			if (arr[1])
				m_dxmtl.Diffuse.g = *arr[1];
			if (arr[2])
				m_dxmtl.Diffuse.b = *arr[2];
			m_dxmtl.Diffuse.a = opacity;
		}
	}
	
	// Emission color
	var = plist->FindVariable("EmissionColor");
	if (var == mmnull)
		g_plog->Out("clsBaseMaterial::LoadBaseMaterial[" + m_name + "] >> emission color variable not founded", enLCYellow);
	else
	{
		arr    = *var;
		if (arr.Count() < 3)
			g_plog->Out("clsBaseMaterial::LoadBaseMaterial[" + m_name + "] >> invalid emission color", enLCYellow);		
		else
		{
			if (arr[0])
				m_dxmtl.Emissive.r = *arr[0];
			if (arr[1])
				m_dxmtl.Emissive.g = *arr[1];
			if (arr[2])
				m_dxmtl.Emissive.b = *arr[2];
			m_dxmtl.Emissive.a = opacity;
		}
	}
	
	// Ambient color
	var = plist->FindVariable("AmbientColor");
	if (var == mmnull)
		g_plog->Out("clsBaseMaterial::LoadBaseMaterial[" + m_name + "] >> ambient color variable not founded", enLCYellow);
	else
	{
		arr    = *var;
		if (arr.Count() < 3)
			g_plog->Out("clsBaseMaterial::LoadBaseMaterial[" + m_name + "] >> invalid ambient color", enLCYellow);		
		else
		{
			if (arr[0])
				m_dxmtl.Ambient.r = *arr[0];
			if (arr[1])
				m_dxmtl.Ambient.g = *arr[1];
			if (arr[2])
				m_dxmtl.Ambient.b = *arr[2];
			m_dxmtl.Ambient.a = opacity;
		}
	}
	
	// Specular color
	var = plist->FindVariable("SpecularColor");
	if (var == mmnull)
		g_plog->Out("clsBaseMaterial::LoadBaseMaterial[" + m_name + "] >> specular color variable not founded", enLCYellow);
	else
	{
		arr = *var;
		if (arr.Count() < 3)
			g_plog->Out("clsBaseMaterial::LoadBaseMaterial[" + m_name + "] >> invalid specular color", enLCYellow);		
		else
		{
			if (arr[0])
				m_dxmtl.Specular.r = strength * (mmFloat)(*arr[0]);
			if (arr[1])
				m_dxmtl.Specular.g = strength * (mmFloat)(*arr[1]);
			if (arr[2])
				m_dxmtl.Specular.b = strength * (mmFloat)(*arr[2]);
			m_dxmtl.Specular.a = opacity;
		}
	}
	
	// Transparency type
	var = plist->FindVariable("TransparencyType");
	if (var == mmnull)
		g_plog->Out("clsBaseMaterial::LoadBaseMaterial[" + m_name + "] >> transparency type variable not founded", enLCYellow);
	else
	{
		clsString sType = (mmString)*var;
		if (sType == "Filter")
			m_eBlendType = enBlendNone;
		else if (sType == "Additive")
			m_eBlendType = enBlendAdditive;
		else if (sType == "Subtractive")
			m_eBlendType = enBlendSubtractive;
	}
	if ((opacity < 0.98f) && (m_eBlendType == enBlendNone))
	{
		m_eBlendType = enBlendAlpha;
	}
	
	// Two sided flag
	var = plist->FindVariable("TwoSided");
	if (var == mmnull)
		g_plog->Out("clsBaseMaterial::LoadBaseMaterial[" + m_name + "] >> two sided flag not founded, old exporter version?", enLCYellow);
	else
		m_bTwoSided = (mmBool)*var;		
	
	// Wireframe flag flag
	var = plist->FindVariable("Wireframe");
	if (var == mmnull)
		g_plog->Out("clsBaseMaterial::LoadBaseMaterial[" + m_name + "] >> wireframe flag not founded, old exporter version?", enLCYellow);
	else
		m_bWireframe = (mmBool)*var;		
	
	
	// Maps
	var = plist->FindVariable("DiffuseMap");
	if (MHMFAIL(LoadMap(&m_map_diffuse, var)))
		g_plog->Out("clsBaseMaterial::LoadBaseMaterial[" + m_name + "] >> cant load diffuse map");
	var = plist->FindVariable("EmissionMap");
	if (MHMFAIL(LoadMap(&m_map_emission, var)))
		g_plog->Out("clsBaseMaterial::LoadBaseMaterial[" + m_name + "] >> cant load emission map");
	var = plist->FindVariable("ReflectionMap");
	if (MHMFAIL(LoadMap(&m_map_reflection, var)))
		g_plog->Out("clsBaseMaterial::LoadBaseMaterial[" + m_name + "] >> cant load reflection map");
	var = plist->FindVariable("BumpMap");
	if (MHMFAIL(LoadMap(&m_map_bump, var)))
		g_plog->Out("clsBaseMaterial::LoadBaseMaterial[" + m_name + "] >> cant load bump map");
	if (m_map_bump.m_texture != mmnull)
	{
//		m_map_bump.m_texture->ConvertToBump();
//		m_map_bump.m_pvshader = (clsVertexShader*)m_presman->GetResource("bump.vsh", MHMRES_SHADER_ID);
//		m_map_bump.m_ppshader = (clsPixelShader*)m_presman->GetResource("myshader.psh", MHMRES_SHADER_ID);
		// Vertex shader test
		m_vshader = (clsVertexShader*)m_presman->GetResource("bump.vsh", MHMRES_SHADER_ID);
		m_pshader = (clsPixelShader*)m_presman->GetResource("bump.pso", MHMRES_SHADER_ID);
	}
	return mmOk;
}

// LoadMap: loads map
//==========================================================================================
mmRes clsBaseMaterial::LoadMap(clsBaseMap *pmap, clsVariable* var)
{
	if (var != mmnull)
	{
		if (var->GetType() != enVarArray)
			g_plog->Out("clsBaseMaterial::LoadMap[" + m_name + "] >> map component invalid, old version?", enLCYellow);
		else
		{
			clsVarList arr = *var;
			if (arr.Count() > 0)
				pmap->m_texture  = (clsBaseTexture*)m_presman->GetResource((mmString)*arr[0], MHMRES_TEXTURE_ID);
			if (arr.Count() > 1)
				pmap->m_speed.x  = (mmFloat)*arr[1];
			if (arr.Count() > 2)
				pmap->m_speed.y  = (mmFloat)*arr[2];
			if (arr.Count() > 3)
				pmap->m_speed_r  = (mmFloat)*arr[3];
			pmap->m_animated = ((pmap->m_speed.x != 0.0f) ||
								(pmap->m_speed.y != 0.0f) ||
								(pmap->m_speed_r != 0.0f));

			if (arr.Count() >= 15)
				pmap->m_alphablend = (mmBool)*arr[14];
			if ((pmap->m_alphablend) && (m_eBlendType == enBlendNone)) // material blend has higher priority
				 SetBlendType(enBlendAlpha);
			
			if (arr.Count() >= 17)
				pmap->m_bAdditive = (mmBool)*arr[16];
			
		}
	}
	return mmOk;
}

// NeedTangents: returns true if material needs vertices tangents
//==========================================================================================
mmBool clsBaseMaterial::NeedTangents()
{
	return (m_map_bump.m_texture != mmnull);
}

// EnableStageFiltering: enables (disables) linear texture filtering on specified stage
// Mozg: some performance optimized 
//==========================================================================================
mmRes clsBaseMaterial::EnableStageFiltering(mmUint stage, mmBool enable)
{
#ifdef MHM_USE_DX8
	if (g_StageFilteringStates[stage] != enable)
	{
		MHMDirect3DDevice* pddev = *m_presman->GetDevice();
		mmDword	F = enable?D3DTEXF_LINEAR:D3DTEXF_POINT;
		pddev->SetTextureStageState(stage, D3DTSS_MAGFILTER, F);
		pddev->SetTextureStageState(stage, D3DTSS_MINFILTER, F);
		pddev->SetTextureStageState(stage, D3DTSS_MIPFILTER, F);
		g_StageFilteringStates[stage] = enable;
	}
#else 
	if (g_StageFilteringStates[stage] != enable)
	{
		MHMDirect3DDevice* pddev = *m_presman->GetDevice();
		mmDword	F = enable?D3DTEXF_LINEAR:D3DTEXF_NONE;
		pddev->SetSamplerState(stage, D3DSAMP_MAGFILTER, F);
		pddev->SetSamplerState(stage, D3DSAMP_MINFILTER, F);
		pddev->SetSamplerState(stage, D3DSAMP_MIPFILTER, F);
		g_StageFilteringStates[stage] = enable;
	}
#endif // MHM_USE_DX8
	return mmOk;
}

// EnableStageFiltering: enables (disables) wrapping of texture on specified stage
//==========================================================================================
mmRes clsBaseMaterial::EnableStageWrapping(mmUint stage, mmBool enable)
{
#ifdef MHM_USE_DX8
	if (g_StageWrappingStates[stage] != enable)
	{
		MHMDirect3DDevice* pddev = *m_presman->GetDevice();
		mmDword	W = enable?D3DTADDRESS_WRAP:D3DTADDRESS_CLAMP;
		pddev->SetTextureStageState(stage, D3DTSS_ADDRESSU, W);
		pddev->SetTextureStageState(stage, D3DTSS_ADDRESSV, W);
		pddev->SetTextureStageState(stage, D3DTSS_ADDRESSW, W);
		g_StageWrappingStates[stage] = enable;
	}
#else
	if (g_StageWrappingStates[stage] != enable)
	{
		MHMDirect3DDevice* pddev = *m_presman->GetDevice();
		mmDword	W = enable?D3DTADDRESS_WRAP:D3DTADDRESS_CLAMP;
		pddev->SetSamplerState(stage, D3DSAMP_ADDRESSU, W);
		pddev->SetSamplerState(stage, D3DSAMP_ADDRESSV, W);
		pddev->SetSamplerState(stage, D3DSAMP_ADDRESSW, W);
		g_StageWrappingStates[stage] = enable;
	}
#endif // MHM_USE_DX8
	return mmOk;
}

// SetStageMap: sets texture on specified stage
//==========================================================================================
mmRes clsBaseMaterial::SetStageMap(mmUint nStage, clsBaseMap *pMap, mmBool bDiffuse, mmBool bReflection)
{
	MHMDirect3DDevice* pddev = *m_pDevice;
	if (FAILED(pddev->SetTexture(nStage, *pMap->m_texture)))
		return mmFail;
	
	if (FAILED(pddev->SetTextureStageState(nStage, D3DTSS_COLORARG1, D3DTA_TEXTURE)))
		return mmFail;
		
	if (pMap->m_bAdditive)
	{
		if (FAILED(pddev->SetTextureStageState(nStage, D3DTSS_COLOROP, D3DTOP_ADD)))
			return mmFail;
	}
	else
	{
		if (bDiffuse)
		{
			if (FAILED(pddev->SetTextureStageState(nStage, D3DTSS_COLOROP, D3DTOP_SELECTARG1)))
				return mmFail;
		}
		else
		{
			if (FAILED(pddev->SetTextureStageState(nStage, D3DTSS_COLOROP, D3DTOP_MODULATE)))
				return mmFail;
		}
	}

	if (pMap->m_alphablend)
	{
		if (FAILED(pddev->SetTextureStageState(nStage, D3DTSS_ALPHAARG1, D3DTA_TEXTURE )))
			return mmFail;
		if (FAILED(pddev->SetTextureStageState(nStage, D3DTSS_ALPHAARG2, D3DTA_CURRENT )))
			return mmFail;
		if (FAILED(pddev->SetTextureStageState(nStage, D3DTSS_ALPHAOP, D3DTOP_MODULATE)))
			return mmFail;
	}
	else
		if (FAILED(pddev->SetTextureStageState(nStage, D3DTSS_ALPHAOP, D3DTOP_MODULATE)))
			return mmFail;
	
	if (bReflection)
#ifdef MHM_USE_DX8
		pddev->SetTextureStageState(nStage, D3DTSS_TEXCOORDINDEX, D3DTSS_TCI_CAMERASPACEREFLECTIONVECTOR);
#else
		pddev->SetTextureStageState(nStage, D3DTSS_TEXCOORDINDEX, D3DTSS_TCI_SPHEREMAP);
#endif
	else
		pddev->SetTextureStageState(nStage, D3DTSS_TEXCOORDINDEX, 0);
	
	if (pMap->m_animated)
	{
		if (FAILED(pddev->SetTextureStageState(nStage, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_COUNT2)))
			return mmFail;
		if (FAILED(pddev->SetTransform(D3DTRANSFORMSTATETYPE(16 + nStage), (D3DMATRIX*)&pMap->m_tform)))
			return mmFail;
	}
	else
	{
		if (FAILED(pddev->SetTextureStageState(nStage, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE)))
			return mmFail;
	}
	EnableStageFiltering(nStage, pMap->m_filtering_enable);
	EnableStageWrapping(nStage, pMap->m_wrapping_enable);
	return mmOk;
}


