//===============================================================================================
//  File:		GLHexagon.h
//  Project:	OriginGameToolkit
//	Desc:		Hexagon shape OpenGL graphics wrapper
//
//  Created by Max Vasin on 5/18/11.
//  Copyright 2011 Origin Games. All rights reserved.
//===============================================================================================

// ****** IMPORTS ******

#import <Cocoa/Cocoa.h>
#import "Base.h"
#import "BaseObject.h"

// ****** INTERFACE ******

@interface GLHexagon : BaseObject
{
    float		m_s;				// side of hexagon shape
	float		m_a;				// hexagon bounding rectangle width
	float		m_b;				// hexagon bounding rectangke height
	float		m_r;				// hexagon projection on X (half of a)
	float		m_h;				// hexagon projection on Y (two veritces on b)
	NSColor*	m_colorFill;		// hexagon fill color (only for graphic mode)
	NSColor*	m_colorEdge;		// hexagon edge color (only for graphic mode)
	NSColor*	m_colorVertex;		// hexagon vertex color (only for graphic mode)
	BOOL		m_renderSolid;		// solid hexagon rendering enabled
	BOOL		m_renderEdges;		// hexagon edges rendeing enabled
	BOOL		m_renderVertices;	// hexagon vertices rendering enabled
	OGLVertex	m_vertices[6];		// shape vertices
	float		m_vertexSize;		// vertex rendering size
	float		m_aspect;			// hexagon scale distortion
}

// ****** PROPERTIES ******

@property (nonatomic, assign)		float		propSize;
@property (nonatomic, readonly)		float		propBoundWidth;
@property (nonatomic, readonly)		float		propBoundHeight;
@property (nonatomic, readonly)		float		propProjectionX;
@property (nonatomic, readonly)		float		propProjectionY;
@property (nonatomic, copy)			NSColor*	propColorFill;
@property (nonatomic, copy)			NSColor*	propColorEdge;
@property (nonatomic, copy)			NSColor*	propColorVertex;
@property (nonatomic, assign)		BOOL		propRenderSolid;
@property (nonatomic, assign)		BOOL		propRenderEdges;
@property (nonatomic, assign)		BOOL		propRenderVertices;
@property (nonatomic, assign)		float		propVertexSize;
@property (nonatomic, assign)		float		propAspect;

// ****** INSTANCE METHODS ******

- (id)		initWithSize:(float)size;
- (void)	render;

@end
