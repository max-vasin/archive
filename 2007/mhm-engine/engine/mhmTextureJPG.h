//==========================================================================================
// Mayhem engine JPEG baseline texture class
// Coded:	MozG based on code provided by Pink
// Created:	14.06.2006
//==========================================================================================

#ifndef _MHMTEXTUREJPG_INCLUDED
#define _MHMTEXTUREJPG_INCLUDED

class clsTextureJPG: public clsBaseTexture
{
protected:
	struct jpeg_huffman_table_t // Huffman coding tables
	{    
		mmByte bits[16];
		mmByte hval[256];
		mmByte size[256];
		mmWord code[256];
	};
	
	struct st_jpeg_file_s   // Jpeg file parameters
	{
		mmByte *file;				// file
		mmInt   width;				// width image
		mmInt   height;				// height image
		mmByte *data;				// image
		mmInt   data_precision;		// bit per component
		mmInt   num_components;		// number component
		mmInt   restart_interval;   // restart interval
		struct st_component_info
		{
			mmInt id;				// identifier
			mmInt h;				// horizontal sampling factor
			mmInt v;				// vertical sampling factor
			mmInt t;				// Quantization table selector
			mmInt td;				// DC table selector
			mmInt ta;				// AC table selector
		}component_info[3];
		jpeg_huffman_table_t hac[4];// AC table
		jpeg_huffman_table_t hdc[4];// DC table
		mmInt qtable[4][64];		// Quantization table
		struct st_scan
		{
			mmInt ss,se;  // progressive jpeg spectral selection
			mmInt ah,al;  // progressive jpeg successive approx
		}scan;
		mmInt  dc[3];
		mmInt  curbit;
		mmByte curbyte;
	}m_jpeg_file_s;
	mmInt m_id;

public:
	clsTextureJPG(clsResourceManager *presman, clsString name);
   ~clsTextureJPG();
    
    mmRes Create(clsResourceParams* params);

private:
	mmRes ReadMarkers();
	mmInt ReadByte();
	mmInt ReadWord();
	mmVoid Decompress();
	mmInt HuffmanCode(jpeg_huffman_table_t *table);
	mmInt Bit2Int(mmInt bit, mmInt i);
	mmInt ReadBits(int num);
	mmVoid Idct(mmFloat *data);
	mmInt ReadBit();
	mmVoid YCBCR2RGB();
	mmVoid Gray2RGB();
	
};


#endif // _MHMTEXTUREJPG_INCLUDED