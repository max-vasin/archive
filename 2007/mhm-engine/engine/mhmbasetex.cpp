//==========================================================================================
// Mayhem engine texture resource class implementation
// Coded:	MozG
// Created:	14 may 2006
//------
// Edited	16 may 2006
// see .h for details
//==========================================================================================
#include "mhmbase.h"

// base texture constructor
//==========================================================================================
clsBaseTexture::clsBaseTexture(clsResourceManager *presman, clsString name, enResType type):
						   clsResource(presman, name, type)
{
	m_fmt	 = D3DFMT_UNKNOWN;
	m_numlvl = 0;
	m_pool	 = D3DPOOL_DEFAULT;
	m_width  = 0;
	m_height = 0;
	m_usage  = 0;
	m_ptex   = mmnull;
}

// base texture destructor
//==========================================================================================
clsBaseTexture::~clsBaseTexture()
{
	DestroyTexture();
}

// Create: creates empty texture
//==========================================================================================
mmRes clsBaseTexture::Create(clsResourceParams *params)
{
	//g_plog->Out("Create Base texture!!!", enLCYellow);
	clsVar Vars,*V;

	Vars.ParseFromMem(m_name.Data(),m_name.Length());
	if(V = Vars.FindVar("width"))
	{
		m_width = V->operator mmInt();
		if(params){((clsBaseTextureParams *)(params))->m_width = m_width;}
	}
	if(V = Vars.FindVar("height"))
	{
		m_height = V->operator mmInt();
		if(params){((clsBaseTextureParams *)(params))->m_height = m_height;}
	}
	if(V = Vars.FindVar("depth"))
	{
		m_texdepth = enTexDepth32;
		if(V->operator mmInt() == 15)
		{
			m_texdepth = enTexDepth15;
		}
		else if(V->operator mmInt() == 16)
		{
			m_texdepth = enTexDepth16;
		}
		if(params){((clsBaseTextureParams *)(params))->m_depth = m_texdepth;}
	}

	return CreateTexture(params);
}

// Invalidate: checks texture pool and destroy this if needed
//==========================================================================================
mmRes clsBaseTexture::Invalidate(clsDeviceInvalidateEvent* pevent)
{
	if (m_ptex == mmnull)	// nothing to destroy
		return mmOk;
	if (m_pool == D3DPOOL_DEFAULT)
	{
		mmSafeRelease(m_ptex);
		m_invalid = mmtrue;
	}
	return mmOk;
}

// Restore: checks texture pool and recreate it (texture) if needed
//==========================================================================================
mmRes clsBaseTexture::Restore(clsDeviceRestoreEvent *pevent)
{
	if (m_invalid)
		return Create(mmnull);
	return mmOk;
}

// GwtWidth: returns texture width
//==========================================================================================
mmUint clsBaseTexture::GetWidth()
{
	return m_width;
}

// GetHeight: returns texture height
//==========================================================================================
mmUint clsBaseTexture::GetHeight()
{
	return m_height;
}

// Direct3D typecast operator
//==========================================================================================
clsBaseTexture::operator MHMDirect3DTexture*()
{
	return m_ptex;
}

