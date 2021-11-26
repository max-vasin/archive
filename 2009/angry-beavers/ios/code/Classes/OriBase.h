//=======================================================================================================================
//  File:       OriBase.h
//  Project:    Heli
//  Desc:       Program base headers and configurations constatns
//
//  Created by Max Vasin on 3/22/11.
//  Copyright 2011 Origin. All rights reserved.
//=======================================================================================================================

#import <malloc/malloc.h>
#import <time.h>
#import <signal.h>
#import <UIKit/UIKit.h>
#import <OpenGLES/EAGL.h>
#import <OpenGLES/ES1/gl.h>
#import <OpenGLES/ES1/glext.h>
#import <OpenGLES/ES2/gl.h>
#import <OpenGLES/ES2/glext.h>

#import "NSMutableArrayExtension.h"

//#######################################################################################################################
// ENGINE CONSTANTS
//#######################################################################################################################

#define		ORIENGINE_RESOURCE_TEXTURES_PATH		@"textures"
#define		ORIENGINE_RESOURCE_SPRITES_PATH			@"sprites"
#define		ORIENGINE_RESOURCE_MULTISPRITES_PATH	@"multisprites"
#define		ORIENGINE_RESOURCE_TILESETS_PATH		@"tilesets"
#define		ORIENGINE_RESOURCE_TILEMAPS_PATH		@"tilemaps"

#define		ORIENGINE_RENDERER_BACKBUFFER_FORMAT	kEAGLColorFormatRGBA8

//#define		ORIENGINE_STRING_DRAWER_USES_VBO		YES

//#######################################################################################################################
// MACROS
//#######################################################################################################################

#define		ORIENGINE_ALLOC_LOG_LEVEL_0

#if defined(DEBUG)

#define		DBGLOG0A(str)							NSLog(str)
#define		DBGLOG1A(str, a)						NSLog(str, a)
#define		DBGLOG2A(str, a, b)						NSLog(str, a, b)
#define		DBGLOG3A(str, a, b, c)					NSLog(str, a, b, c)
#define		DBG_ASSERT_LOG0A(exp, str)				if (exp)	{ NSLog(@"<ASSERT FAIL>%@", str); }
#define		DBG_ASSERT_LOG1A(exp, str, a)			if (exp)	{ NSLog(@"<ASSERT FAIL>%@", [NSString stringWithFormat:str, a]);		}
#define		DBG_ASSERT_LOG2A(exp, str, a, b)		if (exp)	{ NSLog(@"<ASSERT FAIL>%@", [NSString stringWithFormat:str, a, b]);		}
#define		DBG_ASSERT_LOG3A(exp, str, a, b, c)		if (exp)	{ NSLog(@"<ASSERT FAIL>%@", [NSString stringWithFormat:str, a, b, c]);	}

#if defined (ORIENGINE_ALLOC_LOG_LEVEL_0)

#define		DBG_INIT_LOG_0(self)					NSLog(@"<INIT>%@",			NSStringFromClass([self class]))
#define		DBG_INIT_LOG_1(self)					{}
#define		DBG_INIT_LOG_2(self)					{}
#define		DBG_INIT_LOG_NAMED_0(self, name)		NSLog(@"<INIT>%@ (%@)",		NSStringFromClass([self class]), name)
#define		DBG_INIT_LOG_NAMED_1(self, name)		{}
#define		DBG_INIT_LOG_NAMED_2(self, name)		{}
#define		DBG_DEALLOC_LOG_0(self)					NSLog(@"<DEALLOC>%@",		NSStringFromClass([self class]))
#define		DBG_DEALLOC_LOG_1(self)					{}
#define		DBG_DEALLOC_LOG_2(self)					{}
#define		DBG_DEALLOC_LOG_NAMED_0(self, name)		NSLog(@"<DEALLOC>%@ (%@)",	NSStringFromClass([self class]), name)
#define		DBG_DEALLOC_LOG_NAMED_1(self, name)		{}
#define		DBG_DEALLOC_LOG_NAMED_2(self, name)		{}


#elif defined (ORIENGINE_ALLOC_LOG_LEVEL_1)

#define		DBG_INIT_LOG_0(self)					NSLog(@"<INIT>%@",		NSStringFromClass([self class]))
#define		DBG_INIT_LOG_1(self)					NSLog(@"<INIT>%@",		NSStringFromClass([self class]))
#define		DBG_INIT_LOG_2(self)					{}
#define		DBG_INIT_LOG_NAMED_0(self, name)		NSLog(@"<INIT>%@ (%@)",		NSStringFromClass([self class]), name)
#define		DBG_INIT_LOG_NAMED_1(self, name)		NSLog(@"<INIT>%@ (%@)",		NSStringFromClass([self class]), name)
#define		DBG_INIT_LOG_NAMED_2(self, name)		{}
#define		DBG_DEALLOC_LOG_0(self)					NSLog(@"<DEALLOC>%@",	NSStringFromClass([self class]))
#define		DBG_DEALLOC_LOG_1(self)					NSLog(@"<DEALLOC>%@",	NSStringFromClass([self class]))
#define		DBG_DEALLOC_LOG_2(self)					{}
#define		DBG_DEALLOC_LOG_NAMED_0(self, name)		NSLog(@"<DEALLOC>%@ (%@)",	NSStringFromClass([self class]), name)
#define		DBG_DEALLOC_LOG_NAMED_1(self, name)		NSLog(@"<DEALLOC>%@ (%@)",	NSStringFromClass([self class]), name)
#define		DBG_DEALLOC_LOG_NAMED_2(self, name)		{}

#elseif defined (ORIENGINE_ALLOC_LOG_LEVEL_2)

