//==========================================================================================
// Mayhem engine TGA texture class implementation
// Coded:	MozG
// Created:	18 may 2006
//==========================================================================================

#include "mhmbase.h"

// TGA texture class constructor
//==========================================================================================
clsTextureTGA::clsTextureTGA(clsResourceManager *presman, clsString name):
							clsBaseTexture(presman, name, enResTextureTGA)
{

}


// TGA texture class destructor
//==========================================================================================
clsTextureTGA::~clsTextureTGA()
{
}


#define CALC_POS() {\
	offset  = i%tgahead.Width; \
	mempos  = (mmDword)(( (tgahead.ImageDescriptor & 0x20)?(i / tgahead.Width):((tgahead.Height-1) - (i / tgahead.Width))) * Pitch) + (offset * sizeof(mmByte) * ((m_texdepth == enTexDepth16) ? 2 : 4));\
	}

#define SAVE_PIXEL(mem, _r, _g, _b)\
				switch(m_fmt)\
				{\
				case D3DFMT_R5G6B5:\
					*(mmWord*)&pmem[mem]  = mmMakeR5G6B5(_r, _g, _b);\
					break;\
				case D3DFMT_X8R8G8B8:\
					*(mmDword*)&pmem[mem] = mmMakeX8R8G8B8(_r, _g, _b);\
					break;\
				case D3DFMT_A8R8G8B8:\
					if ((pp != mmnull) && (_r >= pp->m_key_low.r) && (_r <= pp->m_key_hi.r)\
									   && (_g >= pp->m_key_low.g) && (_g <= pp->m_key_hi.g)\
									   && (_b >= pp->m_key_low.b) && (_b <= pp->m_key_hi.b))\
					{\
						*(mmDword*)&pmem[mem] = mmMakeA8R8G8B8(_r, _g, _b, pp->m_key_alpha);\
					}\
					else\
						*(mmDword*)&pmem[mem] = mmMakeX8R8G8B8(_r, _g, _b);\
					break;\
				}

#define SAVE_PIXEL32(mem, _r, _g, _b, _a )\
				if(m_fmt == D3DFMT_A8R8G8B8)\
				{\
					*(mmDword*)&pmem[mempos] = mmMakeA8R8G8B8(_r, _g, _b, _a);\
				}

#define TEXTURE_ERROR(str) {err = str; break;}



