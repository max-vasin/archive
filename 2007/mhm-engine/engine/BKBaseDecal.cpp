//==========================================================================================
// BIOKRIZ base decals class
//==========================================================================================
#include "mhmbase.h"
#include "BKBaseDecal.h"

//==========================================================================================
clsBKBaseDecal::clsBKBaseDecal(clsEntityFactory *pFactory, 
							   const clsString &sType,
							   const clsString &sName):
				clsEntity(pFactory, sType, sName)			   
				
{
	m_bNeedUpdateEvent	= mmtrue;
	m_fAlphaValue		= 255;
	m_fFadeTime			= 15.0f;
	m_fCurrentFade		= 0.0f;
	
}
//==========================================================================================
clsBKBaseDecal::~clsBKBaseDecal()
{
}
//==========================================================================================
mmVoid clsBKBaseDecal::OnInitialize(mmBool bClone)
{
	//g_plog->Out("Decal initialized: " + clsString((mmUint)this));
}
//==========================================================================================
mmRes clsBKBaseDecal::OnRender()
{
	if ((Res) && (mmHLID(Res->GetType()) == MHMRES_MESH_ID))
	{
		clsBaseMesh* pMesh = (clsBaseMesh*)Res;
		clsBaseMaterial* pMtl = (clsBaseMaterial*)pMesh->GetMaterial(0);
		if (pMtl != mmnull)
			pMtl->m_dxmtl.Diffuse.a = m_fAlphaValue;
	}
	return mmOk;
}
//==========================================================================================
mmVoid clsBKBaseDecal::OnUpdate(const clsTimer *pTimer)
{
	m_fCurrentFade += pTimer->GetDeltaSecs();
	if (m_fCurrentFade > m_fFadeTime)
	{
		m_bKillMe		= mmtrue;
		m_fAlphaValue	= 0.0f;
	}
	else
	{
		m_fAlphaValue = 1.0f - (m_fCurrentFade / m_fFadeTime);
	}
}
//==========================================================================================
//==========================================================================================
//==========================================================================================
