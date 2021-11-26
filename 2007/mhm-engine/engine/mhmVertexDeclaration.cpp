//==========================================================================================
// Mayhem engine vertex declaration class
// Coded:	MozG
// Created:	29 june 2006
//==========================================================================================
#include "mhmbase.h"
// Vertex declaration constructor
//==========================================================================================
clsVertexDecl::clsVertexDecl(clsDevice *pdev, mmUint numitems)
{
	m_numitems = numitems;
	m_pdev     = pdev;
#ifndef MHM_USE_DX8
	m_elements = new D3DVERTEXELEMENT9[m_numitems + 1];
	m_decl     = mmnull;
	mmZeroMem(m_elements, sizeof(D3DVERTEXELEMENT9) * (m_numitems + 1));
	for (mmUint i = 0; i <= m_numitems; i++)
	{
		m_elements[i].Stream = 0xFF;
		m_elements[i].Type	 = D3DDECLTYPE_UNUSED;
	}
#else
	m_elements = new stVertexElement[m_numitems + 1];
	mmZeroMem(m_elements, sizeof(stVertexElement) * (m_numitems + 1));
	for (mmUint i = 0; i <= m_numitems; i++)
		m_elements[i].Usage	 = enUsageNone;
#endif // MHM_USE_DX8
	m_stride = 0;
}

// Vertex declaration destructor
//==========================================================================================
clsVertexDecl::~clsVertexDecl()
{
#ifndef MHM_USE_DX8
	mmSafeRelease(m_decl);
#endif // MHM_USE_DX8
	mmSafeDeleteAr(m_elements);
}

// SetItem: sets vertex declaration item
//==========================================================================================
mmRes clsVertexDecl::SetElement(mmUint index, clsVertexDecl::enElementUsage usage,
								mmUint stream, mmUint offset, mmUint usageindex)
{
	if (index >= m_numitems)
		return mmFail;
#ifndef MHM_USE_DX8
	m_elements[index].Stream = stream;
	m_elements[index].Offset = offset;
	switch(usage)
	{
	case enUsagePosition:
		m_elements[index].Usage = D3DDECLUSAGE_POSITION;
		m_elements[index].Type  = D3DDECLTYPE_FLOAT3;
		break;
	case enUsageNormal:
		m_elements[index].Usage = D3DDECLUSAGE_NORMAL;
		m_elements[index].Type  = D3DDECLTYPE_FLOAT3;
		break;
	case enUsageTexcoord:
		m_elements[index].Usage = D3DDECLUSAGE_TEXCOORD;
		m_elements[index].Type  = D3DDECLTYPE_FLOAT2;
		break;
	case enUsageTangent:
		m_elements[index].Usage = D3DDECLUSAGE_TANGENT;
		m_elements[index].Type  = D3DDECLTYPE_FLOAT3;
		break;
	}
	m_elements[index].UsageIndex = usageindex;
	m_elements[index].Method     = D3DDECLMETHOD_DEFAULT;
#else
	m_elements[index].Offset = offset;
	m_elements[index].Usage  = usage;
#endif // MHM_USE_DX8
	return mmOk;
}

// GetElementUsage: returns usage of element
//==========================================================================================
clsVertexDecl::enElementUsage clsVertexDecl::GetElementUsage(mmUint index)
{
	if (index >= m_numitems)
		return enUsageNone;
#ifndef MHM_USE_DX8
	switch(m_elements[index].Usage)
	{
	case D3DDECLUSAGE_POSITION:
		return enUsagePosition;
	case D3DDECLUSAGE_NORMAL:
		return enUsageNormal;
	case D3DDECLUSAGE_TEXCOORD:
		return enUsageTexcoord;
	case D3DDECLUSAGE_TANGENT:
		return enUsageTangent;
	}
	return enUsageNone;
#else
	return m_elements[index].Usage;
#endif // MHM_USE_DX8
}

// GetElementOffset: returns offset of vertex element
//==========================================================================================
mmUint clsVertexDecl::GetElementOffset(mmUint index)
{
	if (index >= m_numitems)
		return 0xFFFFFFFF;
	return m_elements[index].Offset;
}

// Create: creates current vertex declaration
//==========================================================================================
mmRes clsVertexDecl::Create()
{
#ifndef MHM_USE_DX8
	MHMDirect3DDevice* pdev = (MHMDirect3DDevice*)*m_pdev;
	if (pdev == mmnull)
	{
		g_plog->Out("clsVertexDecl::Create >> no device", enLCRed);
		return mmFail;
	}
	HRESULT res = pdev->CreateVertexDeclaration(m_elements, &m_decl);
	if (FAILED(res))
	{
		g_plog->Out("clsVertexDecl::Create >> Direct3D method call failed", enLCRed);
		
		return mmFail;
	}
	CalcStride();
#endif // MHM_USE_DX8
	return mmOk;
}

// Activate: sets this object as current vertex declaration
//==========================================================================================
mmRes clsVertexDecl::Activate()
{
#ifndef MHM_USE_DX8
	if (m_elements == mmnull)
		return mmFail;
	MHMDirect3DDevice* pdev = (MHMDirect3DDevice*)*m_pdev;
	if (pdev == mmnull)
	{
		g_plog->Out("clsVertexDecl::Activate >> no device", enLCRed);
		return mmFail;
	}
	if (FAILED(pdev->SetVertexDeclaration(m_decl)))
	{
		g_plog->Out("clsVertexDecl::Activate >> Direct3D method call failed", enLCRed);
		return mmFail;
	}
#endif // MHM_USE_DX8
	return mmOk;
}

// CalcStride: calculates stride of vertex
//==========================================================================================
mmVoid clsVertexDecl::CalcStride()
{
#ifndef MHM_USE_DX8
	m_stride = 0;
	for (mmUint i = 0; i < m_numitems; i++)
		switch(m_elements[i].Type)
		{
		case D3DDECLTYPE_FLOAT1: m_stride += sizeof(mmFloat); break;
		case D3DDECLTYPE_FLOAT2: m_stride += sizeof(mmFloat)*2; break;
		case D3DDECLTYPE_FLOAT3: m_stride += sizeof(mmFloat)*3; break;
		case D3DDECLTYPE_FLOAT4: m_stride += sizeof(mmFloat)*4; break;
		}
#endif // MHM_USE_DX8
}

// GetStride: returns stride of vertex declaration
//==========================================================================================
mmDword clsVertexDecl::GetStride()
{
	return m_stride;
}






