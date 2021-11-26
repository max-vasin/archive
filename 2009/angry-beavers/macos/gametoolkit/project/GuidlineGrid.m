//===============================================================================================
//  File:		GuidlineGrid.m
//  Project:	OriginGameToolkit
//	Desc:		View guidline grid utility class implementation
//
//  Created by Max Vasin on 12/3/10.
//  Copyright 2010 Origin. All rights reserved.
//===============================================================================================

// **** IMPORTS ****

#import "GuidlineGrid.h"

// **** IMPLEMENTATION ****

@implementation GuidlineGrid

// **** SYNTHESIZED PROPERTIES ****

@synthesize propDashMultiplier  = m_dashMultiplier;
@synthesize propAxisWidth		= m_axisWidth;

// **** IMPLEMENTED PROPERTIES ****

// propZoom
//-----------------------------------------------------------------------------------------------

- (GLfloat)propZoom
{
	return m_zoom;
}

- (void)setPropZoom:(GLfloat)zoom
{
	m_zoom = zoom;
}

// propVSpace
//-----------------------------------------------------------------------------------------------

- (GLfloat)propVSpace
{
	return m_vSpace;
}

- (void)setPropVSpace:(GLfloat)value
{
	if (!value)
		value = 1.0f;
	m_vSpace = floor(value);
	m_builded = NO;
}

// propHSpace
//-----------------------------------------------------------------------------------------------

- (GLfloat)propHSpace
{
	return m_hSpace;
}

- (void)setPropHSpace:(GLfloat)value
{
	if (!value)
		value = 1.0f;
	m_hSpace = floor(value);
	m_builded = NO;
}

// propFrame
//-----------------------------------------------------------------------------------------------

- (NSRect)propFrame
{
	return m_rectFrame;
}

- (void)setPropFrame:(NSRect)rect
{
	m_rectFrame = rect;
	m_builded = NO;
}

// propOrigin
//-----------------------------------------------------------------------------------------------

- (Vector2*)propOrigin
{
	return [[m_vOrigin copy] autorelease];
}

- (void)setPropOrigin:(Vector2*)origin
{
	if (m_vOrigin != origin)
	{
		[m_vOrigin autorelease];
		m_vOrigin = [origin copy];
		
		if (m_lockScaleOrigin)
			self.propZoomOrigin = m_vOrigin;
	}
}

// propZoomOrigin
//-----------------------------------------------------------------------------------------------

- (Vector2*)propZoomOrigin
{
	return [[m_vScaleOrigin copy] autorelease];
}

- (void)setPropZoomOrigin:(Vector2*)origin
{
	if (m_vScaleOrigin != origin)
	{
		[m_vScaleOrigin autorelease];
		m_vScaleOrigin = [origin copy];
	}
}

// propLockZoomOrigin
//-----------------------------------------------------------------------------------------------

- (BOOL)propLockZoomOrigin
{
	return m_lockScaleOrigin;
}

- (void)setPropLockZoomOrigin:(BOOL)lock
{
	m_lockScaleOrigin = lock;
	if (m_lockScaleOrigin)
		self.propZoomOrigin = self.propOrigin;
}


// propDashColor
//-----------------------------------------------------------------------------------------------

- (NSColor*)propDashColor
{
	return m_dashColor;
}

- (void)setPropDashColor:(NSColor*)color
{
	if (m_dashColor != color)
	{
		[m_dashColor autorelease];
		m_dashColor = [color copy];
		m_builded = NO;
	}
}

// propAxisColor
//-----------------------------------------------------------------------------------------------

- (NSColor*)propAxisColor
{
	return m_axisColor;
}

- (void)setPropAxisColor:(NSColor*)color
{
	if (m_axisColor != color)
	{
		[m_axisColor autorelease];
		m_axisColor = [color copy];
		m_builded = NO;
	}
}

// **** INSTANCE METHODS IMPLEMENTATION ****

// init
// This initializer not allowed
//-----------------------------------------------------------------------------------------------

- (id)init
{
	DBG_START [self log:"$CERROR - method (init) not allowed for instances of this class\n"]; DBG_END
	[super dealloc];
	return nil;	
}

// initWithContext
// Designated initializer with OpenGL context
//-----------------------------------------------------------------------------------------------

