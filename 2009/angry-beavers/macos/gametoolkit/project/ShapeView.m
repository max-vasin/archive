//===============================================================================================
//  File:		ShapeView.m
//  Project:	OriginGameToolkit
//	Desc:		Shape OpenGL view subclass implementation
//
//  Created by Max Vasin on 12/21/10.
//  Copyright 2010 Origin. All rights reserved.
//===============================================================================================

// ************* IMPORTS *************

#import "ShapeView.h"

// ************* SHAPE VIEW PRIVATE METHODS *************	

@interface ShapeView (Private)

- (void)		clear;
- (void)		cleanResources;
- (void)		rebuildResources;
- (void)		renderShape;
- (NSPoint)		getEventLocation:(NSEvent*)theEvent;

@end

// ************* SHAPE VIEW IMPLEMENTATION *************

@implementation ShapeView

// ************* SHAPE VIEW SYNTESIZED PROPERTIES *************

@synthesize propCurrentItem			= m_itemCurrent;
@synthesize propBackColor			= m_backColor;
@synthesize propViewOffsetXMin		= m_offsetXMin;
@synthesize propViewOffsetXMax		= m_offsetXMax;
@synthesize propViewOffsetYMin		= m_offsetYMin;
@synthesize propViewOffsetYMax		= m_offsetYMax;
@synthesize propShapeColor			= m_shapeColor;
@synthesize propClosingLineColor	= m_shapeClosingColor;
@synthesize propVertexColor			= m_shapeVertexColor;
@synthesize propSelectLineColor		= m_shapeSelectLineColor;
@synthesize propSelectVertexColor	= m_shapeSelectVertexColor;
@synthesize propShapeWidth			= m_shapeWidth;
@synthesize propVertexSize			= m_shapeVertexSize;
@synthesize propSnapEnabled			= m_snapEnabled;

// ************* SHAPE VIEW IMPLEMENTED PROPERTIES *************

// propViewOffsetX - offset of view by X axis (used by sliders)
//-----------------------------------------------------------------------------------------------

- (GLfloat)propViewOffsetX
{
	return m_offsetX;
}

- (void)setPropViewOffsetX:(GLfloat)offset
{
	m_offsetX = offset;
	[self drawRect:[self frame]];
}

// propViewOffsetY - offset of view by Y axis (used by sliders)
//-----------------------------------------------------------------------------------------------

- (GLfloat)propViewOffsetY
{
	return m_offsetY;
}

- (void)setPropViewOffsetY:(GLfloat)offset
{
	m_offsetY = offset;
	[self drawRect:[self frame]];
}

// propIsEditState - used by boolean mode-switch buttons
//-----------------------------------------------------------------------------------------------

- (BOOL)propIsEditState
{
	return (m_state == eShapeViewStateEdit);
}

- (void)setPropIsEditState:(BOOL)enable
{
	if (!enable)
	{
		m_state			= eShapeViewStateNop;
		m_selLineMode	= NO;
		
		if ([m_itemCurrent propVerticesCount] < 3)
			[m_itemCurrent cleanVertices];
		
		m_lastVertex = nil;
	}
	else
	{
		self.propIsMoveState = NO;
		
		m_state = eShapeViewStateEdit;
		m_selLineMode = [m_itemCurrent propVerticesCount] > 2;
	}
	
	[self rebuildResources];
	[self drawRect:[self frame]];
}

// propIsMoveState - used by boolean mode-switch buttons
//-----------------------------------------------------------------------------------------------

- (BOOL)propIsMoveState
{
	return (m_state == eShapeViewStateMove);
}

- (void)setPropIsMoveState:(BOOL)enable
{
	if (!enable)
	{
		m_state = eShapeViewStateNop;
		return;
	}
	else 
	{
		self.propIsEditState = NO;
		
		m_state = eShapeViewStateMove;
	}
	
	[self rebuildResources];
	[self drawRect:[self frame]];
}

// ************* SHAPE VIEW INSTANCE METHODS *************

// awakeFromNib
// This view just finished loading from NIB - do initialization
//-----------------------------------------------------------------------------------------------

