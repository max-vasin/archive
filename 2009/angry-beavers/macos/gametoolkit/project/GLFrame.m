//===============================================================================================
//  File:		GLFrame.m
//  Project:	OriginGameToolkit
//	Desc:		OpenGL frame class implementation
//
//  Created by Max Vasin on 4/27/11.
//  Copyright 2011 Origin Games. All rights reserved.
//===============================================================================================

// ***** IMPORTS *****

#import "GLFrame.h"

// ***** PRIVATE METHODS *****

@interface GLFrame (Private)

- (void)	rebuild;
- (void)	setColor:(OGLVertex*)array;
- (void)	renderRect:(OGLVertex*)array;

@end

// ***** IMPLEMENTATION *****

@implementation GLFrame

//###############################################################################################
// PROPERTIES
//###############################################################################################

// Synthesized
//-----------------------------------------------------------------------------------------------
@synthesize propVisible		= m_visible;

// Frame position X
//-----------------------------------------------------------------------------------------------
- (void)setPropPositionX:(int)x
{
	m_position.x = x;
	m_rebuild = YES;
}

- (int)propPositionX
{
	return m_position.x;
}

// Frame position Y
//-----------------------------------------------------------------------------------------------
- (void)setPropPositionY:(int)y
{
	m_position.y = y;
	m_rebuild = YES;
}

- (int)propPositionY
{
	return m_position.y;
}

// Frame width
//-----------------------------------------------------------------------------------------------
- (void)setPropWidth:(uint)w
{
	m_width = w;
	m_rebuild = YES;
}

- (uint)propWidth
{
	return m_width;
}

// Frame height
//-----------------------------------------------------------------------------------------------
- (void)setPropHeight:(uint)h
{
	m_height = h;
	m_rebuild = YES;
}

- (uint)propHeight
{
	return m_height;
}

// Frame modulation color
//-----------------------------------------------------------------------------------------------
- (void)setPropColor:(NSColor*)color
{
	if (m_color != color)
	{
		[m_color release];
		
		m_color = [color copy];
		
		m_rebuild = YES;
	}
}

- (NSColor*)propColor
{
	return m_color;
}

// Frame border width
//-----------------------------------------------------------------------------------------------
- (void)setPropBorder:(uint)b
{
	m_border = b;

	m_rebuild = YES;
}

- (uint)propBorder
{
	return m_border;
}

// Frame texture
//-----------------------------------------------------------------------------------------------
- (void)setPropTexture:(GLuint)tex
{
	m_texture = tex;
}

- (GLuint)propTexture
{
	return m_texture;
}

// Frame texture size
//-----------------------------------------------------------------------------------------------
- (void)setPropTextureSize:(IntSize)size
{
	m_texSize = size;
	
	m_rebuild = YES;
}

- (IntSize)propTextureSize
{
	return m_texSize;
}

//###############################################################################################
// INITIALIZERS\DESTRUCTORS
//###############################################################################################

// Frame default initializer
//-----------------------------------------------------------------------------------------------
- (id)init
{
	if ( (self = [super init]) )
	{
		self.propColor = [NSColor colorWithCalibratedRed:1.0f green:0.0f blue:0.0f alpha:1.0f];
	}
	return self;
}

// Destructor
//-----------------------------------------------------------------------------------------------
- (void)dealloc
{
	[m_color release];
	
	[super dealloc];
}

//###############################################################################################
// DRAWING
//###############################################################################################

// Frame rendering
//-----------------------------------------------------------------------------------------------
- (void)render
{
	if (!m_visible)
		return;
	
	if (m_rebuild)
		[self rebuild];
	
	// Set blending ops
	
	glEnable( GL_BLEND );
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
	glEnable( GL_TEXTURE_2D );
	
	glBindTexture( GL_TEXTURE_2D, m_texture );
	
	// Preparing sources
	
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
	
	[self renderRect:m_rectL];
	[self renderRect:m_rectR];
	[self renderRect:m_rectT];
	[self renderRect:m_rectB];
}

//###############################################################################################
// PRIVATE METHODS
//###############################################################################################