- (id)initWithContext:(NSOpenGLContext*)context andRect:(NSRect)frame
{
	if (self = [super init])
	{
		m_context			= context;
		m_vOrigin			= [Vector2 create];
		m_vScaleOrigin		= [Vector2 create];
		m_vSpace			= 25.0f;
		m_hSpace			= 25.0f;
		m_pVertices			= NULL;
		m_pOriginVertices	= (OGLColorVertex*)calloc(4, sizeof(OGLColorVertex));
		m_primitiveCount	= 0;
		m_rectFrame			= frame;
		m_builded			= NO;
		m_zoom				= 1.0f;
		m_axisWidth			= 3.0f;
		m_pVertices			= (OGLColorVertex*)calloc(GUIDGRID_LINE_COUNT * 4, sizeof(OGLColorVertex));
	}
	return self;
}

// dealloc
// Grid destruction
//-----------------------------------------------------------------------------------------------

- (void)dealloc
{
	SAFE_RELEASE(m_vOrigin);
	SAFE_RELEASE(m_vScaleOrigin);
	SAFE_FREE(m_pVertices);
	SAFE_FREE(m_pOriginVertices);
	
	[super dealloc];
}


// reBuild
// Rebuilds grid with current data
//-----------------------------------------------------------------------------------------------
- (void)reBuild
{
	// Build whole grid
	
	Vec2 start;
	start.x = -floor((GUIDGRID_LINE_COUNT / 2.0f) * m_hSpace);
	start.y = -floor((GUIDGRID_LINE_COUNT / 2.0f) * m_vSpace);
		
	// Build axis
	
	m_pOriginVertices[0].pos.x = start.x;
	m_pOriginVertices[0].pos.y = 0.0f;
	m_pOriginVertices[0].pos.z = 0.0f;
	[m_axisColor getComponents:(CGFloat*)&m_pOriginVertices[0].color];
	
	m_pOriginVertices[1].pos.x = -start.x;
	m_pOriginVertices[1].pos.y = 0.0f;
	m_pOriginVertices[1].pos.z = 0.0f;
	[m_axisColor getComponents:(CGFloat*)&m_pOriginVertices[1].color];
	
	m_axisXVisible = YES;
		
	m_pOriginVertices[2].pos.x = 0.0f;
	m_pOriginVertices[2].pos.y = start.y;
	m_pOriginVertices[2].pos.z = 0.0f;
	[m_axisColor getComponents:(CGFloat*)&m_pOriginVertices[2].color];
	
	m_pOriginVertices[3].pos.x = 0.0f;
	m_pOriginVertices[3].pos.y = -start.y;
	m_pOriginVertices[3].pos.z = 0.0f;
	[m_axisColor getComponents:(CGFloat*)&m_pOriginVertices[3].color];
	
	m_axisYVisible = YES;
	
	NSInteger i;
	
	// Vertical lines
	
	for (i = 0; i < GUIDGRID_LINE_COUNT; i++)
	{
		GLfloat offset = floor(i * m_hSpace);
		
		m_pVertices[i * 2 + 0].pos.x = start.x + offset;
		m_pVertices[i * 2 + 0].pos.y = start.y;
		m_pVertices[i * 2 + 0].pos.z = 0.0f;
		[m_dashColor getComponents:(CGFloat*)&m_pVertices[i * 2 + 0].color];
		
		m_pVertices[i * 2 + 1].pos.x = start.x + offset;
		m_pVertices[i * 2 + 1].pos.y = -start.y;
		m_pVertices[i * 2 + 1].pos.z = 0.0f;
		[m_dashColor getComponents:(CGFloat*)&m_pVertices[i * 2 + 1].color];
		
	}
	
	// Horizontal lines
	
	NSInteger base = GUIDGRID_LINE_COUNT * 2;
	
	for (i = 0; i < GUIDGRID_LINE_COUNT; i++)
	{
		GLfloat offset = floor(i * m_vSpace);
		
		m_pVertices[base + i * 2 + 0].pos.x = start.x;
		m_pVertices[base + i * 2 + 0].pos.y = start.y + offset;
		m_pVertices[base + i * 2 + 0].pos.z = 0.0f;
		[m_dashColor getComponents:(CGFloat*)&m_pVertices[base + i * 2 + 0].color];
		
		m_pVertices[base + i * 2 + 1].pos.x = -start.x;
		m_pVertices[base + i * 2 + 1].pos.y = start.y + offset;
		m_pVertices[base + i * 2 + 1].pos.z = 0.0f;
		[m_dashColor getComponents:(CGFloat*)&m_pVertices[base + i * 2 + 1].color];
	}
	
	m_primitiveCount = GUIDGRID_LINE_COUNT;
	
	m_builded = YES;
}



// render
// Renders grid with associated OpenGL context
//-----------------------------------------------------------------------------------------------

