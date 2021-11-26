//==========================================================================================
// BIOKRIZ shadow entity
//==========================================================================================
#include "mhmbase.h"
#include "BKShadow.h"


// Shadow constructor
//===============================================================
clsBKShadow::clsBKShadow(clsEntityFactory *pFactory, 
						 const clsString &sType,
						 const clsString &sName):
			 clsEntity(pFactory, sType, sName)
{
	
}

// Shadow destructor
//===============================================================
clsBKShadow::~clsBKShadow()
{

}

// Shadow initialization
//===============================================================
mmVoid clsBKShadow::OnInitialize()
{
	// make shadow resource
	Res = Renderer->GetResourceManager()->GetResource("ShadowMesh" + clsString((mmDword)this), enResBaseMesh);
	clsBaseMesh* pMesh = (clsBaseMesh*)Res;
//	pMesh->m_basevbuf->Create(4, D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_TEXCOORD0);
	pMesh->m_indexed = mmfalse;
	
	clsBaseTextureParams btParams;
	mmZeroVar(btParams);
	btParams.m_alpha     = mmtrue;
	btParams.m_depth     = enTexDepth32;
	btParams.m_width     = 128;
	btParams.m_height    = 128;
	btParams.m_mipgen    = mmtrue;
	btParams.m_levels    = 0;
	btParams.m_rentarget = mmtrue;
	
	pMesh->m_materials = new clsResource*[1];
	pMesh->m_materials[0] = Renderer->GetResourceManager()->GetResource("ShadowTexture" + clsString((mmDword)this), enResBaseTexture, &btParams);
}

// Shadow rendering
//===============================================================
mmRes clsBKShadow::OnRender()
{
	return mmOk;
}