// MakeParameters: makestexture parameters from given object params
//==========================================================================================
mmRes clsBaseTexture::MakeParameters(clsBaseTextureParams *params)
{
	m_width  = params->m_width;
	m_height = params->m_height;
	m_numlvl = params->m_levels;
	m_texdepth = params->m_depth;
	
	if (params->m_alpha && params->m_bump)
	{
		g_plog->Out("clsBaseTexture::MakeParameters >> m_alpha and m_bump exclusive flags, alpha disabled, nump forced", enLCYellow);
		params->m_alpha = mmfalse;
	}
	
	if (params->m_rentarget)
	{
		g_plog->Out("clsBaseTexture::MakeParameters >> m_rentarget and m_bump exclusive flags, render target disabled, bump forced", enLCYellow);
		params->m_rentarget = mmfalse;
	}
	
	if (m_texdepth == enTexDepth15)
	{
		if (!params->m_bump)
		{
			m_fmt = (params->m_alpha ? D3DFMT_A1R5G5B5 : D3DFMT_X1R5G5B5);
		}
		else
			m_fmt = (params->m_luminance ? D3DFMT_L6V5U5 : D3DFMT_V8U8);
	}
	else if (m_texdepth == enTexDepth16)
	{
		if (!params->m_bump)
		{
			m_fmt = (params->m_alpha ? D3DFMT_A8R8G8B8 : D3DFMT_R5G6B5);
			if(params->m_alpha) m_texdepth = enTexDepth32; // BUG
		}
		else
			m_fmt = (params->m_luminance ? D3DFMT_L6V5U5 : D3DFMT_V8U8);
	}
	else
	{
		if (!params->m_bump)
			m_fmt = (params->m_alpha ? D3DFMT_A8R8G8B8 : D3DFMT_X8R8G8B8);
		else
			m_fmt = (params->m_luminance ?  D3DFMT_X8L8V8U8 : D3DFMT_V16U16);
	}
	

	//D3DUSAGE_RENDERTARGET
#ifdef MHM_USE_DX8
	m_usage = 0;
#else
	m_usage = (params->m_mipgen ? 0/*D3DUSAGE_AUTOGENMIPMAP*/ : 0);
#endif // MHM_USE_DX8

	if (params->m_rentarget)
		m_usage |= D3DUSAGE_RENDERTARGET;
	
	m_pool = (params->m_rentarget ? D3DPOOL_DEFAULT : D3DPOOL_MANAGED);
	//m_pool = D3DPOOL_DEFAULT;
	return mmOk;
}

//==========================================================================================
// Base texture generated events
//==========================================================================================

// OnPreCreate: called before texture creation
//==========================================================================================
mmRes clsBaseTexture::OnPreCreate(clsBaseTextureParams* params)
{
	return mmOk;
}

// OnPostCreate: called after texture creation
//==========================================================================================
mmRes clsBaseTexture::OnPostCreate(clsBaseTextureParams* params)
{
	return mmOk;
}

// CreateTexture: creates texture from parameters
//==========================================================================================
mmRes clsBaseTexture::CreateTexture(clsResourceParams *params)
{
	//g_plog->Out("clsBaseTexture::Create >> texture creation started");
	MHMDirect3DDevice* pdev = *m_presman->GetDevice();
	if (pdev == mmnull)
	{
		g_plog->Out("clsBaseTexture::CreateTexture[" + m_name + "] >> no device created", enLCYellow);
		return MHMRESOURCE_NO_DEVICE;
	}
	
	clsBaseTextureParams* tparams = (clsBaseTextureParams*)params;
	
	mmRes res = OnPreCreate(tparams);
	if (res != mmOk)
	{
		g_plog->Out("clsBaseTexture::CreateTexture[" + m_name + "] >> OnPreCreate event returns error", enLCRed);
		return res;
	}
	
	if (params != mmnull)
	{
		res = MakeParameters(tparams);
		if (res != mmOk)
			return res;
	}
	

#ifdef MHM_USE_DX8
	if (FAILED(pdev->CreateTexture(m_width, m_height, m_numlvl,
								   m_usage, m_fmt, m_pool, &m_ptex)))
	{
		g_plog->Out("clsBaseTexture::CreateTexture[" + m_name + "] >> Direct3D error while calling CreateTexture", enLCRed);
		return MHMRESOURCE_LIB_ERROR;
	}
#else

	if (FAILED(pdev->CreateTexture(m_width, m_height, m_numlvl,
								   m_usage, m_fmt, m_pool, &m_ptex, NULL)))
	{
		g_plog->Out("clsBaseTexture::CreateTexture[" + m_name + "] >> Direct3D error while calling CreateTexture", enLCRed);
		return MHMRESOURCE_LIB_ERROR;
	}
#endif // MHM_USE_DX8
	
	res = OnPostCreate(tparams);
	if (res != mmOk)
	{
		g_plog->Out("clsBaseTexture::CreateTexture[" + m_name + "] >> OnPostCreate event returns error", enLCRed);
		mmSafeRelease(m_ptex);
		return res;
	}
	return mmOk;
}

