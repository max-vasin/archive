//==========================================================================================
// Mayhem engine sprite resource
// Coded:	MozG
// Created:	29 july 2006
//==========================================================================================
#include "mhmbase.h"

// Sprite constructor
//==========================================================================================
clsSprite::clsSprite(clsResourceManager *presman, clsString name, enResType type):
		   clsResource(presman, name, type),
		   m_vPosition(0.0f, 0.0f, 0.1f),
		   m_vScale(1.0f, 1.0f, 1.0f)
{
	m_pTexture = mmnull;
	m_pVBuf    = mmnull;
	m_nWidth   = 0;
	m_nHeight  = 0;
	m_nColor   = 0xFFFFFFFF;
	m_fAngle   = 0.0f;
}

// Sprite destructor
//==========================================================================================
clsSprite::~clsSprite()
{
	mmSafeRelease(m_pTexture);
	mmSafeDelete(m_pVBuf);
}


mmRes clsSprite::SetTexture(clsString p_nam)
{
		mmBool Fail;
		m_pTexture = (clsBaseTexture*)m_presman->GetResource(p_nam, MHMRES_TEXTURE_ID, mmnull, &Fail);
		if (Fail)
		{
			g_plog->Out("clsSprite::Create[" + p_nam + "] source texture not found", enLCRed);
			return mmFail;	
		}
		return mmOk;
}

mmVoid clsSprite::SetWidthHeight(mmInt pWidth, mmInt pHeight)
{
	m_nWidth = pWidth;
	m_nHeight = pHeight;
	m_nCenterX = m_nWidth >> 1;
	m_nCenterY = m_nHeight >> 1;
}
// Create: creates sprite
//==========================================================================================
mmRes clsSprite::Create(clsResourceParams *params)
{
	if(m_name[0]=='$')
	{ // custom sprite
		clsString s = m_name.Mid(1);
		m_nWidth = s.CutBy(':').ToInt();
		m_nHeight = s.CutBy(':',mmfalse).CutBy('.').ToInt();
		m_pTexture = mmnull;

		m_nCenterX = m_nWidth >> 1;
		m_nCenterY = m_nHeight >> 1;
	}
	else
	{
		clsString filename = (m_fullname.IsEmpty()) ? m_name : m_fullname;
		clsFile file(filename);
		clsVarParser Parser;
		if (MHMFAIL(Parser.ParseFile(filename)))
		{
			g_plog->Out("clsSprite::Create[" + m_name + "] cant parse file [" + filename + "]", enLCRed);
			return mmFail;
		}
		clsVarList* pVarList = Parser.GetVarList();
		clsVariable* pVar = pVarList->FindVariable("SourceTexture");
		if (pVar == mmnull)
		{
			g_plog->Out("clsSprite::Create[" + m_name + "] SourceTexture variable not found in [" + filename + "]", enLCRed);
			return mmFail;
		}
		mmBool Fail;
		m_pTexture = (clsBaseTexture*)m_presman->GetResource((mmString)*pVar, MHMRES_TEXTURE_ID, mmnull, &Fail);
		if (Fail)
		{
			g_plog->Out("clsSprite::Create[" + m_name + "] source texture not found", enLCRed);
			return mmFail;	
		}
		pVar = pVarList->FindVariable("Width");
		if (pVar == mmnull)
		{
			g_plog->Out("clsSprite::Create[" + m_name + "] Width variable not found in [" + filename + "]", enLCRed);
			return mmFail;
		}
		m_nWidth = (mmInt)*pVar;
		pVar = pVarList->FindVariable("Height");
		if (pVar == mmnull)
		{
			g_plog->Out("clsSprite::Create[" + m_name + "] Height variable not found in [" + filename + "]", enLCRed);
			return mmFail;
		}
		m_nHeight = (mmInt)*pVar;
		pVar = pVarList->FindVariable("CenterX");
		if (pVar == mmnull)
			m_nCenterX = m_nWidth >> 1;
		else
			m_nCenterX = (mmInt)*pVar;
		pVar = pVarList->FindVariable("CenterY");
		if (pVar == mmnull)
			m_nCenterY = m_nHeight >> 1;
		else
			m_nCenterY = (mmInt)*pVar;
	}
	m_pVBuf = m_presman->CreateVertexBuffer();
	if (MHMFAIL(m_pVBuf->Create(4, D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1)))
	{
		g_plog->Out("clsSprite::Create[" + m_name + "] cant create vertex buffer", enLCRed);
		mmSafeRelease(m_pTexture);
		return mmFail;
	}

	if (MHMFAIL(ReBuild(mmtrue)))
		return mmFail;

	g_plog->Out("clsSprite::Create[" + m_name + "] sprite created successfully", enLCGreen);
	return mmOk;
}

