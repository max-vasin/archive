//=======================================================================================================================
//  File:		OriStringDrawer.m
//  Project:	heli
//	Desc:		String drawer utility class implementation
//
//  Created by Max Vasin on 6/9/11.
//  Copyright 2011 Origin. All rights reserved.
//=======================================================================================================================

// ******** IMPORTS ********

#import "OriStringDrawer.h"
#import "OriResourceManager.h"

// ******** PRIVATE METHODS ********

@interface OriStringDrawer (Private)

- (void)			updateGeometry;
- (oriFontGlyph*)	makeChar:(oglVertex*)shape Code:(oriChar)code Position:(oriVector2)pos;

@end


// ******** IMPLEMENTATION ********

@implementation OriStringDrawer

//#######################################################################################################################
// PROPERTIES
//#######################################################################################################################

//-----------------------------------------------------------------------------------------------------------------------
// SYNTHESIZED PROPERTIES
//-----------------------------------------------------------------------------------------------------------------------

@synthesize propFont		= m_font;

//-----------------------------------------------------------------------------------------------------------------------
// PROPERTY: string contents
//-----------------------------------------------------------------------------------------------------------------------

- (void)setPropString:(NSString*)string
{
	if (string != m_string)
	{
		[m_string release];
		
		m_string = [string copy];
		
		[self updateGeometry];
	}
}

- (NSString*)propString
{
	return m_string;
}

//-----------------------------------------------------------------------------------------------------------------------
// PROPERTY: string alignment
//-----------------------------------------------------------------------------------------------------------------------

- (void)setPropAlign:(enStringDrawerAlign)align
{
	if (m_align != align)
	{
		m_align = align;
		
		[self updateGeometry];
	}
}

- (enStringDrawerAlign)propAlign
{
	return m_align;
}

//-----------------------------------------------------------------------------------------------------------------------
// PROPERTY: color
//-----------------------------------------------------------------------------------------------------------------------

- (void)setPropColor:(oglColor)color
{
	if (!EQUAL_COLORS(m_color, color))
	{
		m_color = color;
		
		[self updateGeometry];
	}
}

- (oglColor)propColor
{
	return m_color;
}

//-----------------------------------------------------------------------------------------------------------------------
// PROPERTY: spacing
//-----------------------------------------------------------------------------------------------------------------------

- (void)setPropSpacing:(oriInt)spacing
{
	if (m_spacing != spacing)
	{
		m_spacing = spacing;
		
		[self updateGeometry];
	}
}

- (oriInt)propSpacing
{
	return m_spacing;
}

//#######################################################################################################################
// INITIALIZERS\DESTRUCTORS
//#######################################################################################################################

//-----------------------------------------------------------------------------------------------------------------------
// Designated initializer
//-----------------------------------------------------------------------------------------------------------------------

- (id)initWithFont:(OriFont*)font withCapacity:(oriInt)capacity
{
	if ( (self = [self init]) )
	{
		m_font	= [font retain];

#if defined (ORIENGINE_STRING_DRAWER_USES_VBO)
		
		glGenBuffers(1, &m_vbo);
		
		if (capacity)
		{
			glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
			glBufferData(GL_ARRAY_BUFFER, sizeof(oglVertex) * capacity * VERTEX_PER_CHAR, NULL, GL_DYNAMIC_DRAW);
		}
#else
		if (capacity)
		m_vertexBuffer	= (oglVertex*)calloc(capacity * VERTEX_PER_CHAR, sizeof(capacity));
#endif
		m_color.r = m_color.g = m_color.b = m_color.a = 255;
	}
	
	return self;
}

//-----------------------------------------------------------------------------------------------------------------------
// Designated initializer with font name
//-----------------------------------------------------------------------------------------------------------------------

- (id)initWithFontName:(NSString*)name	withCapacity:(oriInt)capacity
{
	OriFont* font = [[OriResourceManager sharedManager] getFont:name];
	
	if (!font)
		return nil;
	
	self = [self initWithFont:font withCapacity:capacity];
	
	[font release];
	
	return self;
}


//-----------------------------------------------------------------------------------------------------------------------
// Destructor
//-----------------------------------------------------------------------------------------------------------------------

- (void)dealloc
{
	[m_string release];
	[m_font	release];

#if defined (ORIENGINE_STRING_DRAWER_USES_VBO)

	glDeleteBuffers(1, &m_vbo);

#else
	
	SAFE_FREE(m_vertexBuffer);
	
#endif
	
	[super dealloc];
}

//-----------------------------------------------------------------------------------------------------------------------
// Resets internal buffers
//-----------------------------------------------------------------------------------------------------------------------

- (void)resetBuffers
{

	oriInt newSize = sizeof(oglVertex) * [m_string length] * VERTEX_PER_CHAR;
	
	if (!newSize)
		newSize = sizeof(oglVertex);
	
#if defined (ORIENGINE_STRING_DRAWER_USES_VBO)

	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, newSize, NULL, GL_DYNAMIC_DRAW);

#else

	SAFE_FREE(m_vertexBuffer);
	m_vertexBuffer = (oglVertex*)calloc(newSize, 1);

#endif

}


//#######################################################################################################################
// TEXT DRAWING
//#######################################################################################################################

//-----------------------------------------------------------------------------------------------------------------------
// Renders text shape
//-----------------------------------------------------------------------------------------------------------------------

