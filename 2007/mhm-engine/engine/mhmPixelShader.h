//==========================================================================================
// Mayhem engine pixel shader class
// Coded:	MozG
// Created:	26 june 2006
//==========================================================================================
#ifndef MHMPIXELSHADER_INCLUDED
#define MHMPIXELSHADER_INCLUDED

class clsPixelShader: public clsResource
{
protected:
//	IDirect3DPixelShader9* m_shader;
//	LPD3DXCONSTANTTABLE	   m_table;
	MHMDirect3DDevice*			m_device;	// <-- to improve performance
	
public:
	clsPixelShader(clsResourceManager* presman, clsString name);
    virtual ~clsPixelShader();
    
    mmRes Create(clsResourceParams* params);
	
/*	operator IDirect3DPixelShader9*();
//=================================================================    
// Constant table operations
//=================================================================
    mmRes SetConstant(const clsString& name, mmBool			   val);
    mmRes SetConstant(const clsString& name, mmFloat		   val);
    mmRes SetConstant(const clsString& name, mmInt			   val);
    mmRes SetConstant(const clsString& name, const clsMatrix&  val);
    mmRes SetConstant(const clsString& name, const clsVector4& val);

private:
	mmRes CompileShader(clsString& filename, LPD3DXBUFFER* pcode);
	mmRes AssembleShader(clsString& filename, LPD3DXBUFFER* pcode);
	mmRes CreateShader(const mmUlong* pcode);*/
};

#endif // MHMPIXELSHADER_INCLUDED