- (void)awakeFromNib
{
	m_contextReady	= NO;
	
	// Connect to application preferences container to reflect its changes immediately
	
	[m_appPrefs addObserver:self forKeyPath:@"propShapeViewGridDashMultiplier"	options:NSKeyValueObservingOptionNew context:nil];
	[m_appPrefs addObserver:self forKeyPath:@"propShapeViewGridHSpace"			options:NSKeyValueObservingOptionNew context:nil];
	[m_appPrefs addObserver:self forKeyPath:@"propShapeViewGridVSpace"			options:NSKeyValueObservingOptionNew context:nil];
	[m_appPrefs addObserver:self forKeyPath:@"propShapeViewGridBackColor"		options:NSKeyValueObservingOptionNew context:nil];
	[m_appPrefs addObserver:self forKeyPath:@"propShapeViewGridDashColor"		options:NSKeyValueObservingOptionNew context:nil];
	[m_appPrefs addObserver:self forKeyPath:@"propShapeViewGridOriginColor"		options:NSKeyValueObservingOptionNew context:nil];
	[m_appPrefs addObserver:self forKeyPath:@"propShapeViewShapeColor"			options:NSKeyValueObservingOptionNew context:nil];
	[m_appPrefs addObserver:self forKeyPath:@"propShapeviewClosingLineColor"	options:NSKeyValueObservingOptionNew context:nil];
	[m_appPrefs addObserver:self forKeyPath:@"propShapeViewVertexColor"			options:NSKeyValueObservingOptionNew context:nil];
	[m_appPrefs addObserver:self forKeyPath:@"propShapeViewSelectLineColor"		options:NSKeyValueObservingOptionNew context:nil];
	[m_appPrefs addObserver:self forKeyPath:@"propShapeViewSelectVertexColor"	options:NSKeyValueObservingOptionNew context:nil];
	[m_appPrefs addObserver:self forKeyPath:@"propShapeViewShapeWidth"			options:NSKeyValueObservingOptionNew context:nil];
	[m_appPrefs addObserver:self forKeyPath:@"propShapeViewVertexSize"			options:NSKeyValueObservingOptionNew context:nil];
	
	// Connect to tab view to watch items changing
	
	[m_viewsTab addObserver:self forKeyPath:@"propCurrentItem"					options:NSKeyValueObservingOptionNew context:nil];
}

// dealloc
// Destructor
//-----------------------------------------------------------------------------------------------

- (void)dealloc
{
	// Unregister all observing objects
	
	[m_appPrefs removeObserver:self forKeyPath:@"propShapeViewGridDashMultiplier"];
	[m_appPrefs removeObserver:self forKeyPath:@"propShapeViewGridHSpace"];
	[m_appPrefs removeObserver:self forKeyPath:@"propShapeViewGridVSpace"];		
	[m_appPrefs removeObserver:self forKeyPath:@"propShapeViewGridBackColor"];		
	[m_appPrefs removeObserver:self forKeyPath:@"propShapeViewGridDashColor"];		
	[m_appPrefs removeObserver:self forKeyPath:@"propShapeViewGridOriginColor"];	
	[m_appPrefs removeObserver:self forKeyPath:@"propShapeViewShapeColor"];	
	[m_appPrefs removeObserver:self forKeyPath:@"propShapeviewClosingLineColor"];	
	[m_appPrefs removeObserver:self forKeyPath:@"propShapeViewVertexColor"];	
	[m_appPrefs removeObserver:self forKeyPath:@"propShapeViewSelectLineColor"];	
	[m_appPrefs removeObserver:self forKeyPath:@"propShapeViewSelectVertexColor"];	
	[m_appPrefs removeObserver:self forKeyPath:@"propShapeViewShapeWidth"];	
	[m_appPrefs removeObserver:self forKeyPath:@"propShapeViewVertexSize"];	
	[m_viewsTab removeObserver:self forKeyPath:@"propCurrentItem"];
	
	// Cleanup any OpenGL resources
	
	[self cleanResources];
	
	// Destory any retained objects
	
	SAFE_RELEASE(m_backGrid);
	SAFE_RELEASE(m_backColor);
	SAFE_RELEASE(m_shapeColor);
	SAFE_RELEASE(m_shapeClosingColor);
	SAFE_RELEASE(m_shapeVertexColor);
	SAFE_RELEASE(m_shapeSelectLineColor);
	SAFE_RELEASE(m_shapeSelectVertexColor);
	SAFE_RELEASE(m_selLineShapeVertices);
	SAFE_RELEASE(m_trackArea);
	
	[super dealloc];
}

