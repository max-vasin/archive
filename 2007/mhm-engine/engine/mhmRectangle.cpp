
#include "mhmbase.h"

#define _rhw (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)
#define _norhw (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1)


clsRectangle::~clsRectangle(void)
{
	
}
clsRectangle::clsRectangle(clsResourceManager* presman, clsString name):
				  clsBaseMesh(presman, name, enResStaticMeshMMH)
{
	m_fvf = _rhw;
	m_rhw = mmtrue;
	m_centr = mmfalse;

	 m_x = 0;
	 m_y = 0;
	 m_z = 0;
	 m_h = 1;
	 m_w = 1;

	 m_color = 0xFFFFFFFF;

	 m_pttype = D3DPT_TRIANGLESTRIP;

}
mmVoid clsRectangle::SetCentrCentr(mmBool p_center)
{
	if(m_centr == p_center)
		return;
	m_centr = p_center;
	Build();
}
mmVoid clsRectangle::UseRHW(mmBool p_rhw)
{
	if(m_rhw == p_rhw)
		return;
	m_rhw = p_rhw;
	if(p_rhw)
		m_fvf = _rhw;
	else
		m_fvf = _norhw;
	

	if(m_basevbuf)
		m_basevbuf->Destroy();
	m_basevbuf = m_presman->CreateVertexBuffer();
	m_basevbuf->Create(4,m_fvf);
	Build();

}
//----------------------------------------------------------------------------
mmRes clsRectangle::OnCreateMesh(clsResourceParams *params)
{

	g_plog->Out("clsRectangel:Create[" + m_name + "] >> Create successfully", enLCGreen);

	Create();
	
	
/*	

	m_Count_FC.Size(m_font->items.Count());
	m_Pos_V.Size(m_font->items.Count());


	m_materials  = new clsResource*[m_font->items.Count()];
	m_psubsets = new clsBaseMeshSubset[m_font->items.Count()];
	m_numsubs = m_font->items.Count();
	m_nummtls = m_numsubs;
	for(mmUint z=0;z<m_font->items.Count();z++)
	{
		m_psubsets[z].m_count = 0;
		m_psubsets[z].m_start = 0;
		m_psubsets[z].m_mtlindex = z;

		mmBool fail = mmfalse;
		m_materials[z] = (clsBaseMaterial*)m_presman->GetResource(m_font->items[z]->m_name, enResBaseMaterial, mmnull, &fail);

		if (!fail)
			m_size = (mmFloat)((clsBaseMaterial*)m_materials[z])->m_map_diffuse.m_texture->GetWidth();
		else
			m_size = 0;
	}

	m_basevbuf = m_presman->CreateVertexBuffer();
	m_baseibuf = m_presman->CreateIndexBuffer();

	m_basevbuf->Create(4,m_ch_fvf);
	m_baseibuf->Create(6);
*/
	return mmOk;
}
//-------------------------------------------------------
mmVoid clsRectangle::Create()
{
	

	if(m_basevbuf)
		m_basevbuf->Destroy();

	m_basevbuf = m_presman->CreateVertexBuffer();


	m_numsubs = 1;
	m_nummtls = 1;
	m_materials  = new clsResource*[m_numsubs];
	m_psubsets = new clsBaseMeshSubset[m_numsubs];
	m_psubsets[0].m_start = 0;
	m_psubsets[0].m_count = 2;
	m_psubsets[0].m_mtlindex = 0;
	mmBool fail = mmfalse;
	m_materials[0] = (clsBaseMaterial*)m_presman->GetResource("m_name", enResBaseMaterial, mmnull, &fail);

	clsBaseMaterial * mat;
	mat = (clsBaseMaterial*)m_materials[0];
	mat->m_bTwoSided = mmtrue;

	mat->m_map_diffuse.m_filtering_enable = mmfalse;

	mat->m_dxmtl.Diffuse.a = 1.0f;
	mat->m_dxmtl.Diffuse.r = 1.0f;
	mat->m_dxmtl.Diffuse.g = 1.0f;
	mat->m_dxmtl.Diffuse.b = 1.0f;
	mat->m_dxmtl.Ambient = mat->m_dxmtl.Diffuse;
	


	m_basevbuf->Create(4,m_fvf);

	Build();
	
}
//---------------------
mmVoid clsRectangle::Build()
{
	if(m_rhw)
	{
		BuildRHW();
		return;
	}

	mmFloat d1_h,d1_w,d2_h,d2_w;

	if(m_centr)
	{
		d1_h = m_h * 0.5f;
		d1_w = m_w * 0.5f;
		d2_h = m_h * 0.5f;
		d2_w = m_w * 0.5f;
	}else{
		d1_h = 0;
		d1_w = 0;
		d2_h = m_h;
		d2_w = m_w;
	}

	mmVertex * v;

	m_basevbuf->Lock((mmVoid**)(&v));



	v->x = m_x - d1_w;
	v->y = m_y - d1_h;
	v->z = m_z;
		v->nx = 0.0f;
		v->ny = 0.0f;
		v->nz = 1.0f;
	v->c = m_color;
	v->u = 0.0f;
	v->v = 0.0f;

		v++;

	v->x = m_x + d2_w;
	v->y = m_y - d1_h;
	v->z = m_z;
		v->nx = 0.0f;
		v->ny = 0.0f;
		v->nz = 1.0f;
	v->c = m_color;
	v->u = 1.0f;
	v->v = 0.0f;

	v++;

	v->x = m_x - d1_w;
	v->y = m_y + d2_h;
	v->z = m_z;
		v->nx = 0.0f;
		v->ny = 0.0f;
		v->nz = 1.0f;
	v->c = m_color;
	v->u = 0.0f;
	v->v = 1.0f;

	v++;

	v->x = m_x + d2_w;
	v->y = m_y + d2_h;
	v->z = m_z;
		v->nx = 0.0f;
		v->ny = 0.0f;
		v->nz = 1.0f;
	v->c = m_color;
	v->u = 1.0f;
	v->v = 1.0f;

	v++;

	
	m_basevbuf->Unlock();
}
//---------------------
mmVoid clsRectangle::BuildRHW()
{
	mmFloat d1_h,d1_w,d2_h,d2_w;

	if(m_centr)
	{
		d1_h = m_h * 0.5f;
		d1_w = m_w * 0.5f;
		d2_h = m_h * 0.5f;
		d2_w = m_w * 0.5f;
	}else{
		d1_h = 0;
		d1_w = 0;
		d2_h = m_h;
		d2_w = m_w;
	}

	mmVertexRHW * v;

	m_basevbuf->Lock((mmVoid**)(&v));



		v->x = m_x - d1_w;
	v->y = m_y - d1_h;
	v->z = m_z;
		v->h = 1.0f;
	v->c = m_color;
	v->u = 0.0f;
	v->v = 0.0f;

		v++;

	v->x = m_x + d2_w;
	v->y = m_y - d1_h;
	v->z = m_z;
		v->h = 1.0f;
	v->c = m_color;
	v->u = 1.0f;
	v->v = 0.0f;

	v++;

	v->x = m_x - d1_w;
	v->y = m_y + d2_h;
	v->z = m_z;
		v->h = 1.0f;
	v->c = m_color;
	v->u = 0.0f;
	v->v = 1.0f;

	v++;

	v->x = m_x + d2_w;
	v->y = m_y + d2_h;
	v->z = m_z;
		v->h = 1.0f;
	v->c = m_color;
	v->u = 1.0f;
	v->v = 1.0f;

	v++;

	
	m_basevbuf->Unlock();
}
//--------------------
/*
mmRes clsRectangel::Render()
{


	return mmOk;
}
*/