// DestroyTexture: destroys texture Direct3D resources
//==========================================================================================
mmRes clsBaseTexture::DestroyTexture()
{
	m_fmt	 = D3DFMT_UNKNOWN;
	m_numlvl = 0;
	m_pool	 = D3DPOOL_DEFAULT;
	m_width  = 0;
	m_height = 0;
	m_usage  = 0;
	mmSafeRelease(m_ptex);
	return mmOk;
}

// CloneBumpTexture: make bump map surface from current texture
//==========================================================================================
mmRes clsBaseTexture::ConvertToBump()
{
	MHMDirect3DTexture* pcurrent = m_ptex;
	
	clsBaseTextureParams params;
	params.m_bump   = mmtrue;
	params.m_width  = m_width;
	params.m_height = m_height;
	params.m_mipgen = mmtrue;
	params.m_levels = m_numlvl;
	
	if (MHMFAIL(CreateTexture(&params)))
	{
		g_plog->Out("clsBaseTexture::ConvertToBump >> cant create bump map texture", enLCRed);
		return mmFail;
	}
	
	D3DLOCKED_RECT	rcur, rbump;
	if (MHMFAIL(pcurrent->LockRect(0, &rcur, NULL, 0)))
	{
		g_plog->Out("clsBaseTexture::ConvertToBump >> cant lock source texture", enLCRed);
		return mmFail;
	}
		
	if (MHMFAIL(m_ptex->LockRect(0, &rbump, NULL, 0)))
	{
		g_plog->Out("clsBaseTexture::ConvertToBump >> cant lock bump texture", enLCRed);
		return mmFail;
	}
	
	mmDword SrcPitch = rcur.Pitch;
	mmDword DstPitch = rbump.Pitch;
	mmByte* pSrcTopRow = (mmByte*)rcur.pBits;
	mmByte* pDstTopRow = (mmByte*)rbump.pBits;
	mmByte* pSrcCurRow = pSrcTopRow;
	mmByte* pDstCurRow = pDstTopRow;
	mmByte* pSrcBotRow = pSrcTopRow + (SrcPitch * (m_height - 1));
	
	for (mmDword y = 0; y < m_height; y++)
	{
		mmByte* pSrcB0;	// addr of current pixel
		mmByte* pSrcB1;	// addr of pixel below current
		mmByte* pSrcB2;	// addr of pixel above current
		mmByte* pDstT;	// addr of dest pixel
		
		pSrcB0 = pSrcCurRow;
		
		if (y == (m_height - 1))
			pSrcB1 = pSrcTopRow;
        else
			pSrcB1 = pSrcCurRow + SrcPitch;
		
		if( y == 0 )
            pSrcB2 = pSrcBotRow;
        else
            pSrcB2 = pSrcCurRow - SrcPitch;
        
        pDstT = pDstCurRow;
		for (mmDword x = 0; x < m_width; x++)
		{
			mmLong v00; // Current pixel
            mmLong v01; // Pixel to the right of current pixel, wrapping to left edge if necessary
            mmLong vM1; // Pixel to the left of current pixel, wrapping to right edge if necessary
            mmLong v10; // Pixel one line below.
            mmLong v1M; // Pixel one line above.
            
            
            v00 = *(pSrcB0+0);

            if(x == (m_width - 1))
                v01 = *(pSrcCurRow);
            else
                v01 = *(pSrcB0+4);

            if(x == 0)
                vM1 = *(pSrcCurRow + (4 * m_width - 1));
            else
                vM1 = *(pSrcB0-4);
            v10 = *(pSrcB1+0);
            v1M = *(pSrcB2+0);

            mmLong iDu = (vM1-v01); // The delta-u bump value
            mmLong iDv = (v1M-v10); // The delta-v bump value

            // The luminance bump value ???????
            // TODO: это здесь явно херь - нада чота предумать
            // скорее всиго передовать какта шоле
            mmWord uL = (v00 > 1) ? 63 : 127;
            
            switch(m_fmt)
            {
                case D3DFMT_V8U8:
                    *pDstT++ = (BYTE)iDu;
                    *pDstT++ = (BYTE)iDv;
                    break;

                case D3DFMT_L6V5U5:
                    *(mmWord*)pDstT  = (mmWord)(((iDu>>3) & 0x1f) <<  0);
                    *(mmWord*)pDstT |= (mmWord)(((iDv>>3) & 0x1f) <<  5);
                    *(mmWord*)pDstT |= (mmWord)((( uL>>2) & 0x3f) << 10);
                    pDstT += 2;
                    break;

                case D3DFMT_X8L8V8U8:
                    *pDstT++ = (BYTE)iDu;
                    *pDstT++ = (BYTE)iDv;
                    *pDstT++ = (BYTE)uL;
                    *pDstT++ = (BYTE)0L;
                    break;
               case D3DFMT_V16U16:
					*pDstT++ = (BYTE)iDu;
                    *pDstT++ = (BYTE)iDv;
                    break;
            }
            // Move one pixel to the right (src is 32-bpp)
            // TODO: 16 bpp format source surface not supported!!!
            // Fix it!
            pSrcB0+=4;
            pSrcB1+=4;
            pSrcB2+=4;
		}
		
        // Move to the next line
        pSrcCurRow += SrcPitch;
        pDstCurRow += DstPitch;
	}
	pcurrent->UnlockRect(0);
	m_ptex->UnlockRect(0);
	mmSafeRelease(pcurrent);
	g_plog->Out("clsBaseTexture::ConvertToBump[" + m_name + "] operation completed successfully", enLCGreen);
	return mmOk;
}

