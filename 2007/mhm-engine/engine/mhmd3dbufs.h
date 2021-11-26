//==========================================================================================
// Mayhem engine Direct3D buffers wrappers classes
// Coded:	MozG
// Created:	9 may 2006
//==========================================================================================

#ifndef _MHMD3DBUFS_INCLUDED
#define _MHMD3DBUFS_INCLUDED

// Error codes
#define MHMBUF_DX_ERROR		mmOk + 1
#define MHMBUF_NO_BUFFER	mmOk + 2

//==========================================================================================
// Vertex buffer format structures
//==========================================================================================




//==========================================================================================
// Vertex buffer class
//==========================================================================================
class clsResourceManager;
class clsVertexBuffer
{
protected:
	clsResourceManager*			m_presman;
	MHMDirect3DDevice*			m_pdev;
	MHMDirect3DVertexBuffer*	m_pbuf;
	clsVertexDecl*			m_vdecl;
	mmDword					m_stride;
	mmDword					m_fvf;
	mmDword					m_count;

public:
	clsVertexBuffer(clsResourceManager* presman);
   ~clsVertexBuffer();
    
    
    mmRes   Create(mmDword vcount, 
				 mmDword fvf,
				 D3DPOOL pool = D3DPOOL_MANAGED,
				 mmDword usage = 0);
	
#ifndef MHM_USE_DX8	
	mmRes   Create(clsVertexDecl* pvdecl, 
				   mmDword vcount,
				   D3DPOOL pool = D3DPOOL_MANAGED,
				   mmDword usage = 0);
#endif // MHM_USE_DX8
    mmRes   Destroy();
    mmRes   Lock(mmVoid** pdata, mmDword flags = 0);
    mmRes   Unlock();
    mmDword GetFVF();
    mmDword GetStride();
    mmDword GetNumVertices();
    mmDword GetSize();
    mmRes   SetStream(mmUint index);
    mmRes	SetFVF();
    operator MHMDirect3DVertexBuffer*();
    clsVertexDecl* GetVertexDecl();
};

//==========================================================================================
// Index buffer class
//==========================================================================================
class clsIndexBuffer
{
protected:
	clsResourceManager*		m_presman;
	MHMDirect3DDevice*		m_pdev;
	MHMDirect3DIndexBuffer*	m_pbuf;
	mmDword					m_stride;
	mmDword					m_count;
	D3DFORMAT				m_fmt;

public:
	clsIndexBuffer(clsResourceManager* presman);
   ~clsIndexBuffer();
    mmRes     Create(mmDword icount, 
					 D3DFORMAT fmt = D3DFMT_UNKNOWN,
					 D3DPOOL pool  = D3DPOOL_MANAGED,
					 mmDword usage = 0);
    mmRes     Destroy();
    mmRes     Lock(mmVoid** pdata, mmDword flags = 0);
    mmRes     Unlock();
    D3DFORMAT GetFormat();
    mmDword   GetStride();
    mmDword   GetNumIndicies();
    mmDword   GetSize(); 
    mmRes	  Set();
	operator  MHMDirect3DIndexBuffer*();
};


#endif //_MHMD3DBUFS_INCLUDED