- (void)render
{
	if (!m_builded)
		[self reBuild];
	
	// Set states
	
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
    glDisable(GL_DEPTH_TEST);
	glDisable(GL_LINE_SMOOTH);
	
	
	// Set sources usage
	
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	
	// Reset current transformations
	
	glMatrixMode(GL_PROJECTION);
    glPushMatrix();
	glLoadIdentity();
    glOrtho(0.0f, m_rectFrame.size.width, m_rectFrame.size.height, 0.0f, -1000.0f, 1000.0f);
	
	glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
	glLoadIdentity();
	
	// Apply zooming
	
	glTranslatef(floor(m_vScaleOrigin->x), floor(m_vScaleOrigin->y), 0.0f);
	glScalef(m_zoom, m_zoom, 1.0f);
	glTranslatef(floor(m_vOrigin->x - m_vScaleOrigin->x), floor(m_vOrigin->y - m_vScaleOrigin->y), 0.0f);
	
	// Dash lines
	
	glVertexPointer(3, GL_FLOAT, sizeof(OGLColorVertex), m_pVertices);
	glColorPointer(4, GL_FLOAT, sizeof(OGLColorVertex), &m_pVertices[0].color);
	
	glLineWidth(1.0f);
	glLineStipple(m_dashMultiplier, 0x5555);
	glEnable(GL_LINE_STIPPLE);
	glDrawArrays(GL_LINES, 0, m_primitiveCount * 4);
	
	// Axis lines
	
	glDisable(GL_LINE_STIPPLE);
	glLineWidth(m_axisWidth);
	
	glVertexPointer(3, GL_FLOAT, sizeof(OGLColorVertex), m_pOriginVertices);
	glColorPointer(4, GL_FLOAT, sizeof(OGLColorVertex), &m_pOriginVertices[0].color);
	
	// X
	if (m_axisXVisible)
		glDrawArrays(GL_LINES, 0, 2);
	
	// Y
	if (m_axisYVisible)
		glDrawArrays(GL_LINES, 2, 2);
	
	// Restore transforms
	
	glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
	glMatrixMode(GL_PROJECTION);
    glPopMatrix();
}


// getSnapPointForX:Y
// Returns snap point if X:Y near node of grid
//-----------------------------------------------------------------------------------------------

- (Vector2*)getSnapPointForX:(GLfloat)x Y:(GLfloat)y onDistance:(GLfloat)dist
{
	// Get min and max
	Vector2* min     = [Vector2 vectorWithX: (floorf(x / (float)m_hSpace) * m_hSpace) Y:(floorf(y / (float)m_vSpace) * m_vSpace)];
	Vector2* max     = [Vector2 vectorWithX: (ceilf(x / (float)m_hSpace) * m_hSpace) Y: (ceilf(y / m_vSpace) * m_vSpace)];
	Vector2* distMin = [Vector2 vectorWithX: (x - min->x) Y: (y - min->y)];
	Vector2* distMax = [Vector2 vectorWithX: (max->x - x) Y: (max->y - y)];
	
	if		( (distMin->x <= dist) && (distMin->y <= dist) )
		return [Vector2 vectorWithX:min->x Y:min->y];
	else if ( (distMin->x <= dist) && (distMax->y <= dist) )
		return [Vector2 vectorWithX:min->x Y:max->y];
	else if ( (distMax->x <= dist) && (distMax->y <= dist) )
		return [Vector2 vectorWithX:max->x Y:max->y];
	else if ( (distMax->x <= dist) && (distMin->y <= dist) )
		return [Vector2 vectorWithX:max->x Y:min->y];
	
	return nil;
}

// getSnapPointForX:Y
// Returns snap point if X:Y near node of grid
//-----------------------------------------------------------------------------------------------
- (Vector2*)getSnapPointForX:(GLfloat)x Y:(GLfloat)y GiveMax:(BOOL)max;
{
	float offsetX = m_vOrigin->x - floor(m_vOrigin->x / m_hSpace) * m_hSpace;
	float offsetY = m_vOrigin->y - floor(m_vOrigin->y / m_vSpace) * m_vSpace;
	
	return [Vector2 vectorWithX:floor(x / m_hSpace) * m_hSpace + offsetX Y:floor(y / m_vSpace) * m_vSpace + offsetY];
}

// getCellPoint
// Returns normalized offsets in grid cell
//-----------------------------------------------------------------------------------------------
- (Vector2*)getCellOffset:(GLfloat)x Y:(GLfloat)y
{
	return [Vector2 vectorWithX:x - floor(x / m_hSpace) * m_hSpace 
							  Y:y - floor(y / m_vSpace) * m_vSpace ];	
}

@end
