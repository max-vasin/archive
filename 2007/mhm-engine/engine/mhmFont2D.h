//==========================================================================================
// Mayhem engine 2D font resource class definition
// Coded:	MozG
// Created:	5 june 2006
//==========================================================================================

#ifndef _MHMFONT2D_INCLUDED
#define _MHMFONT2D_INCLUDED

//=========================================================
// Font parameters
//=========================================================
class clsFont2DParams: public clsResourceParams
{
public:
	mmBool		m_key_enable;	// color key is enabled
	clsColorRGB m_key_low;		// color key low color value
	clsColorRGB m_key_hi;		// color key hi color value
	mmByte		m_key_alpha;	// color key alpha
	mmInt		m_maxchars;		// maximum text length
};

//=========================================================
// Font rectangle
//=========================================================
struct stFontRect
{
	mmInt	m_left;
	mmInt	m_right;
	mmInt	m_top;
	mmInt	m_bottom;
};

//=========================================================
// Font vertex
//=========================================================
struct stFontVertex
{
	clsVector4	m_pos;
	mmDword		m_color;
	clsVector2	m_tex;
};

//=========================================================
// Font
//=========================================================
class clsFont2D: public clsResource
{
protected:
	clsBaseTexture*		m_ptexture;
	clsVertexBuffer*	m_pvbuf;
	mmInt				m_width;
	mmInt				m_height;
	mmInt				m_maxchars;
	clsString			m_text;
	mmBool				m_enablekey;
	mmByte				m_alpharef;
	mmBool				m_created;
	
public:
	clsFont2D(clsResourceManager* presman, clsString name, enResType type = enResFont2D);
    virtual ~clsFont2D();
    virtual mmRes Create(clsResourceParams* params);
    virtual mmRes Render();
    
    mmRes   SetRect(stFontRect* prc, mmDword color = 0xFFFFFFFF);
    mmRes   SetText(clsString   text);
    mmRes   SetColor(mmDword color);
    
protected:
#ifdef _DEBUG
	mmVoid DumpBuffer(stFontVertex* pfv);
#endif
};

#endif // _MHMFONT2D_INCLUDED