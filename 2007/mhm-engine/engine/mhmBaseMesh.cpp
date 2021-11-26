
#include "mhmbase.h"

// Base mesh constructor
//==========================================================================================
clsBaseMesh::clsBaseMesh(clsResourceManager *presman, clsString name, enResType type):
						   clsResource(presman, name, type)
{
	m_basevbuf   = mmnull;
	m_baseibuf   = mmnull;
	m_numsubs    = 0;
	m_nummtls    = 0;
	m_pttype     = D3DPT_TRIANGLELIST;
	m_indexed    = mmfalse;
	m_psubsets   = mmnull;
	m_hassubsets = mmfalse;
	m_materials  = mmnull;
}

// Base mesh destructor
//==========================================================================================
clsBaseMesh::~clsBaseMesh(void)
{
	DestroyBaseMesh();
}

// Create: creates new empty base mesh
//==========================================================================================
mmRes clsBaseMesh::Create(clsResourceParams* params)
{
	if (MHMFAIL(OnCreateMesh(params)))
	{
		g_plog->Out("clsBaseMesh::Create[" + m_name + "] >> cant create high level mesh", enLCRed);
		return mmFail;
	}
	return mmOk;
}

// OnCreateMesh: creates new empty base mes
//==========================================================================================
mmRes clsBaseMesh::OnCreateMesh(clsResourceParams* params)
{
	g_plog->Out("clsBaseMesh::Create[" + m_name + "] >> empty base mesh resource created successfully", enLCGreen);
	return mmOk;
}


// Render: renders base mesh
//==========================================================================================
mmVoid clsBaseMesh::OnRender(){}
//==========================================================================================
mmRes clsBaseMesh::Render()
{
	clsDevice* pdev = m_presman->GetDevice();


	OnRender();
	
	if (pdev == mmnull)
		return MHMRESOURCE_NO_DEVICE;
	
	MHMDirect3DDevice* pddev = *pdev;
	if (pddev == mmnull)
		return MHMRESOURCE_NO_DEVICE;
	
	if ((m_basevbuf == mmnull) && (m_baseibuf == mmnull))
		return mmOk;	// empty mesh
	
	if (m_numsubs == 0)
		return mmOk;	// empty mesh
	
	if (MHMFAIL(m_basevbuf->SetStream(0)))
		return MHMRESOURCE_LIB_ERROR;
	if(m_indexed)
	if (MHMFAIL(m_baseibuf->Set()))
		return MHMRESOURCE_LIB_ERROR;
	
	for (mmUint i = 0; i < m_numsubs; i++)
	{
		clsResource * mat = m_materials[m_psubsets[i].m_mtlindex];
		if(mat)
		mat->Activate(this);

		if (m_indexed)
		{
//			if (m_type != enResStaticMesh)
#ifdef MHM_USE_DX8
			if (FAILED(pddev->DrawIndexedPrimitive(m_pttype, 0, 
												   m_basevbuf->GetNumVertices(),
												   m_psubsets[i].m_start,
												   m_psubsets[i].m_count)))
				return MHMRESOURCE_LIB_ERROR;
#else
			if (FAILED(pddev->DrawIndexedPrimitive(m_pttype, 0, 0, 
												   m_basevbuf->GetNumVertices(),
												   m_psubsets[i].m_start,
												   m_psubsets[i].m_count)))
				return MHMRESOURCE_LIB_ERROR;
			m_presman->m_Stats.AddDIPCall();
			m_presman->m_Stats.AddFrameRenderedTriangles(m_psubsets[i].m_count);
#endif
		}
		else
		{
//			if (m_type != enResStaticMesh)
			if (FAILED(pddev->DrawPrimitive(m_pttype, m_psubsets[i].m_start, 
													  m_psubsets[i].m_count)))
				return MHMRESOURCE_LIB_ERROR;
			m_presman->m_Stats.AddDIPCall();
			m_presman->m_Stats.AddFrameRenderedTriangles(m_psubsets[i].m_count);
		}
		if(mat)
		mat->Deactivate(this);
	}
/*	// test
	if (m_bs != mmnull)
	{
		clsMatrix mtm = *m_presman->GetDevice()->GetModelTM();
		clsMatrix scale, trans;
		scale.Scale(m_bsphere.m_fRadius, m_bsphere.m_fRadius, m_bsphere.m_fRadius);
		trans.Translation(m_bsphere.m_cCenter);
		//mtm *= trans;
		mtm = scale * trans * mtm;
		m_presman->GetDevice()->SetModelTM(&mtm);
		m_bs->Render();
	}*/

	//pddev->SetRenderState(D3DRS_ZFUNC, D3DCMP_NEVER);
	return mmOk;
}

