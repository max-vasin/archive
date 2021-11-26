//===============================================================================================
//  File:		SpriteTextureView.m
//  Project:	OriginGameToolkit
//	Desc:		Sprite view/texture subview implementation
//
//  Created by Max Vasin on 3/12/11.
//  Copyright 2011 Origin Games. All rights reserved.
//===============================================================================================

// ************* IMPORTS *************

#import "SpriteTextureView.h"
#import "AppDelegate.h"
#import <GLUT/GLUT.h>

// ************* PRIVATE METHODS *************

@interface SpriteTextureView (Private)

- (void)	clear;
- (NSPoint)	getEventLocation:		(NSEvent*)theEvent;
- (void)	moveObserver:			(id)oldObject	toObject:(id)newObject	forKey:(NSString*)key;
- (void)	observeProject:			(Project*)project;
- (void)	observeSpriteItem:		(ProjectSpriteItem*)sprite;
- (void)	observeAnimation:		(SpriteAnimation*)animation;
- (void)	observeAnimationFrame:	(AnimationFrame*)frame;
- (void)	updateTexture:			(GLuint*)pTexID  toSize:(IntSize*)pSize withPath:(NSString*)path;
- (void)	updateBaseTexture;
- (void)	updateTextureVertices;
- (void)	updateFrameVertices;
- (void)	lockFrameOffsets;
- (void)	rebuildResources;
- (void)	cleanResources;
- (void)	renderTexture;

@end

// ************* IMPLEMENTATION *************

@implementation SpriteTextureView

//############################################
// PROPERTIES
//############################################

@synthesize propViewOffsetXMin		= m_offsetXMin;
@synthesize propViewOffsetXMax		= m_offsetXMax;
@synthesize propViewOffsetYMin		= m_offsetYMin;
@synthesize propViewOffsetYMax		= m_offsetYMax;
@synthesize propViewZoomMin			= m_zoomMin;
@synthesize propViewZoomMax			= m_zoomMax;
@synthesize propSnapFrames			= m_snapFrames;

// propLockFrameOffsetX: lock offsets to active frame
//-----------------------------------------------------------------------------------------------
- (BOOL)propLockFrameOffsetX
{
	return m_lockFrameOffsetX;
}

- (void)setPropLockFrameOffsetX:(BOOL)lock
{
	m_lockFrameOffsetX = lock;
	[self lockFrameOffsets];
}

// propLockFrameOffsetY: lock offsets to active frame
//-----------------------------------------------------------------------------------------------
- (BOOL)propLockFrameOffsetY
{
	return m_lockFrameOffsetY;
}

- (void)setPropLockFrameOffsetY:(BOOL)lock
{
	m_lockFrameOffsetY = lock;
	[self lockFrameOffsets];
}

// propViewOffsetX: controlling view X scroll
//-----------------------------------------------------------------------------------------------

- (GLfloat)propViewOffsetX
{
	return m_offsetX;
}

- (void)setPropViewOffsetX:(GLfloat)offset
{
	Vector2* gridOrigin = m_backGrid.propOrigin;
	gridOrigin->x = -(GLfloat)offset + (GLfloat)m_gridOrigin.x;
	m_backGrid.propOrigin = gridOrigin;
	
	Vector2* zoomOrigin = m_backGrid.propZoomOrigin;
	zoomOrigin->x = -offset;
	m_backGrid.propZoomOrigin = zoomOrigin;
	
	m_offsetX = offset;
	
	[self drawRect:[self frame]];
}

// propViewOffsetY: controlling view Y scroll
//-----------------------------------------------------------------------------------------------

- (GLfloat)propViewOffsetY
{
	return m_offsetY;
}

- (void)setPropViewOffsetY:(GLfloat)offset
{
	Vector2* gridOrigin = m_backGrid.propOrigin;
	gridOrigin->y = (GLfloat)offset + (GLfloat)m_gridOrigin.y;
	m_backGrid.propOrigin = gridOrigin;
	
	Vector2* zoomOrigin = m_backGrid.propZoomOrigin;
	zoomOrigin->y = offset;
	m_backGrid.propZoomOrigin = zoomOrigin;
	
	m_offsetY = offset;
	
	[self drawRect:[self frame]];
}