mmVoid clsRectangle::OnRender()
{
	clsDevice* pddev = m_presman->GetDevice();

	pddev->EnableLighting(mmfalse);
	pddev->EnableZ(mmfalse);
//	pddev->EnableCulling(mmfalse);

//	mmDword ZFUNC,LIGHTING;
//	pddev->GetRenderState(D3DRS_ZFUNC,&ZFUNC);
//	pddev->GetRenderState(D3DRS_LIGHTING,&LIGHTING);

//	pddev->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
//	pddev->SetRenderState(D3DRS_LIGHTING, mmfalse);
//	pddev->SetRenderState(D3DRS_CULLMODE,D3DCULL_NONE); 
}
mmVoid clsRectangleEntity::SetBlendType(enBlendType p_blend)
{
	((clsBaseMaterial*)((clsBaseMesh*)m_rect)->GetMaterial(0))->SetBlendType(p_blend);
}
//*******************************************************
mmVoid clsRectangleEntity::SetFiltering(mmBool f)
{
	clsBaseMaterial * mat;
	mat = (clsBaseMaterial*)(m_rect->m_materials[0]);
	mat->m_map_diffuse.m_filtering_enable = f;
}

clsRectangleEntity::clsRectangleEntity(clsEntityFactory *pFactory, 
					 const clsString &type,
					 const clsString &name):
		   clsEntity(pFactory, type, name)
{
	Res = m_pEFactory->GetEngine()->GetRenderer()->GetResourceManager()->GetResource("zz",enResRectangle);
	((clsRectangle*)Res)->UseRHW(mmfalse);

	m_rect = (clsRectangle*)Res;

	SetBlendType(enBlendAlpha);
}
clsRectangleEntity::~clsRectangleEntity()
{

}
mmVoid clsRectangleEntity::SetTexture(const mmString name)
{
	clsBaseTextureParams params;
	params.m_levels = 1;

	mmSafeRelease(((clsBaseMaterial*)((clsBaseMesh*)m_rect)->GetMaterial(0))->m_map_diffuse.m_texture)
	((clsBaseMaterial*)((clsBaseMesh*)m_rect)->GetMaterial(0))->m_map_diffuse.m_texture =
	(clsBaseTexture*)m_pEFactory->GetEngine()->GetRenderer()->GetResourceManager()->GetResource(name, MHMRES_TEXTURE_ID,&params);

}

