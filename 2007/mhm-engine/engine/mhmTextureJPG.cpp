//==========================================================================================
// Mayhem engine JPEG baseline texture class implementation
// Coded:	MozG based on code provided by Pink
// Created:	14.06.2006
//==========================================================================================
#include "mhmbase.h"

// JPEG texture constructor
//==========================================================================================
clsTextureJPG::clsTextureJPG(clsResourceManager *presman, clsString name):
			     clsBaseTexture(presman, name, enResTextureJPG)
{
	m_id = 0;
	mmZeroVar(m_jpeg_file_s);
}

// JPEG texture destructor
//==========================================================================================
clsTextureJPG::~clsTextureJPG()
{

}

// Create: creates new JPG texture
//==========================================================================================
mmRes clsTextureJPG::Create(clsResourceParams *params)
{
	clsString filename = (m_fullname.IsEmpty() || m_fullname.Length() == 0) ? m_name : m_fullname;
	if (filename.IsEmpty())
	{
		g_plog->Out("clsTextureJPG::Create >> empty full name passed", enLCRed);
		return mmFail;
	}
	clsFile file(filename);
	if (!file.IsExist())
	{
		g_plog->Out("clsTextureJPG::Create[" + m_name + "] >> file not exist or could not be opened now", enLCRed);
		return mmFail;
	}
	if (MHMFAIL(file.Open()))
	{
		g_plog->Out("clsTextureJPG::Create[" + m_name + "] >> cant open file", enLCRed);
		return mmFail;
	}
	mmVoid* pfile = mmnull;
	if (MHMFAIL(file.ReadToMemory(&pfile)))
	{
		g_plog->Out("clsTextureJPG::Create[" + m_name + "] >> file reading error", enLCRed);
		return mmFail;
	}
	
	m_jpeg_file_s.file = (mmByte*)pfile;
	
	if (MHMFAIL(ReadMarkers()))
	{
		g_plog->Out("clsTextureJPG::Create[" + m_name + "] >> read JPEG header failed", enLCRed);
		return mmFail;
	}
	
	clsBaseTextureParams* pp = (clsBaseTextureParams*)params;
	clsBaseTextureParams baseparams;
	baseparams.m_width		= m_jpeg_file_s.width;
	baseparams.m_height		= m_jpeg_file_s.height;
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
	baseparams.m_alpha = baseparams.m_key_enable;
	
	
	
	if (MHMFAIL(CreateTexture(&baseparams)))
	{
		g_plog->Out("clsTextureJPG::Create[" + m_name + "] >> cant create appropriate base texture", enLCRed);
		return mmFail;
	}

	// Decompress to system memory
	m_jpeg_file_s.data = new mmByte[m_jpeg_file_s.width * m_jpeg_file_s.height * 4];
	if (m_jpeg_file_s.data == mmnull)
	{
		g_plog->Out("clsTextureJPG::Create[" + m_name + "] >> out of system memory", enLCRed);
		return mmFail;
	}
		
	Decompress();
	
	if (m_jpeg_file_s.num_components == 1) Gray2RGB();
	if (m_jpeg_file_s.num_components == 3) YCBCR2RGB();
/*
	D3DLOCKED_RECT lr;
	if (FAILED(m_ptex->LockRect(0, &lr, mmnull, 0)))
	{
		g_plog->Out("clsTextureJPG::Create[" + m_name + "] >> cant lock texture", enLCRed);
		return mmFail;
	}
	
	mmByte* pmem = (mmByte*)lr.pBits;
*/
	mmInt Pitch;
	mmByte* pmem = Lock(&Pitch);
	if(!pmem) return mmFail;

	mmInt i = 0;
	while(i < (m_jpeg_file_s.width * m_jpeg_file_s.height))
	{
		mmDword offset  = i - ((mmDword)(i / m_jpeg_file_s.width) * m_jpeg_file_s.width);
		mmDword mempos  = (mmDword)((i / m_jpeg_file_s.width) * Pitch) + (offset * sizeof(mmByte) * ((m_texdepth == enTexDepth16) ? 2 : 4));
		
		mmByte r = m_jpeg_file_s.data[i*4];
		mmByte g = m_jpeg_file_s.data[i*4+1];
		mmByte b = m_jpeg_file_s.data[i*4+2];
		
		switch(m_fmt)
		{
		case D3DFMT_R5G6B5:
			*(mmWord*)&pmem[mempos]  = mmMakeR5G6B5(r, g, b);
			break;
		case D3DFMT_X8R8G8B8:
			*(mmDword*)&pmem[mempos] = mmMakeX8R8G8B8(r, g, b);
			break;
		case D3DFMT_A8R8G8B8:
			if ((pp != mmnull) && (r >= pp->m_key_low.r) && (r <= pp->m_key_hi.r)
							   && (g >= pp->m_key_low.g) && (g <= pp->m_key_hi.g)
							   && (b >= pp->m_key_low.b) && (b <= pp->m_key_hi.b))
			{
				*(mmDword*)&pmem[mempos] = mmMakeA8R8G8B8(r, g, b, pp->m_key_alpha);
			}
			else
				*(mmDword*)&pmem[mempos] = mmMakeX8R8G8B8(r, g, b);
		}
		i++;
	}
	Unlock();
	GenerateMipMaps();
/*	
	m_ptex->SetAutoGenFilterType(D3DTEXF_NONE);
	m_ptex->GenerateMipSubLevels();
	if (FAILED(m_ptex->SetAutoGenFilterType(D3DTEXF_POINT)))
	{
		int x = 1;
	}
	m_ptex->GenerateMipSubLevels();
	mmUint levels = m_ptex->GetLevelCount();
*/
	mmSafeDeleteAr(m_jpeg_file_s.data);
	file.FreeMemory(pfile);
	g_plog->Out("JPEG texture [" + m_name + "] loaded successfully", enLCGreen);
	return mmOk;
}

