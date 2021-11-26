//==========================================================================================
// Mayhem engine 2D font class implementation
// Coded:	MozG
// Created:	5 june 2006
//==========================================================================================
#include "mhmbase.h"

// 2D font constructor
//==========================================================================================
clsFont2D::clsFont2D(clsResourceManager *presman, clsString name, enResType type):
		   clsResource(presman, name, type)
{
	m_ptexture = mmnull;
	m_pvbuf    = mmnull;
	m_created  = mmfalse;
	m_eBlendType = enBlendNone;
}

// 2D font destructor
//==========================================================================================
clsFont2D::~clsFont2D()
{
}

// Create: creates 2D font resource
//==========================================================================================
mmRes clsFont2D::Create(clsResourceParams *params)
{
	clsFont2DParams* pp = (clsFont2DParams*)params;
	
	clsTextureTGAParams tp;
	tp.m_alpha     = (pp == mmnull) ? mmfalse : mmtrue;
	tp.m_mipgen    = mmfalse;
	tp.m_levels    = 1;
	tp.m_rentarget = mmfalse;
	tp.m_key_enable = (pp == mmnull) ? mmfalse : pp->m_key_enable;
	if ((pp != mmnull) && tp.m_key_enable)
	{
		// copy color key
		tp.m_key_low   = pp->m_key_low;
		tp.m_key_hi    = pp->m_key_hi;
		tp.m_key_alpha = pp->m_key_alpha;
	}
	
	m_ptexture = (clsBaseTexture*)m_presman->GetResource(m_name, MHMRES_TEXTURE_ID, &tp);
	if (m_ptexture == mmnull)
	{
		g_plog->Out("clsFont2D::Create[" + m_name + "] >> cant link texture resource", enLCRed);
		return mmFail;
	}
	
	m_width     = m_ptexture->GetWidth()  / 16;
	m_height    = m_ptexture->GetHeight() / 16;
	m_maxchars  = (pp == mmnull) ? 255 : pp->m_maxchars;
	m_alpharef  = (pp == mmnull) ? 0 :   pp->m_key_alpha + 1;
	m_enablekey = (pp != mmnull);// ? mmfalse : pp->m_enablekey;
	
	m_pvbuf = m_presman->CreateVertexBuffer();
	if (MHMFAIL(m_pvbuf->Create(m_maxchars * 6, D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1)))
	{
		g_plog->Out("clsFont2D::Create[" + m_name + "] >> failed to create vertex buffer");
		return mmFail;
	}
	g_plog->Out("clsFont2D::Create[" + m_name + "] >> created successfully, enjoy :)", enLCGreen);
	m_created = mmtrue;
	return mmOk;
}

// Render: renders the text
//==========================================================================================
mmRes clsFont2D::Render()
{
	if (!m_created)
		return mmFail;
	MHMDirect3DDevice* pddev = *m_presman->GetDevice();
	if (pddev == mmnull)
		return mmFail;
	
	pddev->SetRenderState(D3DRS_LIGHTING, FALSE);
	pddev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	pddev->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
	pddev->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_SELECTARG1);
	if (m_enablekey)
	{
		pddev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
		pddev->SetRenderState(D3DRS_ALPHAREF,  m_alpharef);
		pddev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);
	}
	
/*	// disable all filtering
	pddev->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_NONE);
	pddev->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_NONE);
	pddev->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);*/
	
	//pddev->SetRenderState(D3DRS_DIFFUSEMATERIALSOURCE, D3DMCS_COLOR1 );
	if (MHMFAIL(m_pvbuf->SetStream(0)))
		return mmFail;
	pddev->SetTexture(0, *m_ptexture);
	pddev->SetTexture(1, mmnull);
	pddev->SetTexture(2, mmnull);
	if (FAILED(pddev->DrawPrimitive(D3DPT_TRIANGLELIST, 0, m_text.Length() * 2)))
		return mmFail;
	pddev->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	if (m_enablekey)
		pddev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	
	// enable all filtering
/*	pddev->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	pddev->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	pddev->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);*/
	
	
	pddev->SetRenderState(D3DRS_LIGHTING, TRUE);

	
	return mmOk;
}

