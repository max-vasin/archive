//==========================================================================================
// Mayhem engine TGA texture class
// Coded:	MozG
// Created:	18 may 2006
//==========================================================================================

#ifndef _MHMTEXTGA_INCLUDED
#define _MHMTEXTGA_INCLUDED


//==========================================================
// TGA data types
//==========================================================
#define TARGA_NO_COLORMAP			0
#define TARGA_COLORMAP				1
#define TARGA_EMPTY_IMAGE			0
#define TARGA_INDEXED_IMAGE			1
#define TARGA_TRUECOLOR_IMAGE		2
#define TARGA_BW_IMAGE				3
#define TARGA_INDEXED_RLE_IMAGE		9
#define TARGA_TRUECOLOR_RLE_IMAGE	10
#define TARGA_BW_RLE_IMAGE			11

//==========================================================
// TGA file header
//==========================================================
#pragma pack(push)
#pragma pack(1)
struct stTGAHead
{
	mmByte IdLength;	     // text identifier length
	mmByte ColorMapId;	     // color map identifier (obsolette)
	mmByte DataType;         // data type (compressed/uncompressed)
	mmWord ColorMapStart;    // color map start offset
	mmWord ColorMapLenght;   // color map length
	mmByte ColorMapDepth;    // color map depth
	mmWord XOrigin;		     // image x-axis origin
	mmWord YOrigin;		     // image y-axis origin
	mmWord Width;		     // image width
	mmWord Height;		     // image height
	mmByte ColorBits;		 // image bits per pixel (24/32 only)
	mmByte ImageDescriptor;  // descriptor
};
#pragma pack(pop)

//==========================================================
// TGA texture parameters block
//==========================================================
class clsTextureTGAParams: public clsBaseTextureParams
{
public:
};

//==========================================================
// TGA texture class
//==========================================================
class clsTextureTGA: public clsBaseTexture
{
public:
	clsTextureTGA(clsResourceManager* presman, clsString name);
   ~clsTextureTGA();
    mmRes Create(clsResourceParams* params);

protected:
	mmRes Flip(mmByte* pmem, mmUint pitch);
};

#endif // _MHMTEXTGA_INCLUDED