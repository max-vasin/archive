#pragma once
#include "mhmbasemesh.h"

//enum enAlign {enLeft = 0, enRight, enTop, enBottom, enCenter, enFillRect, enLeftRect, enRightRect, enTopRect, enBottomRect, enCenterRect}; 

enum enOutline {en_ol_None = 0, en_ol_Center , en_ol_Out,en_ol_Innert};

struct mmVertexRHW	//D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1
{
	mmFloat x;
	mmFloat y;
	mmFloat z;
	mmFloat h;

	mmDword c;

	mmFloat u;
	mmFloat v;
};
struct mmVertex	//D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1
{
	mmFloat x;
	mmFloat y;
	mmFloat z;

	mmFloat nx;
	mmFloat ny;
	mmFloat nz;

	mmDword c;

	mmFloat u;
	mmFloat v;
};
class clsRectangleEntity;
class clsRectangle : public clsBaseMesh
{
		friend class clsRectangleEntity;
protected:

	mmBool m_rhw;

	mmBool m_centr;

	mmDword m_fvf;

	mmFloat m_x;
	mmFloat m_y;
	mmFloat m_z;
	mmFloat m_h;
	mmFloat m_w;
	mmDword m_color;

	mmRes OnCreateMesh(clsResourceParams* params);

	mmVoid Create();
	
	//mmRes Render();
	mmVoid OnRender();

	mmVoid BuildRHW();

public:

	clsRectangle(clsResourceManager* presman, clsString name);
	virtual ~clsRectangle(void);
	mmVoid Build();

	mmVoid UseRHW(mmBool p_rhw);

	mmVoid SetCentrCentr(mmBool p_center);

};

//*********************************************************
class clsRectangleEntity: public clsEntity
{
protected:

	clsRectangle * m_rect;

public:
	clsRectangleEntity(clsEntityFactory* pFactory,
			  const clsString& type,
			  const clsString& name);
   ~clsRectangleEntity();

   mmVoid SetTexture(const mmString name);
   clsBaseTexture * GetTexture();
   clsBaseMap * GetMap();

   mmVoid SetColor(mmDword p_color);
   mmVoid SetBlendType(enBlendType p_blend);

   mmVoid SetX(mmFloat x);
   mmVoid SetY(mmFloat y);
   mmVoid SetZ(mmFloat z);
   mmVoid SetWidth(mmFloat w);
   mmVoid SetHeight(mmFloat h);
   mmVoid SetPos(clsVector3 & v);
   mmVoid SetPosWH(clsVector3 & v, mmFloat w, mmFloat h);

   mmFloat GetX();
   mmFloat GetY();
   mmFloat GetZ();
   mmFloat GetWidth();
   mmFloat GetHeight();
   clsVector3 GetPos();


   mmVoid SetCentrCentr(mmBool p_center);

   mmVoid SetTransformation(mmFloat x, mmFloat y, mmFloat w, mmFloat h, mmFloat z = 0.0f);

   mmVoid SetFiltering(mmBool f);
};