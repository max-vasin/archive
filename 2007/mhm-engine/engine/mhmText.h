#pragma once
#include "mhmbasemesh.h"

enum enAlign {enLeft = 0, enRight, enTop, enBottom, enCenter, enFillRect, enLeftRect, enRightRect, enTopRect, enBottomRect, enCenterRect}; 

/*

class clsFontCharInfo
{
protected:
public:
	clsFontCharInfo();
	~clsFontCharInfo();
	mmFloat Width;
	mmFloat Height;
	clsRect UV;
	mmInt MatID;
};
//-----------------------
class clsFontInfo : public clsResource
{
protected:
public:
	clsFontInfo(clsResourceManager* presman, clsString name);
	~clsFontInfo();
	mmRes Create(clsResourceParams* params);
	tmpRefArr<clsFontCharInfo> m_chars;
	tmpRefArr<clsBaseMaterial *> m_map;
	inline clsFontCharInfo & Find(mmChar ch){return m_chars[(mmByte)ch];};
};
//-------------------------
class clsItemFontInfo
{
protected:
public:
	clsItemFontInfo();
	~clsItemFontInfo();

	mmFloat ItemHeight;
	mmFloat CharsWidth;
	mmFloat SpaceWidth;
	mmInt SpaceCount;
	mmInt LastSpace;
	mmFloat WidthPostLastSpace;

};
//-----------------------
class clsMegaText : public clsBaseMesh
{
protected:
	clsFontInfo * m_font;
	mmFloat m_size;
	mmDword m_ch_fvf;
	clsString m_txt;

	mmBool m_wrap;
	mmBool m_use_rect;
	clsRect m_rect;

	tmpRefArr<clsItemFontInfo> m_items;
	mmRes OnCreateMesh(clsResourceParams* params);
	mmVoid Build();
public:
	clsMegaText(clsResourceManager* presman, clsString name);
	virtual ~clsMegaText(void);
	mmVoid SetText(const mmString str);

};
*/
//******************************************************************************
//******************************************************************************
//******************************************************************************
//******************************************************************************

//-----------------------------------------
class clsCharFont
{
protected:
public:
	clsCharFont(){en=mmfalse;}
	mmBool en;
	mmFloat width;
	mmFloat x1;
	mmFloat y1;
	mmFloat x2;
	mmFloat y2;
};
//-----------------------------------------
class clsItemFont
{
protected:
public:

	~clsItemFont(){mmSafeRelease(m_mat)};

	clsString m_name;

	clsBaseMaterial * m_mat;

	mmFloat m_HeightUV;
	mmFloat m_Height;
	mmFloat m_x;
	mmFloat m_y;

	mmInt id;

	clsCharFont m_chars[256];

};
//-----------------------------------------
class clsFont : public clsResource
{
protected:
public:
	clsFont(clsResourceManager* presman, clsString name);
	virtual ~clsFont(void);

	mmRes Create(clsResourceParams* params);

	tmpList<clsItemFont*> items;

	clsItemFont * Find(mmChar ch, clsCharFont ** ch_info);
};
//-----------------------------------------
//-----------------------------------------
class clsParsText
{
protected:
public:

	clsParsText()
	{
		width  = width_space = width_words = count_words = count_space = x = y = 0;
		nChar = 0;
	};

	mmFloat width;
	mmFloat width_words;
	mmFloat count_words;
	mmFloat width_space;
	mmFloat count_space;
	mmFloat width_1space;
	mmFloat x;
	mmFloat y;
	mmUint nChar;

	
	
};
class clsCharInfo
{
public:
	clsItemFont * f;
	clsCharFont * c;
	mmUint color;
	enAlign aligh_x;
	enAlign aligh_y;
};
class clsText : public clsBaseMesh
{
protected:
	clsFont * m_font;

	

	mmFloat m_x;
	mmFloat m_y;
	mmFloat m_z;
	enAlign m_align_x;
	enAlign m_align_y;

	mmFloat m_width;
	mmFloat m_height;

	mmFloat m_size;
	mmDword m_ch_fvf;
	mmUint m_color;

	mmBool m_wrap;

	bool m_colored;
	bool m_aligned;
	mmBool m_rebuild;

	bool m_use_rect;
	clsRect m_rect;

	clsString m_txt;

	clsIntArr m_Count_FC;
	clsIntArr m_Pos_V;
	tmpRefArr<clsParsText> m_tokens;
	tmpPtrArr<clsCharInfo> m_info;

	mmFloat m_min_y;
	mmFloat m_max_y;
	mmFloat m_k_y;

	mmVoid getInfo();
	mmInt BuildChar(mmFloat X, mmFloat Y, mmUint i, mmFloat* vb,mmWord* ib, mmInt nc, mmFloat wSp);
	mmInt getColor(clsString & str,mmInt i, mmUint & co);	
	mmInt getAlign(clsString & str,mmInt i, enAlign & al, enAlign & def);
	mmVoid calculateXY(mmFloat & x, mmFloat & y, enAlign a_x, enAlign a_y);
	mmRes OnCreateMesh(clsResourceParams* params);
	mmVoid OnRender();

public:
	clsText(clsResourceManager* presman, clsString name);
	virtual ~clsText(void);

	mmVoid SetX(mmFloat X);
	mmVoid SetY(mmFloat Y);
	mmVoid SetZ(mmFloat Z);
	mmFloat GetX();
	mmFloat GetY();
	mmFloat GetZ();
	mmVoid SetPos(mmFloat X, mmFloat Y);
	mmVoid SetPos(mmFloat X, mmFloat Y, mmFloat Z);
	mmVoid SetPos(const clsVector3 v);
	mmVoid SetAlignX(enAlign al);
	mmVoid SetAlignY(enAlign al);
	mmVoid SetText(const clsString & str);
	mmVoid SetText(const mmString str);
	mmVoid SetRect(const clsRect & Rect);
	mmVoid SetRect();
	mmVoid SetSize(mmFloat s);
	mmVoid SetColor(mmUint Color);
	mmVoid UseRHW(mmBool rhw);
	mmVoid SetFiltering(mmBool f);
	mmVoid SetWordWrap(mmBool w);

	mmVoid SetYk(mmFloat k);
	mmFloat GetYk();

	mmFloat GetWidth();
	mmFloat GetHeight();

	mmVoid Build();


	clsRect TextRect;

	mmVoid UpdateEffect(mmFloat k, mmBool p_reset = mmfalse);
	clsInterpolation inter;
	mmBool ok;
};
