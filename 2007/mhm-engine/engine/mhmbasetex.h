//==========================================================================================
// Mayhem engine texture resource class
// Coded:	MozG
// Created:	14 may 2006
//------
// Edited	16 may 2006
// no more clsTexture2D class - become clsBaseTexture
// this is not loadable resource
//==========================================================================================

#ifndef _MHMBASETEX_INCLUDED
#define _MHMBASETEX_INCLUDED

// Texture depth enumerator
//==========================================================================================
enum enTexDepth
{
	enTexDepth16	= 16,
	enTexDepth32	= 32,
	enTexDepth15	= 15 // 555 (16 бит)
};

// Texture creation parameters
//==========================================================================================
class clsBaseTextureParams: public clsResourceParams
{
public:
	mmUint		m_width;		// texture width for base texture, from file otherwise
	mmUint		m_height;		// texture height for base texture, from file otherwise
	enTexDepth	m_depth;		// texture depth
	mmBool		m_alpha;		// texture needs alpha channel (from file or colorkey)
	mmBool		m_mipgen;		// texture needs mip generation
	mmBool		m_rentarget;	// texture been used as rendertarget
	mmUint		m_levels;		// texture mip levels
	mmBool		m_bump;			// texture used as bump-map
	mmBool		m_luminance;	// then texture used as bump-map this flag specify luminance
	mmBool		m_key_enable;	// color key flag enabling
	clsColorRGB m_key_low;		// color key low color value
	clsColorRGB	m_key_hi;		// color key hi color value
	mmByte		m_key_alpha;	// color key alpha value which applied to color range
	
	clsBaseTextureParams()
	{
		m_width = m_height = m_levels = 0;
		m_depth = enTexDepth16;
		m_alpha = m_mipgen = m_rentarget = m_bump = m_luminance = mmfalse;
		m_key_enable = mmfalse;
		m_key_alpha = 0;
	}
};

// Color macros
//==========================================================================================
#define mmMakeA4R4G4B4(r, g, b, a)	( (((a) >> 4) << 12)   | (((r) >> 4) << 8)    | (((g) >> 4) << 4) | (b >> 4) )
#define mmMakeR5G6B5(r, g, b)		( (((r) >> 3) << 11)   | (((g) >> 2) << 5)    | (((b) >> 3)) )
#define mmMakeA8R8G8B8(r, g, b, a)  ( ((a) << 24)  | ((r) << 16) | ((g)  << 8)    | (b) )
#define mmMakeX8R8G8B8(r, g, b)	    ( (0xFF << 24) | ((r) << 16) | ((g)  << 8)    | (b) )
// Texture
//==========================================================================================
class clsBaseTexture: public clsResource
{
protected:
	mmUint					m_width;
	mmUint					m_height;
	D3DFORMAT				m_fmt;
	D3DPOOL					m_pool;
	mmDword					m_usage;
	MHMDirect3DTexture*		m_ptex;
	mmUint					m_numlvl;
	enTexDepth				m_texdepth;
	
public:
	clsBaseTexture(clsResourceManager* presman, clsString name, 
				   enResType type = enResBaseTexture);
    virtual ~clsBaseTexture();
    virtual mmRes Create(clsResourceParams* params);
    virtual mmRes Invalidate(clsDeviceInvalidateEvent* pevent);
    virtual mmRes Restore(clsDeviceRestoreEvent* pevent);
    
    mmRes   ConvertToBump();
    
    mmUint	GetWidth();
    mmUint	GetHeight();

    operator MHMDirect3DTexture*();
	mmByte* Lock(mmInt * Pitch=mmnull);
	mmVoid	Unlock();

protected:
	virtual mmRes OnPreCreate(clsBaseTextureParams* params);
	virtual mmRes OnPostCreate(clsBaseTextureParams* params);
	mmRes MakeParameters(clsBaseTextureParams* params);
	
	mmRes CreateTexture(clsResourceParams* params);
	mmRes DestroyTexture();

	mmVoid	GenerateMipMaps();

#ifdef MHM_USE_DX8
	mmRes	LockSurface(MHMDirect3DTexture *m_ptex, mmInt l,  mmInt *dx,mmInt *dy,mmInt *Pitch,mmByte **Data, LPDIRECT3DSURFACE8*);
#else
	mmRes	LockSurface(MHMDirect3DTexture *m_ptex, mmInt l,  mmInt *dx,mmInt *dy,mmInt *Pitch,mmByte **Data, LPDIRECT3DSURFACE9*);
#endif

	void	LinearFilter2(mmUint32 mask, mmByte *s_Data, mmInt s_Pitch, mmByte *d_Data, mmInt d_Pitch, mmInt d_dx, mmInt d_dy);
};



#endif //_MHMBASETEX_INCLUDED