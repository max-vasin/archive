//===============================================================================================
//  File:		GLHexagon.m
//  Project:	OriginGameToolkit
//	Desc:		Hexagon utility and graphics object class implementation
//
//  Created by Max Vasin on 5/18/11.
//  Copyright 2011 Origin Games. All rights reserved.
//===============================================================================================

// ****** IMPORTS ******

#import "GLHexagon.h"

// ****** PRIVATE METHODS ******

@interface GLHexagon (Private)

- (void)	updateVertices;

@end


// ****** IMPLEMENTATION ******

@implementation GLHexagon

// ****** PROPERTIES ******

// Synthesized
//-----------------------------------------------------------------------------------------------
@synthesize propColorFill		= m_colorFill;
@synthesize propColorEdge		= m_colorEdge;
@synthesize propColorVertex		= m_colorVertex;
@synthesize propRenderEdges		= m_renderEdges;
@synthesize propRenderSolid		= m_renderSolid;
@synthesize propRenderVertices	= m_renderVertices;
@synthesize propBoundWidth		= m_a;
@synthesize propBoundHeight		= m_b;
@synthesize propProjectionX		= m_r;
@synthesize propProjectionY		= m_h;
@synthesize propVertexSize		= m_vertexSize;


// Size property
//-----------------------------------------------------------------------------------------------
- (void)setPropSize:(float)size
{
	if (m_s != size)
	{
		m_s	= size;
		m_h = sin(RAD_30) * m_s;
		m_r = cos(RAD_30) * m_s;
		m_b = m_s + 2 * m_h;
		m_a = m_r * 2;
		
		[self updateVertices];
	}
}

- (float)propSize
{
	return m_s;
}

// Aspect property
//-----------------------------------------------------------------------------------------------
- (void)setPropAspect:(float)aspect
{
	if (m_aspect != aspect)
	{
		m_aspect = aspect;
		
		[self updateVertices];
	}
}

- (float)propAspect
{
	return m_aspect;
}

// ****** INSTANCE METHODS ******

// Default initializer
//-----------------------------------------------------------------------------------------------
- (id)init
{
	if ( (self = [super init]) )
	{
		self.propColorFill		= [NSColor colorWithCalibratedRed:1.0f green:1.0f blue:0.0f alpha:0.5f];
		self.propColorEdge		= [NSColor colorWithCalibratedRed:0.0f green:1.0f blue:0.0f alpha:1.0f];
		self.propColorVertex	= [NSColor colorWithCalibratedRed:1.0f green:0.0f blue:0.0f alpha:1.0f];
		m_renderEdges			= YES;
		m_renderSolid			= NO;
		m_renderVertices		= NO;
		m_vertexSize			= 2;
		m_aspect				= 1.0f;
		self.propSize			= 64;
	}
	
	return self;
}

// Designated initializer
//-----------------------------------------------------------------------------------------------
- (id)initWithSize:(float)size
{
	if ( (self = [self init]) )
	{
		self.propSize	= size;
	}
	
	return self;
}

// Destructor
//-----------------------------------------------------------------------------------------------
- (void)dealloc
{
	SAFE_RELEASE(m_colorFill);
	SAFE_RELEASE(m_colorEdge);
	SAFE_RELEASE(m_colorVertex);
	
    [super dealloc];
}

// Rendering hexagon
//-----------------------------------------------------------------------------------------------
- (void)render
{
	glDisable(GL_TEXTURE_2D);
	
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
	
	glVertexPointer(	3, GL_FLOAT, sizeof(OGLVertex), &m_vertices[0].pos);
	glColorPointer(		4, GL_FLOAT, sizeof(OGLVertex), &m_vertices[0].color);
	glTexCoordPointer(	2, GL_FLOAT, sizeof(OGLVertex), &m_vertices[0].uv);
	
	if (m_renderSolid)
	{
		[m_colorFill setOpenGL];
		glDrawArrays(GL_TRIANGLE_FAN, 0, 6);
	}
	
	if (m_renderEdges)
	{
		[m_colorEdge setOpenGL];
		glDrawArrays(GL_LINE_LOOP, 0, 6);
	}
	
	if (m_renderVertices)
	{
		glPointSize(m_vertexSize);
		
		[m_colorVertex setOpenGL];
		
		glDrawArrays(GL_POINTS, 0, 6);
	}
}

// Update hexagon shape vertices
//-----------------------------------------------------------------------------------------------
- (void)updateVertices
{
	m_vertices[0].pos.x	= m_r;		m_vertices[0].pos.y	= 0;						m_vertices[0].pos.z = 0;
	m_vertices[1].pos.x	= 0;		m_vertices[1].pos.y	= -m_h * m_aspect;			m_vertices[1].pos.z = 0;
	m_vertices[2].pos.x	= 0;		m_vertices[2].pos.y	= -(m_h+m_s) * m_aspect;	m_vertices[2].pos.z = 0;
	m_vertices[3].pos.x	= m_r;		m_vertices[3].pos.y	= -m_b * m_aspect;			m_vertices[3].pos.z = 0;
	m_vertices[4].pos.x	= m_a;		m_vertices[4].pos.y	= -(m_h+m_s) * m_aspect;	m_vertices[4].pos.z = 0;
	m_vertices[5].pos.x	= m_a;		m_vertices[5].pos.y	= -m_h * m_aspect;			m_vertices[5].pos.z = 0;
}





@end