clsBaseTexture * clsRectangleEntity::GetTexture()
{
	return ((clsBaseMaterial*)((clsBaseMesh*)m_rect)->GetMaterial(0))->m_map_diffuse.m_texture;
}

clsBaseMap * clsRectangleEntity::GetMap()
{
	return (clsBaseMap*)&(((clsBaseMaterial*)((clsBaseMesh*)m_rect)->GetMaterial(0))->m_map_diffuse);
}

mmVoid clsRectangleEntity::SetColor(mmDword p_color)
{
	if(m_rect->m_color == p_color)
		return;
	m_rect->m_color = p_color;
	m_rect->Build();
}

mmVoid clsRectangleEntity::SetX(mmFloat x)
{
	if(m_rect->m_x == x)
		return;
	m_rect->m_x = x;
	m_rect->Build();
}
mmVoid clsRectangleEntity::SetY(mmFloat y)
{
	if(m_rect->m_y == y)
		return;
	m_rect->m_y = y;
	m_rect->Build();
}
mmVoid clsRectangleEntity::SetZ(mmFloat z)
{
	if(m_rect->m_z == z)
		return;
	m_rect->m_z = z;
	m_rect->Build();
}
mmVoid clsRectangleEntity::SetWidth(mmFloat w)
{
	if(m_rect->m_w == w)
		return;
	m_rect->m_w = w;
	m_rect->Build();
}
mmVoid clsRectangleEntity::SetHeight(mmFloat h)
{
	if(m_rect->m_h == h)
		return;
	m_rect->m_h = h;
	m_rect->Build();
}
mmVoid clsRectangleEntity::SetPos(clsVector3 & v)
{
	if( (m_rect->m_x == v.x)&&(m_rect->m_y == v.y)&&(m_rect->m_z == v.z) )
		return;
	m_rect->m_x = v.x;
	m_rect->m_y = v.y;
	m_rect->m_z = v.z;
	m_rect->Build();
}
mmVoid clsRectangleEntity::SetPosWH(clsVector3 & v, mmFloat w, mmFloat h)
{
	if( (m_rect->m_x == v.x)&&(m_rect->m_y == v.y)&&(m_rect->m_z == v.z)&&(m_rect->m_w == w)&&(m_rect->m_h == h) )
		return;
	m_rect->m_x = v.x;
	m_rect->m_y = v.y;
	m_rect->m_z = v.z;
	m_rect->m_w = w;
	m_rect->m_h = h;
	m_rect->Build();
}
mmFloat clsRectangleEntity::GetX()
{
	return m_rect->m_x;
}
mmFloat clsRectangleEntity::GetY()
{
	return m_rect->m_y;
}
mmFloat clsRectangleEntity::GetZ()
{
	return m_rect->m_z;
}
mmFloat clsRectangleEntity::GetWidth()
{
	return m_rect->m_w;
}
mmFloat clsRectangleEntity::GetHeight()
{
	return m_rect->m_h;
}
clsVector3 clsRectangleEntity::GetPos()
{
	return clsVector3(m_rect->m_x,m_rect->m_y,m_rect->m_z);
}

mmVoid clsRectangleEntity::SetCentrCentr(mmBool p_center)
{
	m_rect->SetCentrCentr(p_center);
}

mmVoid clsRectangleEntity::SetTransformation(mmFloat x, mmFloat y, mmFloat w, mmFloat h, mmFloat z)
{
	m_mxFull.Identity();
	m_mxFull._11 = w;
	m_mxFull._22 = h;
	m_mxFull._33 = 1;
	m_mxFull._41 = x;
	m_mxFull._42 = y;
	m_mxFull._43 = z;

}