// Clone: clones mesh
//==========================================================================================
// WARN: vertex pool dont processed
// WARN: index pool dont processed
mmRes clsBaseMesh::Clone(clsResource* pmesh)
{
	if (pmesh == mmnull)
		return mmFail;
	
	if (mmHLID(pmesh->GetType()) != MHMRES_MESH_ID)
		return mmFail;
	
	clsBaseMesh* pbmesh = (clsBaseMesh*)pmesh;
	pbmesh->DestroyBaseMesh();
	
	mmVoid* pthisbuf;
	mmVoid* pclonebuf;
	
	// Vertex buffer clone
	if ((m_basevbuf != mmnull) && (m_basevbuf->GetNumVertices()))
	{
		pbmesh->m_basevbuf = m_presman->CreateVertexBuffer();
		pbmesh->m_basevbuf->Create(m_basevbuf->GetNumVertices(), m_basevbuf->GetFVF());
		m_basevbuf->Lock(&pthisbuf);
		pbmesh->m_basevbuf->Lock(&pclonebuf);
		int size = m_basevbuf->GetSize();
		memcpy(pclonebuf, pthisbuf, m_basevbuf->GetSize());
		m_basevbuf->Unlock();
		pbmesh->m_basevbuf->Unlock();
	}
	
	// Index buffer clone
	if ((m_baseibuf != mmnull) && (m_baseibuf->GetNumIndicies() == 0))
	{
		
		pbmesh->m_baseibuf= m_presman->CreateIndexBuffer();
		pbmesh->m_baseibuf->Create(m_baseibuf->GetNumIndicies());
		m_baseibuf->Lock(&pthisbuf);
		pbmesh->m_baseibuf->Lock(&pclonebuf);
		memcpy(pclonebuf, pthisbuf, m_baseibuf->GetSize());
		m_baseibuf->Unlock();
		pbmesh->m_baseibuf->Unlock();
	}
	
	// Subsets clone
	pbmesh->m_psubsets = new clsBaseMeshSubset[m_numsubs];
	for (mmUint i = 0; i < m_numsubs; i++)
		pbmesh->m_psubsets[i] = m_psubsets[i];
	
	pbmesh->m_numsubs = m_numsubs;
	
	pbmesh->m_pttype  = m_pttype;
	pbmesh->m_indexed = m_indexed;
	return mmOk;
}

// DestroyBaseMesh: destroys base mesh resources
//==========================================================================================
mmRes clsBaseMesh::DestroyBaseMesh()
{
	mmSafeDelete(m_basevbuf);
	mmSafeDelete(m_baseibuf);
	m_numsubs = 0;
	m_indexed = mmfalse;
	mmSafeDeleteAr(m_psubsets);
	for (mmUint i = 0; i < m_nummtls; i++)
		mmSafeRelease(m_materials[i]);
	mmSafeDeleteAr(m_materials);
	m_nummtls = 0;
	return mmOk;
}

// GetVertexDeclaration: returns mesh vertex declaration
//==========================================================================================
clsVertexDecl* clsBaseMesh::GetVertexDeclaration()
{
	if (m_basevbuf)
		return m_basevbuf->GetVertexDecl();
	return mmnull;
}

// GetMaterial: returns mesh material by its index
//==========================================================================================
clsResource* clsBaseMesh::GetMaterial(mmUint index)
{
	return m_materials[index];
}