// ReadMarkers: reads JPG file header markers
//==========================================================================================
mmRes clsTextureJPG::ReadMarkers()
{
	int marker,length,i,j,k,l,m;
	jpeg_huffman_table_t *hptr;
	for(;;) {
		marker = ReadByte();
		if(marker != 0xFF) return mmFail;
		marker = ReadByte();
		if(marker != 0xD8) {
			length = ReadWord();
			length -= 2;
			switch(marker) {
				case 0xC0:  // Baseline
					m_jpeg_file_s.data_precision = ReadByte();
					m_jpeg_file_s.height = ReadWord();
					m_jpeg_file_s.width = ReadWord();
					m_jpeg_file_s.num_components = ReadByte();
					if(length - 6 != m_jpeg_file_s.num_components * 3) return mmFail;
					for(i = 0; i < m_jpeg_file_s.num_components; i++) {
						m_jpeg_file_s.component_info[i].id = ReadByte();
						j = ReadByte();
						m_jpeg_file_s.component_info[i].h = (j >> 4) & 0x0F;
						m_jpeg_file_s.component_info[i].v = j & 0x0F;
						m_jpeg_file_s.component_info[i].t = ReadByte();
					}
					break;
				case 0xC1:  // Extended sequetial, Huffman
				case 0xC2:  // Progressive, Huffman            
				case 0xC3:  // Lossless, Huffman
				case 0xC5:  // Differential sequential, Huffman
				case 0xC6:  // Differential progressive, Huffman
				case 0xC7:  // Differential lossless, Huffman
				case 0xC8:  // Reserved for JPEG extensions
				case 0xC9:  // Extended sequential, arithmetic
				case 0xCA:  // Progressive, arithmetic
				case 0xCB:  // Lossless, arithmetic
				case 0xCD:  // Differential sequential, arithmetic
				case 0xCE:  // Differential progressive, arithmetic
				case 0xCF:  // Differential lossless, arithmetic
					return mmFail;
					break;
				case 0xC4:  // Huffman table
					while(length > 0) {
						k = ReadByte();
						if(k & 0x10) hptr = &m_jpeg_file_s.hac[k & 0x0F];
						else hptr = &m_jpeg_file_s.hdc[k & 0x0F];
						for(i = 0, j = 0; i < 16; i++) {
							hptr->bits[i] = ReadByte();
							j += hptr->bits[i];
						}
						length -= 17;
						for(i = 0; i < j; i++)
							hptr->hval[i] = ReadByte();
						length -= j;
						for(i = 0, k = 0, l = 0; i < 16; i++) {
							for(j = 0; j < hptr->bits[i]; j++, k++) {
								hptr->size[k] = i + 1;
								hptr->code[k] = l++;
							}
							l <<= 1;
						}
					}
					break;
				case 0xDB:  // Quantization table
					while(length > 0) {
						j = ReadByte();
						k = (j >> 4) & 0x0F;
						for(i = 0; i < 64; i++) {
							if(k) m_jpeg_file_s.qtable[j][i] = ReadWord();
							else  m_jpeg_file_s.qtable[j][i] = ReadByte();
						}
						length -= 65;
						if(k) length -= 64;
					}
					break;
				case 0xD9:  // End of image (EOI)
					return 0;
					break;
				case 0xDA:  // Start of scan (SOS)
					j = ReadByte();
					for(i = 0; i < j; i++) {
						k = ReadByte();
						m = ReadByte();
						for(l = 0; l < m_jpeg_file_s.num_components; l++)
							if(m_jpeg_file_s.component_info[l].id == k) {
								m_jpeg_file_s.component_info[l].td = (m >> 4) & 0x0F;
								m_jpeg_file_s.component_info[l].ta = m & 0x0F;
							}
					}
					m_jpeg_file_s.scan.ss = ReadByte();
					m_jpeg_file_s.scan.se = ReadByte();
					k = ReadByte();
					m_jpeg_file_s.scan.ah = (k >> 4) & 0x0F;
					m_jpeg_file_s.scan.al = k & 0x0F;
					return mmOk;
					break;
				case 0xDD:  // Restart interval
					m_jpeg_file_s.restart_interval = ReadWord();
					break;
				default:
					m_id += length;
					break;
			}
		}
	}
}

