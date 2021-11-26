// header for track-if work
//
//   v0.3

// - fyrex 24 jun 2k3
//   initial
//

#if !defined(AFX_FILEMESH_H__92519C4F_04FC_4297_B96D_400b15679C1D__INCLUDED_)

#include "Ifparser.h" // generic stuff

typedef struct
IFileTrack
{
	UBYTE	type; // file type
//  1  - plain mash (furniture etc, no animations)
//  2  - bones mesh (for subjects, vertexes with wights)
//  3  - morph whole mesh (for subjects, anachronox/anomaly)
//  4  - track file
 
	UBYTE   res1; // type for vertex coordinates

	UBYTE   res2; // type for normals

	UBYTE	fps; // various flags


	ULONG	keys; // fps

	ULONG	frames; // actual frames in this file

} IFileTrack;

typedef struct
IFileTrackMov
{
	ULONG type;
	float x;
	float y;
	float z;
	float rx,ry,rz;
	float ang;
} IFileTrackMov;

typedef struct
IFileTrackRot
{
	ULONG type;
	float rx,ry,rz;
	float ang;
} IFileTrackRot;

// track's ids

#define ID_TRAC		MAKE_ID('T','R','A','C')

#endif // !defined(AFX_FILEMESH_H__92519C4F_04FC_4297_B96D_400A15679C1D__INCLUDED_)
