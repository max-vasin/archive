//==========================================================================================
// Mayhem engine vertex declaration class
// Coded:	MozG
// Created:	29 june 2006
//==========================================================================================

#ifndef MHMVERTEXDECLARATION_INCLUDED
#define MHMVERTEXDECLARATION_INCLUDED

class clsVertexDecl
{
friend class clsResourceManager;
public:
	enum enElementUsage
	{
		enUsagePosition = 0,
		enUsageNormal   = 1,
		enUsageTexcoord	= 2,
		enUsageTangent  = 3,
		enUsageNone     = 255
	};
	
protected:
	clsDevice*						m_pdev;
public:
	mmUint							m_numitems;
#ifndef MHM_USE_DX8
	D3DVERTEXELEMENT9*				m_elements;
	IDirect3DVertexDeclaration9*	m_decl;
#else
	struct stVertexElement
	{
		enElementUsage	Usage;
		mmDword			Offset;
	};
	stVertexElement*				m_elements;
#endif // MHM_USE_DX8
	mmUint							m_stride;
	
protected:
	clsVertexDecl(clsDevice* pdev, mmUint numitems);

public:    
   ~clsVertexDecl();
    mmRes SetElement(mmUint index, enElementUsage usage,
					 mmUint stream = 0, mmUint offset = 0,
					 mmUint usageindex = 0);
	enElementUsage GetElementUsage(mmUint index);
	mmUint  GetElementOffset(mmUint index);
	mmDword GetStride();
	mmVoid  CalcStride();
	mmRes   Create();
	mmRes   Activate();
};

#endif // MHMVERTEXDECLARATION_INCLUDED
