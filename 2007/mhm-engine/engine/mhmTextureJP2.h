//==========================================================================================
// Mayhem engine JPEG 2000 texture class
// Coded:	MozG based on jaspers library forced by fyrex
// Created:	14.06.2006
//==========================================================================================

#ifndef _MHMTEXTUREJP2_INCLUDED
#define _MHMTEXTUREJP2_INCLUDED

class clsTextureJP2: public clsBaseTexture
{
protected:

public:
	clsTextureJP2(clsResourceManager *presman, clsString name);
   ~clsTextureJP2();
    
    mmRes Create(clsResourceParams* params);
};


#endif // _MHMTEXTUREJPG_INCLUDED