// SetRect: sets font rectangle
//==========================================================================================
mmRes clsFont2D::SetRect(stFontRect* prc, mmDword color)
{
	if (!m_created)
		return mmFail;
	
	stFontVertex* pdata;
	m_pvbuf->Lock((mmVoid**)&pdata);
	mmInt height = ((prc->m_bottom - prc->m_top) / m_height);
	mmInt width  = ((prc->m_right - prc->m_left) / m_width);
	// make all other vertices
	for (mmInt y = 0; y < height; y++)
		for (mmInt x = 0; x < width; x++)
		{
			mmInt index = y * width + x;
			if (index >= m_maxchars)
				break;
			
			mmFloat l = (mmFloat)(prc->m_left + (x * m_width));
			mmFloat r = (mmFloat)(prc->m_left + (x * m_width + m_width));
			mmFloat t = (mmFloat)(prc->m_top  + (y * m_height));
			mmFloat b = (mmFloat)(prc->m_top  + (y * m_height + m_height));
			
			pdata[index * 6 + 0].m_color = color;
			pdata[index * 6 + 0].m_pos.x = l;
			pdata[index * 6 + 0].m_pos.y = b;
			pdata[index * 6 + 0].m_pos.z = 0.0f;
			pdata[index * 6 + 0].m_pos.w = 1.0f;
			
			pdata[index * 6 + 1].m_color = color;
			pdata[index * 6 + 1].m_pos.x = l;
			pdata[index * 6 + 1].m_pos.y = t;
			pdata[index * 6 + 1].m_pos.z = 0.0f;
			pdata[index * 6 + 1].m_pos.w = 1.0f;
			
			pdata[index * 6 + 2].m_color = color;
			pdata[index * 6 + 2].m_pos.x = r;
			pdata[index * 6 + 2].m_pos.y = t;
			pdata[index * 6 + 2].m_pos.z = 0.0f;
			pdata[index * 6 + 2].m_pos.w = 1.0f;
			
			pdata[index * 6 + 3].m_color = color;
			pdata[index * 6 + 3].m_pos.x = r;
			pdata[index * 6 + 3].m_pos.y = t;
			pdata[index * 6 + 3].m_pos.z = 0.0f;
			pdata[index * 6 + 3].m_pos.w = 1.0f;
			
			pdata[index * 6 + 4].m_color = color;
			pdata[index * 6 + 4].m_pos.x = r;
			pdata[index * 6 + 4].m_pos.y = b;
			pdata[index * 6 + 4].m_pos.z = 0.0f;
			pdata[index * 6 + 4].m_pos.w = 1.0f;
			
			pdata[index * 6 + 5].m_color = color;
			pdata[index * 6 + 5].m_pos.x = l;
			pdata[index * 6 + 5].m_pos.y = b;
			pdata[index * 6 + 5].m_pos.z = 0.0f;
			pdata[index * 6 + 5].m_pos.w = 1.0f;
		}
#ifdef _DEBUG
//	DumpBuffer(pdata);
#endif
	m_pvbuf->Unlock();
	return mmOk;
}

// SetText: sets current text
//==========================================================================================
mmRes clsFont2D::SetText(clsString text)
{
	if (!m_created)
		return mmFail;
	
	stFontVertex* pdata;
	m_text = text;
	if (MHMFAIL(m_pvbuf->Lock((mmVoid**)&pdata)))
		return mmFail;
	for (mmInt i = 0; i <= (mmInt)text.Length(); i++)
	{
		mmChar  chr = text[i];
		mmFloat v  = (mmFloat)(chr >> 4) / 16.0f;
		mmFloat u  = (mmFloat)(chr & 0x0F) / 16.0f;
		mmFloat u1 = u + 1.0f / 16.0f;
		mmFloat v1 = v + 1.0f / 16.0f;
		
		pdata[i * 6 + 0].m_tex.x  = u;			    
		pdata[i * 6 + 0].m_tex.y  = v1;
		
		pdata[i * 6 + 1].m_tex.x  = u;
		pdata[i * 6 + 1].m_tex.y  = v;
		 
		pdata[i * 6 + 2].m_tex.x  = u1;
		pdata[i * 6 + 2].m_tex.y  = v;
		
		pdata[i * 6 + 3].m_tex.x  = u1;
		pdata[i * 6 + 3].m_tex.y  = v;
		
		pdata[i * 6 + 4].m_tex.x  = u1;
		pdata[i * 6 + 4].m_tex.y  = v1;
		
		pdata[i * 6 + 5].m_tex.x  = u;
		pdata[i * 6 + 5].m_tex.y  = v1;
	}
	m_pvbuf->Unlock();
	return mmOk;
}

// SetColor: sets font color
//==========================================================================================
mmRes clsFont2D::SetColor(mmDword color)
{
	if (!m_created)
		return mmFail;
	
	stFontVertex* pdata;
	m_pvbuf->Lock((mmVoid**)&pdata);
	for (mmInt i = 0; i < m_maxchars * 6; i++)
		pdata[i].m_color = color;
	
	m_pvbuf->Unlock();
	
	return mmOk;
}

// DumpBuffer: dumps vertex buffer in debug version
//==========================================================================================
#ifdef _DEBUG
mmVoid clsFont2D::DumpBuffer(stFontVertex* pfv)
{
	g_plog->Out("----------------------------------------");
	g_plog->Out("Vertex buffer dump");
	g_plog->Out("----------------------------------------");
		
	for (mmInt i = 0; i < m_maxchars * 6; i++)
	{
		clsString x, y;
		x.FromFloat(pfv[i].m_pos.x);	y.FromFloat(pfv[i].m_pos.y);
		g_plog->Out("x=[" + x + "] y=[" + y + "]");
	}
	g_plog->Out("----------------------------------------");
}
#endif