// Load: loads texture from file
//==========================================================================================
mmRes clsTextureTGA::Create(clsResourceParams *params)
{
	clsTextureTGAParams* pp = (clsTextureTGAParams*)params;
	// загрузим заголовок файла и подберем подходящие параметры для создания текстуры
	stTGAHead tgahead;
	clsString filename = (m_fullname.IsEmpty()) ? m_name : m_fullname;
	clsFile file(filename);

	clsString err("");
	do{

	if (!file.IsExist()) TEXTURE_ERROR("file not exist or could'nt be opened now")
	if (MHMFAIL(file.Open())) TEXTURE_ERROR("cant open file")
	if (MHMFAIL(file.Read(&tgahead, sizeof(tgahead)))) TEXTURE_ERROR("file header reading error")

	mmBool bcompress  = mmfalse;
	mmBool bsupported = mmfalse;
	if ((tgahead.DataType == TARGA_TRUECOLOR_RLE_IMAGE) ||
		(tgahead.DataType == TARGA_BW_RLE_IMAGE))
	{
		bcompress	= mmtrue;
		bsupported	= mmtrue;
	}
	
	if ((tgahead.DataType == TARGA_TRUECOLOR_IMAGE) ||
		(tgahead.DataType == TARGA_BW_IMAGE))
			bsupported = mmtrue;
	
	if (!bsupported) TEXTURE_ERROR("unsupported file format")
	
	clsBaseTextureParams btparams;
	btparams.m_alpha     = (tgahead.ColorBits == 32) || ((pp != mmnull) && (pp->m_key_enable));
	btparams.m_depth     = (pp == mmnull) ? enTexDepth16 : pp->m_depth;
	btparams.m_width     = tgahead.Width;
	btparams.m_height    = tgahead.Height;
	btparams.m_rentarget = (pp == mmnull) ? mmfalse : pp->m_rentarget;
	btparams.m_levels    = (pp == mmnull) ? 0 : pp->m_levels;
	btparams.m_mipgen    = (pp == mmnull) ? mmtrue : pp->m_mipgen;
	
	if (MHMFAIL(CreateTexture(&btparams)))  TEXTURE_ERROR("cant create texture")
/*	
	D3DLOCKED_RECT lr;
	if (FAILED(m_ptex->LockRect(0, &lr, mmnull, 0)))   TEXTURE_ERROR("cant lock texture")
	
	mmByte* pmem = (mmByte*)lr.pBits;
*/	
	mmInt Pitch;
	mmByte* pmem = Lock(&Pitch);

	mmByte color[4];
	mmByte blockhead, blocklen;
	mmDword offset, mempos;


	if (tgahead.ColorBits == 24)
	{
		mmInt i = 0;
		while(i < (tgahead.Width * tgahead.Height))
		{
			if (!bcompress)
			{
				if (MHMFAIL(file.Read(color, sizeof(mmByte) * 3))) TEXTURE_ERROR("color data file reading error")
				
				CALC_POS();
				SAVE_PIXEL(mempos, color[2], color[1], color[0]);
				i++;
			}
			else // RLE compressed tga
			{
				if (MHMFAIL(file.Read(&blockhead, sizeof(mmByte)))) TEXTURE_ERROR("RLE block head file reading error")
				
				blocklen = blockhead & 0x7F;
				if (blockhead & 0x80) // RLE block
				{
					if (MHMFAIL(file.Read(color, sizeof(mmByte) * 3))) TEXTURE_ERROR("color data file reading error")

						for (mmUint n = 0; n < (mmUint)blocklen + 1; n++)
					{
						//offset  = i - ((mmDword)(i / tgahead.Width) * tgahead.Width);
						//mempos  = (mmDword)((i / tgahead.Width) * lr.Pitch) + (offset * sizeof(mmByte) * ((m_texdepth == enTexDepth16) ? 2 : 4));
						CALC_POS();
						SAVE_PIXEL(mempos, color[2], color[1], color[0]);
						i++;
					}
				}
				else // simple block
					for (mmUint n = 0; n < (mmUint)blocklen + 1; n++)
					{
						if (MHMFAIL(file.Read(color, sizeof(mmByte) * 3))) TEXTURE_ERROR("color data file reading error")
						//offset  = i - ((mmDword)(i / tgahead.Width) * tgahead.Width);
						//mempos  = (mmDword)((i / tgahead.Width) * lr.Pitch) + (offset * sizeof(mmByte) * ((m_texdepth == enTexDepth16) ? 2 : 4));
						CALC_POS();
						SAVE_PIXEL(mempos, color[2], color[1], color[0]);
						i++;
					} // for
			} // compressed/uncompressed
			if(err!="") break;
		}// while
	}
	else // 32 bit
	{
		mmInt i = 0;
		while(i < (tgahead.Width * tgahead.Height))
		{
			//offset  = i - ((mmDword)(i / tgahead.Width) * tgahead.Width);
			//mempos  = (mmDword)((i / tgahead.Width) * lr.Pitch) + (offset * sizeof(mmByte) * 4);

			CALC_POS();

			if (!bcompress)
			{
				if (MHMFAIL(file.Read(color, sizeof(mmByte) * 4))) TEXTURE_ERROR("color data file reading error")
				
				SAVE_PIXEL32(mempos, color[2], color[1], color[0], color[3])
				i++;
			}
			else // RLE compressed tga
			{
				if (MHMFAIL(file.Read(&blockhead, sizeof(mmByte)))) TEXTURE_ERROR("RLE block head file reading error")
				
				blocklen = blockhead & 0x7F;
				if (blockhead & 0x80) // RLE block
				{
					if (MHMFAIL(file.Read(color, sizeof(mmByte) * 4))) TEXTURE_ERROR("color data file reading error")

					for (mmUint n = 0; n < (mmUint)blocklen + 1; n++)
					{
						//offset  = i - ((mmDword)(i / tgahead.Width) * tgahead.Width);
						//mempos  = (mmDword)((i / tgahead.Width) * lr.Pitch) + (offset * sizeof(mmByte) * 4);

						CALC_POS();
						SAVE_PIXEL32(mempos, color[2], color[1], color[0], color[3])
						i++;
					}
				}
				else // simple block
					for (mmUint n = 0; n < (mmUint)blocklen + 1; n++)
					{
						if (MHMFAIL(file.Read(color, sizeof(mmByte) * 4))) TEXTURE_ERROR("color data file reading error")
						//offset  = i - ((mmDword)(i / tgahead.Width) * tgahead.Width);
						//mempos  = (mmDword)((i / tgahead.Width) * lr.Pitch) + (offset * sizeof(mmByte) * 4);

						CALC_POS();
						SAVE_PIXEL32(mempos, color[2], color[1], color[0], color[3])
						i++;
					}
			}
		
		if(err!="") break;
		}
	}

	
/* SEE CALC_POS !!!
	if (!(tgahead.ImageDescriptor & 0x20))
		if (MHMFAIL(Flip(pmem, lr.Pitch)))
		{
			g_plog->Out("clsTextureTGA::Create[" + m_name + "] >> vertical flip failed", enLCRed);
			m_ptex->UnlockRect(0);
			return mmFail;
		}

*/
//	if(err!="") { m_ptex->UnlockRect(0); break; }
//	if (FAILED(m_ptex->UnlockRect(0))) TEXTURE_ERROR("cant unlock texture");

	Unlock();
	if(err!="") { break; }


	GenerateMipMaps();

	g_plog->Out("clsTextureTGA::Create[" + m_name + "] >> created successfully", enLCGreen);
	return mmOk;
	
	}while(1);
	
	g_plog->Out("clsTextureTGA::Create[" + m_name + "] >> " + err, enLCRed);
	return mmFail;
}
/*
// Flip: flips texture vertical
//==========================================================================================
mmRes clsTextureTGA::Flip(mmByte *pmem, mmUint pitch)
{
	if (pmem == mmnull)
		return mmFail;
	
	mmByte* pbuf = new mmByte[pitch];
	for (mmUint y = 0; y < (m_height >> 1); y++)
	{
		memcpy(pbuf, &pmem[pitch * y], pitch);
		memcpy(&pmem[pitch * y], &pmem[pitch * (m_height - y - 1)], pitch);
		memcpy(&pmem[pitch * (m_height - y - 1)], pbuf, pitch);
	}
	mmSafeDeleteAr(pbuf);
	return mmOk;
}
*/

