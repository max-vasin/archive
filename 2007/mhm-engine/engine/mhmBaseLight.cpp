//==========================================================================================//==========================================================================================
// Mayhem engine light resource class implementation
// Coded:	MozG
// Created:	8 july 2006
//==========================================================================================
#include "mhmbase.h"

// Light constructor
//==========================================================================================
clsBaseLight::clsBaseLight(clsResourceManager* presman, clsString name, enResType type):
			  clsResource(presman, name, type)
{
	this->m_name = clsString(((mmDword)(this)));	
}

// Light destructor
//==========================================================================================
clsBaseLight::~clsBaseLight()
{

}

// Create: creates new light
//==========================================================================================
mmRes clsBaseLight::Create(clsResourceParams* params)
{
	mmZeroVar(m_Light);
	return mmOk;
}

// Activate: activates the light
//==========================================================================================
mmRes clsBaseLight::Activate(clsResource* powner)
{
	return m_presman->GetDevice()->EnableLight(&m_Light, mmtrue, m_Number);
}

// Deactivate: deactivates light
//==========================================================================================
mmRes clsBaseLight::Deactivate(clsResource* powner)
{
	return m_presman->GetDevice()->EnableLight(&m_Light, mmfalse, m_Number);
}

//==========================================================================================
// Light parameters control
//==========================================================================================
mmRes clsBaseLight::SetType(enLightType type)
{
	m_Light.Type = D3DLIGHTTYPE(type);
	return mmOk;
}
mmRes clsBaseLight::SetPosition(clsVector3& val)
{
	m_Light.Position = *((D3DVECTOR*)&val);
	return mmOk;
}
mmRes clsBaseLight::SetDirection(clsVector3& val)
{
	m_Light.Direction = *((D3DVECTOR*)&val);
	return mmOk;
}
mmRes clsBaseLight::SetAttenuation(enAttenuationIndex index, mmFloat val)
{
	switch(index)
	{
	case enAttenuation1:
		m_Light.Attenuation0 = val;
		return mmOk;
	case enAttenuation2:
		m_Light.Attenuation1 = val;
		return mmOk;
	case enAttenuation3:
		m_Light.Attenuation2 = val;
		return mmOk;
	}
	return mmFail;
}
mmRes clsBaseLight::SetRange(mmFloat val)
{
	m_Light.Range = val;
	return mmOk;
}
mmRes clsBaseLight::SetFalloff(mmFloat val)
{
	m_Light.Falloff = val;
	return mmOk;
}
mmRes clsBaseLight::SetTheta(mmFloat val)
{
	m_Light.Theta = val;
	return mmOk;
}
mmRes clsBaseLight::SetPhi(mmFloat val)
{
	m_Light.Phi = val;
	return mmOk;
}
mmRes clsBaseLight::SetDiffuse(const clsColorRGBA& val)
{
	m_Light.Diffuse = val;
	return mmOk;
}
mmRes clsBaseLight::SetSpecular(const clsColorRGBA& val)
{
	m_Light.Specular = val;
	return mmOk;
}
mmRes clsBaseLight::SetAmbient(const clsColorRGBA& val)
{
	m_Light.Ambient = val;
	return mmOk;
}
