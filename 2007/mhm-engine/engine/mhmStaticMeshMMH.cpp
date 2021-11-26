//==========================================================================================
// Mayhem engine static mesh MMH format file
// Coded:	MozG
// Created:	7 june 2006
//==========================================================================================
#include "mhmbase.h"

// Static MMH mesh constructor
//==========================================================================================
clsStaticMeshMMH::clsStaticMeshMMH(clsResourceManager *presman, clsString name):
				  clsBaseMesh(presman, name, enResStaticMeshMMH)
{

}

// Static MMH mesh destructor
//==========================================================================================
clsStaticMeshMMH::~clsStaticMeshMMH()
{

}

// Create: creates mesh from file
//==========================================================================================
mmRes clsStaticMeshMMH::OnCreateMesh(clsResourceParams *params)
{
	clsString filename = (m_fullname.IsEmpty()) ? m_name : m_fullname;
	clsFile file(filename);
	
	if (!file.IsExist())
	{
		g_plog->Out("clsStaticMeshMMH:Create[" + m_name + "] >> file not exist or could not be opened", enLCRed);
		return mmFail;
	}
	
	if (MHMFAIL(file.Open()))
	{
		g_plog->Out("clsStaticMeshMMH:Create[" + m_name + "] >> cant open file", enLCRed);
		return mmFail;
	}
	
	stMMHMeshFileHeader meshhdr;
	
	if (MHMFAIL(file.Read(&meshhdr, sizeof(stMMHMeshFileHeader))))
	{
		g_plog->Out("clsStaticMeshMMH:Create[" + m_name + "] >> file header reading error", enLCRed);
		return mmFail;
	}
	
	DestroyBaseMesh();
	
	if (meshhdr.m_numsubs > 0)
	{
		m_hassubsets = meshhdr.m_numsubs > 0;
		m_materials  = new clsResource*[meshhdr.m_numsubs];
	}
	
	clsString mtlname;
	mmBool needtangents = mmfalse;
	for (mmUint i = 0; i < meshhdr.m_numsubs; i++)
	{
		if (MHMFAIL(file.Read(mtlname, mmtrue)))
		{
			g_plog->Out("clsStaticMeshMMH::Create[" + m_name + "] >> material name reading error", enLCRed);
			DestroyBaseMesh();
			return mmFail;
		}
		m_materials[i] = m_presman->GetResource(mtlname + ".mtl", MHMRES_MATERIAL_ID);
		if (m_materials[i] == mmnull)
		{
			g_plog->Out("clsStaticMeshMMH::Create[" + m_name + "] >> cant get material", enLCRed);
			DestroyBaseMesh();
			return mmFail;
		}
		if (((clsBaseMaterial*)m_materials[i])->NeedTangents())
			needtangents = mmtrue;
	}
	
	m_numsubs  = meshhdr.m_numsubs;
	
	m_basevbuf = m_presman->CreateVertexBuffer();
	m_baseibuf = m_presman->CreateIndexBuffer();
	
	if (!needtangents)
	{
		mmDword fvf = D3DFVF_XYZ;
		if (meshhdr.m_chnl_normals)
			fvf |= D3DFVF_NORMAL;
		if (meshhdr.m_chnl_mapping)
			fvf |= D3DFVF_TEX1;
		
		if (MHMFAIL(m_basevbuf->Create(meshhdr.m_numverts, fvf)))
		{
			g_plog->Out("clsStaticMeshMMH:Create[" + m_name + "] >> cant create vertex buffer", enLCRed);
			DestroyBaseMesh();
			return mmFail;
		}
	}
	else
	{
#ifndef MHM_USE_DX8
		mmUint numitems = 1;
		numitems += (meshhdr.m_chnl_normals) ? 1 : 0;
		numitems += (meshhdr.m_chnl_mapping) ? 1 : 0;
		numitems++;	// tangent
		clsVertexDecl* pvdecl = m_presman->CreateVertexDeclaration(numitems);
		numitems = 1;
		pvdecl->SetElement(0, clsVertexDecl::enUsagePosition);
		mmUint offset = sizeof(mmFloat) * 3;
		if (meshhdr.m_chnl_normals)		
		{
			pvdecl->SetElement(numitems, clsVertexDecl::enUsageNormal, 0, offset);
			numitems++;
			offset += sizeof(mmFloat) * 3;
		}
		if (meshhdr.m_chnl_mapping)		
		{
			pvdecl->SetElement(numitems, clsVertexDecl::enUsageTexcoord, 0, offset);
			offset += sizeof(mmFloat) * 2;
			numitems++;
		}
		pvdecl->SetElement(numitems, clsVertexDecl::enUsageTangent, 0, offset);
		pvdecl->Create();
		if (MHMFAIL(m_basevbuf->Create(pvdecl, meshhdr.m_numverts)))
		{
			g_plog->Out("clsStaticMeshMMH:Create[" + m_name + "] >> cant create vertex buffer (shader)", enLCRed);
			DestroyBaseMesh();
			return mmFail;
		}
#else 
		return mmFail;
#endif // MHM_USE_DX8
	}
	
	//--------------------------------------------
	// Vertex buffer loading
	mmByte* pdata;
	if (MHMFAIL(m_basevbuf->Lock((mmVoid**)&pdata)))
	{
		g_plog->Out("clsStaticMeshMMH:Create[" + m_name + "] >> cant lock vertex buffer", enLCRed);
		DestroyBaseMesh();
		return mmFail;
	}
	if (!needtangents)
	{
		if (MHMFAIL(file.Read(pdata, m_basevbuf->GetSize())))
		{
			g_plog->Out("clsStaticMeshMMH:Create[" + m_name + "] >> vertices reading error", enLCRed);
			m_basevbuf->Unlock();
			DestroyBaseMesh();
			return mmFail;
		}
	}
	else
	{
		mmUint vsize = sizeof(mmFloat) * 3;
		vsize += (meshhdr.m_chnl_normals) ? sizeof(mmFloat) * 3 : 0;
		vsize += (meshhdr.m_chnl_mapping) ? sizeof(mmFloat) * 2 : 0;
	
		for (mmUint z = 0; z < meshhdr.m_numverts; z++)
		{
			if (MHMFAIL(file.Read(&pdata[z * m_basevbuf->GetStride()], vsize)))
			{
				g_plog->Out("clsStaticMeshMMH:Create[" + m_name + "] >> vertices reading error (shader)", enLCRed);
				m_basevbuf->Unlock();
				DestroyBaseMesh();
			}
		}
	}
	m_basevbuf->Unlock();
	
	
	//----------------------------------------------
	// Index buffer loading
	if (MHMFAIL(m_baseibuf->Create(meshhdr.m_numfaces * 3, D3DFMT_INDEX16)))
	{
		g_plog->Out("clsStaticMeshMMH:Create[" + m_name + "] >> cant create index buffer", enLCRed);
		DestroyBaseMesh();
		return mmFail;
	}
	
	mmWord* pfaces;
	if (MHMFAIL(m_baseibuf->Lock((mmVoid**)&pfaces)))
	{
		g_plog->Out("clsStaticMeshMMH:Create[" + m_name + "] >> cant lock index buffer", enLCRed);
		DestroyBaseMesh();
		return mmFail;
	}
	
	if (m_hassubsets)
	{
		m_psubsets = new clsBaseMeshSubset[meshhdr.m_numsubs];
		stMMHMeshSubset subset;
		mmDword counter = 0;
		for (i = 0; i < meshhdr.m_numsubs; i++)
		{
			if (MHMFAIL(file.Read(&subset, sizeof(stMMHMeshSubset))))
			{
				g_plog->Out("clsStaticMeshMMH:Create[" + m_name + "] >> subset header reading error", enLCRed);
				m_baseibuf->Unlock();
				DestroyBaseMesh();
				return mmFail;
			}
			if (MHMFAIL(file.Read(&pfaces[counter], subset.m_numfaces * 3 * sizeof(mmWord))))
			{
				g_plog->Out("clsStaticMeshMMH:Create[" + m_name + "] >> subset faces reading error", enLCRed);
				m_baseibuf->Unlock();
				DestroyBaseMesh();
				return mmFail;
			}
			m_psubsets[i].m_count = subset.m_numfaces;
			m_psubsets[i].m_start = counter;
			m_psubsets[i].m_mtlindex = subset.m_mtlindex;
			counter += subset.m_numfaces * 3;
		}	
	}
	else
	{
		if (MHMFAIL(file.Read(pfaces, m_baseibuf->GetSize())))
		{
			g_plog->Out("clsStaticMeshMMH:Create[" + m_name + "] >> faces reading error", enLCRed);
			m_baseibuf->Unlock();
			DestroyBaseMesh();
			return mmFail;
		}
	}
	
	m_baseibuf->Unlock();
	
	m_pttype  = D3DPT_TRIANGLELIST;
	m_indexed = mmtrue;
	
	if (needtangents)
		ComputeTangents();
	
	g_plog->Out("clsStaticMeshMMH:Create[" + m_name + "] >> loaded successfully", enLCGreen);
	return mmOk;
}