// Lock: standard lock procedure
//==========================================================================================
mmByte* clsBaseTexture::Lock(mmInt * Pitch)
{
	D3DLOCKED_RECT lr;
	if (FAILED(m_ptex->LockRect(0, &lr, mmnull, 0)))
	{
		g_plog->Out("clsBaseTexture::Create[" + m_name + "] >> cant lock texture", enLCRed);
		return mmnull;
	}
	if(Pitch) *Pitch = lr.Pitch;
	return (mmByte*)lr.pBits;
}
// Lock: standard unlock procedure
//==========================================================================================
mmVoid clsBaseTexture::Unlock()
{
	if (FAILED(m_ptex->UnlockRect(0)))
	{
		g_plog->Out("clsBaseTexture::Create[" + m_name + "] >> cant unlock texture", enLCRed);
	}
}

#ifdef MHM_USE_DX8
mmRes clsBaseTexture::LockSurface(MHMDirect3DTexture *m_ptex, mmInt l,  mmInt *dx,mmInt *dy,mmInt *Pitch,mmByte **Data, LPDIRECT3DSURFACE8 *src)
#else
mmRes clsBaseTexture::LockSurface(MHMDirect3DTexture *m_ptex, mmInt l,  mmInt *dx,mmInt *dy,mmInt *Pitch,mmByte **Data, LPDIRECT3DSURFACE9 *src)
#endif
{
		D3DSURFACE_DESC desc;
		D3DLOCKED_RECT lr;

		if(FAILED(m_ptex->GetSurfaceLevel(l,src)))
		{
			g_plog->Out("clsBaseTexture::GenerateMipMaps[" + m_name + "] >> cant get surface #"+l, enLCRed);
			return mmFail;
		}
		if(FAILED((*src)->GetDesc(&desc)))
		{
			g_plog->Out("clsBaseTexture::GenerateMipMaps[" + m_name + "] >> cant get surface desc", enLCRed);
			return mmFail;
		}
		if(FAILED((*src)->LockRect(&lr,NULL,0)))
		{
			g_plog->Out("clsBaseTexture::GenerateMipMaps[" + m_name + "] >> cant lock surface", enLCRed);
			return mmFail;
		}
		*dx = desc.Width;
		*dy = desc.Height;
		*Pitch = lr.Pitch;
		*Data = (mmByte*)lr.pBits;

		return mmOk;
}

