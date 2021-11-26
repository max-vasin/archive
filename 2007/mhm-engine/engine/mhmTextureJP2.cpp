 //==========================================================================================
// Mayhem engine JPEG 2000 texture class
// Coded:	MozG based on jaspers library forced by fyrex
// Created:	14.06.2006
//==========================================================================================
#include "mhmbase.h"
#include <jasper/jasper.h>

static mmInt g_iJ2KInit = 0;

// JPEG 2000 texture constructor
//==========================================================================================
clsTextureJP2::clsTextureJP2(clsResourceManager *presman, clsString name):
							clsBaseTexture(presman, name, enResTextureJP2)
{
	if(g_iJ2KInit==0)
	{
		if (jas_init())
		{
			g_plog->Out("clsTextureJP2::clsTextureJP2[" + name + "] >> cant open JP2 library", enLCRed);
		}
	}
	g_iJ2KInit++;
}

// JPEG 2000 texture destructor
//==========================================================================================
clsTextureJP2::~clsTextureJP2()
{
	g_iJ2KInit--;
	if(g_iJ2KInit==0)
	{
		jas_image_clearfmts();
	}
}

// Creating JPEG 2000 texture
//==========================================================================================
mmRes clsTextureJP2::Create(clsResourceParams *params)
{
	
	clsString filename = (m_fullname.IsEmpty() || m_fullname.Length() == 0) ? m_name : m_fullname;
	if (filename.IsEmpty())
	{
		g_plog->Out("clsTextureJP2::Create >> empty full name passed", enLCRed);
		return mmFail;
	}
	clsFile file(filename);
	if (!file.IsExist())
	{
		g_plog->Out("clsTextureJP2::Create[" + m_name + "] >> file not exist or could not be opened now", enLCRed);
		return mmFail;
	}
	if (MHMFAIL(file.Open()))
	{
		g_plog->Out("clsTextureJP2::Create[" + m_name + "] >> cant open file", enLCRed);
		return mmFail;
	}
	
	mmVoid* pfile = mmnull;
	if (MHMFAIL(file.ReadToMemory(&pfile)))
	{
		g_plog->Out("clsTextureJP2::Create[" + m_name + "] >> file reading error", enLCRed);
		return mmFail;
	}
	
	mmInt			iFormatID	= 0;
	jas_stream_t*	InStream	= mmnull;
	jas_image_t*	Image		= mmnull;
	
	if (!(InStream = jas_stream_memopen((mmString)pfile, file.GetSize())))
	{
		g_plog->Out("clsTextureJP2::Create[" + m_name + "] >> cant open JP2 library stream", enLCRed);
		return mmFail;
	}
		
	if ((iFormatID = jas_image_getfmt(InStream)) < 0) 
	{
		g_plog->Out("clsTextureJP2::Create[" + m_name + "] >> cant get JP2 library image format", enLCRed);
		return mmFail;
	}
	
	if (!(Image = jas_image_decode(InStream, iFormatID, 0)))
	{
		g_plog->Out("clsTextureJP2::Create[" + m_name + "] >> cant decode JP2 image", enLCRed);
		return mmFail;
	}
		
	jas_stream_close(InStream);
	
	mmInt iNumcmpts	= jas_image_numcmpts(Image);
	mmInt iWidth	= jas_image_cmptwidth(Image, 0);
	mmInt iHeight	= jas_image_cmptheight(Image, 0);
	mmInt iDepth	= jas_image_cmptprec(Image, 0);
	
		
	clsBaseTextureParams* pp = (clsBaseTextureParams*)params;
	clsBaseTextureParams baseparams;
	baseparams.m_width		= iWidth;
	baseparams.m_height		= iHeight;
	baseparams.m_depth		= (pp == mmnull) ? enTexDepth16 : pp->m_depth;
	baseparams.m_levels		= (pp == mmnull) ? 0 : pp->m_levels ;
	baseparams.m_mipgen		= (pp == mmnull) ? mmtrue : pp->m_mipgen;
	baseparams.m_rentarget	= (pp == mmnull) ? mmfalse : pp->m_rentarget;
	baseparams.m_key_enable = (pp == mmnull) ? mmfalse : pp->m_key_enable;
	if (baseparams.m_key_enable)
	{
		baseparams.m_key_low = pp->m_key_low;
		baseparams.m_key_hi	 = pp->m_key_hi;
	}
	baseparams.m_alpha = (baseparams.m_key_enable || iNumcmpts == 4);
		
	if (MHMFAIL(CreateTexture(&baseparams)))
	{
		g_plog->Out("clsTextureJP2::Create[" + m_name + "] >> cant create appropriate base texture", enLCRed);
		return mmFail;
	}
	
	// Copy to system memory
	mmByte* pTempBuffer;
	pTempBuffer = new mmByte[iWidth * iHeight * ((iNumcmpts == 4) ? 4 : 3)];
	if(!pTempBuffer) goto Fail;
	
	if (iNumcmpts == 4) // RGBA
	{
		jas_image_read_rgba(Image, 0, 0, iWidth, iHeight, pTempBuffer);
	}
	else // RGB
	{
		jas_image_read_rgb(Image, 0, 0, iWidth, iHeight, pTempBuffer);
	}
	
	{
		mmByte *pt = pTempBuffer;
		mmWord *ps;
		mmByte r,g,b,a;
		mmInt x,y;
		mmInt Pitch;

		mmByte* pmem = Lock(&Pitch);
		if(!pmem) goto Fail;

		if(m_fmt == D3DFMT_R5G6B5)
		{ // 16bit
			for(y=iHeight; y>0; y--)
			{
				ps = (mmWord *)pmem;
				for(x=iWidth; x>0; x--)
				{
					r = *pt++; g = *pt++; b = *pt++; if(iNumcmpts == 4) a = *pt++;
					*((mmWord*)ps) = mmMakeR5G6B5(r, g, b);
					ps++;
				}
				pmem += Pitch;
			}
		}
		else
		{ // 32bit
			a = 255;
			for(y=iHeight; y>0; y--)
			{
				ps = (mmWord *)pmem;
				for(x=iWidth; x>0; x--)
				{
					r = *pt++; g = *pt++; b = *pt++; if(iNumcmpts == 4) a = *pt++;
					*((mmDword*)ps) = mmMakeA8R8G8B8(r, g, b, a);
					ps+=2;
				}
				pmem += Pitch;
			}
		}


/*
		mmInt i = 0;
		while(i < (iWidth * iHeight))
		{
			mmDword offset  = i - ((mmDword)(i / iWidth) * iWidth);
			mmDword mempos  = (mmDword)((i / iWidth) * Pitch) + (offset * sizeof(mmByte) * ((m_fmt == D3DFMT_R5G6B5) ? 2 : 4));

			mmByte r = pTempBuffer[i*iNumcmpts+0];
			mmByte g = pTempBuffer[i*iNumcmpts+1];
			mmByte b = pTempBuffer[i*iNumcmpts+2];
			mmByte a = (iNumcmpts == 4) ? pTempBuffer[i*iNumcmpts+3] : 255;
			*(mmDword*)&pmem[mempos] = mmMakeA8R8G8B8(r, g, b, a);

			switch(m_fmt)
			{
			case D3DFMT_R5G6B5:
				*(mmWord*)&pmem[mempos]  = mmMakeR5G6B5(r, g, b);
				break;
			case D3DFMT_X8R8G8B8:
			case D3DFMT_A8R8G8B8:
				*(mmDword*)&pmem[mempos] = mmMakeA8R8G8B8(r, g, b, a);
				break;
			}
			i++;
		}
*/
		Unlock();
	}
	GenerateMipMaps();
/*
	m_ptex->SetAutoGenFilterType(D3DTEXF_NONE);
	m_ptex->GenerateMipSubLevels();
	if (FAILED(m_ptex->SetAutoGenFilterType(D3DTEXF_POINT)))
	{
		int x = 1;
	}
	m_ptex->GenerateMipSubLevels();
*/
	jas_image_destroy(Image);
	mmSafeDeleteAr(pTempBuffer);
	file.FreeMemory(pfile);
	g_plog->Out("JPEG 2000 texture [" + m_name + "] loaded successfully", enLCGreen);
	return mmOk;

Fail:
	jas_image_destroy(Image);
//	jas_image_clearfmts();
	mmSafeDeleteAr(pTempBuffer);
	file.FreeMemory(pfile);
	g_plog->Out("JPEG 2000 texture [" + m_name + "] load failed", enLCGreen);
	return mmFail;

}