// Renders rectangle vertices
//-----------------------------------------------------------------------------------------------
- (void)renderRect:(OGLVertex*)array
{
	glVertexPointer(	3, GL_FLOAT, sizeof(OGLVertex), &array->pos);
	glColorPointer(		4, GL_FLOAT, sizeof(OGLVertex), &array->color);
	glTexCoordPointer(	2, GL_FLOAT, sizeof(OGLVertex), &array->uv);
	
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

// Builds frame vertices
//-----------------------------------------------------------------------------------------------
- (void)rebuild
{
	// Precalc
	
	float outMinX	= (float)m_position.x - (float)m_border;
	float outMinY	= (float)m_position.y - (float)m_border;
	float inMinX	= (float)m_position.x;
	float inMinY	= (float)m_position.y;
	float outMaxX	= (float)m_position.x + (float)m_width + (float)m_border;
	float outMaxY	= (float)m_position.y + (float)m_height + (float)m_border;
	float inMaxX	= (float)m_position.x + (float)m_width;
	float inMaxY	= (float)m_position.y + (float)m_height;
	
	float du		= m_texSize.width ? (float)m_border / (float)m_texSize.width : 0;
	float dv		= m_texSize.height ? (float)m_border / (float)m_texSize.height : 0;
	float umax		= m_texSize.width ? (float)(m_width + m_border * 2) / (float)m_texSize.width : 0;
	float vmax		= m_texSize.height ? (float)(m_height + m_border * 2) / (float)m_texSize.height : 0;
	
	// Left rectangle
	
	m_rectL[0].pos.x = outMinX; m_rectL[0].pos.y = outMinY; m_rectL[0].pos.z = 0.0f;
	m_rectL[1].pos.x = inMinX;  m_rectL[1].pos.y = outMinY; m_rectL[1].pos.z = 0.0f;
	m_rectL[2].pos.x = outMinX; m_rectL[2].pos.y = outMaxY; m_rectL[2].pos.z = 0.0f;
	m_rectL[3].pos.x = inMinX;  m_rectL[3].pos.y = outMaxY; m_rectL[3].pos.z = 0.0f;
	
	m_rectL[0].uv.x	= 0.0f;	m_rectL[0].uv.y	= 0.0f;
	m_rectL[1].uv.x = du;	m_rectL[1].uv.y	= 0.0f;
	m_rectL[2].uv.x = 0.0f;	m_rectL[2].uv.y	= vmax;
	m_rectL[3].uv.x = du;	m_rectL[3].uv.y	= vmax;
	
	// Right rectangle
	
	m_rectR[0].pos.x = inMaxX;  m_rectR[0].pos.y = outMinY; m_rectR[0].pos.z = 0.0f;
	m_rectR[1].pos.x = outMaxX; m_rectR[1].pos.y = outMinY; m_rectR[1].pos.z = 0.0f;
	m_rectR[2].pos.x = inMaxX;  m_rectR[2].pos.y = outMaxY; m_rectR[2].pos.z = 0.0f;
	m_rectR[3].pos.x = outMaxX; m_rectR[3].pos.y = outMaxY; m_rectR[3].pos.z = 0.0f;

	m_rectR[0].uv.x	= umax - du;	m_rectR[0].uv.y	= 0.0f;
	m_rectR[1].uv.x = umax;			m_rectR[1].uv.y	= 0.0f;
	m_rectR[2].uv.x = umax - du;	m_rectR[2].uv.y	= vmax;
	m_rectR[3].uv.x = umax;			m_rectR[3].uv.y	= vmax;

	// Top rectangle
	
	m_rectT[0].pos.x = inMinX;  m_rectT[0].pos.y = inMinY;  m_rectT[0].pos.z = 0.0f;
	m_rectT[1].pos.x = inMaxX;  m_rectT[1].pos.y = inMinY;  m_rectT[1].pos.z = 0.0f;
	m_rectT[2].pos.x = inMinX;  m_rectT[2].pos.y = outMinY; m_rectT[2].pos.z = 0.0f;
	m_rectT[3].pos.x = inMaxX;  m_rectT[3].pos.y = outMinY; m_rectT[3].pos.z = 0.0f;

	m_rectT[0].uv.x	= du;			m_rectT[0].uv.y	= dv;
	m_rectT[1].uv.x = umax - du;	m_rectT[1].uv.y	= dv;
	m_rectT[2].uv.x = du;			m_rectT[2].uv.y	= 0.0f;
	m_rectT[3].uv.x = umax - du;	m_rectT[3].uv.y	= 0.0f;

	// Bottom rectangle
	
	m_rectB[0].pos.x = inMinX;  m_rectB[0].pos.y = outMaxY; m_rectB[0].pos.z = 0.0f;
	m_rectB[1].pos.x = inMaxX;  m_rectB[1].pos.y = outMaxY; m_rectB[1].pos.z = 0.0f;
	m_rectB[2].pos.x = inMinX;  m_rectB[2].pos.y = inMaxY;  m_rectB[2].pos.z = 0.0f;
	m_rectB[3].pos.x = inMaxX;  m_rectB[3].pos.y = inMaxY;  m_rectB[3].pos.z = 0.0f;
	
	m_rectB[0].uv.x	= du;			m_rectB[0].uv.y	= vmax;
	m_rectB[1].uv.x = umax - du;	m_rectB[1].uv.y	= vmax;
	m_rectB[2].uv.x = du;			m_rectB[2].uv.y	= vmax - dv;
	m_rectB[3].uv.x = umax - du;	m_rectB[3].uv.y	= vmax - dv;

	// Set rectangles color
	
	[self setColor:m_rectL];
	[self setColor:m_rectR];
	[self setColor:m_rectT];
	[self setColor:m_rectB];
	
	m_rebuild = NO;
}

// Applies color to all rectange vertices in array
//-----------------------------------------------------------------------------------------------
- (void)setColor:(OGLVertex*)array
{
	for (int i = 0; i < 4; i++)
		[m_color getComponents:(CGFloat*)&array[i].color];
}

@end