void	clsBaseTexture::LinearFilter2(mmUint32 mask, mmByte *s_Data, mmInt s_Pitch, mmByte *d_Data, mmInt d_Pitch, mmInt d_dx, mmInt d_dy)
{
		mmInt y;

	if(mask == 0xff00ff)
	{
				for(y = d_dy; y>0; y--)
				{
#ifdef WIN32
					__asm{
						mov ecx,d_dx
						mov esi,s_Data
						mov edx,s_Pitch
						mov edi,d_Data
						add edx,esi
mipmap_loop:
						movq mm0,[esi]
						add  esi,8
						pxor mm7,mm7
						add  edi,4
						movq mm1,[edx]
						add  edx,8
						movq mm2,mm0
						movq mm3,mm1
						punpckhbw  mm0,mm7
						punpckhbw  mm1,mm7
						punpcklbw  mm2,mm7
						punpcklbw  mm3,mm7
						paddw  mm0,mm1
						paddw  mm2,mm3
						paddw  mm2,mm0
						psrlw  mm2,2
						packuswb mm2,mm2
						movd   [edi-4],mm2
						dec	ecx
						jnz mipmap_loop
						emms
					}
#else
					{
					register mmUint r,g,b,a;
					mmByte *s_Line, *d_Line;
					s_Line = s_Data;
					d_Line = d_Data;
					for(mmInt x = d_dx; x>0; x--)
					{
						r = *s_Line++;
						g = *s_Line++;
						b = *s_Line++;
						a = *s_Line++;
						r += *s_Line++;
						g += *s_Line++;
						b += *s_Line++;
						a += *s_Line++;

						r += s_Line[s_Pitch-8];
						g += s_Line[s_Pitch-7];
						b += s_Line[s_Pitch-6];
						a += s_Line[s_Pitch-5];

						r += s_Line[s_Pitch-4];
						g += s_Line[s_Pitch-3];
						b += s_Line[s_Pitch-2];
						a += s_Line[s_Pitch-1];

						*d_Line++ = r>>2;
						*d_Line++ = g>>2;
						*d_Line++ = b>>2;
						*d_Line++ = a>>2;
					}
					}
#endif
					s_Data += s_Pitch;
					s_Data += s_Pitch;
					d_Data += d_Pitch;
				}
	}
	else // 16 bit !!!
	{
#ifdef WIN32
			if( (d_dx&1) == 0)
			{
				{
				mmUint32 mask2 = mask | (((~mask)<<14) & 0x3fff0000);
				for(y = d_dy; y>0; y--)
				{
				__asm{
						mov ecx,d_dx
						mov esi,s_Data
						mov edx,s_Pitch
						mov edi,d_Data
						movd mm0,mask2
						movq mm7,mm0
						psllq mm7,32
						add edx,esi
						por mm7,mm0
mipmap_loop16:
						movq mm0,[esi]
						movq mm1,mm0
						add  esi,8
						movq mm2,[edx]
						movq mm3,mm2
						add  edx,8
						movq mm4,mm1
						movq mm5,mm3
						psrlw mm4,2
						psrlw mm5,2
						punpckhwd  mm0,mm4
						punpckhwd  mm2,mm5
						punpcklwd  mm1,mm4
						punpcklwd  mm3,mm5
						movq mm4,mm1
						movq mm5,mm3
						punpckhdq  mm1,mm0
						punpckhdq  mm3,mm2
						punpckldq  mm4,mm0
						punpckldq  mm5,mm2
						pand mm1,mm7
						pand mm3,mm7
						pand mm4,mm7
						pand mm5,mm7
						paddd mm1,mm3
						paddd mm4,mm5
						paddd mm1,mm4
						psrld mm1,2
						pand  mm1,mm7
						movq  mm0,mm1
						pslld mm1,14
						por   mm1,mm0
						pslld mm1,2
						psrad mm1,16
						add  edi,4
						packssdw mm1,mm1
						movd   [edi-4],mm1
						sub    ecx,2
						jnz    mipmap_loop16
						emms
					}
					s_Data += s_Pitch;
					s_Data += s_Pitch;
					d_Data += d_Pitch;
				}
				}
			}
			else
#endif
			{
				for(y = d_dy; y>0; y--)
				{
					{
					register mmUint32 col,col0;
					register mmUint16 *s_Line, *d_Line;
					s_Line = (mmUint16 *)s_Data;
					d_Line = (mmUint16 *)d_Data;
					for(mmInt x = d_dx; x>0; x--)
					{
						col0 = *s_Line++;
						col0 = (col0&mask)|((col0&(~mask))<<14);
						col = *s_Line++;
						col = (col&mask)|((col&(~mask))<<14);
						col0 += col;
						col = *(s_Line+s_Pitch-2);
						col = (col&mask)|((col&(~mask))<<14);
						col0 += col;
						col = *(s_Line+s_Pitch-1);
						col = (col&mask)|((col&(~mask))<<14);
						col0 += col;
						col0 >>= 2;
						*d_Line++ = (col0&mask)|((col0>>14)&(~mask));
					}
					}
					s_Data += s_Pitch;
					s_Data += s_Pitch;
					d_Data += d_Pitch;
				}
			}
	}
}