// Render: renders sprite
//==========================================================================================
mmRes clsSprite::Render(mmBool Filtering)
{
	MHMDirect3DDevice* pDev = *m_presman->GetDevice();

	m_presman->GetDevice()->EnableCulling(mmtrue);
	m_presman->GetDevice()->EnableWireframe(mmfalse);
	
	if (FAILED(pDev->SetTexture(0, *m_pTexture)))
		return mmFail;
	if (FAILED(pDev->SetTexture(1, NULL)))
		return mmFail;
	if (FAILED(pDev->SetTexture(2, NULL)))
		return mmFail;
	
//	pDev->SetPixelShader(NULL);
//	pDev->SetVertexShader(NULL);

	if (MHMFAIL(SetBlendMode()))
		return mmFail;
	
	pDev->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	pDev->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_CURRENT);
	pDev->SetTextureStageState(0, D3DTSS_COLOROP,   D3DTOP_MODULATE);
	pDev->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	pDev->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);
	pDev->SetTextureStageState(0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE);

	//mmDword	F = enable?D3DTEXF_LINEAR:D3DTEXF_NONE;
		mmDword	F;
		if(Filtering)
			F = D3DTEXF_LINEAR;
		else
			F = D3DTEXF_NONE;

		#ifdef MHM_USE_DX8
			pDev->SetTextureStageState(0, D3DTSS_MAGFILTER, F);
			pDev->SetTextureStageState(0, D3DTSS_MINFILTER, F);
			pDev->SetTextureStageState(0, D3DTSS_MIPFILTER, F);
		#else
			pDev->SetSamplerState(0, D3DSAMP_MAGFILTER, F);
			pDev->SetSamplerState(0, D3DSAMP_MINFILTER, F);
			pDev->SetSamplerState(0, D3DSAMP_MIPFILTER, F);
		#endif



	
	if (MHMFAIL(m_pVBuf->SetStream(0)))
		return mmFail;
	if (FAILED(pDev->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2)))
		return mmFail;
	
	if (m_bNeedAlphaBlend)
		pDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);



		F = D3DTEXF_LINEAR;
		#ifdef MHM_USE_DX8
		
			pDev->SetTextureStageState(0, D3DTSS_MAGFILTER, F);
			pDev->SetTextureStageState(0, D3DTSS_MINFILTER, F);
			pDev->SetTextureStageState(0, D3DTSS_MIPFILTER, F);
			

		#else
			pDev->SetSamplerState(0, D3DSAMP_MAGFILTER, F);
			pDev->SetSamplerState(0, D3DSAMP_MINFILTER, F);
			pDev->SetSamplerState(0, D3DSAMP_MIPFILTER, F);
		#endif

	return mmOk;
}

// SetPosition: sets sprite center position
//==========================================================================================
mmVoid clsSprite::SetPosition(const clsVector3& vPos)
{
	m_vPosition = vPos;
}

// SetRotation: sets sprite rotation
//==========================================================================================
mmVoid clsSprite::SetRotation(mmFloat fAngle)
{
	m_fAngle = fAngle;
}

// SetScale: sets sprite scale factor
//==========================================================================================
mmVoid clsSprite::SetScale(const clsVector3& vScale)
{
	m_vScale = vScale;
}


// SetColor: sets sprite color
//==========================================================================================
mmVoid clsSprite::SetColor(mmDword nColor)
{
	m_nColor = nColor;
}