// propViewZoom: controlling view zooming
//-----------------------------------------------------------------------------------------------

- (GLfloat)propViewZoom
{
	return m_zoom;
}

- (void)setPropViewZoom:(GLfloat)zoom
{
	m_zoom = zoom;
	m_backGrid.propZoom = zoom;
	[self drawRect:[self frame]];
}

// ************* INSTANCE METHODS *************


//############################################
// CONSTRUCTION\DESTRUCTION
//############################################

// awakeFromNib: this view just finished loading from NIB - do initialization
//-----------------------------------------------------------------------------------------------
- (void)awakeFromNib
{
	m_contextReady	= NO;
	
	m_zoom			= 1.0f;
	m_zoomMin		= 1.0f;
	m_zoomMax		= 8.0f;
	
	[m_viewController setContent:self];
	
	// Connect to data
	
	AppDelegate* appDelegate = (AppDelegate*)[NSApp delegate];
	[appDelegate addObserver:self forKeyPath:@"propCurrentProject"	options:NSKeyValueObservingOptionNew context:nil];
	[self observeProject:appDelegate.propCurrentProject];
	
	// Connect to application preferences
	
	[m_appPrefs addObserver:self forKeyPath:@"propSpriteViewGridDashMultiplier"	options:NSKeyValueObservingOptionNew context:nil];
	[m_appPrefs addObserver:self forKeyPath:@"propSpriteViewGridBackColor"		options:NSKeyValueObservingOptionNew context:nil];
	[m_appPrefs addObserver:self forKeyPath:@"propSpriteViewGridDashColor"		options:NSKeyValueObservingOptionNew context:nil];
	[m_appPrefs addObserver:self forKeyPath:@"propSpriteViewGridOriginColor"	options:NSKeyValueObservingOptionNew context:nil];
	[m_appPrefs addObserver:self forKeyPath:@"propSpriteViewFrameColor"			options:NSKeyValueObservingOptionNew context:nil];
	[m_appPrefs addObserver:self forKeyPath:@"propSpriteViewFrameSelectColor"	options:NSKeyValueObservingOptionNew context:nil];
	[m_appPrefs addObserver:self forKeyPath:@"propSpriteViewFrameAlpha"			options:NSKeyValueObservingOptionNew context:nil];
	[m_appPrefs addObserver:self forKeyPath:@"propSpriteViewFrameSelectAlpha"	options:NSKeyValueObservingOptionNew context:nil];
	[m_appPrefs addObserver:self forKeyPath:@"propSpriteViewAxisWidth"			options:NSKeyValueObservingOptionNew context:nil];
}

// dealloc: destructor
//-----------------------------------------------------------------------------------------------
- (void)dealloc
{
	// remove all observing objects
	
	AppDelegate* appDelegate = (AppDelegate*)[NSApp delegate];
	
	[appDelegate removeObserver:self forKeyPath:@"propCurrentProject"];
	
	[self observeProject:nil];
	
	// delete grid
	
	SAFE_RELEASE(m_backGrid);
	
	// clean resources
	
	[self cleanResources];

	[super dealloc];
}

//############################################
// OBJECTS OBSERVATION CONTROL 
//############################################

// moveObserver: safely removes self-observer for old object and assigns to new
//-----------------------------------------------------------------------------------------------
- (void)moveObserver:(id)oldObject toObject:(id)newObject forKey:(NSString*)key
{
	if (oldObject)
		[oldObject removeObserver:self forKeyPath:key];
	
	if (newObject) 
		[newObject addObserver:self forKeyPath:key options:NSKeyValueObservingOptionNew context:nil];
}

// observeProject: start watching for project change
//-----------------------------------------------------------------------------------------------
- (void)observeProject: (Project*)project
{
	if (m_obsrvProject != project)
	{
		[self moveObserver:m_obsrvProject toObject:project forKey:@"propCurrentSpriteItem"];
		
		m_obsrvProject = project;
		
		if (m_obsrvProject)
			[self observeSpriteItem:m_obsrvProject.propCurrentSpriteItem];
		else
			[self observeSpriteItem:nil];
	}
}

