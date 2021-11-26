//==========================================================================================
// Mayhem engine sprite resource
// Coded:	MozG
// Created:	29 july 2006
//==========================================================================================
#ifndef MHM_SPRITE_INCLUDED
#define MHM_SPRITE_INCLUDED

struct stSpriteVertex
{
	clsVector4	m_vPosition;
	mmDword		m_nColor;
	clsVector2	m_vTexCoords;
};

class clsSprite: public clsResource
{
protected:
	clsBaseTexture*		m_pTexture;
	clsVertexBuffer*	m_pVBuf;
	mmUint				m_nWidth;
	mmUint				m_nHeight;
	mmUint				m_nCenterX;
	mmUint				m_nCenterY;
	clsVector3			m_vPosition;
	mmDword				m_nColor;
	mmBool				m_bNeedAlphaBlend;
	mmFloat				m_fAngle;
	clsVector3			m_vScale;
	
	
public:
	clsSprite(clsResourceManager* presman, 
			  clsString name,
			  enResType type = enResSprite);
   ~clsSprite();

   mmRes SetTexture(clsString p_nam);
   mmVoid SetWidthHeight(mmInt pWidth, mmInt pHeight);
    
	mmVoid SetPosition(const clsVector3& vPos);
	mmVoid SetRotation(mmFloat fAngle);
	mmVoid SetScale(const clsVector3& vScale);
	mmVoid SetColor(mmDword nColor);
    
    mmRes Create(clsResourceParams* params);
    mmRes Render(mmBool Filtering = mmtrue);
	mmRes ReBuild(mmBool bMakeTexCoords = mmfalse);
	mmRes Activate(clsResource* pOwner);
};

#endif // MHM_SPRITE_INCLUDED