// GenerateMipMaps: standard generation of all mip levels
//==========================================================================================
mmVoid clsBaseTexture::GenerateMipMaps()
{
	#ifdef MHM_USE_DX8
		LPDIRECT3DSURFACE8 src,dst;
	#else
		LPDIRECT3DSURFACE9 src,dst;
	#endif
	mmInt s_Pitch,d_Pitch;
	mmByte *s_Data, *d_Data;
	mmByte *Data;
	mmInt s_dx,s_dy;
	mmInt d_dx,d_dy;
	mmInt lc = m_ptex->GetLevelCount();
	mmInt i;
	mmUint32 mask;

	if(lc>1)
	{
		if(MHMSUCC(LockSurface(m_ptex, 0, &s_dx, &s_dy,  &s_Pitch, &s_Data, &src)))
		{
			for(i=1; i<lc;i++)
			{
				if(MHMFAIL(LockSurface(m_ptex, i, &d_dx, &d_dy,  &d_Pitch, &d_Data, &dst)))
				{
					break;
				}
				Data = d_Data;

				// determine T-FORMAT
				if( (m_fmt == D3DFMT_A8R8G8B8)
				||  (m_fmt == D3DFMT_X8R8G8B8)
				||  (m_fmt == D3DFMT_X8L8V8U8)) mask = 0xff00ff;
				else 
					if( (m_fmt == D3DFMT_X1R5G5B5)
				||  (m_fmt == D3DFMT_A1R5G5B5)) mask = 0x781f;
				else 
					if(m_fmt == D3DFMT_R5G6B5)  mask = 0xf81f;
				else 
					if(m_fmt == D3DFMT_V8U8)  mask = 0xff;
				else 
					if(m_fmt == D3DFMT_L6V5U5)  mask = 0xfc1f;

				LinearFilter2(mask, s_Data, s_Pitch, d_Data, d_Pitch, d_dx, d_dy);

				src->UnlockRect();
				src = dst;
				s_dx = s_dy;
				s_dy = s_dy;
				s_Pitch = d_Pitch;
				s_Data = Data;
			}
			src->UnlockRect();
		}
	}
	m_ptex->PreLoad();
}