// RayIntersect: ray intersects mesh
//==========================================================================================
mmBool clsBaseMesh::RayIntersect(const clsRay &ray, 
								 clsMatrix* tform, 
								 clsVector3* pvint,
								 mmFloat* pdist,
								 clsPlane* pplane)
{
	// check vertex buffer structure
	clsVertexDecl* pvdecl = GetVertexDeclaration();
	if (pvdecl == mmnull)
		return mmfalse;
	mmUint offs = 0xFFFFFFFF;
	for (mmUint i = 0; i < pvdecl->m_numitems; i++)
	{		
		switch(pvdecl->GetElementUsage(i))
		{
		case clsVertexDecl::enUsagePosition:		
			offs = pvdecl->GetElementOffset(i);  
			break;
		}
	}
	if (offs == 0xFFFFFFFF)
		return mmfalse;
	
	mmByte* pindc  = mmnull;
	mmByte* pverts = mmnull;
	
	if (MHMFAIL(m_basevbuf->Lock((mmVoid**)&pverts)))
		return mmfalse;
	if (MHMFAIL(m_baseibuf->Lock((mmVoid**)&pindc)))
		return mmfalse;
	
	pverts += offs;
	mmUint a, b, c;
	clsVector3 iva, ivb, ivc;
	clsVector3 va, vb, vc;
	mmUint f = 0;
	mmBool intersect = mmfalse;
	mmFloat mindist = 1000000000.0f;
	while(f < m_baseibuf->GetNumIndicies())
	{
		if (m_baseibuf->GetFormat() == D3DFMT_INDEX16)
		{
			a = (mmUint)(*(mmWord*)&pindc[f]);	f += sizeof(mmWord);
			b = (mmUint)(*(mmWord*)&pindc[f]);	f += sizeof(mmWord);
			c = (mmUint)(*(mmWord*)&pindc[f]);	f += sizeof(mmWord);
		}
		else
		{
			a = (mmUint)(*(mmDword*)&pindc[f]);	f += sizeof(mmDword);
			b = (mmUint)(*(mmDword*)&pindc[f]);	f += sizeof(mmDword);
			c = (mmUint)(*(mmDword*)&pindc[f]);	f += sizeof(mmDword);
		}
		// pick triangle
		va = *(clsVector3*)&pverts[a * m_basevbuf->GetStride()];
		vb = *(clsVector3*)&pverts[b * m_basevbuf->GetStride()];
		vc = *(clsVector3*)&pverts[c * m_basevbuf->GetStride()];
		if (tform != mmnull)
		{
			va.Transform(*tform);
			vb.Transform(*tform);
			vc.Transform(*tform);
		}
		mmFloat dist;
		clsVector3 point;
		if (!ray.IntersectTriangle(va, vb, vc, dist, point))
			continue;
	
		intersect = mmtrue;
		if (dist < mindist)
		{
			if (pvint != mmnull)
				*pvint = point;
			if (pdist != mmnull)
				*pdist = dist;
			if (pplane != mmnull)
			{
				iva = va;
				ivb = vb;
				ivc = vc;
			}
			mindist = dist;
		}
		
		if ((pvint == mmnull) && (pdist == mmnull))
			break;
	}
	if (intersect)
	{
		if (pplane != mmnull)
			pplane->FromPoints(iva, ivb, ivc);
	}
	m_basevbuf->Unlock();
	m_baseibuf->Unlock();
	return intersect;
}