// observeSpriteItem: start watching for current sprite item change
//-----------------------------------------------------------------------------------------------
- (void)observeSpriteItem:(ProjectSpriteItem*)sprite
{
	if (m_obsrvSprite != sprite)
	{
		[self moveObserver:m_obsrvSprite toObject:sprite forKey:@"propCurrentAnim"];
		[self moveObserver:m_obsrvSprite toObject:sprite forKey:@"propTexture"];
		
		m_obsrvSprite = sprite;
		
		if (m_obsrvSprite)
			[self observeAnimation:m_obsrvSprite.propCurrentAnim];
		else
			[self observeAnimation:nil];
	}
}

// observeSpriteItem: start watching for current sprite animation change
//-----------------------------------------------------------------------------------------------
- (void)observeAnimation:(SpriteAnimation*)animation
{
	if (m_obsrvAnimation != animation)
	{
		[self moveObserver:m_obsrvAnimation toObject:animation forKey:@"propCurrentFrame"];
		[self moveObserver:m_obsrvAnimation toObject:animation forKey:@"propGridSpaceH"];
		[self moveObserver:m_obsrvAnimation toObject:animation forKey:@"propGridSpaceV"];
		[self moveObserver:m_obsrvAnimation toObject:animation forKey:@"propGridOriginX"];
		[self moveObserver:m_obsrvAnimation toObject:animation forKey:@"propGridOriginY"];
		
		m_obsrvAnimation = animation;
		
		m_backGrid.propHSpace	= m_obsrvAnimation.propGridSpaceH;
		m_backGrid.propVSpace	= m_obsrvAnimation.propGridSpaceV;
		m_gridOrigin.x			= m_obsrvAnimation.propGridOriginX;
		m_gridOrigin.y			= m_obsrvAnimation.propGridOriginY;
		
		if (m_obsrvAnimation)
			[self observeAnimationFrame:m_obsrvAnimation.propCurrentFrame];
		else
			[self observeAnimationFrame:nil];
		
		[self updateBaseTexture];
		[self updateTextureVertices];
		[self updateFrameVertices];
	}
}

// observeAnimationFrame: start watching for current animation frame change
//-----------------------------------------------------------------------------------------------
- (void)observeAnimationFrame:(AnimationFrame*)frame
{
	if (m_obsrvFrame != frame)
	{
		[self moveObserver:m_obsrvFrame toObject:frame forKey:@"propPositionX"];
		[self moveObserver:m_obsrvFrame toObject:frame forKey:@"propPositionY"];
		[self moveObserver:m_obsrvFrame toObject:frame forKey:@"propWidth"];
		[self moveObserver:m_obsrvFrame toObject:frame forKey:@"propHeight"];
		
		m_obsrvFrame = frame;
		
		[self updateFrameVertices];
		[self lockFrameOffsets];
	}
}

