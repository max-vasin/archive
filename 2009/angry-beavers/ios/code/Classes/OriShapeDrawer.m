//=======================================================================================================================
//  File:		OriShapeDrawer.m
//  Project;	heli
//	Desc:		Shape drawer utility class implementation
//
//  Created by Max Vasin on 6/3/11.
//  Copyright 2011 Origin. All rights reserved.
//=======================================================================================================================

// ******** IMPORTS ********

#import "OriShapeDrawer.h"

// ******** PRIVATE METHODS ********

@interface OriShapeDrawer (Private)

- (void) setZ:(oriFloat)value;

@end


// ******** IMPLEMENTATION ********

@implementation OriShapeDrawer

//#######################################################################################################################
// PROPERTIES
//#######################################################################################################################

//-----------------------------------------------------------------------------------------------------------------------
// SYNTHESIZED PROPERTIES
//-----------------------------------------------------------------------------------------------------------------------

@synthesize propTexture		= m_texture;
@synthesize propOffset		= m_offset;

//-----------------------------------------------------------------------------------------------------------------------
// PROPERTY: shape vertices color
//-----------------------------------------------------------------------------------------------------------------------

- (void)setPropColor:(oglColor)color
{
	m_color = color;
	
	[self setAlpha:color.a];
}

- (oglColor)propColor
{
	return m_color;
}

//#######################################################################################################################
// CONSTRUCTION\DESTRUCTION
//#######################################################################################################################

//-----------------------------------------------------------------------------------------------------------------------
// Rectangle initializer
//-----------------------------------------------------------------------------------------------------------------------

- (id)initRectangle
{
	if ( (self = [self init]) )
	{
		m_type			= eShapeDrawerRectangle;
		m_vertexBuffer	= calloc(4, sizeof(oglVertex));
		m_vertexCount	= 4;
		m_color.r		= 255;
		m_color.g		= 255;
		m_color.b		= 255;
		
		[self setZ:0.0f];
		[self setAlpha:255];
	}
	return self;
}

//-----------------------------------------------------------------------------------------------------------------------
// Destructor
//-----------------------------------------------------------------------------------------------------------------------

- (void)dealloc
{
	SAFE_FREE(m_vertexBuffer);
	
	[super dealloc];
}

//#######################################################################################################################
// ALL SHAPE TYPES METHODS
//#######################################################################################################################

//-----------------------------------------------------------------------------------------------------------------------
// Sets vertices alpha
//-----------------------------------------------------------------------------------------------------------------------

- (void)setAlpha:(GLubyte)alpha
{
	m_color.a = alpha;
	
	for (oriInt i = 0; i < m_vertexCount; i++)
	{
		m_vertexBuffer[i].color.r = m_color.r;
		m_vertexBuffer[i].color.g = m_color.g;
		m_vertexBuffer[i].color.b = m_color.b;
		m_vertexBuffer[i].color.a = alpha;
	}
}


//#######################################################################################################################
// RECTANGLE RELATED METHODS
//#######################################################################################################################

//-----------------------------------------------------------------------------------------------------------------------
// Sets size of rectangle in pixels
//-----------------------------------------------------------------------------------------------------------------------

- (void)rectangleSetSize:(oriIntSize)size
{
	if (m_type != eShapeDrawerRectangle)
		return;

	m_vertexBuffer[0].pos.x	= m_offset.x;					m_vertexBuffer[0].pos.y	= m_offset.y + size.height;
	m_vertexBuffer[1].pos.x	= m_offset.x + size.width;		m_vertexBuffer[1].pos.y	= m_offset.y + size.height;
	m_vertexBuffer[2].pos.x	= m_offset.x;					m_vertexBuffer[2].pos.y	= m_offset.y;
	m_vertexBuffer[3].pos.x	= m_offset.x + size.width;		m_vertexBuffer[3].pos.y	= m_offset.y;
}

//-----------------------------------------------------------------------------------------------------------------------
// Sets rectangle vertices texture coordinates
//-----------------------------------------------------------------------------------------------------------------------

- (void)rectangleSetMapping:(oriRect)map
{
	if (m_type != eShapeDrawerRectangle)
		return;

	m_vertexBuffer[0].map.u	= map.min.x;		m_vertexBuffer[0].map.v	= map.min.y;
	m_vertexBuffer[1].map.u	= map.max.x;		m_vertexBuffer[1].map.v	= map.min.y;
	m_vertexBuffer[2].map.u	= map.min.x;		m_vertexBuffer[2].map.v	= map.max.y;
	m_vertexBuffer[3].map.u	= map.max.x;		m_vertexBuffer[3].map.v	= map.max.y;
}

//#######################################################################################################################
// SHAPE DRAWING
//#######################################################################################################################

//-----------------------------------------------------------------------------------------------------------------------
// Renders shape with current texture
//-----------------------------------------------------------------------------------------------------------------------

- (void)render
{
	// Set shape texture
	
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, m_texture.propHandle);
	
	// Enable alpha blending
	
	glEnable(GL_BLEND);
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
	
	// Set streams
	
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
	
	glVertexPointer(3,		GL_FLOAT,			sizeof(oglVertex), &m_vertexBuffer[0].pos);
	glColorPointer(4,		GL_UNSIGNED_BYTE,	sizeof(oglVertex), &m_vertexBuffer[0].color);
	glTexCoordPointer(2,	GL_FLOAT,			sizeof(oglVertex), &m_vertexBuffer[0].map);
	
	// Draw
	
	glDrawArrays(GL_TRIANGLE_STRIP, 0, m_vertexCount);
}

//#######################################################################################################################
// PRIVATE METHODS
//#######################################################################################################################

//-----------------------------------------------------------------------------------------------------------------------
// Sets Z component of buffer to specified value
//-----------------------------------------------------------------------------------------------------------------------

- (void)setZ:(oriFloat)value
{
	for (oriInt i = 0; i < m_vertexCount; i++)
		m_vertexBuffer[i].pos.z = value;
}

@end