// ReadByte: reads byte from JPG file data
//==========================================================================================
mmInt clsTextureJPG::ReadByte()
{
	m_jpeg_file_s.curbyte = m_jpeg_file_s.file[m_id++];
	m_jpeg_file_s.curbit = 0;
	return m_jpeg_file_s.curbyte;
}

// ReadWord: reads word from JPG file data
//==========================================================================================
mmInt clsTextureJPG::ReadWord()
{
	unsigned short i;
	i = *((unsigned short *)(&(m_jpeg_file_s.file[m_id])));
	m_id += 2;
	i = ((i << 8) & 0xFF00) + ((i >> 8) & 0x00FF);
	return i;
}

// Decompress: decompress JPEG compressed image onto texture surface
//==========================================================================================
mmVoid clsTextureJPG::Decompress()
{
	register mmInt x,y,i,j,k,l,c;
	mmInt X,Y,H,V,plane,scaleh[3],scalev[3];
	static mmFloat vector[64],dct[64];
	static const mmInt jpeg_zigzag[64] = {
		0, 1, 5, 6,14,15,27,28,
		2, 4, 7,13,16,26,29,42,
		3, 8,12,17,25,30,41,43,
		9,11,18,24,31,40,44,53,
		10,19,23,32,39,45,52,54,
		20,22,33,38,46,51,55,60,
		21,34,37,47,50,56,59,61,
		35,36,48,49,57,58,62,63 };
		static const mmFloat aanscale[8] = {  // 1.0, k = 0; cos(k * PI / 16) * sqrt(2), k = 1...7
			1.0f, 1.387039845f, 1.306562965f, 1.175875602f,
			1.0f, 0.785694958f, 0.541196100f, 0.275899379f };
			scaleh[0] = 1;
			scalev[0] = 1;
			if(m_jpeg_file_s.num_components == 3) {
				scaleh[1] = m_jpeg_file_s.component_info[0].h / m_jpeg_file_s.component_info[1].h;
				scalev[1] = m_jpeg_file_s.component_info[0].v / m_jpeg_file_s.component_info[1].v;
				scaleh[2] = m_jpeg_file_s.component_info[0].h / m_jpeg_file_s.component_info[2].h;
				scalev[2] = m_jpeg_file_s.component_info[0].v / m_jpeg_file_s.component_info[2].v;
			}
			memset(m_jpeg_file_s.dc,0,sizeof(mmInt) * 3);
			for(Y = 0; Y < m_jpeg_file_s.height; Y += m_jpeg_file_s.component_info[0].v << 3) {
				if(m_jpeg_file_s.restart_interval > 0) m_jpeg_file_s.curbit = 0;
				for(X = 0; X < m_jpeg_file_s.width; X += m_jpeg_file_s.component_info[0].h << 3) {
					for(plane = 0; plane < m_jpeg_file_s.num_components; plane++)
						for(V = 0; V < m_jpeg_file_s.component_info[plane].v; V++)
							for(H = 0; H < m_jpeg_file_s.component_info[plane].h; H++) {
								i = HuffmanCode(&m_jpeg_file_s.hdc[m_jpeg_file_s.component_info[plane].td]);
								i &= 0x0F;
								vector[0] = (mmFloat)m_jpeg_file_s.dc[plane] + Bit2Int(i,ReadBits(i));
								m_jpeg_file_s.dc[plane] = (mmInt)vector[0];
								i = 1;
								while(i < 64) {
									j = HuffmanCode(&m_jpeg_file_s.hac[m_jpeg_file_s.component_info[plane].ta]);
									if(j == 0) while(i < 64) vector[i++] = 0;
									else {
										k = i + ((j >> 4) & 0x0F);
										while(i < k) vector[i++] = 0;
										j &= 0x0F;
										vector[i++] = (mmFloat)Bit2Int(j,ReadBits(j));
									}
								}
								k = m_jpeg_file_s.component_info[plane].t;
								for(y = 0, i = 0; y < 8; y++)
									for(x = 0; x < 8; x++, i++) {
										j = jpeg_zigzag[i];
										dct[i] = vector[j] * m_jpeg_file_s.qtable[k][j] *
											aanscale[x] * aanscale[y];
									}
									Idct(dct);
									for(y = 0; y < 8; y++)
										for(x = 0; x < 8; x++) {
											c = ((mmInt)dct[(y << 3) + x] >> 3) + 128;
											if(c < 0) c = 0;
											else if(c > 255) c = 255;
											if(scaleh[plane] == 1 && scalev[plane] == 1) {
												i = X + x + (H << 3);
												j = Y + y + (V << 3);
												if(i < m_jpeg_file_s.width && j < m_jpeg_file_s.height)
													m_jpeg_file_s.data[((j * m_jpeg_file_s.width + i) << 2) + plane] = c;
											}
											else for(l = 0; l < scalev[plane]; l++)
												for(k = 0; k < scaleh[plane]; k++) {
													i = X + (x + (H << 3)) * scaleh[plane] + k;
													j = Y + (y + (V << 3)) * scalev[plane] + l;
													if(i < m_jpeg_file_s.width && j < m_jpeg_file_s.height)
														m_jpeg_file_s.data[((j * m_jpeg_file_s.width + i) << 2) + plane] = c;
												}
										}
							}
				}
			}
}