// Values observer
//-----------------------------------------------------------------------------------------------

- (void)observeValueForKeyPath:(NSString*)keyPath ofObject:(id)object change:(NSDictionary*)change context:(void*)context
{
	if		([keyPath isEqualToString:@"propShapeViewGridDashMultiplier"])
		m_backGrid.propDashMultiplier = [[change valueForKey:NSKeyValueChangeNewKey] intValue];
	else if ([keyPath isEqualToString:@"propShapeViewGridHSpace"])
		m_backGrid.propHSpace = [[change valueForKey:NSKeyValueChangeNewKey] intValue];
	else if ([keyPath isEqualToString:@"propShapeViewGridVSpace"])
		m_backGrid.propVSpace = [[change valueForKey:NSKeyValueChangeNewKey] intValue];
	else if ([keyPath isEqualToString:@"propShapeViewGridBackColor"])
		self.propBackColor = [change valueForKey:NSKeyValueChangeNewKey];
	else if ([keyPath isEqualToString:@"propShapeViewGridDashColor"])
		m_backGrid.propDashColor = [change valueForKey:NSKeyValueChangeNewKey];
	else if ([keyPath isEqualToString:@"propShapeViewGridOriginColor"])
		m_backGrid.propAxisColor = [change valueForKey:NSKeyValueChangeNewKey];			
	else if ([keyPath isEqualToString:@"propShapeViewShapeColor"])
		self.propShapeColor = [change valueForKey:NSKeyValueChangeNewKey];			
	else if ([keyPath isEqualToString:@"propShapeviewClosingLineColor"])
		self.propClosingLineColor = [change valueForKey:NSKeyValueChangeNewKey];			
	else if ([keyPath isEqualToString:@"propShapeViewVertexColor"])
		self.propVertexColor = [change valueForKey:NSKeyValueChangeNewKey];			
	else if ([keyPath isEqualToString:@"propShapeViewSelectLineColor"])
		self.propSelectLineColor = [change valueForKey:NSKeyValueChangeNewKey];			
	else if ([keyPath isEqualToString:@"propShapeViewSelectVertexColor"])
		self.propSelectVertexColor = [change valueForKey:NSKeyValueChangeNewKey];			
	else if ([keyPath isEqualToString:@"propShapeViewShapeWidth"])
		self.propShapeWidth = [[change valueForKey:NSKeyValueChangeNewKey] floatValue];			
	else if ([keyPath isEqualToString:@"propShapeViewVertexSize"])
		self.propVertexSize = [[change valueForKey:NSKeyValueChangeNewKey] floatValue];			
	else if ([keyPath isEqualToString:@"propCurrentItem"])
	{
		id newItem = [change valueForKey:NSKeyValueChangeNewKey];
		if ([newItem class] == [ProjectShapeItem class])
			self.propCurrentItem = newItem; 
		else
			self.propCurrentItem = nil;
		
		// Drop any references to previous item
		
		SAFE_RELEASE(m_selLineShapeVertices);
		
		self.propIsEditState	= NO;
		self.propIsMoveState	= NO;
	}	
	
	[self rebuildResources];
	[self drawRect:[self frame]];
}


// drawRect
// Called each time need to draw some portion of view - we redraw everything
//-----------------------------------------------------------------------------------------------