// ComputeTangents: computes tangent spaces for each vertex in mesh
//==========================================================================================
mmRes clsBaseMesh::ComputeTangents()
{
	mmUint i;

	struct stTangentFace
	{
		mmWord a, b, c;
	};
	struct stTangentVector
	{
		mmFloat x, y, z;
	};


	// check vertex buffer structure
	clsVertexDecl* pvdecl = GetVertexDeclaration();
	if (pvdecl == mmnull)
	{
		g_plog->Out("clsBaseMesh::ComputeTangents >> no vertex declaration", enLCRed);
		return mmFail;
	}
	mmUint offset_tangent  = 0xFFFFFFFF;
	mmUint offset_position = 0xFFFFFFFF;
	mmUint offset_uv       = 0xFFFFFFFF;

	for (i = 0; i < pvdecl->m_numitems; i++)
	{		
		switch(pvdecl->GetElementUsage(i))
		{
		case clsVertexDecl::enUsagePosition:		
			offset_position = pvdecl->GetElementOffset(i);  
			break;
		case clsVertexDecl::enUsageTexcoord:	
			offset_uv = pvdecl->GetElementOffset(i);
			break;
		case clsVertexDecl::enUsageTangent:
			offset_tangent = pvdecl->GetElementOffset(i);
			break;
		}
	}

	if ((offset_tangent  == 0xFFFFFFFF) ||
		(offset_position == 0xFFFFFFFF) ||
		(offset_uv       == 0xFFFFFFFF))
	{
		g_plog->Out("clsBaseMesh::ComputeTangents >> one of needed vertex elements dont presented", enLCRed);
		return mmFail;
	}
	
	stTangentFace* pfaces;
	mmByte*        pverts;
	
	mmUint numfaces = 0;
	mmUint stride = pvdecl->GetStride();
	for (i = 0; i < m_numsubs; i++)
		numfaces += m_psubsets[i].m_count;
	
	m_baseibuf->Lock((mmVoid**)&pfaces);
	m_basevbuf->Lock((mmVoid**)&pverts);
	for (i = 0; i < numfaces; i++)
	{
		
		clsVector3 v1 = *(clsVector3*)&pverts[pfaces[i].a * stride + offset_position];
		clsVector3 v2 = *(clsVector3*)&pverts[pfaces[i].b * stride + offset_position];
		clsVector3 v3 = *(clsVector3*)&pverts[pfaces[i].c * stride + offset_position];
		clsVector2 t1 = *(clsVector2*)&pverts[pfaces[i].a * stride + offset_uv];
		clsVector2 t2 = *(clsVector2*)&pverts[pfaces[i].b * stride + offset_uv];
		clsVector2 t3 = *(clsVector2*)&pverts[pfaces[i].c * stride + offset_uv];
		clsVector3 tangent;
		CalcTangent(v1, v2, v3, t1, t2, t3, 1.0f, tangent);
		*(clsVector3*)&pverts[pfaces[i].a * stride + offset_tangent] += tangent;
		*(clsVector3*)&pverts[pfaces[i].b * stride + offset_tangent] += tangent;
		*(clsVector3*)&pverts[pfaces[i].c * stride + offset_tangent] += tangent;
	}
	// now renormalize all tangents
	for (i = 0; i < numfaces; i++)
	{
		((clsVector3*)&pverts[pfaces[i].a * stride + offset_tangent])->Normalize();
		((clsVector3*)&pverts[pfaces[i].b * stride + offset_tangent])->Normalize();
		((clsVector3*)&pverts[pfaces[i].c * stride + offset_tangent])->Normalize();
	}	
	m_baseibuf->Unlock();
	m_basevbuf->Unlock();
	return mmOk;
}

// CalcTangent: calculates vertex tangent
//==========================================================================================
mmVoid clsBaseMesh::CalcTangent(clsVector3& v1,
							   clsVector3& v2,
							   clsVector3& v3,
							   clsVector2& t1,
							   clsVector2& t2,
							   clsVector2& t3,
							   mmFloat bumpfactor,
							   clsVector3& tangent)
{
	mmFloat dv1 = t1.y - t3.y;
	mmFloat dv2 = t3.y - t2.y;
	mmFloat du1 = t3.x - t1.x;
	mmFloat du2 = t2.x - t3.x;
	mmFloat f = bumpfactor / (dv1 * du2 - dv2 * du1);
	//clsVector3 dr_dv = (v1 - v3) * (du2 * f) + (v2 - v3) * (du1 * f);
	clsVector3 dr_du = (v1 - v3) * (dv2 * f) + (v2 - v3) * (dv1 * f);
	tangent = dr_du;
}

