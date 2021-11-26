//==========================================================================================
// Mayhem engine MESH resource class
// Coded:	fyrex
// Created:	18 may 2006
//------
// Edited	20 may 2006

//==========================================================================================

#ifndef _MHMSTATICMESH_INCLUDED
#define _MHMSTATICMESH_INCLUDED

class clsEntity;
class clsStaticMesh :
	public clsBaseMesh
{
	class CFrame
	{
	public:
		clsVertexBuffer * vb;
		mmFloat xmin,ymin,zmin;
		mmFloat xmax,ymax,zmax;
	};


private:
//	clsVertexBuffer *vb;
//	clsIndexBuffer *ib;
	mmUint n_indexes;
	mmUint n_verteces;
	mmUint n_bones;
	mmUint n_duples;
	mmUint n_frames;

	mmUint morphid;
	mmFloat morph_f;
	mmUint morph_a,morph_b;
	mmUint mode;
	mmDword fvf;
	mmUint size;

	CFrame * frames;
	mmWord * pack;
	mmWord * dupl;

	mmFloat xmin,ymin,zmin;
	mmFloat xmax,ymax,zmax;

	mmChar *BoneNames;
	mmFloat *bones;
	mmFloat *phys;
/*
	mmUint n_opacity;
	mmDword ** opacity;
	mmUint	n_transparent;
	mmDword ** transparent;
*/
	mmUint skinning(mmFloat *vbuf, mmUint count, mmFloat *pout, mmUint stride, mmUint16 * dup);
	mmRes getIndeces(mmUint n, mmInt *i1, mmInt *i2, mmInt *i3);
	mmRes getCoord(mmUint i, mmVector *v);
	mmRes Morph(mmUint id, mmFloat f, mmInt a, mmInt b);
	mmRes LoadIF();

public:
	clsStaticMesh(clsResourceManager* presman, clsString name);
	virtual ~clsStaticMesh(void);

	virtual mmRes OnCreateMesh(clsResourceParams* params);
	mmVoid Destroy();

	mmUint getBones();
	mmChar * getBoneName(mmUint i);
	mmDword  getBoneHandle(mmUint i);
	mmVoid   setBoneHandle(mmUint i, mmUint h);
	mmVoid   setBoneInit(mmUint i, clsMatrix *m);
	mmVoid   setBoneMatrix(mmUint i, clsMatrix *m);

	mmVoid	Animate(clsEntity* pBonesOwner);


};

#endif