- (void)drawRect:(NSRect)dirtyRect
{
	[[self openGLContext] makeCurrentContext];
	
	if (!m_contextReady)
	{
		// Initialising OpenGL context
		
		m_backGrid		= [[GuidlineGrid alloc] initWithContext:[self openGLContext] andRect:[self visibleRect]];
		
		m_backGrid.propDashMultiplier	= m_appPrefs.propShapeViewGridDashMultiplier;
		m_backGrid.propHSpace			= m_appPrefs.propShapeViewGridHSpace;
		m_backGrid.propVSpace			= m_appPrefs.propShapeViewGridVSpace;
		m_backGrid.propDashColor		= m_appPrefs.propShapeViewGridDashColor;
		m_backGrid.propAxisColor		= m_appPrefs.propShapeViewGridOriginColor;
		
		self.propBackColor				= m_appPrefs.propShapeViewGridBackColor;
		self.propShapeColor				= m_appPrefs.propShapeViewShapeColor;
		self.propVertexColor			= m_appPrefs.propShapeViewVertexColor;
		self.propClosingLineColor		= m_appPrefs.propShapeviewClosingLineColor;
		self.propSelectLineColor		= m_appPrefs.propShapeViewSelectLineColor;
		self.propSelectVertexColor		= m_appPrefs.propShapeViewSelectVertexColor;
		self.propShapeWidth				= m_appPrefs.propShapeViewShapeWidth;
		self.propVertexSize				= m_appPrefs.propShapeViewVertexSize;
		
		m_contextReady	= YES;
	}
	
	// Do reshape anyway
	
	[self reshape];
	
	// Render objects
	
	[self clear];
	
	[m_backGrid render];
	
	[self renderShape];
	
	[[self openGLContext] flushBuffer];
}

// clear
// Clear OpenGL backbuffer
//-----------------------------------------------------------------------------------------------