// ComputeBounds: computes bounds
//==========================================================================================
mmRes clsBaseMesh::ComputeBounds(clsBoundBox* bbox,
								 clsBoundSphere* bsphere,
								 clsMatrix* tform)
{
	if (m_basevbuf == mmnull)
		return mmFail;
	clsVertexDecl* pvdecl = m_basevbuf->GetVertexDecl();
	mmBool ok   = mmfalse;
	mmUint offs = 0;
	for (mmUint i = 0; i < pvdecl->m_numitems; i++)
	{
		if (pvdecl->GetElementUsage(i) == clsVertexDecl::enUsagePosition)
		{
			ok = mmtrue;
			offs = pvdecl->GetElementOffset(i);
			break;
		}
	}
	if (!ok)
		return mmFail;
	mmByte* pverts = mmnull;
	if (MHMFAIL(m_basevbuf->Lock((mmVoid**)&pverts)))
		return mmFail;
	if (pverts == mmnull)
	{
		m_basevbuf->Unlock();
		return mmFail;
	}
	pverts += offs;
	mmUint numverts = m_basevbuf->GetNumVertices(); 
	mmUint stride   = m_basevbuf->GetStride();
	// transform all verts
	clsVector3* ptverts = mmnull;
	if (tform != mmnull)
	{
		ptverts = new clsVector3[numverts];
		if (ptverts == mmnull)
			mmThrow(enExceptOutOfMemory, clsBaseMesh, ComputeBounds, Out of memory while alloc vertex buffer);
		for (i = 0; i < numverts; i++)
		{
			ptverts[i] = *(clsVector3*)&pverts[i * m_basevbuf->GetStride()];
			ptverts[i].Transform(*tform);
		}
		pverts = (mmByte*)ptverts;
		stride = sizeof(clsVector3);
	}
	
	clsBoundBox lbbox;
	lbbox.m_cMax = *(clsVector3*)pverts;
	lbbox.m_cMin = *(clsVector3*)pverts;
	
	clsVector3 avrg(0.0f, 0.0f, 0.0f);
	// compute bounding box and average coords
	for (i = 0; i < numverts; i++)
	{
		clsVector3* pv;
		pv = (clsVector3*)&pverts[i * stride];
		if (pv->x > lbbox.m_cMax.x)
			lbbox.m_cMax.x = pv->x;
		if (pv->y > lbbox.m_cMax.y)
			lbbox.m_cMax.y = pv->y;
		if (pv->z > lbbox.m_cMax.z)
			lbbox.m_cMax.z = pv->z;
		if (pv->x < lbbox.m_cMin.x)
			lbbox.m_cMin.x = pv->x;
		if (pv->y < lbbox.m_cMin.y)
			lbbox.m_cMin.y = pv->y;
		if (pv->z < lbbox.m_cMin.z)
			lbbox.m_cMin.z = pv->z;
		avrg += *pv;
	}
	avrg.x = lbbox.m_cMin.x + ((lbbox.m_cMax.x - lbbox.m_cMin.x) / 2.0f);///= numverts;
	avrg.y = lbbox.m_cMin.y + ((lbbox.m_cMax.y - lbbox.m_cMin.y) / 2.0f);///= numverts;
	avrg.z = lbbox.m_cMin.z + ((lbbox.m_cMax.z - lbbox.m_cMin.z) / 2.0f);///= numverts;
	// compute bounding sphere radius
	clsBoundSphere lbsphere;
	lbsphere.m_cCenter = avrg;
	lbsphere.m_fRadius = 0.0f;
	for (i = 0; i < numverts; i++)
	{
		clsVector3* pv = (clsVector3*)&pverts[i * stride];
		clsVector3  dv = avrg - *pv;
		if (dv.Length() > lbsphere.m_fRadius)
			lbsphere.m_fRadius = dv.Length();
	}
	if (tform != mmnull)
	{
		clsMatrix mxInverse;
		tform->Inverse(mxInverse);
		lbsphere.m_cCenter.Transform(mxInverse);
		lbbox.m_cMax.Transform(mxInverse);
		lbbox.m_cMin.Transform(mxInverse);
		mmSafeDeleteAr(ptverts);
	}
	m_basevbuf->Unlock();
	if (bbox != mmnull)
		*bbox = lbbox;
	if (bsphere != mmnull)
		*bsphere = lbsphere;
	return mmOk;
}
							   