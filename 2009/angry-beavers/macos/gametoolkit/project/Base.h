//===============================================================================================
//  File:		Base.h
//  Project:	Fenix
//	Desc:		base definitons, types and macros
//
//  Created by Max Vasin on 11/10/10.
//  Copyright 2010 Origin. All rights reserved.
//===============================================================================================

typedef	int		result;

#define	RES_OK					0x00000000
#define RES_ERR_GRAPHICS		0x00000001
#define RES_ERR_NO_OBJECT		0x00000002
#define RES_ERR_INVALID_ARGS	0x00000003
#define RES_ERR_FILESYSTEM		0x00000004


#define ISOK(res)	((res) == RES_OK)
#define FAIL(res)	((res) != RES_OK)

//#define MAX(arga, argb)	( ((arga) > (argb)) ? (arga) : (argb) )

#define DBG_BLOCK(code)	(code)

#define DBG_START
#define DBG_END


#define SAFE_RELEASE(object)	if ((object)) { [(object) release];		(object) = nil; }
#define SAFE_INVALIDATE(object)	if ((object)) { [(object) invalidate];	(object) = nil; }
#define SAFE_FREE(ptr)			if ((ptr))    { free((ptr)); (ptr) = NULL; }

#define APP_PREFERENCES_FILE	@"/Contents/Preferences.xml"

#define INITIAL_SCENE_CAPACITY	100

#define GUIDGRID_LINE_COUNT		1000


#define SWAPFLOAT(a, b)		{ GLfloat buf = a; a = b; b = buf; }

#define MB					1048576

#define PI					3.14159265
#define RAD_30				0.52359878


//#define DBG(code)


// **** TYPES AND STUCTS ****

typedef unsigned char	byte;
typedef unsigned short	word;
typedef unsigned int	dword;
typedef unsigned int	uint;

typedef struct
{
	int	width;
	int height;
}IntSize;

typedef struct
{
	int x;
	int y;
}IntPoint;

typedef struct
{
	uint width;
	uint height;
}UintSize;

typedef struct
{
	uint x;
	uint y;
}UintPoint;

// Vec2 definition

typedef struct
{
	GLfloat x, y;
}Vec2;

// Vec3 definition

typedef struct
{
	GLfloat x, y, z;
}Vec3;


// OpenGL color data structure

typedef struct
{
	GLfloat	r, g, b, a;
}OGLColor;

// OpenGL colored vertex data structure

typedef struct
{
	Vec3		pos;
	OGLColor	color;
}OGLColorVertex;

// OpenGL mapped vertex data structure

typedef struct
{
	Vec3		pos;
	Vec2		uv;
}OGLMappedVertex;

// OpenGL commonly used vertex data structure

typedef struct
{
	Vec3		pos;
	OGLColor	color;
	Vec2		uv;
}OGLVertex;


// **** EXTENSIONS ****

#import "NSStringExtension.h"
#import "NSXMLElementExtension.h"
#import "NSBitmapImageRepExtension.h"
#import "NSMutableArrayExtension.h"
#import "NSColorExtension.h"