- (void)clear
{
	glClearColor([m_backColor redComponent], 
				 [m_backColor greenComponent], 
				 [m_backColor blueComponent],
				 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

// reshape
// View size has been changed
//-----------------------------------------------------------------------------------------------

- (void)reshape
{
	NSRect frame = [self visibleRect];
	
	if ( (m_lastFrameRect.size.width != frame.size.width) || (m_lastFrameRect.size.height != frame.size.height) )
	{
		// Set new OpenGL viewport
		
		if (NSIsEmptyRect([self visibleRect]))
			glViewport(0, 0, 1, 1);
		else
			glViewport(0, 0, frame.size.width, frame.size.height);
	
		// Set tracking area to detect mouse move
		
		if (m_trackArea)
		{
			[self removeTrackingArea:m_trackArea];	
			SAFE_RELEASE(m_trackArea);
		}
		
		m_trackArea = [[NSTrackingArea alloc] initWithRect:frame 
												   options:(NSTrackingMouseEnteredAndExited | NSTrackingMouseMoved | NSTrackingActiveInKeyWindow)
													 owner:self userInfo:nil];
		
		[self addTrackingArea:m_trackArea];
		
		m_lastFrameRect = frame;
	}
	
	// Set new guid grid origin point
	
	Vector2* origin = [Vector2 vectorWithX:frame.size.width / 2.0f - m_offsetX Y:frame.size.height / 2.0f + m_offsetY ];
	
	m_backGrid.propOrigin	= origin;
	m_backGrid.propFrame	= frame;
	
	// Set new offsets bounds
	
	NSRect bounds = [m_itemCurrent propBoundRect];
	
	self.propViewOffsetXMax		= bounds.size.width;
	self.propViewOffsetXMin		= -bounds.size.width;
	self.propViewOffsetYMax		= bounds.size.height;
	self.propViewOffsetYMin		= -bounds.size.height;
}

// rebuildResources
// Rebuilds OpenGL related resources for this view 
//-----------------------------------------------------------------------------------------------

- (void)rebuildResources
{
	[self cleanResources];
	
	if (!m_itemCurrent)
		return;
	
	NSArray* shapeVerts = [m_itemCurrent propVertices];
	
	if (![shapeVerts count])
		return;
	
	m_shapeVertices = calloc([shapeVerts count], sizeof(OGLColorVertex));
	
	if (!m_shapeVertices)
		[NSException raise:@"Out of memory" format:@"ShapeView: cant alloc %i vertices buffer", [shapeVerts count]];
	
	m_shapePoints   = calloc([shapeVerts count], sizeof(OGLColorVertex));
	
	if (!m_shapePoints)
		[NSException raise:@"Out of memory" format:@"ShapeView: cant alloc %i vertices buffer", [shapeVerts count]];
	
	m_shapeVertexCount = [shapeVerts count];
	
	// Building shape vertices
	
	for (int i = 0; i < m_shapeVertexCount; i++)
	{
		ShapeVertex* vert = [shapeVerts objectAtIndex:i];
		
		m_shapeVertices[i].pos.x = vert->m_pos->x;
		m_shapeVertices[i].pos.y = vert->m_pos->y;
		m_shapeVertices[i].pos.z = 0.0f;
		
		[m_shapeColor getComponents:(CGFloat*)&m_shapeVertices[i].color];
		
		m_shapePoints[i].pos.x = vert->m_pos->x;
		m_shapePoints[i].pos.y = vert->m_pos->y;
		m_shapePoints[i].pos.z = 0.0f;
		
		if ((m_state == eShapeViewStateMove) && (m_lastVertex) && (vert == m_lastVertex))
			[m_shapeSelectVertexColor getComponents:(CGFloat*)&m_shapePoints[i].color];
		else
			[m_shapeVertexColor getComponents:(CGFloat*)&m_shapePoints[i].color];
		
	}
	
	// Build closing line
	
	if ((m_state == eShapeViewStateEdit) && !m_selLineMode && m_lastVertex)
	{
		ShapeVertex* nextVertex = [m_itemCurrent getNextVertex:m_lastVertex]; 
		
		m_closeVertices[0].pos.x	= m_lastVertex->m_pos->x;
		m_closeVertices[0].pos.y	= m_lastVertex->m_pos->y;
		m_closeVertices[0].pos.z	= 0.0f;
		[m_shapeClosingColor getComponents:(CGFloat*)&m_closeVertices[0].color];
	
		m_closeVertices[2].pos.x	= nextVertex->m_pos->x;
		m_closeVertices[2].pos.y	= nextVertex->m_pos->y;
		m_closeVertices[2].pos.z	= 0.0f;
		[m_shapeClosingColor getComponents:(CGFloat*)&m_closeVertices[2].color];
		
		[m_shapeClosingColor getComponents:(CGFloat*)&m_closeVertices[1].color];
	}
}

// cleanResources
// Cleans any OpenGL related resources for this view 
//-----------------------------------------------------------------------------------------------

- (void)cleanResources
{
	SAFE_FREE(m_shapeVertices);
	SAFE_FREE(m_shapePoints);
	m_shapeVertexCount = 0;
}

// renderShape
// Rendres current shape 
//-----------------------------------------------------------------------------------------------

- (void)renderShape
{
	if (!m_shapeVertices)
		return;
	
	NSRect frameRect = [self frame];
	
	// Set states
	
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
    glDisable(GL_DEPTH_TEST);

	// Set sources usage
	
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	
	// Reset current transformations
	
	glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
	glLoadIdentity();
	
	glMatrixMode(GL_PROJECTION);
    glPushMatrix();
	glLoadIdentity();
    glOrtho(-frameRect.size.width / 2.0f + m_offsetX,
			 frameRect.size.width / 2.0f + m_offsetX,
			-frameRect.size.height / 2.0f + m_offsetY,
			 frameRect.size.height / 2.0f + m_offsetY,
			-1000.0f,
			 1000.0f);
	
	
	glLineWidth(m_shapeWidth);

	// Closing lines
	
	if (m_state == eShapeViewStateEdit && !m_selLineMode)
	{
		glEnable(GL_LINE_STIPPLE);
		glLineStipple(3, 0x1111);
	
		glVertexPointer(3, GL_FLOAT, sizeof(OGLColorVertex), m_closeVertices);
		glColorPointer(4, GL_FLOAT, sizeof(OGLColorVertex), &m_closeVertices[0].color);
		glDrawArrays(GL_LINE_STRIP, 0, 3);
	}
	
	
	// Shape lines
	
	if (m_shapeVertexCount > 1)
	{
		glDisable(GL_LINE_STIPPLE);

		glVertexPointer(3, GL_FLOAT, sizeof(OGLColorVertex), m_shapeVertices);
		glColorPointer(4, GL_FLOAT, sizeof(OGLColorVertex), &m_shapeVertices[0].color);
		glDrawArrays(GL_LINE_LOOP, 0, m_shapeVertexCount);
	}
	
	// Shape points
	
	glPointSize(m_shapeVertexSize);
	glVertexPointer(3, GL_FLOAT, sizeof(OGLColorVertex), m_shapePoints);
	glColorPointer(4, GL_FLOAT, sizeof(OGLColorVertex), &m_shapePoints[0].color);
	glDrawArrays(GL_POINTS, 0, m_shapeVertexCount);
	
	// Current line selection
	
	if (m_selLineExist)
	{
		glDisable(GL_LINE_STIPPLE);
		glVertexPointer(3, GL_FLOAT, sizeof(OGLColorVertex), m_selLineVertices);
		glColorPointer(4, GL_FLOAT, sizeof(OGLColorVertex), &m_selLineVertices[0].color);
		glDrawArrays(GL_LINES, 0, 2);
	}
	
	// Snap point
	
	if (m_snapped)
	{
		glDisable(GL_LINE_STIPPLE);
		glPointSize(m_shapeVertexSize);
		glVertexPointer(3, GL_FLOAT, sizeof(OGLColorVertex), &m_snapPoint);
		glColorPointer(4, GL_FLOAT, sizeof(OGLColorVertex), &m_snapPoint.color);
		glDrawArrays(GL_POINTS, 0, 1);
	}
	
	// Restore transforms
	
	glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
	glMatrixMode(GL_PROJECTION);
    glPopMatrix();
}

// getEventLocation
// Returns mouse event location transformed to view coordinates (and snapped) 
//-----------------------------------------------------------------------------------------------

- (NSPoint)getEventLocation:(NSEvent*)theEvent
{
	// Get window-relative location
	
	NSPoint winLoc = [theEvent locationInWindow];
	
	// Transform into view
	
	winLoc = [self convertPoint:winLoc fromView:nil];
	
	// Find view origin
	
	NSRect rect = [self frame];
	
	winLoc.x -= rect.size.width / 2.0f - m_offsetX;
	winLoc.y -= rect.size.height / 2.0f - m_offsetY;
	
	// Snapping
	
	if (m_snapEnabled)
	{
		Vector2* snap = [m_backGrid getSnapPointForX:winLoc.x Y:winLoc.y onDistance:5];
		
		m_snapped = (snap != nil);
		
		if (m_snapped)
		{
			m_snapPoint.pos.x = snap->x;
			m_snapPoint.pos.y = snap->y;
			m_snapPoint.pos.z = 0.0f;
			[m_shapeVertexColor getComponents:(CGFloat*)&m_snapPoint.color];
			
			winLoc.x = snap->x;
			winLoc.y = snap->y;
		}
	}
	
	return winLoc;
}

// ************* ACTIONS IMPLEMENTATION *************

// resetOffsets
// Resets current offsets
//-----------------------------------------------------------------------------------------------

- (IBAction) resetOffsets:(id)sender
{
	self.propViewOffsetX	= 0.0f;
	self.propViewOffsetY	= 0.0f;
	
	[self drawRect:[self frame]];
}

// ************* MOUSE EVENT HANDLERS IMPLEMENTATION *************

// mouseDown
// Left mouse button pressed
//-----------------------------------------------------------------------------------------------

- (void)mouseDown:(NSEvent*)theEvent
{
	NSPoint loc = [self getEventLocation:theEvent];
	
	switch (m_state)
	{
		case eShapeViewStateMove:
			m_lastVertex = [m_itemCurrent getVertexAroundX:loc.x Y:loc.y onSquare:m_shapeVertexSize];
			break;
	}
	
	[self drawRect:[self frame]];
}

// mouseUp
// Left mouse button released
//-----------------------------------------------------------------------------------------------

- (void)mouseUp:(NSEvent *)theEvent
{
	NSPoint loc = [self getEventLocation:theEvent];
	
	switch (m_state)
	{
		case eShapeViewStateEdit:
			
			// If we are in line select submode - do line selection
			
			if (m_selLineMode)
			{
				if (m_selLineExist)
				{
					m_lastVertex = [m_selLineShapeVertices objectAtIndex:0];
					m_selLineMode = NO;
					m_selLineExist = NO;
			
					[self rebuildResources];
				}
			}
			
			// Else we need to add new vertex to shape
			
			else
			{
				m_lastVertex = [m_itemCurrent addVertexAtX:loc.x Y:loc.y afterVertex:m_lastVertex];
				
				[self rebuildResources];
			}
			break;
			
		case eShapeViewStateMove:
			break;
	}
	
	[self drawRect:[self frame]];
}

// rightMouseUp
// Right mouse button released
//-----------------------------------------------------------------------------------------------

- (void)rightMouseUp:(NSEvent*)theEvent
{
	switch (m_state)
	{
		case eShapeViewStateEdit:
			if (!m_selLineMode)
				m_selLineMode = YES;
			break;
			
		case eShapeViewStateMove:
			if (m_lastVertex)
			{
				[m_itemCurrent deleteVertex:m_lastVertex];
				[self rebuildResources];
			}
			break;
	}
	
	[self drawRect:[self frame]];
}

// mouseDragged
// Left button pressed and mouse moves
//-----------------------------------------------------------------------------------------------

- (void)mouseDragged:(NSEvent *)theEvent
{
	NSPoint loc = [self getEventLocation:theEvent];
	
	switch (m_state)
	{
		case eShapeViewStateMove:
			if (m_lastVertex)
			{
				m_lastVertex->m_pos->x = loc.x;
				m_lastVertex->m_pos->y = loc.y;
				
				[self rebuildResources];
			}
			break;
	}
	
	[self drawRect:[self frame]];
}


// mouseMoved
// Mouse moves over view frame
//-----------------------------------------------------------------------------------------------

- (void)mouseMoved:(NSEvent *)theEvent 
{
	NSPoint loc = [self getEventLocation:theEvent];
	
	
	switch (m_state)
	{
		case eShapeViewStateEdit:
			if (m_selLineMode)
			{
				// Hittest shape lines
				
				SAFE_RELEASE(m_selLineShapeVertices);
				
				m_selLineShapeVertices = [[m_itemCurrent getLineAroundX:loc.x Y:loc.y onDistance:10] retain];
				
				if (m_selLineShapeVertices)
				{
					ShapeVertex* v = [m_selLineShapeVertices objectAtIndex:0];
					
					m_selLineVertices[0].pos.x = v->m_pos->x;
					m_selLineVertices[0].pos.y = v->m_pos->y;
					m_selLineVertices[0].pos.z = 0.0f;
					[m_shapeSelectLineColor getComponents:(CGFloat*)&m_selLineVertices[0].color];
					
					v = [m_selLineShapeVertices objectAtIndex:1];
					
					m_selLineVertices[1].pos.x = v->m_pos->x;
					m_selLineVertices[1].pos.y = v->m_pos->y;
					m_selLineVertices[1].pos.z = 0.0f;
					[m_shapeSelectLineColor getComponents:(CGFloat*)&m_selLineVertices[1].color];
					
					m_selLineExist = YES;
					
					// Also tracks closing line location
					
					m_closeVertices[1].pos.x = loc.x;
					m_closeVertices[1].pos.y = loc.y;
					m_closeVertices[1].pos.z = 0.0f;					
				}
				else
					m_selLineExist = NO;
			}
			else 
			{
				// Tracks closing line
				
				m_closeVertices[1].pos.x = loc.x;
				m_closeVertices[1].pos.y = loc.y;
				m_closeVertices[1].pos.z = 0.0f;
			}
			break;

		case eShapeViewStateMove:
			// If moving state - just remember vertex under mouse (if any)
			
			m_lastVertex = [m_itemCurrent getVertexAroundX:loc.x Y:loc.y onSquare:m_shapeVertexSize];
			[self rebuildResources];
			break;
	}
	
	[self drawRect:[self frame]];
}

// mouseExited
// Mouse exited frame tracking area
//-----------------------------------------------------------------------------------------------

- (void)mouseExited:(NSEvent*)theEvent 
{
	// Drop mouse to first vertex of closing line
	
	memcpy(&m_closeVertices[1], &m_closeVertices[0], sizeof(OGLColorVertex));
	
	[self drawRect:[self frame]];
}


@end
