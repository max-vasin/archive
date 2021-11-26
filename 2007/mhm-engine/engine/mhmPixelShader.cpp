//==========================================================================================
// Mayhem engine pixel shader class implementation
// Coded:	MozG
// Created:	26 june 2006
//==========================================================================================
#include "mhmbase.h"

// Constructor
//==========================================================================================
clsPixelShader::clsPixelShader(clsResourceManager* presman, clsString name):
				clsResource(presman, name, enResPixelShader)
{
//	m_shader = mmnull;
//	m_table  = mmnull;
}

// Destructor
//==========================================================================================
clsPixelShader::~clsPixelShader()
{
//	mmSafeRelease(m_shader);
//	mmSafeRelease(m_table);
}

// Create: creates pixel shader from file
//==========================================================================================
mmRes clsPixelShader::Create(clsResourceParams* params)
{
/*	if (m_name.IsEmpty() && m_fullname.IsEmpty())
	{
		g_plog->Out("clsPixelShader::Create >> empty shader name, cant create!", enLCRed);
		return mmFail;
	}
	clsString filename = m_fullname.IsEmpty() ? m_name : m_fullname;
	
	m_device = *m_presman->GetDevice();
		
	// trying to get file extension
	clsString ext = filename.CutBy('.', mmfalse);
	ext.UpperCase();
	
	LPD3DXBUFFER pcode = mmnull;
	clsFile file(filename);
	mmUlong* pfcode = mmnull;
	if (ext == "PSH")
	{
		if (MHMFAIL(CompileShader(filename, &pcode)))
			return mmFail;
		if (MHMFAIL(CreateShader((const mmUlong*)pcode->GetBufferPointer())))
			return mmFail;
	}
	else if (ext == "PSO")
	{
		if (MHMFAIL(file.Open()))
		{
			g_plog->Out("clsPixelShader::Create[" + m_name + "] >> cant open shader file", enLCRed);
			return mmFail;
		}
		if (MHMFAIL(file.ReadToMemory((mmVoid**)&pfcode)))
		{
			g_plog->Out("clsPixelShader::Create[" + m_name + "] >> cant load shader file", enLCRed);
			return mmFail;
		}
		if (MHMFAIL(CreateShader((const mmUlong*)pfcode)))
		{
			mmSafeDelete(pfcode);
			return mmFail;
		}
		if (FAILED(D3DXGetShaderConstantTable((const mmUlong*)pfcode, &m_table)))
		{
			g_plog->Out("clsPixelShader::Create[" + m_name + "] >> cant get shader constant table", enLCRed);
			return mmFail;
		}
		mmSafeDelete(pfcode);
	}
	// TODO: add assembly shader*/
	g_plog->Out("clsPixelShader::Create[" + m_name + "] shader create successfully", enLCGreen);
	return mmOk;
}
/*
// Direct3D typecast operator
//==========================================================================================
clsPixelShader::operator IDirect3DPixelShader9*()
{
	return m_shader;
}
//==================================
// Constants table operations
//==========================================================================================
mmRes clsPixelShader::SetConstant(const clsString &name, mmBool val)
{
	if (m_table == mmnull)
		return mmFail;
	if (FAILED(m_table->SetBool(m_device, name.Data(), val)))
		return mmFail;
	return mmOk;
}
//==========================================================================================
mmRes clsPixelShader::SetConstant(const clsString &name, mmInt val)
{
	if (m_table == mmnull)
		return mmFail;
	if (FAILED(m_table->SetInt(m_device, name.Data(), val)))
		return mmFail;
	return mmOk;
}
//==========================================================================================
mmRes clsPixelShader::SetConstant(const clsString &name, mmFloat val)
{
	if (m_table == mmnull)
		return mmFail;
	if (FAILED(m_table->SetFloat(m_device, name.Data(), val)))
		return mmFail;
	return mmOk;
}
//==========================================================================================
mmRes clsPixelShader::SetConstant(const clsString &name, const clsMatrix& val)
{
	if (m_table == mmnull)
		return mmFail;
	if (FAILED(m_table->SetMatrix(m_device, name.Data(), (D3DXMATRIX*)&val)))//&(mxWorld * mxView * mxProj))))
		return mmFail;
	return mmOk;
}
//==========================================================================================
mmRes clsPixelShader::SetConstant(const clsString &name, const clsVector4& val)
{
	if (m_table == mmnull)
		return mmFail;
	if (FAILED(m_table->SetVector(m_device, name.Data(), (D3DXVECTOR4*)&val)))
		return mmFail;
	return mmOk;
}

// CompileShader: compile HLSL shader code
//==========================================================================================
mmRes clsPixelShader::CompileShader(clsString &filename, LPD3DXBUFFER* pcode)
{
#ifdef _DEBUG
	mmDword flags = D3DXSHADER_DEBUG|D3DXSHADER_SKIPOPTIMIZATION;
#else
	mmDword flags = 0;
#endif // _DEBUG

	LPD3DXBUFFER errors;
	if (FAILED(D3DXCompileShaderFromFile(filename, NULL, NULL,
										 "main", LPSTR("ps_1_4"), flags, pcode, &errors, &m_table)))
	{
		g_plog->Out("clsPixelShader::Create[" + m_name + "] compilation error, check shader code", enLCRed);
		g_plog->Out("-- Compiler errors --", enLCRed);
		clsString errout = (mmString)errors->GetBufferPointer();
		g_plog->Out(errout, enLCRed);
		pcode   = mmnull;
		m_table = mmnull;
		return mmFail;
	}
	mmSafeRelease(errors);
	g_plog->Out("clsPixelShader::CompileShader[" + m_name + "] >> shader successfully compiled into [" +
				clsString((*pcode)->GetBufferSize()) + "] bytes of code", enLCGreen);
	
	return mmOk;
}

// AssembleShader: assembles shader file
//==========================================================================================
mmRes clsPixelShader::AssembleShader(clsString &filename, LPD3DXBUFFER* pcode)
{
	return mmOk;
}

// CreateShader: creates shader from existing code
//==========================================================================================
mmRes clsPixelShader::CreateShader(const mmUlong *pcode)
{
	if (FAILED(m_device->CreatePixelShader(pcode, &m_shader)))
	{
		g_plog->Out("clsPixelShader::CreateShader[" + m_name + "] cant create shader with given code", enLCRed);
		m_table = mmnull;
		return mmFail;
	}
	return mmOk;
}
*/