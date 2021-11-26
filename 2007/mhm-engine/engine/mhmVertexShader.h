//==========================================================================================
// Mayhem engine vertex shader resource class
// Coded:	MozG
// Created:	26 june 2006
//==========================================================================================
#ifndef MHMVERTEXSHADER_INCLUDED
#define MHMVERTEXSHADER_INCLUDED

class clsVertexShader: public clsResource
{
protected:
//	IDirect3DVertexShader9*		m_shader;
//	LPD3DXCONSTANTTABLE			m_table;
	MHMDirect3DDevice*			m_device;	// <-- to improve performance
	
	
public:
	clsVertexShader(clsResourceManager* presman, clsString name);
    virtual ~clsVertexShader();
    
    mmRes Create(clsResourceParams* params);
/*
    operator IDirect3DVertexShader9*();

//=================================================================    
// Constant table operations
//=================================================================
    mmRes SetConstant(const clsString& name, mmBool			   val);
    mmRes SetConstant(const clsString& name, mmFloat		   val);
    mmRes SetConstant(const clsString& name, mmInt			   val);
    mmRes SetConstant(const clsString& name, const clsMatrix&  val);
    mmRes SetConstant(const clsString& name, const clsVector4& val);*/
};

#endif //MHMVERTEXSHADER_INCLUDED