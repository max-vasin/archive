//==========================================================================================
// Mayhem engine MESH resource class
// Coded:	fyrex
// Created:	18 may 2006
//------
// Edited	20 may 2006
// Edited	6 june 2006 [MozG]
// Edited   16 june 2006 [MozG]
//==========================================================================================

#ifndef _MHMBASEMAT_INCLUDED
#define _MHMBASEMAT_INCLUDED

//===================================================================
// Base material map
//===================================================================
class clsDevice;
class clsBaseMap
{
public:
	clsBaseTexture*		m_texture;
	clsVector2			m_uvoffset;
	clsVector2			m_speed;
	mmFloat				m_speed_r;
	mmBool				m_animated;
	clsMatrix			m_tform;
	clsVertexShader*	m_pvshader;
	clsPixelShader*		m_ppshader;
	mmBool				m_alphablend;
	mmBool				m_filtering_enable;
	mmBool				m_wrapping_enable;
	mmBool				m_enabled;
	mmBool				m_bAdditive;

protected:
	clsVector2			m_curpos;
	mmFloat				m_currot;

public:
	clsBaseMap();
   ~clsBaseMap();
    
    mmVoid UpdateTransform(mmFloat k);
};

//===================================================================
// Base material class
//===================================================================
class clsVariable;
class clsBaseMaterial:public clsResource
{
public:
	clsDevice*			m_pDevice;
	mmFloat			reflection;
//	mmDword			two;
	
	
	clsBaseMap		 m_map_diffuse;
	clsBaseMap		 m_map_reflection;
	clsBaseMap		 m_map_emission;
	clsBaseMap		 m_map_bump;
	clsVertexShader* m_vshader;
	clsPixelShader*  m_pshader;

	// Materials params
	MHMMaterial	m_dxmtl;
	
	mmBool				m_bTwoSided;
	mmBool				m_bWireframe;


public:
	clsBaseMaterial(clsResourceManager* presman, 
					clsString name,
					enResType type = enResBaseMaterial);
	virtual ~clsBaseMaterial(void);
	mmRes  Create(clsResourceParams* params);
	mmVoid SetColorLong(D3DCOLORVALUE *, mmDword);
	mmVoid SetColor(D3DCOLORVALUE *c, mmFloat *, mmFloat);
	mmRes  Activate(clsResource* powner);
	mmRes  Deactivate(clsResource* powner);
	mmRes  Update();
	mmBool NeedTangents();

protected:
	mmRes  LoadBaseMaterial(clsString& file);
	mmRes  LoadMap(clsBaseMap* pmap, clsVariable* var);
	mmRes  EnableStageFiltering(mmUint stage, mmBool enable);
	mmRes  EnableStageWrapping(mmUint stage, mmBool enable);
	mmRes  SetStageMap(mmUint nStage, clsBaseMap* pMap, mmBool bDiffuse, mmBool bReflection);
};

#endif