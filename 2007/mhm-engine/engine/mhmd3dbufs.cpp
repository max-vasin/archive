//==========================================================================================
// Mayhem engine Direct3D buffers classes implementation
// Coded:	MozG
// Created:	9 may 2006
//==========================================================================================
#include "mhmbase.h"

//==========================================================================================
// Vertex buffer class implementation
//==========================================================================================

// Vertex buffer constructor
//==========================================================================================
clsVertexBuffer::clsVertexBuffer(clsResourceManager *presman)
{
	m_presman = presman;
#ifndef MHM_USE_DX8
	m_vdecl   = mmnull;
#endif // MHM_USE_DX8
	m_pdev    = (MHMDirect3DDevice*)*m_presman->GetDevice();
	m_pbuf    = mmnull;
	m_stride  = 0;
	m_count   = 0;
	m_fvf     = 0;
}

// Vertex buffer destructor
//==========================================================================================
clsVertexBuffer::~clsVertexBuffer()
{
	Destroy();
	m_pdev   = mmnull;
}

// Create: creates new vertex buffer
//==========================================================================================
mmRes clsVertexBuffer::Create(mmDword vcount, mmDword fvf, D3DPOOL pool, mmDword usage)
{
	mmUint stride = clsDXUtils::GetFVFSize(fvf);

	if(m_pdev==NULL) {m_pbuf = mmnull;return mmErr;}

#ifdef MHM_USE_DX8
	if (FAILED(m_pdev->CreateVertexBuffer(vcount * stride, usage, fvf, pool, &m_pbuf)))
	{
		g_plog->Out("clsVertexBuffer::Create >> Direct3D method call failed", enLCRed);
		return MHMBUF_DX_ERROR;
	}
#else
	//usage = D3DUSAGE_WRITEONLY;
	//pool = D3DPOOL_SYSTEMMEM;
	if (FAILED(m_pdev->CreateVertexBuffer(vcount * stride, usage, fvf, pool, &m_pbuf, NULL)))
	{
		g_plog->Out("clsVertexBuffer::Create >> Direct3D method call failed", enLCRed);
		return MHMBUF_DX_ERROR;
	}
#endif // MHM_USE_DX8
	m_stride = stride;
	m_fvf	 = fvf;
	m_count  = vcount;
	
	mmUint items = 0;
//	if (fvf & D3DFVF_DIFFUSE)	items++;
//	if (fvf & D3DFVF_SPECULAR)	items++;
	if (fvf & D3DFVF_XYZ)		items++;
	if (fvf & D3DFVF_NORMAL)	items++;
	items += (fvf & 0xF00) >> 8;
	if (fvf & D3DFVF_XYZRHW)	items++;

//#ifndef MHM_USE_DX8
	m_vdecl = m_presman->CreateVertexDeclaration(items);
	items = 0;
	mmUint offset = 0;
	if (fvf & D3DFVF_XYZ)		
	{
		m_vdecl->SetElement(items, clsVertexDecl::enUsagePosition, 0, offset);
		items++; 
		offset += sizeof(mmFloat) * 3;
	}
	if (fvf & D3DFVF_NORMAL)	
	{
		m_vdecl->SetElement(items, clsVertexDecl::enUsageNormal, 0, offset);
		items++;
		offset += sizeof(mmFloat) * 3;
	}
	for (mmUint i = 0; i < (fvf & 0xF00) >> 8; i++)
	{
		m_vdecl->SetElement(items, clsVertexDecl::enUsageTexcoord, 0, offset, i);
		items++;
		offset += sizeof(mmFloat) * 2;
	}
	if (MHMFAIL(m_vdecl->Create()))
	{
		g_plog->Out("clsVertexBuffer::Create >> cant create vertex declaration", enLCRed);
		return mmFail;	
	}
//#endif // MHM_USE_DX8
	return mmOk;
}

#ifndef MHM_USE_DX8
// Create: create custom vertex buffer, used only with shaders
//==========================================================================================
mmRes clsVertexBuffer::Create(clsVertexDecl* pvdecl, 
							  mmDword vcount,
							  D3DPOOL pool,
							  mmDword usage)
{
	if(m_pdev==NULL) {m_pbuf = mmnull;return mmErr;}

	if (FAILED(m_pdev->CreateVertexBuffer(vcount * pvdecl->GetStride(),
										  usage, 0, pool, &m_pbuf, NULL)))
	{
		g_plog->Out("clsVertexBuffer::Create >> Direct3D method call failed", enLCRed);
		return MHMBUF_DX_ERROR;
	}
	m_vdecl = pvdecl;
	m_stride = m_vdecl->GetStride();
	m_fvf	 = 0;
	m_count  = vcount;
	return mmOk;
}
#endif // MHM_USE_DX8

