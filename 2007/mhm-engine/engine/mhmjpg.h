//==========================================================================================
// Mayhem engine clsJPEG class
// Coded:	Pink
// Created:	14.06.2006
//==========================================================================================

#ifndef _MHMJPG_INCLUDED
#define _MHMJPG_INCLUDED

//****************************

class clsJPEG
{
protected:

	typedef struct {    // Huffman coding tables
		unsigned char bits[16];
		unsigned char hval[256];
		unsigned char size[256];
		unsigned short code[256];
	} jpeg_huffman_table_t;

	struct {            // Jpeg file parameter

		unsigned char * file;  // file
		int     width;  // width image
		int     height; // height image
		unsigned char *data;   // image
		int     data_precision; // bit per component
		int     num_components; // number component
		int     restart_interval;   // restart interval
		struct {
			int     id; // identifier
			int     h;  // horizontal sampling factor
			int     v;  // vertical sampling factor
			int     t;  // Quantization table selector
			int     td; // DC table selector
			int     ta; // AC table selector
		} component_info[3];
		jpeg_huffman_table_t hac[4];    // AC table
		jpeg_huffman_table_t hdc[4];    // DC table
		int     qtable[4][64];  // Quantization table
		struct {
			int     ss,se;  // progressive jpeg spectral selection
			int     ah,al;  // progressive jpeg successive approx
		} scan;
		int     dc[3];
		int     curbit;
		unsigned char   curbyte;
	} jpeg_file_s;


	int id;
	int jpeg_readmarkers();
	int jpeg_read_byte();
	int jpeg_read_word();
	void jpeg_decompress();
	int jpeg_huffmancode(jpeg_huffman_table_t *table);
	int jpeg_bit2int(int bit,int i);
	int jpeg_read_bits(int num);
	void jpeg_idct(float *data);
	int jpeg_read_bit();
	void jpeg_ycbcr2rgb();
	void jpeg_gray2rgb();



public:

	clsJPEG();
	~clsJPEG();

	bool Decompress(void * stream);
	void * GetData();
	int Width();
	int Height();
	void Clear();


};


//****************************

#endif // _MHMJPG_INCLUDED