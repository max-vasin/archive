//==========================================================================================
// Mayhem engine MESH resource class
// Coded:	fyrex
// Created:	18 may 2006
//------
// Edited	20 may 2006

//==========================================================================================

#ifndef _MHMBASEMESH_INCLUDED
#define _MHMBASEMESH_INCLUDED

typedef struct {
	mmFloat x,y,z,n;
} mmVector;

typedef struct {
	mmFloat _11,_21,_31,_41;
	mmFloat _12,_22,_32,_42;
	mmFloat _13,_23,_33,_43;
	mmFloat _14,_24,_34,_44;
} mmMatrix;

__inline mmVoid mmMatrixMultiply(mmMatrix *,mmMatrix *){};

//================================================
// Base mesh subset structured class
//================================================
class clsBaseMeshSubset
{
public:
	mmUint	m_start;	// start index or vertex (if mesh non indexed)
	mmUint	m_count;	// primitives count
	mmWord  m_mtlindex;	// material index

	//
	//mmUint   a,b,c,d;	// ????
	// 1 july 06 fyrex
	mmUint  m_vstart;
	mmUint  m_vcount;
};

//================================================
// Base mesh class
//================================================
class clsBaseMesh:public clsResource
{
public:
	clsBaseMeshSubset*	m_psubsets;	  // mesh subsets list
	clsVertexBuffer*	m_basevbuf;	  // base vertex buffer, can be unused
	clsIndexBuffer*		m_baseibuf;   // base index buffer, can be usused
	clsResource**		m_materials;  // materials list
	mmUint				m_nummtls;	  // number of materials
	mmUint				m_numsubs;	  // number of subsets
	D3DPRIMITIVETYPE	m_pttype;	  // mesh primitives type
	mmBool				m_indexed;	  // indexed mesh flag
	mmBool				m_hassubsets; // subsets mesh
	

	// test
	clsBaseMesh*		m_bs;

public:
	clsBaseMesh(clsResourceManager* presman, clsString name, enResType type = enResBaseMesh);
    virtual ~clsBaseMesh();

	virtual mmRes Create(clsResourceParams* params);
	virtual mmRes Render();
	virtual mmRes Clone(clsResource* pmesh);
	
	
	virtual mmRes OnCreateMesh(clsResourceParams* params);
	clsResource*    GetMaterial(mmUint index);
	mmBool RayIntersect(const clsRay& ray,
						clsMatrix* tform = mmnull,
						clsVector3* pvint = mmnull,
						mmFloat* pdist = mmnull,
						clsPlane* pplane = mmnull);
	mmRes ComputeTangents();
	
	mmRes ComputeBounds(clsBoundBox* bbox, clsBoundSphere* bsphere, clsMatrix* tform);

	clsVertexDecl*  GetVertexDeclaration();


protected:
	mmRes  DestroyBaseMesh();
	mmVoid CalcTangent(clsVector3& v1,
					   clsVector3& v2,
					   clsVector3& v3,
					   clsVector2& t1,
					   clsVector2& t2,
					   clsVector2& t3,
					   mmFloat bumpfactor,
					   clsVector3& tangent);
	
	virtual mmVoid OnRender();
};


#endif