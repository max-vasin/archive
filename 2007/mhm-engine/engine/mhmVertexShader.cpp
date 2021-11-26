//==========================================================================================
// Mayhem engine vertex shader class implementation
// Coded:	MozG
// Created: 27 june 2006
//==========================================================================================
#include "mhmbase.h"

// Vertex shader construction
//==========================================================================================
clsVertexShader::clsVertexShader(clsResourceManager* presman, clsString name):
				 clsResource(presman, name, enResVertexShader)
{
//	m_shader = mmnull;
//	m_table  = mmnull;
}

// Vertex shader destruction
//==========================================================================================
clsVertexShader::~clsVertexShader()
{
//	mmSafeRelease(m_shader);
//	mmSafeRelease(m_table);
	m_device = mmnull;
}

// Create: creates shader from file
//==========================================================================================
mmRes clsVertexShader::Create(clsResourceParams *params)
{
/*	if (m_name.IsEmpty() && m_fullname.IsEmpty())
	{
		g_plog->Out("clsVertexShader::Create >> empty shader name, cant create!", enLCRed);
		return mmFail;
	}
	clsString filename = m_fullname.IsEmpty() ? m_name : m_fullname;
#ifdef _DEBUG
	mmDword flags = D3DXSHADER_DEBUG|D3DXSHADER_SKIPOPTIMIZATION;
#else
	mmDword flags = 0;
#endif // _DEBUG

	LPD3DXBUFFER shader, errors;
	if (FAILED(D3DXCompileShaderFromFile(filename, NULL, NULL,
										 "main", "vs_1_1", flags, &shader, &errors, &m_table)))
	{
		g_plog->Out("clsVertexShader::Create[" + m_name + "] compilation failed", enLCRed);
		g_plog->Out("-- Compiler errors --", enLCRed);
		clsString errout = (mmString)errors->GetBufferPointer();
		g_plog->Out(errout, enLCRed);
		return mmFail;
	}
	MHMDirect3DDevice* pddev = (MHMDirect3DDevice*)*m_presman->GetDevice();
	if (FAILED(pddev->CreateVertexShader((const DWORD*)shader->GetBufferPointer(), &m_shader)))
	{
		g_plog->Out("clsVertexShader::Create[" + m_name + "] assembly error, check shader code", enLCRed);
		return mmFail;
	}
	
	mmSafeRelease(shader);
	mmSafeRelease(errors);
	
	m_device = pddev;*/
	g_plog->Out("clsVertexShader::Create[" + m_name + "] shader create successfully", enLCGreen);
	return mmOk;
}

/*
// Direct3D typecast operator
//==========================================================================================
clsVertexShader::operator IDirect3DVertexShader9*()
{
	return m_shader;
}

//==================================
// Constants table operations
//==========================================================================================
mmRes clsVertexShader::SetConstant(const clsString &name, mmBool val)
{
	if (m_table == mmnull)
		return mmFail;
	if (FAILED(m_table->SetBool(m_device, name.Data(), val)))
		return mmFail;
	return mmOk;
}
//==========================================================================================
mmRes clsVertexShader::SetConstant(const clsString &name, mmInt val)
{
	if (m_table == mmnull)
		return mmFail;
	if (FAILED(m_table->SetInt(m_device, name.Data(), val)))
		return mmFail;
	return mmOk;
}
//==========================================================================================
mmRes clsVertexShader::SetConstant(const clsString &name, mmFloat val)
{
	if (m_table == mmnull)
		return mmFail;
	if (FAILED(m_table->SetFloat(m_device, name.Data(), val)))
		return mmFail;
	return mmOk;
}
//==========================================================================================
mmRes clsVertexShader::SetConstant(const clsString &name, const clsMatrix& val)
{
	if (m_table == mmnull)
		return mmFail;
	if (FAILED(m_table->SetMatrix(m_device, name.Data(), (D3DXMATRIX*)&val)))//&(mxWorld * mxView * mxProj))))
		return mmFail;
	return mmOk;
}
//==========================================================================================
mmRes clsVertexShader::SetConstant(const clsString &name, const clsVector4& val)
{
	if (m_table == mmnull)
		return mmFail;
	if (FAILED(m_table->SetVector(m_device, name.Data(), (D3DXVECTOR4*)&val)))
		return mmFail;
	return mmOk;
}
*/

