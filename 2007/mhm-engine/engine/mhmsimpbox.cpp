//==========================================================================================
// Mayhem engine simple box class implementation
// Coded:	MozG
// Created:	29 may 2006
//==========================================================================================
#include "mhmbase.h"

//==================================================
// Simple box vertex structure
//==================================================
struct SimpleBoxVertex
{
	clsVector3	m_pos;
	clsVector3	m_norm;
	mmDword		m_clr;
	clsVector2	m_tex;
	
	SimpleBoxVertex& operator = (SimpleBoxVertex& vtx)
	{
		m_pos  = vtx.m_pos;
		m_norm = vtx.m_norm;
		m_clr  = vtx.m_clr;
		m_tex  = vtx.m_tex;
		return *this;
	}
};

// Simple box constructor
//==========================================================================================
clsSimpleBox::clsSimpleBox(clsResourceManager *presman, clsString name):
			  clsBaseMesh(presman, name, enResSimpleBox)
{
	m_size = 0.0f;
}

// Simple box destructor
//==========================================================================================
clsSimpleBox::~clsSimpleBox()
{
}

// Create: creates simple box
//==========================================================================================
mmRes clsSimpleBox::Create(clsResourceParams *params)
{
	if (params == mmnull)
		return mmOk;
	
	clsSimpleBoxParams* prm = (clsSimpleBoxParams*)params;
	DestroyBaseMesh();
	
	m_basevbuf = m_presman->CreateVertexBuffer();
	m_baseibuf = m_presman->CreateIndexBuffer();
	
	m_basevbuf->Create(36, D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_DIFFUSE|D3DFVF_TEX1);
	
	SimpleBoxVertex* pbuf = mmnull;
	int size = sizeof(SimpleBoxVertex);
	m_basevbuf->Lock((mmVoid**)&pbuf);
	mmFloat halfsize = prm->m_size * 0.5f;
	
	// Front plane
	//=================================
	pbuf[0].m_pos.x  = -halfsize; pbuf[0].m_pos.y  = -halfsize; pbuf[0].m_pos.z  = -halfsize;
	pbuf[0].m_norm.x = 0.0f;      pbuf[0].m_norm.y = 0.0f;      pbuf[0].m_norm.z = -1.0f;
	pbuf[0].m_tex.x  = 0.0f;      pbuf[0].m_tex.y  = 0.0f;
	pbuf[0].m_clr    = prm->m_vclr;
	
	pbuf[1].m_pos.x  = halfsize;  pbuf[1].m_pos.y  = halfsize; pbuf[1].m_pos.z   = -halfsize;
	pbuf[1].m_norm.x = 0.0f;      pbuf[1].m_norm.y = 0.0f;      pbuf[1].m_norm.z = -1.0f;
	pbuf[1].m_tex.x  = 1.0f;      pbuf[1].m_tex.y  = 1.0f;
	pbuf[1].m_clr    = prm->m_vclr;
	
	pbuf[2].m_pos.x  = halfsize;  pbuf[2].m_pos.y  = -halfsize; pbuf[2].m_pos.z  = -halfsize;
	pbuf[2].m_norm.x = 0.0f;      pbuf[2].m_norm.y = 0.0f;      pbuf[2].m_norm.z = -1.0f;
	pbuf[2].m_tex.x  = 1.0f;      pbuf[2].m_tex.y  = 0.0f;
	pbuf[2].m_clr    = prm->m_vclr;
	
	pbuf[3].m_pos.x  = -halfsize; pbuf[3].m_pos.y  = -halfsize; pbuf[3].m_pos.z  = -halfsize;
	pbuf[3].m_norm.x = 0.0f;      pbuf[3].m_norm.y = 0.0f;      pbuf[3].m_norm.z = -1.0f;
	pbuf[3].m_tex.x  = 0.0f;      pbuf[3].m_tex.y  = 0.0f;
	pbuf[3].m_clr    = prm->m_vclr;
	
	pbuf[4].m_pos.x  = -halfsize; pbuf[4].m_pos.y  = halfsize;  pbuf[4].m_pos.z  = -halfsize;
	pbuf[4].m_norm.x = 0.0f;      pbuf[4].m_norm.y = 0.0f;      pbuf[4].m_norm.z = -1.0f;
	pbuf[4].m_tex.x  = 0.0f;      pbuf[4].m_tex.y  = 1.0f;
	pbuf[4].m_clr    = prm->m_vclr;
	
	pbuf[5].m_pos.x  = halfsize;  pbuf[5].m_pos.y  = halfsize;  pbuf[5].m_pos.z  = -halfsize;
	pbuf[5].m_norm.x = 0.0f;      pbuf[5].m_norm.y = 0.0f;      pbuf[5].m_norm.z = -1.0f;
	pbuf[5].m_tex.x  = 1.0f;      pbuf[5].m_tex.y  = 1.0f;
	pbuf[5].m_clr    = prm->m_vclr;
	
	// Back plane
	//=================================
	pbuf[6].m_pos.x  = halfsize;  pbuf[6].m_pos.y  = -halfsize; pbuf[6].m_pos.z  = halfsize;
	pbuf[6].m_norm.x = 0.0f;      pbuf[6].m_norm.y = 0.0f;      pbuf[6].m_norm.z = 1.0f;
	pbuf[6].m_tex.x  = 1.0f;      pbuf[6].m_tex.y  = 0.0f;
	pbuf[6].m_clr    = prm->m_vclr;
	
	pbuf[7].m_pos.x  = -halfsize; pbuf[7].m_pos.y  = halfsize;  pbuf[7].m_pos.z  = halfsize;
	pbuf[7].m_norm.x = 0.0f;      pbuf[7].m_norm.y = 0.0f;      pbuf[7].m_norm.z = 1.0f;
	pbuf[7].m_tex.x  = 0.0f;      pbuf[7].m_tex.y  = 1.0f;
	pbuf[7].m_clr    = prm->m_vclr;
	
	pbuf[8].m_pos.x  = -halfsize; pbuf[8].m_pos.y  = -halfsize; pbuf[8].m_pos.z  = halfsize;
	pbuf[8].m_norm.x = 0.0f;      pbuf[8].m_norm.y = 0.0f;      pbuf[8].m_norm.z = 1.0f;
	pbuf[8].m_tex.x  = 0.0f;      pbuf[8].m_tex.y  = 0.0f;
	pbuf[8].m_clr    = prm->m_vclr;
	
	pbuf[9].m_pos.x  = halfsize;  pbuf[9].m_pos.y  = -halfsize; pbuf[9].m_pos.z  = halfsize;
	pbuf[9].m_norm.x = 0.0f;      pbuf[9].m_norm.y = 0.0f;      pbuf[9].m_norm.z = 1.0f;
	pbuf[9].m_tex.x  = 1.0f;      pbuf[9].m_tex.y  = 0.0f;
	pbuf[9].m_clr    = prm->m_vclr;
	
	pbuf[10].m_pos.x  = halfsize;  pbuf[10].m_pos.y  = halfsize;  pbuf[10].m_pos.z  = halfsize;
	pbuf[10].m_norm.x = 0.0f;      pbuf[10].m_norm.y = 0.0f;      pbuf[10].m_norm.z = 1.0f;
	pbuf[10].m_tex.x  = 1.0f;      pbuf[10].m_tex.y  = 1.0f;
	pbuf[10].m_clr    = prm->m_vclr;
	
	pbuf[11].m_pos.x  = -halfsize; pbuf[11].m_pos.y  = halfsize;  pbuf[11].m_pos.z  = halfsize;
	pbuf[11].m_norm.x = 0.0f;      pbuf[11].m_norm.y = 0.0f;      pbuf[11].m_norm.z = 1.0f;
	pbuf[11].m_tex.x  = 0.0f;      pbuf[11].m_tex.y  = 1.0f;
	pbuf[11].m_clr    = prm->m_vclr;
	
	// Left plane
	//=================================
	pbuf[12].m_pos.x  = -halfsize; pbuf[12].m_pos.y  = -halfsize; pbuf[12].m_pos.z  = halfsize;
	pbuf[12].m_norm.x = -1.0f;     pbuf[12].m_norm.y = 0.0f;      pbuf[12].m_norm.z = 0.0f;
	pbuf[12].m_tex.x  = 0.0f;      pbuf[12].m_tex.y  = 1.0f;
	pbuf[12].m_clr    = prm->m_vclr;
	
	pbuf[13].m_pos.x  = -halfsize; pbuf[13].m_pos.y  = halfsize;  pbuf[13].m_pos.z  = -halfsize;
	pbuf[13].m_norm.x = -1.0f;     pbuf[13].m_norm.y = 0.0f;      pbuf[13].m_norm.z = 0.0f;
	pbuf[13].m_tex.x  = 1.0f;      pbuf[13].m_tex.y  = 0.0f;
	pbuf[13].m_clr    = prm->m_vclr;
	
	pbuf[14].m_pos.x  = -halfsize; pbuf[14].m_pos.y  = -halfsize; pbuf[14].m_pos.z  = -halfsize;
	pbuf[14].m_norm.x = -1.0f;     pbuf[14].m_norm.y = 0.0f;      pbuf[14].m_norm.z = 0.0f;
	pbuf[14].m_tex.x  = 0.0f;      pbuf[14].m_tex.y  = 0.0f;
	pbuf[14].m_clr    = prm->m_vclr;
	
	pbuf[15].m_pos.x  = -halfsize; pbuf[15].m_pos.y  = -halfsize; pbuf[15].m_pos.z  = halfsize;
	pbuf[15].m_norm.x = -1.0f;     pbuf[15].m_norm.y = 0.0f;      pbuf[15].m_norm.z = 0.0f;
	pbuf[15].m_tex.x  = 0.0f;      pbuf[15].m_tex.y  = 1.0f;
	pbuf[15].m_clr    = prm->m_vclr;
	
	pbuf[16].m_pos.x  = -halfsize; pbuf[16].m_pos.y  = halfsize;  pbuf[16].m_pos.z  = halfsize;
	pbuf[16].m_norm.x = -1.0f;     pbuf[16].m_norm.y = 0.0f;      pbuf[16].m_norm.z = 0.0f;
	pbuf[16].m_tex.x  = 1.0f;      pbuf[16].m_tex.y  = 1.0f;
	pbuf[16].m_clr    = prm->m_vclr;
	
	pbuf[17].m_pos.x  = -halfsize; pbuf[17].m_pos.y  = halfsize;  pbuf[17].m_pos.z  = -halfsize;
	pbuf[17].m_norm.x = -1.0f;     pbuf[17].m_norm.y = 0.0f;      pbuf[17].m_norm.z = 0.0f;
	pbuf[17].m_tex.x  = 1.0f;      pbuf[17].m_tex.y  = 0.0f;
	pbuf[17].m_clr    = prm->m_vclr;
	
	// Right plane
	//=================================
	pbuf[18].m_pos.x  = halfsize;  pbuf[18].m_pos.y  = -halfsize; pbuf[18].m_pos.z  = -halfsize;
	pbuf[18].m_norm.x = 1.0f;      pbuf[18].m_norm.y = 0.0f;      pbuf[18].m_norm.z = 0.0f;
	pbuf[18].m_tex.x  = 0.0f;      pbuf[18].m_tex.y  = 0.0f;
	pbuf[18].m_clr    = prm->m_vclr;
	
	pbuf[19].m_pos.x  = halfsize;  pbuf[19].m_pos.y  = halfsize;  pbuf[19].m_pos.z  = halfsize;
	pbuf[19].m_norm.x = 1.0f;      pbuf[19].m_norm.y = 0.0f;      pbuf[19].m_norm.z = 0.0f;
	pbuf[19].m_tex.x  = 1.0f;      pbuf[19].m_tex.y  = 1.0f;
	pbuf[19].m_clr    = prm->m_vclr;
	
	pbuf[20].m_pos.x  = halfsize;  pbuf[20].m_pos.y  = -halfsize; pbuf[20].m_pos.z  = halfsize;
	pbuf[20].m_norm.x = 1.0f;      pbuf[20].m_norm.y = 0.0f;      pbuf[20].m_norm.z = 0.0f;
	pbuf[20].m_tex.x  = 0.0f;      pbuf[20].m_tex.y  = 1.0f;
	pbuf[20].m_clr    = prm->m_vclr;
	
	pbuf[21].m_pos.x  = halfsize;  pbuf[21].m_pos.y  = -halfsize; pbuf[21].m_pos.z  = -halfsize;
	pbuf[21].m_norm.x = 1.0f;      pbuf[21].m_norm.y = 0.0f;      pbuf[21].m_norm.z = 0.0f;
	pbuf[21].m_tex.x  = 0.0f;      pbuf[21].m_tex.y  = 0.0f;
	pbuf[21].m_clr    = prm->m_vclr;
	
	pbuf[22].m_pos.x  = halfsize;  pbuf[22].m_pos.y  = halfsize;  pbuf[22].m_pos.z  = -halfsize;
	pbuf[22].m_norm.x = 1.0f;      pbuf[22].m_norm.y = 0.0f;      pbuf[22].m_norm.z = 0.0f;
	pbuf[22].m_tex.x  = 1.0f;      pbuf[22].m_tex.y  = 0.0f;
	pbuf[22].m_clr    = prm->m_vclr;
	
	pbuf[23].m_pos.x  = halfsize;  pbuf[23].m_pos.y  = halfsize;  pbuf[23].m_pos.z  = halfsize;
	pbuf[23].m_norm.x = 1.0f;      pbuf[23].m_norm.y = 0.0f;      pbuf[23].m_norm.z = 0.0f;
	pbuf[23].m_tex.x  = 1.0f;      pbuf[23].m_tex.y  = 1.0f;
	pbuf[23].m_clr    = prm->m_vclr;
	
	// Top plane
	//=================================
	pbuf[24].m_pos.x  = -halfsize; pbuf[24].m_pos.y  = halfsize;  pbuf[24].m_pos.z  = -halfsize;
	pbuf[24].m_norm.x = 0.0f;      pbuf[24].m_norm.y = 1.0f;      pbuf[24].m_norm.z = 0.0f;
	pbuf[24].m_tex.x  = 0.0f;      pbuf[24].m_tex.y  = 0.0f;
	pbuf[24].m_clr    = prm->m_vclr;
	
	pbuf[25].m_pos.x  = halfsize;  pbuf[25].m_pos.y  = halfsize;  pbuf[25].m_pos.z  = halfsize;
	pbuf[25].m_norm.x = 0.0f;      pbuf[25].m_norm.y = 1.0f;      pbuf[25].m_norm.z = 0.0f;
	pbuf[25].m_tex.x  = 1.0f;      pbuf[25].m_tex.y  = 1.0f;
	pbuf[25].m_clr    = prm->m_vclr;
	
	pbuf[26].m_pos.x  = halfsize;  pbuf[26].m_pos.y  = halfsize;  pbuf[26].m_pos.z  = -halfsize;
	pbuf[26].m_norm.x = 0.0f;      pbuf[26].m_norm.y = 1.0f;      pbuf[26].m_norm.z = 0.0f;
	pbuf[26].m_tex.x  = 1.0f;      pbuf[26].m_tex.y  = 0.0f;
	pbuf[26].m_clr    = prm->m_vclr;
	
	pbuf[27].m_pos.x  = -halfsize; pbuf[27].m_pos.y  = halfsize;  pbuf[27].m_pos.z  = -halfsize;
	pbuf[27].m_norm.x = 0.0f;      pbuf[27].m_norm.y = 1.0f;      pbuf[27].m_norm.z = 0.0f;
	pbuf[27].m_tex.x  = 0.0f;      pbuf[27].m_tex.y  = 0.0f;
	pbuf[27].m_clr    = prm->m_vclr;
	
	pbuf[28].m_pos.x  = -halfsize; pbuf[28].m_pos.y  = halfsize;  pbuf[28].m_pos.z  = halfsize;
	pbuf[28].m_norm.x = 0.0f;      pbuf[28].m_norm.y = 1.0f;      pbuf[28].m_norm.z = 0.0f;
	pbuf[28].m_tex.x  = 0.0f;      pbuf[28].m_tex.y  = 1.0f;
	pbuf[28].m_clr    = prm->m_vclr;
	
	pbuf[29].m_pos.x  = halfsize;  pbuf[29].m_pos.y  = halfsize;  pbuf[29].m_pos.z  = halfsize;
	pbuf[29].m_norm.x = 0.0f;      pbuf[29].m_norm.y = 1.0f;      pbuf[29].m_norm.z = 0.0f;
	pbuf[29].m_tex.x  = 1.0f;      pbuf[29].m_tex.y  = 1.0f;
	pbuf[29].m_clr    = prm->m_vclr;
	
	// Bottom plane
	//=================================
	pbuf[30].m_pos.x  = halfsize;  pbuf[30].m_pos.y  = -halfsize;  pbuf[30].m_pos.z  = halfsize;
	pbuf[30].m_norm.x = 0.0f;      pbuf[30].m_norm.y = -1.0f;     pbuf[30].m_norm.z = 0.0f;
	pbuf[30].m_tex.x  = 0.0f;      pbuf[30].m_tex.y  = 0.0f;
	pbuf[30].m_clr    = prm->m_vclr;
	
	pbuf[31].m_pos.x  = -halfsize; pbuf[31].m_pos.y  = -halfsize;  pbuf[31].m_pos.z  = -halfsize;
	pbuf[31].m_norm.x = 0.0f;      pbuf[31].m_norm.y = -1.0f;      pbuf[31].m_norm.z = 0.0f;
	pbuf[31].m_tex.x  = 1.0f;      pbuf[31].m_tex.y  = 1.0f;
	pbuf[31].m_clr    = prm->m_vclr;
	
	pbuf[32].m_pos.x  = halfsize;  pbuf[32].m_pos.y  = -halfsize;  pbuf[32].m_pos.z  = -halfsize;
	pbuf[32].m_norm.x = 0.0f;      pbuf[32].m_norm.y = -1.0f;      pbuf[32].m_norm.z = 0.0f;
	pbuf[32].m_tex.x  = 0.0f;      pbuf[32].m_tex.y  = 1.0f;
	pbuf[32].m_clr    = prm->m_vclr;
	
	pbuf[33].m_pos.x  = halfsize; pbuf[33].m_pos.y   = -halfsize;  pbuf[33].m_pos.z  = halfsize;
	pbuf[33].m_norm.x = 0.0f;      pbuf[33].m_norm.y = -1.0f;      pbuf[33].m_norm.z = 0.0f;
	pbuf[33].m_tex.x  = 0.0f;      pbuf[33].m_tex.y  = 0.0f;
	pbuf[33].m_clr    = prm->m_vclr;
	
	pbuf[34].m_pos.x  = -halfsize; pbuf[34].m_pos.y  = -halfsize;  pbuf[34].m_pos.z  = halfsize;
	pbuf[34].m_norm.x = 0.0f;      pbuf[34].m_norm.y = -1.0f;      pbuf[34].m_norm.z = 0.0f;
	pbuf[34].m_tex.x  = 1.0f;      pbuf[34].m_tex.y  = 0.0f;
	pbuf[34].m_clr    = prm->m_vclr;
	
	pbuf[35].m_pos.x  = -halfsize;  pbuf[35].m_pos.y = -halfsize;  pbuf[35].m_pos.z  = -halfsize;
	pbuf[35].m_norm.x = 0.0f;      pbuf[35].m_norm.y = -1.0f;      pbuf[35].m_norm.z = 0.0f;
	pbuf[35].m_tex.x  = 1.0f;      pbuf[35].m_tex.y  = 1.0f;
	pbuf[35].m_clr    = prm->m_vclr;
	
	m_basevbuf->Unlock();
	m_size  = prm->m_size;
	// Build just one subset
	m_psubsets = new clsBaseMeshSubset[1];
	m_psubsets[0].m_start = 0;
	m_psubsets[0].m_count = 12;
	m_numsubs = 1;
	
	g_plog->Out("clsSimpleBox::Create[" + m_name + "] >> simple box resource created successfully", enLCGreen);
	return mmOk;
}

// FlipNormals: flips box normals
//==========================================================================================
mmRes clsSimpleBox::FlipNormals()
{
	SimpleBoxVertex* pbuf;
	m_basevbuf->Lock((mmVoid**)&pbuf);
	for (mmUint i = 0; i < m_basevbuf->GetNumVertices(); i++)
	{
		pbuf[i].m_norm.x = -pbuf[i].m_norm.x;
		pbuf[i].m_norm.y = -pbuf[i].m_norm.y;
		pbuf[i].m_norm.z = -pbuf[i].m_norm.z;
	}
	m_basevbuf->Unlock();
	return mmOk;
}

// FlipVertexOrder: flips vertex order CW-CCW
//==========================================================================================
mmRes clsSimpleBox::FlipVertexOrder()
{
	SimpleBoxVertex* pbuf;
	m_basevbuf->Lock((mmVoid**)&pbuf);
	SimpleBoxVertex exch;
	for (mmUint i = 0; i < m_basevbuf->GetNumVertices() / 3; i++)
	{
		exch = pbuf[i];
		pbuf[i*3] = pbuf[i*3 + 1];
		pbuf[i*3+1] = exch;
	}
	m_basevbuf->Unlock();
	return mmOk;
}