// Values observer
//-----------------------------------------------------------------------------------------------
- (void)observeValueForKeyPath:(NSString*)keyPath ofObject:(id)object change:(NSDictionary*)change context:(void*)context
{
	id newValue = [change valueForKey:NSKeyValueChangeNewKey];
	
	if (newValue == [NSNull null])
		newValue = nil;
	
	// Observing objects change
	
	if		([keyPath isEqualToString:@"propCurrentProject"])
		[self observeProject:newValue];
	
	else if	([keyPath isEqualToString:@"propCurrentSpriteItem"])
		[self observeSpriteItem:newValue];
	
	else if	([keyPath isEqualToString:@"propCurrentAnim"])
		[self observeAnimation:newValue];
	
	else if	([keyPath isEqualToString:@"propCurrentFrame"])
		[self observeAnimationFrame:newValue];
	
	// Observing animation changings
	
	if		([keyPath isEqualToString:@"propTexture"])
		[self updateBaseTexture];
		
	// Preferences changed
	
	if		([keyPath isEqualToString:@"propSpriteViewGridDashMultiplier"])
		m_backGrid.propDashMultiplier = [newValue intValue];
	
	else if ([keyPath isEqualToString:@"propSpriteViewGridBackColor"])
		m_backColor = newValue;
	
	else if ([keyPath isEqualToString:@"propSpriteViewGridDashColor"])
		m_backGrid.propDashColor = newValue;
	
	else if ([keyPath isEqualToString:@"propSpriteViewGridOriginColor"])
		m_backGrid.propAxisColor = newValue;	
	
	else if ([keyPath isEqualToString:@"propGridSpaceH"])
		m_backGrid.propHSpace = [newValue intValue];	
	
	else if ([keyPath isEqualToString:@"propGridSpaceV"])
		m_backGrid.propVSpace = [newValue intValue];	

	else if ([keyPath isEqualToString:@"propGridOriginX"])
	{
		m_gridOrigin.x = [newValue intValue];
		self.propViewOffsetX = self.propViewOffsetX;
	}
	
	else if ([keyPath isEqualToString:@"propGridOriginY"])
	{
		m_gridOrigin.y = [newValue intValue];
		self.propViewOffsetY = self.propViewOffsetY;
	}

	else if ([keyPath isEqualToString:@"propSpriteViewAxisWidth"])
		m_backGrid.propAxisWidth = [newValue floatValue];	

	else if ([keyPath isEqualToString:@"propSpriteViewFrameColor"])
		m_frameColor = [newValue copy];	
	
	else if ([keyPath isEqualToString:@"propSpriteViewFrameSelectColor"])
		m_frameSelectColor = [newValue copy];	
	
	else if ([keyPath isEqualToString:@"propSpriteViewFrameAlpha"])
		m_frameAlpha = [newValue floatValue];	
	
	else if ([keyPath isEqualToString:@"propSpriteViewFrameSelectAlpha"])
		m_frameSelectAlpha = [newValue floatValue];	
	
	[self updateFrameVertices];
	
	[self drawRect:[self frame]];
}


//############################################
// VIEW DRAWING CONTROL
//############################################

// reshape: view size has been changed
//-----------------------------------------------------------------------------------------------
- (void)reshape
{
	[[self openGLContext] makeCurrentContext];
	
	NSRect frame = [self visibleRect];
	
	if (NSIsEmptyRect(frame))
	{
		glViewport(0, 0, 1, 1);
	}
	else
	{
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
	}
	
	m_backGrid.propFrame = frame;
	
	[self updateTextureVertices];
	[self updateFrameVertices];
}

// drawRect: called each time need to draw some portion of view - we redraw everything
//-----------------------------------------------------------------------------------------------
- (void)drawRect:(NSRect)dirtyRect
{
	[[self openGLContext] makeCurrentContext];
	
	// Our context not initialized
	if (!m_contextReady)
	{
		m_contextReady	= YES;
		
		m_backGrid		= [[GuidlineGrid alloc] initWithContext:[self openGLContext] andRect:[self visibleRect]];
		
		m_backGrid.propDashMultiplier	= m_appPrefs.propSpriteViewGridDashMultiplier;
		m_backGrid.propDashColor		= m_appPrefs.propSpriteViewGridDashColor;
		m_backGrid.propAxisColor		= m_appPrefs.propSpriteViewGridOriginColor;
		m_backGrid.propAxisWidth		= m_appPrefs.propSpriteViewAxisWidth;
		
		m_backColor						= [m_appPrefs.propSpriteViewGridBackColor copy];
		m_frameColor					= [m_appPrefs.propSpriteViewFrameColor copy];
		m_frameSelectColor				= [m_appPrefs.propSpriteViewFrameSelectColor copy];
		m_frameAlpha					= m_appPrefs.propSpriteViewFrameAlpha;
		m_frameSelectAlpha				= m_appPrefs.propSpriteViewFrameSelectAlpha;
		
		[self rebuildResources];
	}
	
	[self clear];
	
	[m_backGrid render];

	[self renderTexture];
	
	[[self openGLContext] flushBuffer];
}