// Direct3D typecast operator
//==========================================================================================
clsVertexBuffer::operator MHMDirect3DVertexBuffer *()
{
	return m_pbuf;
}

// Destroy: destroys vertex buffer
//==========================================================================================
mmRes clsVertexBuffer::Destroy()
{
	mmSafeRelease(m_pbuf);
#ifndef MHM_USE_DX8
	mmSafeDelete(m_vdecl);
#endif // MHM_USE_DX8
	m_stride = 0;
	m_count  = 0;
	m_fvf    = 0;
	return mmOk;
}

// Lock: locks entrie buffer
//==========================================================================================
mmRes clsVertexBuffer::Lock(mmVoid** pdata, mmDword flags)
{
	if (m_pbuf == mmnull)
	{
		g_plog->Out("clsVertexBuffer::Lock >> no buffer created!", enLCRed);
		return MHMBUF_NO_BUFFER;
	}
#ifdef MHM_USE_DX8
	if (FAILED(m_pbuf->Lock(0, 0, (mmByte**)pdata, flags)))
	{
		g_plog->Out("clsVertexBuffer::Lock >> Direct3D method call failed", enLCRed);
		return MHMBUF_DX_ERROR;
	}
#else
	if (FAILED(m_pbuf->Lock(0, 0, pdata, flags)))
	{
		g_plog->Out("clsVertexBuffer::Lock >> Direct3D method call failed", enLCRed);
		return MHMBUF_DX_ERROR;
	}
#endif // MHM_USE_DX8
	return mmOk;
}

// Unlock: unlocks buffer
//==========================================================================================
mmRes clsVertexBuffer::Unlock()
{
	if (m_pbuf == mmnull)
	{
		g_plog->Out("clsVertexBuffer::Unlock >> no buffer created!", enLCRed);
		return MHMBUF_NO_BUFFER;
	}
	if (FAILED(m_pbuf->Unlock()))
	{
		g_plog->Out("clsVertexBuffer::Unlock >> Direct3D method call failed", enLCRed);
		return MHMBUF_DX_ERROR;
	}
	return mmOk;
}

// GetFVF: returns buffer format flags
//==========================================================================================
mmDword clsVertexBuffer::GetFVF()
{
	return m_fvf;
}

// GetStride: returns buffer stride
//==========================================================================================
mmDword clsVertexBuffer::GetStride()
{
	return m_stride;
}

// GetSize: returns buffer size in bytes
//==========================================================================================
mmDword clsVertexBuffer::GetSize()
{
	return m_count * m_stride;
}

// GetNumVertices: returns vertices number
//==========================================================================================
mmDword clsVertexBuffer::GetNumVertices()
{
	return m_count;
}

// SetStream: sets this vertex buffer on stream
//==========================================================================================
mmRes clsVertexBuffer::SetStream(mmUint index)
{
#ifdef MHM_USE_DX8
	if (FAILED(m_pdev->SetVertexShader(m_fvf)))
		return mmFail;
	if (FAILED(m_pdev->SetStreamSource(index, m_pbuf, m_stride)))
		return mmFail;
#else
	if (FAILED(m_pdev->SetFVF(m_fvf)))
		return mmFail;
	if (FAILED(m_pdev->SetStreamSource(index, m_pbuf, 0, m_stride)))
		return mmFail;
	m_presman->m_Stats.AddStreamChange();
#endif // MHM_USE_DX8
	return mmOk;
}

// SetStream: sets this vertex buffer on stream
//==========================================================================================
mmRes clsVertexBuffer::SetFVF()
{
#ifdef MHM_USE_DX8
	if (FAILED(m_pdev->SetVertexShader(m_fvf)))
		return mmFail;
#else
	if (FAILED(m_pdev->SetFVF(m_fvf)))
		return mmFail;
#endif // MHM_USE_DX8
	return mmOk;
}


// GetVertexDecl: returns buffer vertex declarartion
//==========================================================================================
clsVertexDecl* clsVertexBuffer::GetVertexDecl()
{
	return m_vdecl;
}

//==========================================================================================
// Index buffer class implementation
//==========================================================================================

// Index buffer constructor
//==========================================================================================
clsIndexBuffer::clsIndexBuffer(clsResourceManager *presman)
{
	m_presman = presman;
	m_pdev = (MHMDirect3DDevice*)*presman->GetDevice();
	m_pbuf = mmnull;
}

// Index buffer destructor
//==========================================================================================
clsIndexBuffer::~clsIndexBuffer()
{
	Destroy();
	m_pdev = mmnull;
}

