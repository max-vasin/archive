//==========================================================================================
// Mayhem engine 3DS Max tools - mesh exportert class interface
// Coded:	MozG
// Created:	7 june 2006
//==========================================================================================

#ifndef _MESHEXPORT_INCLUDED
#define _MESHEXPORT_INCLUDED

//==============================================================
// Safe array pointer template
//==============================================================
template <class type>
class expSafeArPtr
{
protected:
	type*	m_ptr;

public:
	expSafeArPtr()
	{
		m_ptr = mmnull;
	}
	
	~expSafeArPtr()
	{
		mmSafeDeleteAr(m_ptr);
	}
	
	type* operator = (type* ptr)
	{
		return m_ptr = ptr;
	}
	operator type*()
	{
		return m_ptr;
	}
};

//==============================================================
// Export vertex struct
//==============================================================
struct stMeshExpVertex
{
	mmFloat x, y, z;
	stMeshExpVertex()
	{
		x = y = z= 0.0f;
	}
	stMeshExpVertex(mmFloat _x, mmFloat _y, mmFloat _z)
	{
		x = _x;
		y = _y;
		z = _z;
	}
	mmBool operator == (stMeshExpVertex& vtx)
	{
		return ((x == vtx.x) &&
				(y == vtx.y) &&
				(z == vtx.z));
	}
};

//==============================================================
// Export mapping struct
//==============================================================
struct stMeshExpUV
{
	mmFloat u, v;
	stMeshExpUV()
	{
		u = v = 0.0f;
	}
	stMeshExpUV(mmFloat _u, mmFloat _v)
	{
		u = _u;
		v = _v;
	}
	mmBool operator == (stMeshExpUV& vtx)
	{
		return ((u == vtx.u) &&
				(v == vtx.v));
	}
};

//==============================================================
// Export face struct
//==============================================================
struct stMeshExpFace16
{
	union
	{
		mmWord  a, b, c;
		mmWord  verts[3];
	}f;
	
	stMeshExpFace16()
	{
		f.a = f.b = f.c = 0;
	}
	stMeshExpFace16(mmWord _a, mmWord _b, mmWord _c)
	{
		f.a = _a;
		f.b = _b;
		f.c = _c;
	}
	mmBool operator == (stMeshExpFace16& vtx)
	{
		return ((f.a == vtx.f.a) &&
				(f.b == vtx.f.b) &&
				(f.c == vtx.f.c));
	}
};

//==============================================================
// Mesh subset
//==============================================================
struct stMeshSubset
{
	mmDword	m_numfaces;
	mmDword m_mtlindex;
};

//==============================================================
// Mesh file header
//==============================================================
struct stMeshFileHeader
{
	mmDword		m_numsubs;
	mmDword		m_numverts;
	mmDword		m_numfaces;
	mmBool		m_chnl_normals;
	mmBool		m_chnl_mapping;
};

//==============================================================
// Packed mesh file header
//==============================================================
struct stPackedMeshFileHeader
{
	mmDword		m_numverts;
	mmDword		m_numfaces;
	mmBool		m_normals;
	mmBool		m_mapping;
};


typedef tmpPtrList<tmpList<mmDword>*> expSubsetList;

//==============================================================
// Exporter class
//==============================================================
class clsMeshExporter
{
protected:
	clsString	m_root;
	mmBool		m_channel_normals;
	mmBool		m_channel_mapping;
	mmBool		m_channel_vcolors;
	mmBool		m_bpacked;

public:
	clsMeshExporter();
   ~clsMeshExporter();
    
    mmVoid EnableChannels(mmBool normals, mmBool mapping, mmBool vcolors);
    mmVoid EnablePacked(mmBool packed);
    mmVoid SetRoot(clsString root);
    mmRes  ExportMesh(INode* pnode);

protected:
	mmRes CollapseMesh(Mesh *pmesh, stMeshExpVertex *ppos,
					   stMeshExpVertex *pnorm,  stMeshExpUV *ptc,
					   stMeshExpFace16* pfaces, Matrix3* ptform,
					   mmDword* collapsed);
	Point3 GetVertexNormal(Mesh *pmesh, mmUint face, RVertex *prv);
	mmRes  BuildSubsets(Mesh *pmesh, 
						stMeshExpFace16 *pfaces, 
						mmDword nummtls,
						expSubsetList* psublist);
	mmRes  ExportPackedMesh(INode* pnode);
};


#endif //_MESHEXPORT_INCLUDED