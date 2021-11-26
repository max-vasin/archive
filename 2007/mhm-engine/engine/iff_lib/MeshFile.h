// header for mesh-if work
//
//   v0.4
//
// - fyrex 4 nov 2003
//   change TAGS to PHYS (tags are obsolete)

//   v0.3
//
// - fyrex 25 apr 2k2
//   change latlong normal to tabl norm

// - fyrex 6 apr 2k2
//   initial
//
// - fyrex 9 apr 2k2
//   add bone's vertex types
//   add flags
//   add tags' id
//   add material and tags struct type

#if !defined(AFX_FILEMESH_H__92519C4F_04FC_4297_B96D_400A15679C1D__INCLUDED_)

#include "Ifparser.h" // generic stuff

#define	MESH_TYPE_PLAIN		1
#define	MESH_TYPE_BONES		2
#define	MESH_TYPE_MORPH		3

#define	MESH_TXYZ_FLOAT		1
#define	MESH_TXYZ_SHORT		2
#define	MESH_TXYZ_1BONE		16
#define	MESH_TXYZ_2BONE		17
#define	MESH_TXYZ_3BONE		18
#define	MESH_TXYZ_4BONE		19

#define	MESH_TNOR_NO		0
#define	MESH_TNOR_FLOAT		1
#define	MESH_TNOR_SHORT		2
#define	MESH_TNOR_TABL		3

#define	MESH_FLAG_BBOX		1
#define	MESH_FLAG_DUVM		2
#define	MESH_FLAG_BUMP		128

#define	MMAT_FLAG_ALPHA		1
#define	MMAT_FLAG_MASK		2
#define	MMAT_FLAG_TWOS		4
#define	MMAT_FLAG_NMIP		8

#define	MMAT_FLAG_EXTN		0x80 // extended flag for mat2

typedef struct
IFileMesh
{
	UBYTE	type; // file type
//  1  - plain mash (furniture etc, no animations)
//  2  - bones mesh (for subjects, vertexes with wights)
//  3  - morph whole mesh (for subjects, anachronox/anomaly)
 
	UBYTE   type_xyz; // type for vertex coordinates
//  1  - three floats
//  2  - three short words (bboxes needed)
//  16 - three floats with 1 bone's index
//  17 - three floats with 2 bone's indexes and 1 weight
//  18 - three floats with 3 bone's indexes and 2 weight
//  19 - three floats with 4 bone's indexes and 3 weight

	UBYTE   type_n; // type for normals
//  0 - no normals
//  1 - three floats
//  2 - three short words
//  3 - tabl

	UBYTE	flags; // various flags
// 0x01 - bboxes present
// 0x02 - diffuse uv map coords present
//...
// 0x80 - bump map coords present


	ULONG	frames; // number of integrated anim. frames

	ULONG	vertexes; // number of vertexes with duplicates

	ULONG	duplicates; // number of duplicates

	ULONG	tris; // number of triangles (by three indexes)

	ULONG	materials; // number of materials

	ULONG	tags; // number of tags

	ULONG	bones; // number of bones

} IFileMesh;

typedef struct
IFileMeshMaterial
{
	ULONG type;
	ULONG i_vtx;
	ULONG n_vtx;
	ULONG i_idx;
	ULONG n_idx;
	ULONG amb;
	ULONG dif;
	ULONG spe;
	ULONG pow;
	ULONG ref;
	ULONG emi;
	char name[64];

} IFileMeshMaterial;

typedef struct
IFileMeshMaterial2
{
	ULONG type;
	ULONG i_vtx;
	ULONG n_vtx;
	ULONG i_idx;
	ULONG n_idx;
	ULONG amb;
	ULONG dif;
	ULONG spe;
	ULONG pow;
	ULONG ref;
	ULONG emi;
	char name[64];

	char detail_name[64];
	char dot3_name[64];
	char shader_name[64];

} IFileMeshMaterial2;

typedef struct
IFileMeshTag
{
	ULONG type;
	
} IFileMeshTag;

// mesh ids
#define ID_DUPL		MAKE_ID('D','U','P','L')
#define ID_XYZN		MAKE_ID('X','Y','Z','N')
#define ID_XYZW		MAKE_ID('X','Y','Z','W')
#define ID_TRIS		MAKE_ID('T','R','I','S')
#define ID_DUVM		MAKE_ID('D','U','V','M')
#define ID_BBOX		MAKE_ID('B','B','O','X')
#define ID_MTLS		MAKE_ID('M','T','L','S')
#define ID_PHYS		MAKE_ID('P','H','Y','S')

#define ID_DTUV		MAKE_ID('D','T','U','V')
#define ID_D3UV		MAKE_ID('D','3','U','V')
#define ID_SHAD		MAKE_ID('S','H','A','D')

#endif // !defined(AFX_FILEMESH_H__92519C4F_04FC_4297_B96D_400A15679C1D__INCLUDED_)