- (void)render
{
	// Set shape texture
	
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, m_font.propTexture.propHandle);
	
	// Enable alpha blending
	
	glEnable(GL_BLEND);
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
	
	// Set streams

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);

#if defined (ORIENGINE_STRING_DRAWER_USES_VBO)

	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

	glVertexPointer(3,		GL_FLOAT,			sizeof(oglVertex), (GLvoid*)0);
	glColorPointer(4,		GL_UNSIGNED_BYTE,	sizeof(oglVertex), (GLvoid*)sizeof(oglVector3));
	glTexCoordPointer(2,	GL_FLOAT,			sizeof(oglVertex), (GLvoid*)(sizeof(oglVector3) + sizeof(oglColor)));

#else

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glVertexPointer(3,		GL_FLOAT,			sizeof(oglVertex), &m_vertexBuffer[0].pos);
	glColorPointer(4,		GL_UNSIGNED_BYTE,	sizeof(oglVertex), &m_vertexBuffer[0].color);
	glTexCoordPointer(2,	GL_FLOAT,			sizeof(oglVertex), &m_vertexBuffer[0].map);

#endif
	
	// Draw
	
	glDrawArrays(GL_TRIANGLES, 0, m_vertexCount);

}

//#######################################################################################################################
// PRIVATE METHODS
//#######################################################################################################################

//-----------------------------------------------------------------------------------------------------------------------
// Updates text shape
//-----------------------------------------------------------------------------------------------------------------------

- (void)updateGeometry
{
	m_vertexCount = [m_string length] * VERTEX_PER_CHAR;
	
	oriInt newSize = sizeof(oglVertex) * m_vertexCount;
	oriInt oldSize = 0;

#if defined (ORIENGINE_STRING_DRAWER_USES_VBO)

	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	
	glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &oldSize);
	
	if (newSize > oldSize)
		glBufferData(GL_ARRAY_BUFFER, newSize, NULL, GL_DYNAMIC_DRAW);
	
	oglVertex* data = (oglVertex*)glMapBufferOES(GL_ARRAY_BUFFER, GL_WRITE_ONLY_OES);
	
#else
	
	if (m_vertexBuffer)
		oldSize = malloc_size(m_vertexBuffer);

	if (newSize > oldSize)
	{
		SAFE_FREE(m_vertexBuffer);
		m_vertexBuffer = (oglVertex*)calloc(newSize, 1);
	}
	
	oglVertex* data = m_vertexBuffer;

#endif
	
	if (!data)
		return;
	
	oriInt		textLength = [m_string length];
	oriVector2	cursor;
	
	for (oriInt charIndex = 0; charIndex < textLength; charIndex++)
	{
		oriChar code = [m_string characterAtIndex:charIndex];
		
		oriFontGlyph* glyph = [self makeChar:&data[charIndex * VERTEX_PER_CHAR] Code:code Position:cursor];
		
		switch (m_align)
		{
			case eStringDrawerAlignLeft:
				cursor.x += glyph->size.width + m_spacing;
				break;
			case eStringDrawerAlignRight:
				cursor.x += glyph->size.width + m_spacing;
				break;
			case eStringDrawerAlignMiddle:
				cursor.x += glyph->size.width + m_spacing;
				break;
		}
	}

#if defined (ORIENGINE_STRING_DRAWER_USES_VBO)

	glUnmapBufferOES(GL_ARRAY_BUFFER);

#endif
}

//-----------------------------------------------------------------------------------------------------------------------
// Updates single charater
//-----------------------------------------------------------------------------------------------------------------------

- (oriFontGlyph*)makeChar:(oglVertex*)shape Code:(oriChar)code Position:(oriVector2)pos
{
	oriFontGlyph* glyph = [m_font glyph:code];
	
	if (!glyph)
		return NULL;

	shape[0].pos.x	= pos.x;						shape[0].pos.y = pos.y + glyph->size.height;
	shape[1].pos.x	= pos.x + glyph->size.width;	shape[1].pos.y = pos.y + glyph->size.height;
	shape[2].pos.x	= pos.x;						shape[2].pos.y = pos.y;
	shape[3].pos.x	= pos.x;						shape[3].pos.y = pos.y;
	shape[4].pos.x	= pos.x + glyph->size.width;	shape[4].pos.y = pos.y;
	shape[5].pos.x	= pos.x + glyph->size.width;	shape[5].pos.y = pos.y + glyph->size.height;

	shape[0].map.u	= glyph->mapmin.u;		shape[0].map.v	= glyph->mapmin.v;
	shape[1].map.u	= glyph->mapmax.u;		shape[1].map.v	= glyph->mapmin.v;
	shape[2].map.u	= glyph->mapmin.u;		shape[2].map.v	= glyph->mapmax.v;
	shape[3].map.u	= glyph->mapmin.u;		shape[3].map.v	= glyph->mapmax.v;
	shape[4].map.u	= glyph->mapmax.u;		shape[4].map.v	= glyph->mapmax.v;
	shape[5].map.u	= glyph->mapmax.u;		shape[5].map.v	= glyph->mapmin.v;
	
	shape[0].color	= m_color;
	shape[1].color	= m_color;
	shape[2].color	= m_color;
	shape[3].color	= m_color;
	shape[4].color	= m_color;
	shape[5].color	= m_color;
	
	return glyph;
}

@end