#define		DBG_INIT_LOG_0(self)					NSLog(@"<INIT>%@",		NSStringFromClass([self class]))
#define		DBG_INIT_LOG_1(self)					NSLog(@"<INIT>%@",		NSStringFromClass([self class]))
#define		DBG_INIT_LOG_2(self)					NSLog(@"<INIT>%@",		NSStringFromClass([self class]))
#define		DBG_INIT_LOG_NAMED_0(self, name)		NSLog(@"<INIT>%@ (%@)",		NSStringFromClass([self class]), name)
#define		DBG_INIT_LOG_NAMED_1(self, name)		NSLog(@"<INIT>%@ (%@)",		NSStringFromClass([self class]), name)
#define		DBG_INIT_LOG_NAMED_2(self, name)		NSLog(@"<INIT>%@ (%@)",		NSStringFromClass([self class]), name)
#define		DBG_DEALLOC_LOG_0(self)					NSLog(@"<DEALLOC>%@",	NSStringFromClass([self class]))
#define		DBG_DEALLOC_LOG_1(self)					NSLog(@"<DEALLOC>%@",	NSStringFromClass([self class]))
#define		DBG_DEALLOC_LOG_2(self)					NSLog(@"<DEALLOC>%@",	NSStringFromClass([self class]))
#define		DBG_DEALLOC_LOG_NAMED_0(self, name)		NSLog(@"<DEALLOC>%@ (%@)",	NSStringFromClass([self class]), name)
#define		DBG_DEALLOC_LOG_NAMED_1(self, name)		NSLog(@"<DEALLOC>%@ (%@)",	NSStringFromClass([self class]), name)
#define		DBG_DEALLOC_LOG_NAMED_2(self, name)		NSLog(@"<DEALLOC>%@ (%@)",	NSStringFromClass([self class]), name)


#else

#define		DBG_INIT_LOG_0(self)					{}
#define		DBG_DEALLOC_LOG_0(self)					{}
#define		DBG_INIT_LOG_1(self)					{}
#define		DBG_DEALLOC_LOG_1(self)					{}
#define		DBG_INIT_LOG_2(self)					{}
#define		DBG_DEALLOC_LOG_2(self)					{}
#define		DBG_INIT_LOG_NAMED_0(self, name)		{}
#define		DBG_INIT_LOG_NAMED_1(self, name)		{}
#define		DBG_INIT_LOG_NAMED_2(self, name)		{}
#define		DBG_DEALLOC_LOG_NAMED_0(self, name)		{}
#define		DBG_DEALLOC_LOG_NAMED_1(self, name)		{}
#define		DBG_DEALLOC_LOG_NAMED_2(self, name)		{}

#endif // #if defined (ORIENGINE_ALLOC_LOG_LEVEL_0)

#else

#define		DBGLOG0A(str)							{}
#define		DBGLOG1A(str, a)						{}
#define		DBGLOG2A(str, a, b)						{}
#define		DBGLOG3A(str, a, b, c)					{}
#define		DBG_ASSERT_LOG0A(exp, str)				{}
#define		DBG_ASSERT_LOG1A(exp, str, a)			{}
#define		DBG_ASSERT_LOG2A(exp, str, a, b)		{}
#define		DBG_ASSERT_LOG3A(exp, str, a, b, c)		{}
#define		DBG_INIT_LOG_0(self)					{}
#define		DBG_DEALLOC_LOG_0(self)					{}
#define		DBG_INIT_LOG_1(self)					{}
#define		DBG_DEALLOC_LOG_1(self)					{}
#define		DBG_INIT_LOG_2(self)					{}
#define		DBG_DEALLOC_LOG_2(self)					{}
#define		DBG_INIT_LOG_NAMED_0(self, name)		{}
#define		DBG_INIT_LOG_NAMED_1(self, name)		{}
#define		DBG_INIT_LOG_NAMED_2(self, name)		{}
#define		DBG_DEALLOC_LOG_NAMED_0(self, name)		{}
#define		DBG_DEALLOC_LOG_NAMED_1(self, name)		{}
#define		DBG_DEALLOC_LOG_NAMED_2(self, name)		{}

#endif // #if defined(DEBUG)



#define		SAFE_RELEASE(obj)		if (obj != nil)	{ [obj release];	obj = nil; }
#define		SAFE_FREE(ptr)			if (ptr) { free(ptr); ptr = 0; }

#define		PI					3.14159265
#define		RAD_30				0.52359878

//#######################################################################################################################
// TYPES
//#######################################################################################################################

typedef int					oriInt;
typedef unsigned int		oriUint;
typedef unsigned char		oriByte;
typedef unichar				oriUniChar;
typedef float				oriFloat;
typedef char				oriChar;

//#######################################################################################################################
// STRUCTURES
//#######################################################################################################################

typedef struct
{
	oriInt	x, y;
}oriIntPoint;

typedef struct
{
	oriInt	width, height;
}oriIntSize;

typedef struct
{
	oriFloat	x, y;
}oriVector2;

typedef struct
{
	oriVector2	min;
	oriVector2	max;
}oriRect;

//#######################################################################################################################
// OpenGL ES data structures
//#######################################################################################################################

typedef struct
{
	GLfloat	x, y, z;
}oglVector3;

typedef struct
{
	GLfloat u, v;
}oglMapVector2;

typedef struct
{
	GLubyte r, g, b, a;
}oglColor;

#define  EQUAL_COLORS(cla, clb)		( (cla.r == clb.r) && (cla.g = clb.g) && (cla.b == clb.b) && (cla.a == clb.a) )

typedef struct
{
	oglVector3		pos;
	oglColor		color;
	oglMapVector2	map;
}oglVertex;

//#######################################################################################################################
// Math utilities
//#######################################################################################################################

#import "Vector2.h"
#import "Matrix.h"