// HuffmanCode: returns huffman code from table
//==========================================================================================
mmInt clsTextureJPG::HuffmanCode(jpeg_huffman_table_t *table)
{
	register mmInt i,size,code;
	for(size = 1, code = 0, i = 0; size < 17; size++) {
		code <<= 1;
		code |= ReadBit();
		while(table->size[i] <= size) {
			if(table->code[i] == code) return table->hval[i];
			i++;
		}
	}
	return code;
}


// Bit2Int: converts bit value to integer value
//==========================================================================================
mmInt clsTextureJPG::Bit2Int(mmInt bit, mmInt i)
{
	if((i & (1 << (bit - 1))) > 0) return i;
	return -(i ^ ((1 << bit) - 1));
}

// ReadBits: reads given number of bits
//==========================================================================================
mmInt clsTextureJPG::ReadBits(mmInt num)
{
	register mmInt i,j;
	for(i = 0, j = 0; i < num; i++) {
		j <<= 1;
		j |= ReadBit();
	}
	return j;
}

// Idct: ???
//==========================================================================================
mmVoid clsTextureJPG::Idct(mmFloat *data)
{
	mmFloat   t0,t1,t2,t3,t4,t5,t6,t7;
	mmFloat   t10,t11,t12,t13;
	mmFloat   z5,z10,z11,z12,z13;
	mmFloat   *dataptr;
	mmInt i;
	dataptr = data;
	for(i = 0; i < 8; i++) {
		t0 = dataptr[8 * 0];
		t1 = dataptr[8 * 2];
		t2 = dataptr[8 * 4];
		t3 = dataptr[8 * 6];
		t10 = t0 + t2;
		t11 = t0 - t2;
		t13 = t1 + t3;
		t12 = - t13 + (t1 - t3) * 1.414213562f;
		t0 = t10 + t13;
		t3 = t10 - t13;
		t1 = t11 + t12;
		t2 = t11 - t12;
		t4 = dataptr[8 * 1];
		t5 = dataptr[8 * 3];
		t6 = dataptr[8 * 5];
		t7 = dataptr[8 * 7];
		z13 = t6 + t5;
		z10 = t6 - t5;
		z11 = t4 + t7;
		z12 = t4 - t7;
		t7 = z11 + z13;
		t11 = (z11 - z13) * 1.414213562f;
		z5 = (z10 + z12) * 1.847759065f;
		t10 = - z5 + z12 * 1.082392200f;
		t12 = z5 - z10 * 2.613125930f;
		t6 = t12 - t7;
		t5 = t11 - t6;
		t4 = t10 + t5;
		dataptr[8 * 0] = t0 + t7;
		dataptr[8 * 7] = t0 - t7;
		dataptr[8 * 1] = t1 + t6;
		dataptr[8 * 6] = t1 - t6;
		dataptr[8 * 2] = t2 + t5;
		dataptr[8 * 5] = t2 - t5;
		dataptr[8 * 4] = t3 + t4;
		dataptr[8 * 3] = t3 - t4;
		dataptr++;
	}
	dataptr = data;
	for(i = 0; i < 8; i++) {
		t10 = dataptr[0] + dataptr[4];
		t11 = dataptr[0] - dataptr[4];
		t13 = dataptr[2] + dataptr[6];
		t12 = - t13 + (dataptr[2] - dataptr[6]) * 1.414213562f;
		t0 = t10 + t13;
		t3 = t10 - t13;
		t1 = t11 + t12;
		t2 = t11 - t12;
		z13 = dataptr[5] + dataptr[3];
		z10 = dataptr[5] - dataptr[3];
		z11 = dataptr[1] + dataptr[7];
		z12 = dataptr[1] - dataptr[7];
		t7 = z11 + z13;
		t11 = (z11 - z13) * 1.414213562f;
		z5 = (z10 + z12) * 1.847759065f;
		t10 = - z5 + z12 * 1.082392200f;
		t12 = z5 - z10 * 2.613125930f;
		t6 = t12 - t7;
		t5 = t11 - t6;
		t4 = t10 + t5;
		dataptr[0] = t0 + t7;
		dataptr[7] = t0 - t7;
		dataptr[1] = t1 + t6;
		dataptr[6] = t1 - t6;
		dataptr[2] = t2 + t5;
		dataptr[5] = t2 - t5;
		dataptr[4] = t3 + t4;
		dataptr[3] = t3 - t4;
		dataptr += 8;
	}
}