//############################################
// RENDERING CONTROL
//############################################

// clear: clear OpenGL backbuffer
//-----------------------------------------------------------------------------------------------
- (void)clear
{
	glClearColor([m_backColor redComponent], [m_backColor greenComponent], [m_backColor blueComponent], 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

// renderTexture: renders current texture
//-----------------------------------------------------------------------------------------------
- (void)renderTexture
{
	if (!m_oglTexture)
		return;
	
	NSRect rect = [self visibleRect];
	
	glDisable(GL_DEPTH_TEST);
	
	glMatrixMode(GL_PROJECTION);
    glPushMatrix();
	glLoadIdentity();
    glOrtho(0.0f,
			rect.size.width,
		    0.0f,
			rect.size.height,
			-1000.0f, 1000.0f);
	
	glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
	glLoadIdentity();
	
	// Offset transform
	glTranslatef(-floor(m_offsetX), rect.size.height - floor(m_offsetY), 0.0f);
	
	// Zoom transform
	glScalef(m_zoom, m_zoom, 1.0f);
	
	// Render base texture sprite
	
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
	
	glVertexPointer(3, GL_FLOAT, sizeof(OGLVertex), &m_vertices);
	glColorPointer(4, GL_FLOAT, sizeof(OGLVertex), &m_vertices[0].color);
	glTexCoordPointer(2, GL_FLOAT, sizeof(OGLVertex), &m_vertices[0].uv);
	
	glEnable( GL_TEXTURE_2D );
    glBindTexture( GL_TEXTURE_2D, m_oglTexture );
	
	glEnable( GL_BLEND );
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
	
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	
	// Render current frame 
	
	if (!m_bNoFrame)
	{
		glDisable( GL_TEXTURE_2D );
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		glDisableClientState(GL_NORMAL_ARRAY);
		
		glVertexPointer(3, GL_FLOAT, sizeof(OGLColorVertex), &m_frameVertices[0]);
		glColorPointer(4, GL_FLOAT, sizeof(OGLColorVertex), &m_frameVertices[0].color);
		
		glLineWidth(1.0f);
		
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	}
	
	glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
	glMatrixMode(GL_PROJECTION);
    glPopMatrix();	
}


//############################################
// RENDER RESOURCES CONTROL
//############################################

// rebuildResources: rebuilds all resources with current texture item
//-----------------------------------------------------------------------------------------------
- (void)rebuildResources
{
	[[self openGLContext] makeCurrentContext];
	
	[self cleanResources];
	
	[self updateBaseTexture];
	
	[self updateTextureVertices];
	
	[self updateFrameVertices];
}

// updateTextureVertices: updates base texture sprite shape vertices
//-----------------------------------------------------------------------------------------------
- (void)updateTextureVertices
{
	m_vertices[0].pos.x		= 0.0f;
	m_vertices[0].pos.y		= 0.0f;
	m_vertices[0].pos.z		= 0.0f;
	m_vertices[0].color.r	= m_vertices[0].color.g = m_vertices[0].color.b = 1.0f; m_vertices[0].color.a = 1.0f;
	m_vertices[0].uv.x		= 0.0f;
	m_vertices[0].uv.y		= 0.0f;
	
	m_vertices[1].pos.x		= m_texSize.width;
	m_vertices[1].pos.y		= 0.0f;
	m_vertices[1].pos.z		= 0.0f;
	m_vertices[1].color.r	= m_vertices[1].color.g = m_vertices[1].color.b = m_vertices[1].color.a = 1.0f;
	m_vertices[1].uv.x		= 1.0f;
	m_vertices[1].uv.y		= 0.0f;
	
	m_vertices[2].pos.x		= 0.0f;
	m_vertices[2].pos.y		= -m_texSize.height;
	m_vertices[2].pos.z		= 0.0f;
	m_vertices[2].color.r	= m_vertices[2].color.g = m_vertices[2].color.b = m_vertices[2].color.a = 1.0f;
	m_vertices[2].uv.x		= 0.0f;
	m_vertices[2].uv.y		= 1.0f;
	
	m_vertices[3].pos.x		= m_texSize.width;
	m_vertices[3].pos.y		= -m_texSize.height;
	m_vertices[3].pos.z		= 0.0f;
	m_vertices[3].color.r	= m_vertices[3].color.g = m_vertices[3].color.b = m_vertices[3].color.a = 1.0f;
	m_vertices[3].uv.x		= 1.0f;
	m_vertices[3].uv.y		= 1.0f;

}

// updateFrameVertices: updates frame vertices
//-----------------------------------------------------------------------------------------------
- (void)updateFrameVertices
{
	m_bNoFrame = YES;
	
	if (!m_obsrvFrame)
		return;
	
	if (!m_obsrvFrame.propWidth || !m_obsrvFrame.propHeight)
		return;
	
	m_bNoFrame = NO;
	
	m_frameVertices[0].pos.x	= m_obsrvFrame.propPositionX;
	m_frameVertices[0].pos.y	= -m_obsrvFrame.propPositionY;
	m_frameVertices[0].pos.z	= 0.0f;
	
	m_frameVertices[1].pos.x	= m_obsrvFrame.propPositionX + m_obsrvFrame.propWidth;
	m_frameVertices[1].pos.y	= -m_obsrvFrame.propPositionY;
	m_frameVertices[1].pos.z	= 0.0f;
	
	m_frameVertices[2].pos.x	= m_obsrvFrame.propPositionX;
	m_frameVertices[2].pos.y	= -m_obsrvFrame.propPositionY - m_obsrvFrame.propHeight;
	m_frameVertices[2].pos.z	= 0.0f;
	
	m_frameVertices[3].pos.x	= m_obsrvFrame.propPositionX + m_obsrvFrame.propWidth;
	m_frameVertices[3].pos.y	= -m_obsrvFrame.propPositionY - m_obsrvFrame.propHeight;
	m_frameVertices[3].pos.z	= 0.0f;
	
	if (m_frameSelected)
	{
		[m_frameSelectColor getComponents:(CGFloat*)&m_frameVertices[0].color];
		[m_frameSelectColor getComponents:(CGFloat*)&m_frameVertices[1].color];
		[m_frameSelectColor getComponents:(CGFloat*)&m_frameVertices[2].color];
		[m_frameSelectColor getComponents:(CGFloat*)&m_frameVertices[3].color];
		m_frameVertices[0].color.a = 
		m_frameVertices[1].color.a =
		m_frameVertices[2].color.a =
		m_frameVertices[3].color.a = m_frameSelectAlpha;
	}
	else 
	{
		[m_frameColor getComponents:(CGFloat*)&m_frameVertices[0].color];
		[m_frameColor getComponents:(CGFloat*)&m_frameVertices[1].color];
		[m_frameColor getComponents:(CGFloat*)&m_frameVertices[2].color];
		[m_frameColor getComponents:(CGFloat*)&m_frameVertices[3].color];
		m_frameVertices[0].color.a = 
		m_frameVertices[1].color.a =
		m_frameVertices[2].color.a =
		m_frameVertices[3].color.a = m_frameAlpha;
	}
}

// cleanResources: cleans all OpenGL stuff
//-----------------------------------------------------------------------------------------------
- (void)cleanResources
{
	[[self openGLContext] makeCurrentContext];
	
	glDeleteTextures(1, &m_oglTexture);
	m_oglTexture = 0;
}

// updateBaseTexture: updating sprite base texture
//-----------------------------------------------------------------------------------------------
- (void)updateBaseTexture
{
	[[self openGLContext] makeCurrentContext];
	
	glDeleteTextures(1, &m_oglTexture);
	m_oglTexture = 0;
	
	if (!m_obsrvSprite)
		return;
	
	ProjectTextureItem* itemTexture = m_obsrvSprite.propTexture;
	
	if (!itemTexture)
		return;
	
	NSString* texPath = [itemTexture propBuildPath];
	
	[self updateTexture:&m_oglTexture toSize:&m_texSize withPath:texPath];
	
	self.propViewOffsetX		= 0.0f;
	self.propViewOffsetY		= 0.0f;
	self.propViewOffsetXMin		= 0.0f;
	self.propViewOffsetXMax		= m_texSize.width * 2;
	self.propViewOffsetYMin		= -m_texSize.height * 2;
	self.propViewOffsetYMax		= 0.0f;
	
	[self lockFrameOffsets];
}

// updateTexture: updating texture by path
//-----------------------------------------------------------------------------------------------
- (void)updateTexture:(GLuint*)pTexID  toSize:(IntSize*)pSize withPath:(NSString*)path
{
	[[self openGLContext] makeCurrentContext];
	
	glDeleteTextures(1, pTexID);
	*pTexID = 0;
	
	NSData* fileContents = [NSData dataWithContentsOfFile:path];
	
	if (!fileContents)
		return;
	
	NSBitmapImageRep* img = [[[NSBitmapImageRep alloc] initWithDataAndConvert:fileContents] autorelease];
	if(!img)
		return;
	
	glGenTextures(1, pTexID);
	
	[img uploadToTexture:*pTexID];
	
	*pSize = [img intSize];
}


//############################################
// ACTIONS
//############################################

- (IBAction) resetOffsets:(id)sender
{
	self.propViewOffsetX	= 0.0f;
	self.propViewOffsetY	= 0.0f;
	
	[self drawRect:[self frame]];
}

//############################################
// MOUSE INPUT CONTROL
//############################################

// getEventLocation: returns mouse event location transformed to view coordinates (and snapped) 
//-----------------------------------------------------------------------------------------------
- (NSPoint)getEventLocation:(NSEvent*)theEvent
{
	// Get window-relative location
	
	NSPoint winLoc = [theEvent locationInWindow];
	
	// Transform into view
	
	winLoc = [self convertPoint:winLoc fromView:nil];
	
	// Find view origin
	
	NSRect rect = [self visibleRect];
	
	winLoc.x += m_offsetX;
	winLoc.y -= rect.size.height - m_offsetY;
	
	return winLoc;
}

// mouseDown: left mouse button pressed
//-----------------------------------------------------------------------------------------------
- (void)mouseDown:(NSEvent*)theEvent
{
	NSPoint loc = [self getEventLocation:theEvent];
	
	m_scaleFrame   = NO;
	if (m_frameSelected)
	{
		m_moveFrame    = YES;
		
		if (m_snapFrames)
		{
			Vector2* snap = [m_backGrid getSnapPointForX:loc.x Y:loc.y GiveMax:NO];
			
			m_selectPoint.x = snap->x;
			m_selectPoint.y = snap->y;
			
			snap = [m_backGrid getSnapPointForX:m_obsrvFrame.propPositionX Y:m_obsrvFrame.propPositionY GiveMax:NO];
			
			m_framePoint.x  = snap->x;
			m_framePoint.y  = snap->y;
		}
		else
		{
			m_selectPoint  = loc;
			m_framePoint.x = m_obsrvFrame.propPositionX;
			m_framePoint.y = m_obsrvFrame.propPositionY;
		}
	}
		
	[self drawRect:[self frame]];
}

// mouseUp: left mouse button released
//-----------------------------------------------------------------------------------------------
- (void)mouseUp:(NSEvent*)theEvent
{
	m_moveFrame = NO;
	
	[self drawRect:[self frame]];
}

// rightMouseDown: right mouse button pressed
//-----------------------------------------------------------------------------------------------
- (void)rightMouseDown:(NSEvent*)theEvent
{
	NSPoint loc = [self getEventLocation:theEvent];
	
	m_moveFrame    = NO;
	if (m_frameSelected)
	{
		m_scaleFrame   = YES;
		m_selectPoint  = loc;
		m_framePoint.x = m_obsrvFrame.propWidth;
		m_framePoint.y = m_obsrvFrame.propHeight;	
	}
	
	[self drawRect:[self frame]];
}

// rightMouseUp: right mouse button released
//-----------------------------------------------------------------------------------------------
- (void)rightMouseUp:(NSEvent*)theEvent
{
	m_scaleFrame = NO;
	
	[self drawRect:[self frame]];
}

// mouseDragged: left button pressed and mouse moves
//-----------------------------------------------------------------------------------------------
- (void)mouseDragged:(NSEvent*)theEvent
{
	NSPoint loc = [self getEventLocation:theEvent];

	if (m_moveFrame)
	{
		if (m_snapFrames)
		{
			Vector2* offs = [m_backGrid getCellOffset:m_offsetX Y:m_offsetY];
			Vector2* snap = [m_backGrid getSnapPointForX:loc.x Y:loc.y GiveMax:NO];
			
			loc.x = snap->x + offs->x;
			loc.y = snap->y + offs->y;
		}
		
		m_obsrvFrame.propPositionX = MAX(m_framePoint.x + floor((loc.x - m_selectPoint.x) / m_zoom), 0.0f);
		m_obsrvFrame.propPositionY = MAX(m_framePoint.y - floor((loc.y - m_selectPoint.y) / m_zoom), 0.0f);
		
		[self updateFrameVertices];
	}

	[self drawRect:[self frame]];
}

// rightMoustDragger: right button pressed and mouse moves
//-----------------------------------------------------------------------------------------------
- (void)rightMouseDragged:(NSEvent*)theEvent
{
	NSPoint loc = [self getEventLocation:theEvent];
	
	if (m_scaleFrame)
	{
		m_obsrvFrame.propWidth  = MAX(m_framePoint.x + floor((loc.x - m_selectPoint.x) / m_zoom), 0.0f);
		m_obsrvFrame.propHeight = MAX(m_framePoint.y - floor((loc.y - m_selectPoint.y) / m_zoom), 0.0f);
		
		[self updateFrameVertices];
	}
	
	[self drawRect:[self frame]];
}


// mouseMoved: mouse moves over view frame
//-----------------------------------------------------------------------------------------------
- (void)mouseMoved:(NSEvent*)theEvent 
{
	NSPoint loc = [self getEventLocation:theEvent];
	
	if ( (loc.x >= floor(  (m_obsrvFrame.propPositionX) * m_zoom)) &&
		 (loc.y <= -floor( (m_obsrvFrame.propPositionY) * m_zoom)) &&
		 (loc.x <  floor(  (m_obsrvFrame.propPositionX  + m_obsrvFrame.propWidth)  * m_zoom) ) &&
		 (loc.y >  -floor( (m_obsrvFrame.propPositionY  + m_obsrvFrame.propHeight) * m_zoom) ) )
	{
		if (m_frameSelected != YES)
		{
			m_frameSelected = YES;
			[self updateFrameVertices];
		}
	}
	else
	{
		if (m_frameSelected != NO)
		{
			m_frameSelected = NO;
			[self updateFrameVertices];
		}
	}
	
	[self drawRect:[self frame]];
}

// mouseExited: mouse exited frame tracking area
//-----------------------------------------------------------------------------------------------
- (void)mouseExited:(NSEvent*)theEvent 
{
	m_frameSelected = NO;
	m_moveFrame		= NO;
	m_scaleFrame	= NO;
	
	[self updateFrameVertices];
	
	[self drawRect:[self frame]];
}

//############################################
// OTHER METHODS
//############################################

// lockFrameOffsets: sets current offsets to center active frame
//-----------------------------------------------------------------------------------------------
- (void)lockFrameOffsets
{
	NSRect frame = [self visibleRect];
	
	if (m_lockFrameOffsetX && m_obsrvFrame)
		self.propViewOffsetX = (GLfloat)m_obsrvFrame.propPositionX + (GLfloat)m_obsrvFrame.propWidth / 2.0f - (GLfloat)frame.size.width / 2.0f;
	
	if (m_lockFrameOffsetY && m_obsrvFrame)
		self.propViewOffsetY = (GLfloat)frame.size.height / 2.0f - (GLfloat)m_obsrvFrame.propPositionY - (GLfloat)m_obsrvFrame.propHeight / 2.0f;
}

@end