// Create: creates index buffer
//==========================================================================================
mmRes clsIndexBuffer::Create(mmDword icount, D3DFORMAT fmt, D3DPOOL pool, mmDword usage)
{
	Destroy();
	mmDword stride = 0;
	if (fmt == D3DFMT_UNKNOWN)
	{
		if (icount > 65535)
			fmt = D3DFMT_INDEX32;
		else
			fmt = D3DFMT_INDEX16;
	}
	
	if      (fmt == D3DFMT_INDEX32)
		stride = sizeof(mmDword);
	else if (fmt == D3DFMT_INDEX16)
		stride = sizeof(mmWord);

	if(m_pdev==mmnull) return mmError;

#ifdef MHM_USE_DX8
	if (FAILED(m_pdev->CreateIndexBuffer(icount * stride, usage, fmt, pool, &m_pbuf)))
	{
		g_plog->Out("clsIndexBuffer::Create >> Direct3D method call failed", enLCRed);
		return MHMBUF_DX_ERROR;
	}
#else
	//usage = D3DUSAGE_WRITEONLY;
	//pool = D3DPOOL_SYSTEMMEM;
	if (FAILED(m_pdev->CreateIndexBuffer(icount * stride, usage, fmt, pool, &m_pbuf, NULL)))
	{
		g_plog->Out("clsIndexBuffer::Create >> Direct3D method call failed", enLCRed);
		return MHMBUF_DX_ERROR;
	}
#endif // MHM_USE_DX8
	m_fmt    = fmt;
	m_stride = stride;
	m_count	 = icount;
	return mmOk;
}

// Destroy: destroy buffer
//==========================================================================================
mmRes clsIndexBuffer::Destroy()
{
	mmSafeRelease(m_pbuf);
	m_count  = 0;
	m_fmt    = D3DFMT_UNKNOWN;
	m_stride = 0;
	return mmOk;
}

// Lock: locks entrie buffer
//==========================================================================================
mmRes clsIndexBuffer::Lock(mmVoid** pdata, mmDword flags)
{
	if (m_pbuf == mmnull)
	{
		g_plog->Out("clsIndexBuffer::Lock >> no buffer created!", enLCRed);
		return MHMBUF_NO_BUFFER;
	}
#ifdef MHM_USE_DX8
	if (FAILED(m_pbuf->Lock(0, 0, (mmByte**)pdata, flags)))
	{
		g_plog->Out("clsIndexBuffer::Lock >> Direct3D method call failed", enLCRed);
		return MHMBUF_DX_ERROR;
	}
#else
	if (FAILED(m_pbuf->Lock(0, 0, pdata, flags)))
	{
		g_plog->Out("clsIndexBuffer::Lock >> Direct3D method call failed", enLCRed);
		return MHMBUF_DX_ERROR;
	}
#endif // MHM_USE_DX8
	return mmOk;
}

// Unlock: unlocks buffer
//==========================================================================================
mmRes clsIndexBuffer::Unlock()
{
	if (m_pbuf == mmnull)
	{
		g_plog->Out("clsIndexBuffer::Unlock >> no buffer created!", enLCRed);
		return MHMBUF_NO_BUFFER;
	}
	if (FAILED(m_pbuf->Unlock()))
	{
		g_plog->Out("clsIndexBuffer::Unlock >> Direct3D method call failed", enLCRed);
		return MHMBUF_DX_ERROR;
	}
	return mmOk;
}

// GetFormat: returns buffer format
//==========================================================================================
D3DFORMAT clsIndexBuffer::GetFormat()
{
	return m_fmt;
}

// GetStride: returns buffer stride
//==========================================================================================
mmDword clsIndexBuffer::GetStride()
{
	return m_stride;
}

// GetNumIndicies: return number of buffer items
//==========================================================================================
mmDword clsIndexBuffer::GetNumIndicies()
{
	return m_count;
}

// GetSize: returns buffer size in bytes
//==========================================================================================
mmDword clsIndexBuffer::GetSize()
{
	return m_count * m_stride;
}

// Set: sets indicies as current
//==========================================================================================
mmRes clsIndexBuffer::Set()
{
#ifdef MHM_USE_DX8
	if (FAILED(m_pdev->SetIndices(m_pbuf, 0)))
		return mmFail;
#else
	if (FAILED(m_pdev->SetIndices(m_pbuf)))
		return mmFail;
#endif
	return mmOk;
}


// Direct3D typecast operator
//==========================================================================================
clsIndexBuffer::operator MHMDirect3DIndexBuffer *()
{
	return m_pbuf;
}