// ReadBit: reads bit value
//==========================================================================================
mmInt clsTextureJPG::ReadBit()
{
	register mmInt i;
	if(m_jpeg_file_s.curbit == 0) {
		ReadByte();
		if(m_jpeg_file_s.curbyte == 0xFF) {
			while(m_jpeg_file_s.curbyte == 0xFF) ReadByte();
			if(m_jpeg_file_s.curbyte >= 0xD0 && m_jpeg_file_s.curbyte <= 0xD7)
				memset(m_jpeg_file_s.dc,0,sizeof(mmInt) * 3);
			if(m_jpeg_file_s.curbyte == 0) m_jpeg_file_s.curbyte = 0xFF;
			else ReadByte();
		}
	}
	i = (m_jpeg_file_s.curbyte >> (7 - m_jpeg_file_s.curbit++)) & 0x01;
	if(m_jpeg_file_s.curbit == 8) m_jpeg_file_s.curbit = 0;
	return i;
}

// YCBCR2RGB: image convert
//==========================================================================================
mmVoid clsTextureJPG::YCBCR2RGB()
{
	mmInt i,Y,Cb,Cr,R,G,B;
	for(i = 0; i < m_jpeg_file_s.width * m_jpeg_file_s.height << 2; i += 4) {
		Y = m_jpeg_file_s.data[i];
		Cb = m_jpeg_file_s.data[i + 1] - 128;
		Cr = m_jpeg_file_s.data[i + 2] - 128;
		R = (mmInt)((mmFloat)Y + 1.40200f * (mmFloat)Cr);
		G = (mmInt)((mmFloat)Y - 0.34414f * (mmFloat)Cb - 0.71414f * (mmFloat)Cr);
		B = (mmInt)((mmFloat)Y + 1.77200f * (mmFloat)Cb);
		if(R < 0) R = 0;
		else if(R > 255) R = 255;
		if(G < 0) G = 0;
		else if(G > 255) G = 255;
		if(B < 0) B = 0;
		else if(B > 255) B = 255;
		m_jpeg_file_s.data[i] = R;
		m_jpeg_file_s.data[i + 1] = G;
		m_jpeg_file_s.data[i + 2] = B;
		m_jpeg_file_s.data[i + 3] = 255;
	}
}

// Gray2RGB: image convert
//==========================================================================================
mmVoid clsTextureJPG::Gray2RGB()
{
	mmInt i,j;
	for(i = 0; i < m_jpeg_file_s.width * m_jpeg_file_s.height << 2; i += 4) {
		j = m_jpeg_file_s.data[i];
		m_jpeg_file_s.data[i + 1] = j;
		m_jpeg_file_s.data[i + 2] = j;
		m_jpeg_file_s.data[i + 3] = 255;
	}
}