// ReBuild: reBuilds vertex buffer
//==========================================================================================
mmRes clsSprite::ReBuild(mmBool bMakeTexCoords)
{
	stSpriteVertex* pVerts = mmnull;
	if (MHMFAIL(m_pVBuf->Lock((mmVoid**)&pVerts)))
	{
		g_plog->Out("clsSprite::Create[" + m_name + "] cant lock vertex buffer", enLCRed);
		mmSafeRelease(m_pTexture);
		return mmFail;
	}
	
	mmFloat fCenterX = (mmFloat)m_nCenterX;
	mmFloat fCenterY = (mmFloat)m_nCenterY;
	
	clsVector3 vLT, vLB, vRT, vRB;
	vLT.x = -fCenterX;
	vLT.y = -fCenterY;
	vLT.z = m_vPosition.z;
	
	vLB.x = -fCenterX;
	vLB.y = (mmFloat)m_nHeight - fCenterY;
	vLB.z = m_vPosition.z;
	
	vRT.x = (mmFloat)m_nWidth - fCenterX;
	vRT.y = -fCenterY;
	vRT.z = m_vPosition.z;
	
	vRB.x = (mmFloat)m_nWidth  - fCenterX;
	vRB.y = (mmFloat)m_nHeight - fCenterY;
	vRB.z = m_vPosition.z;
	
	clsMatrix mxRotation, mxScale, mxFull;
	mxRotation.RotationZ(m_fAngle);
	mxScale.Scale(m_vScale);
	mxFull = mxScale * mxRotation;
	vLT.Transform(mxFull);
	vRT.Transform(mxFull);
	vLB.Transform(mxFull);
	vRB.Transform(mxFull);
	
	
	
	pVerts[0].m_vPosition.x = m_vPosition.x + vLT.x; 
	pVerts[0].m_vPosition.y = m_vPosition.y + vLT.y;
	pVerts[0].m_vPosition.z = m_vPosition.z;
	pVerts[0].m_nColor      = m_nColor;
	if (bMakeTexCoords)
	{
		pVerts[0].m_vPosition.w = 1.0f; 
		pVerts[0].m_vTexCoords.x = 0.0f;
		pVerts[0].m_vTexCoords.y = 0.0f;
	}
	
	pVerts[1].m_vPosition.x = m_vPosition.x + vRT.x; 
	pVerts[1].m_vPosition.y = m_vPosition.y + vRT.y;
	pVerts[1].m_vPosition.z = m_vPosition.z;
	pVerts[1].m_nColor      = m_nColor;
	if (bMakeTexCoords)
	{
		pVerts[1].m_vPosition.w = 1.0f; 
		pVerts[1].m_vTexCoords.x = 1.0f;
		pVerts[1].m_vTexCoords.y = 0.0f;
	}
	
	pVerts[2].m_vPosition.x = m_vPosition.x + vLB.x; 
	pVerts[2].m_vPosition.y = m_vPosition.y + vLB.y;
	pVerts[2].m_vPosition.z = m_vPosition.z;
	pVerts[2].m_nColor      = m_nColor;
	if (bMakeTexCoords)
	{
		pVerts[2].m_vPosition.w = 1.0f; 
		pVerts[2].m_vTexCoords.x = 0.0f;
		pVerts[2].m_vTexCoords.y = 1.0f;
	}
	
	pVerts[3].m_vPosition.x = m_vPosition.x + vRB.x; 
	pVerts[3].m_vPosition.y = m_vPosition.y + vRB.y;
	pVerts[3].m_vPosition.z = m_vPosition.z;
	pVerts[3].m_nColor      = m_nColor;
	if (bMakeTexCoords)
	{
		pVerts[3].m_vPosition.w = 1.0f; 
		pVerts[3].m_vTexCoords.x = 1.0f;
		pVerts[3].m_vTexCoords.y = 1.0f;
	}
	m_pVBuf->Unlock();
	return mmOk;
}

mmRes clsSprite::Activate(clsResource* pOwner)
{
/*
	MHMDirect3DDevice* pDev = *m_presman->GetDevice();

	pDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	pDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	
	if (FAILED(pDev->SetTexture(0, *m_pTexture)))
		return mmFail;
	if (FAILED(pDev->SetTexture(1, NULL)))
		return mmFail;
	if (FAILED(pDev->SetTexture(2, NULL)))
		return mmFail;
	
	pDev->SetPixelShader(NULL);
	pDev->SetVertexShader(NULL);

	if (MHMFAIL(SetBlendMode()))
		return mmFail;
	
	pDev->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	pDev->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_CURRENT);
	pDev->SetTextureStageState(0, D3DTSS_COLOROP,   D3DTOP_MODULATE);
	pDev->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	pDev->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);
	pDev->SetTextureStageState(0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE);
	
	if (MHMFAIL(m_pVBuf->SetStream(0)))
		return mmFail;*/
	return mmOk;
}