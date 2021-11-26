//==========================================================================================
// Mayhem engine light resource class
// Coded:	MozG
// Created:	8 july 2006
//==========================================================================================
#ifndef MHMLIGHT_INCLUDED
#define MHMLIGHT_INCLUDED

enum enLightType
{
	enLightPoint		= 1,
	enLightSpot			= 2,
	enLightDirectional	= 3,
};

enum enAttenuationIndex
{
	enAttenuation1		= 1,
	enAttenuation2		= 2,
	enAttenuation3		= 3
};

class clsBaseLight: public clsResource
{
protected:
	MHMLight	m_Light;
	
public:
	clsBaseLight(clsResourceManager* presman, clsString name, enResType type = enResBaseLight);
    virtual ~clsBaseLight();
    
    virtual mmRes Create(clsResourceParams* params);
    virtual mmRes Activate(clsResource* powner = mmnull);
    virtual mmRes Deactivate(clsResource* powner = mmnull);
    
    mmRes SetType(enLightType type);
    mmRes SetPosition(clsVector3& val);
    mmRes SetDirection(clsVector3& val);
    mmRes SetAttenuation(enAttenuationIndex index, mmFloat val);
    mmRes SetRange(mmFloat val);
    mmRes SetFalloff(mmFloat val);
    mmRes SetTheta(mmFloat val);
    mmRes SetPhi(mmFloat val);
    mmRes SetDiffuse(const clsColorRGBA& val);
    mmRes SetSpecular(const clsColorRGBA& val);
    mmRes SetAmbient(const clsColorRGBA& val);
	mmInt		m_Number;
};


#endif // MHMLIGHT_INCLUDED