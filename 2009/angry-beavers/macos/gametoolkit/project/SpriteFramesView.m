//===============================================================================================
//  File:		SpriteFramesView.m
//  Project:	OriginGameToolkit
//	Desc:		Sprite editor frame view
//
//  Created by Max Vasin on 3/12/11.
//  Copyright 2011 Origin Games. All rights reserved.
//===============================================================================================

// ************* IMPORTS *************

#import "SpriteFramesView.h"
#import <GLUT/GLUT.h>

// ************* PRIVATE METHODS *************

@interface SpriteFramesView (Private)

- (void)	clear;
- (void)	moveObserver:			(id)oldObject	toObject:(id)newObject	forKey:(NSString*)key;
- (void)	observeProject:			(Project*)project;
- (void)	observeSpriteItem:		(ProjectSpriteItem*)sprite;
- (void)	observeAnimation:		(SpriteAnimation*)animation;
- (void)	observeAnimationFrame:	(AnimationFrame*)frame;
- (void)	observeFramePoint:		(FramePoint*)point;
- (void)	updateBaseTexture;
- (void)	updateFrames;
- (void)	updateAnimation;
- (void)	updateNoFrameTexture;
- (void)	updateCheckerTexture;
- (void)	updateBackground;
- (void)	updatePoints;
- (void)	renderBackground;
- (void)	updateTexture:			(GLuint*)pTexID  toSize:(IntSize*)pSize withPath:(NSString*)path;
- (void)	rebuildResources;
- (void)	cleanResources;
- (void)	renderFrames;
- (void)	onTimer:				(NSTimer*)timer;
- (NSPoint)	getEventLocation:		(NSEvent*)theEvent;
- (BOOL)	inFrame:				(NSPoint)point;

@end

// ************* INTERFACE *************

@implementation SpriteFramesView

// ************* PROPERTIES *************

@synthesize propLockControls		= m_lockControls;
@synthesize propUseOriginPoint		= m_useOriginPoint;

// propZoom: controlling view zooming
//-----------------------------------------------------------------------------------------------
- (void)setPropZoom:(GLfloat)zoom
{
	m_zoom = zoom;
	[self drawRect:[self frame]];
}

- (GLfloat)propZoom
{
	return m_zoom;
}

// Lock animation to origin point
//-----------------------------------------------------------------------------------------------
- (void)setPropUseOriginPoint:(BOOL)use
{
	m_useOriginPoint = use;
	
	[self updateFrames];
	[self updatePoints];
	[self drawRect:[self frame]];
}

- (BOOL)propUseOriginPoint
{
	return m_useOriginPoint;
}


// ************* METHODS *************

// awakeFromNib: this view just finished loading from NIB - do initialization
//-----------------------------------------------------------------------------------------------
- (void)awakeFromNib
{
	m_contextReady	= NO;
	
	m_zoom			= 1.0f;
	
	AppDelegate* appDelegate = (AppDelegate*)[NSApp delegate];
	
	[appDelegate addObserver:self forKeyPath:@"propCurrentProject" options:NSKeyValueObservingOptionNew context:nil];
	
	// Connect to app preferences
	
	[m_appPrefs addObserver:self forKeyPath:@"propSpriteSeqViewCheckerColor"	options:NSKeyValueObservingOptionNew context:nil];
	[m_appPrefs addObserver:self forKeyPath:@"propSpriteSeqViewActiveAlpha"		options:NSKeyValueObservingOptionNew context:nil];
	[m_appPrefs addObserver:self forKeyPath:@"propSpriteSeqViewInactiveAlpha"	options:NSKeyValueObservingOptionNew context:nil];
	
	m_colorChecker	= [m_appPrefs.propSpriteSeqViewCheckerColor copy];
	m_alphaActive	= m_appPrefs.propSpriteSeqViewActiveAlpha;
	m_alphaInactive	= m_appPrefs.propSpriteSeqViewInactiveAlpha;
	
	[self observeProject:appDelegate.propCurrentProject];

	// set view controller
	
	[m_viewController setContent:self];
}

// dealloc: destructor
//-----------------------------------------------------------------------------------------------
- (void)dealloc
{
	// remove all observers
	
	[self observeProject:nil];
	
	AppDelegate* appDelegate = (AppDelegate*)[NSApp delegate];
	[appDelegate removeObserver:self forKeyPath:@"propCurrentProject"];
	
	[m_appPrefs removeObserver:self forKeyPath:@"propSpriteSeqViewCheckerColor"];
	[m_appPrefs removeObserver:self forKeyPath:@"propSpriteSeqViewActiveAlpha"];
	[m_appPrefs removeObserver:self forKeyPath:@"propSpriteSeqViewInactiveAlpha"];
	
	[self cleanResources];
	[super dealloc];
}


// ************* OBJECTS OBSERVATION CONTROL *************


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
		
		[self observeSpriteItem:m_obsrvProject.propCurrentSpriteItem];
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
		
		[m_movie release];
		
		if (m_obsrvSprite.propType == eSpriteTypeMovie)
		{
			m_movie = [[GLMovie alloc] initWithContext:[self openGLContext] pixelFormat:[NSOpenGLView defaultPixelFormat] fromSource:@"/Users/mozg/Developer/ios/heli/toolkit/sequences/retina/alien/alien_run/alien_run_12f.mov"];
		}
		
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
		
		m_obsrvAnimation = animation;
		
		if (m_obsrvAnimation)
			[self observeAnimationFrame:m_obsrvAnimation.propCurrentFrame];
		else
			[self observeAnimationFrame:nil];
		
		[self updateBaseTexture];
		[self updateFrames];
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
		[self moveObserver:m_obsrvFrame toObject:frame forKey:@"propCurrentPoint"];
		[self moveObserver:m_obsrvFrame toObject:frame forKey:@"propMirrorX"];
		[self moveObserver:m_obsrvFrame toObject:frame forKey:@"propMirrorY"];
		
		m_obsrvFrame = frame;
		
		if (m_obsrvFrame)
			[self observeFramePoint:m_obsrvFrame.propCurrentPoint];
		else
			[self observeFramePoint:nil];
	}
}

// Starts watching for current frame point change
//-----------------------------------------------------------------------------------------------
- (void)observeFramePoint:(FramePoint*)point
{
	if (m_obsrvPoint != point)
	{
		[self moveObserver:m_obsrvPoint toObject:point forKey:@"propPositionX"];
		[self moveObserver:m_obsrvPoint toObject:point forKey:@"propPositionY"];
		[self moveObserver:m_obsrvPoint toObject:point forKey:@"propColor"];
	
		m_obsrvPoint = point;
	}
}


// observeValueForKeyPath: observing values changing
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
	
	else if	([keyPath isEqualToString:@"propCurrentPoint"])
		[self observeFramePoint:newValue];

	// Observing preferences changings
	
	if		([keyPath isEqualToString:@"propSpriteSeqViewCheckerColor"])
		m_colorChecker	= [newValue copy];
	
	else if	([keyPath isEqualToString:@"propSpriteSeqViewActiveAlpha"])
		m_alphaActive	= [newValue floatValue];
	
	else if	([keyPath isEqualToString:@"propSpriteSeqViewInactiveAlpha"])
		m_alphaInactive	= [newValue floatValue];
	
	
	// Observing animation changings
	
	if		([keyPath isEqualToString:@"propTexture"])
		[self updateBaseTexture];
	
	
	[self updateBackground];
	[self updateFrames];
	[self updatePoints];
	[self drawRect:[self frame]];
}

// ************* VIEW DRAWING CONTROL *************

// reshape: view size has been changed
//-----------------------------------------------------------------------------------------------
- (void)reshape
{
	[[self openGLContext] makeCurrentContext];

	NSRect frame = [self visibleRect];
	
	if (NSIsEmptyRect([self visibleRect]))
		glViewport(0, 0, 1, 1);
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
	
	[self updateBackground];
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
		
		[self rebuildResources];
	}
	
	[self reshape];
	
	[self renderBackground];
	
	[self renderFrames];
	
	[[self openGLContext] flushBuffer];
}

// ************* OPENGL RENDERING CONTROL *************

// clear: clear OpenGL backbuffer
//-----------------------------------------------------------------------------------------------
- (void)clear
{
	glClearColor(0.0f, 1.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

// renderBackground: renders background
//-----------------------------------------------------------------------------------------------
- (void)renderBackground
{
	NSRect rect = [self visibleRect];
	
	glDisable(GL_DEPTH_TEST);
	
	// Set projection
	glMatrixMode(GL_PROJECTION);
    glPushMatrix();
	glLoadIdentity();
	glOrtho(0.0f,
			rect.size.width,
		    0.0f,
			rect.size.height,
			-1000.0f, 1000.0f);
	
	// Set model
	glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
	glLoadIdentity();
		
	// Preparing sources
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
	
	// Set source
	glVertexPointer(	3, GL_FLOAT, sizeof(OGLVertex), &m_vertsBack[0].pos);
	glColorPointer(		4, GL_FLOAT, sizeof(OGLVertex), &m_vertsBack[0].color);
	glTexCoordPointer(	2, GL_FLOAT, sizeof(OGLVertex), &m_vertsBack[0].uv);
	
	// Set blending ops
	glEnable( GL_BLEND );
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
	glEnable( GL_TEXTURE_2D );
	
	// Draw background
	glBindTexture( GL_TEXTURE_2D, m_oglCheckerTexture );
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	
	// Restore transformations
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
    glPopMatrix();
}

// renderFrames: renders current animation sequence
//-----------------------------------------------------------------------------------------------
- (void)renderFrames
{
	if (!m_oglTexture || !m_numFramesVerts)
		return;
	
	NSRect rect = [self visibleRect];
	
	GLfloat originX = 0.0f, originY = 0.0f, frameW = 0.0f, frameH = 0.0f;
	
	NSString* originPointName = nil;
	
	if (m_useOriginPoint)
	{
		originPointName = m_obsrvAnimation.propOriginPoint.propName;
		
		FramePoint* curFramePoint = [m_obsrvFrame getPoint:originPointName];
		
		originX = curFramePoint.propPositionX;
		originY = curFramePoint.propPositionY;
		
		frameW  = m_obsrvFrame.propWidth;
		frameH  = m_obsrvFrame.propHeight;
	}
		
	glDisable(GL_DEPTH_TEST);
	
	// Set projection
	glMatrixMode(GL_PROJECTION);
    glPushMatrix();
	glLoadIdentity();
	glOrtho(0.0f,
			rect.size.width,
		    0.0f,
			rect.size.height,
			-1000.0f, 1000.0f);
	
	// Set model
	glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
	glLoadIdentity();
	
	if (m_obsrvSprite.propType == eSpriteTypeSequence)
	{
	
	// Center view by Y axis
	glTranslatef(floor(rect.size.width / 2.0f), floor(rect.size.height / 2.0f), 0.0f);
		
	// Apply zoom
	glScalef(m_zoom, m_zoom, 1.0f);
	
	glTranslatef(-m_currentFramePos, 0.0f, 0.0f);
	
	glTranslatef(-originX, originY, 0.0f);
	glTranslatef(frameW / 2.0f, -frameH / 2.0f, 0.0f);
	
	// Preparing sources
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
	
	// Draw frame rectangle
	glVertexPointer(	3, GL_FLOAT, sizeof(OGLVertex), &m_vertsFrame[0].pos);
	glColorPointer(		4, GL_FLOAT, sizeof(OGLVertex), &m_vertsFrame[0].color);
	glTexCoordPointer(	2, GL_FLOAT, sizeof(OGLVertex), &m_vertsFrame[0].uv);
	
	glDisable(GL_TEXTURE_2D);
	
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	
	// Set source
	glVertexPointer(	3, GL_FLOAT, sizeof(OGLVertex), &m_vertsFrames[0].pos);
	glColorPointer(		4, GL_FLOAT, sizeof(OGLVertex), &m_vertsFrames[0].color);
	glTexCoordPointer(	2, GL_FLOAT, sizeof(OGLVertex), &m_vertsFrames[0].uv);
	
	glEnable(GL_TEXTURE_2D);
	
	// Set blending ops
	glEnable( GL_BLEND );
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
	glEnable( GL_TEXTURE_2D );
		
	// Draw frames sequence
	
	for (NSInteger i = 0; i < m_numFrames; i++)
	{
		// Set texture
		if (m_framesValid[i])
			glBindTexture( GL_TEXTURE_2D, m_oglTexture );
		else
			glBindTexture( GL_TEXTURE_2D, m_oglNoFrameTexture );
		
		glDrawArrays(GL_TRIANGLE_STRIP, i*4, 4);
	}
	
	// Draw frame points
	if (m_obsrvPoint)
	{
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		glDisable(GL_TEXTURE_2D);
		
		glLineWidth(m_zoom);
		
		glVertexPointer(	3, GL_FLOAT, sizeof(OGLColorVertex), &m_vertsPoint[0].pos);
		glColorPointer(		4, GL_FLOAT, sizeof(OGLColorVertex), &m_vertsPoint[0].color);
		
		glDrawArrays(GL_LINES, 0, 4);
	}

	
	// Restore transformations
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	}
	else
	{
		[m_movie render];
	}
	
	
	glMatrixMode(GL_PROJECTION);
    glPopMatrix();
}

// ************* RESOURCES BUILDING *************

// rebuildResources: rebuilds all resources
//-----------------------------------------------------------------------------------------------
- (void)rebuildResources
{
	[[self openGLContext] makeCurrentContext];
	
	[self cleanResources];
	
	[self updateBaseTexture];
	[self updateNoFrameTexture];
	[self updateCheckerTexture];
	[self updateFrames];
}

// cleanResources: cleans all OpenGL stuff
//-----------------------------------------------------------------------------------------------
- (void)cleanResources
{
	[[self openGLContext] makeCurrentContext];
	
	SAFE_FREE(m_vertsFrames);
	SAFE_FREE(m_framesValid);
	m_numFramesVerts = 0;
	m_numFrames		 = 0;
	
	glDeleteTextures(1, &m_oglTexture);
	glDeleteTextures(1, &m_oglNoFrameTexture);
	glDeleteTextures(1, &m_oglCheckerTexture);

	m_oglTexture		= 0;
	m_oglNoFrameTexture = 0;
	m_oglCheckerTexture = 0;
}

// updateBackgroung: recreating background sprite
//-----------------------------------------------------------------------------------------------
- (void)updateBackground
{
	NSRect frame = [self visibleRect];
	
	if (!m_texCheckerSize.width || !m_texCheckerSize.height)
		return;
	
	GLfloat w = frame.size.width;
	GLfloat h = frame.size.height;
	GLfloat u = w / (GLfloat)m_texCheckerSize.width;
	GLfloat v = h / (GLfloat)m_texCheckerSize.height;
	
	m_vertsBack[0].pos.x = 0.0f;
	m_vertsBack[0].pos.y = 0.0f;
	m_vertsBack[0].pos.z = 0.0f;
	m_vertsBack[0].uv.x  = 0.0f;
	m_vertsBack[0].uv.y  = 0.0f;
	
	m_vertsBack[1].pos.x = w;
	m_vertsBack[1].pos.y = 0.0f;
	m_vertsBack[1].pos.z = 0.0f;
	m_vertsBack[1].uv.x  = u;
	m_vertsBack[1].uv.y  = 0.0f;

	m_vertsBack[2].pos.x = 0.0f;
	m_vertsBack[2].pos.y = h;
	m_vertsBack[2].pos.z = 0.0f;
	m_vertsBack[2].uv.x  = 0.0f;
	m_vertsBack[2].uv.y  = v;
	
	m_vertsBack[3].pos.x = w;
	m_vertsBack[3].pos.y = h;
	m_vertsBack[3].pos.z = 0.0f;
	m_vertsBack[3].uv.x  = u;
	m_vertsBack[3].uv.y  = v;
	
	[m_colorChecker getComponents:(CGFloat*)&m_vertsBack[0].color];
	[m_colorChecker getComponents:(CGFloat*)&m_vertsBack[1].color];
	[m_colorChecker getComponents:(CGFloat*)&m_vertsBack[2].color];
	[m_colorChecker getComponents:(CGFloat*)&m_vertsBack[3].color];
}

// updateFrames: recreating frames vertices
//-----------------------------------------------------------------------------------------------
- (void)updateFrames
{
	SAFE_FREE(m_vertsFrames);
	SAFE_FREE(m_framesValid);
	m_numFramesVerts = 0;
	m_numFrames		 = 0;
	
	if (!m_texSize.width || !m_texSize.height)
		return;
	
	if (!m_obsrvAnimation)
		return;
	
	NSArray* frames = m_obsrvAnimation.propFrames;
	if (!frames || ![frames count])
		return;
	
	m_numFrames		 = [frames count];
	m_numFramesVerts = m_numFrames * 4;
	m_vertsFrames    = calloc(m_numFramesVerts, sizeof(OGLVertex));
	m_framesValid	 = calloc(m_numFrames,		sizeof(BOOL));
	
	NSInteger curFrame	= 0;
	GLfloat	  curWidth	= 0.0f;
	m_framesHeight		= 0.0f;
	m_currentFramePos	= 0.0f;
	
	NSColor* clrWhite = [[NSColor colorWithCalibratedRed:1.0f green:1.0f blue:1.0f alpha:1.0f] autorelease];
	NSColor* clrGreen = [[NSColor colorWithCalibratedRed:0.0f green:1.0f blue:0.0f alpha:1.0f] autorelease];
	
	NSInteger curFrameIndex		= m_obsrvAnimation.propCurrentFrameIndex;
	NSString* originPointName	= nil;
	
	if (m_useOriginPoint)
	{
		originPointName = m_obsrvAnimation.propOriginPoint.propName;
	}
	
	for (AnimationFrame* frame in frames)
	{
		NSInteger x = frame.propPositionX;
		NSInteger y = frame.propPositionY;
		NSInteger w = frame.propWidth;
		NSInteger h = frame.propHeight;
		
		if (m_framesHeight < h)
			m_framesHeight = h;
		
		// Find UV
		GLfloat	  u  = (GLfloat)x / (GLfloat)m_texSize.width;
		GLfloat	  v  = (GLfloat)y / (GLfloat)m_texSize.height;
		GLfloat	  u1 = (GLfloat)(x+w) / m_texSize.width;
		GLfloat	  v1 = (GLfloat)(y+h) / m_texSize.height;
		
		if (!w || !h)
		{
			m_framesValid[curFrame] = NO;
			w = m_texNoFrameSize.width;
			h = m_texNoFrameSize.height;
			u = v   = 0.0f;
			u1 = v1 = 1.0f;
		}
		else
		{
			m_framesValid[curFrame] = YES;
		}
		
		if (frame.propMirrorX)
			SWAPFLOAT(u, u1);
		
		if (frame.propMirrorY)
			SWAPFLOAT(v, v1);
		
		GLfloat	  l = curWidth;
		GLfloat	  r = curWidth + w;
		GLfloat	  t = floor(h / 2.0f);
		GLfloat	  b = -ceil(h / 2.0f);
		
		// Make vertices
		m_vertsFrames[curFrame * 4 + 0].pos.x = l;
		m_vertsFrames[curFrame * 4 + 0].pos.y = t;
		m_vertsFrames[curFrame * 4 + 0].pos.z = 0.0f;
		m_vertsFrames[curFrame * 4 + 0].uv.x  = u;
		m_vertsFrames[curFrame * 4 + 0].uv.y  = v;
		
		m_vertsFrames[curFrame * 4 + 1].pos.x = r;
		m_vertsFrames[curFrame * 4 + 1].pos.y = t;
		m_vertsFrames[curFrame * 4 + 1].pos.z = 0.0f;
		m_vertsFrames[curFrame * 4 + 1].uv.x  = u1;
		m_vertsFrames[curFrame * 4 + 1].uv.y  = v;
		
		m_vertsFrames[curFrame * 4 + 2].pos.x = l;
		m_vertsFrames[curFrame * 4 + 2].pos.y = b;
		m_vertsFrames[curFrame * 4 + 2].pos.z = 0.0f;
		m_vertsFrames[curFrame * 4 + 2].uv.x  = u;
		m_vertsFrames[curFrame * 4 + 2].uv.y  = v1;
		
		m_vertsFrames[curFrame * 4 + 3].pos.x = r;
		m_vertsFrames[curFrame * 4 + 3].pos.y = b;
		m_vertsFrames[curFrame * 4 + 3].pos.z = 0.0f;
		m_vertsFrames[curFrame * 4 + 3].uv.x  = u1;
		m_vertsFrames[curFrame * 4 + 3].uv.y  = v1;
		
		[clrWhite getComponents:(CGFloat*)&m_vertsFrames[curFrame * 4 + 0].color];
		[clrWhite getComponents:(CGFloat*)&m_vertsFrames[curFrame * 4 + 1].color];
		[clrWhite getComponents:(CGFloat*)&m_vertsFrames[curFrame * 4 + 2].color];
		[clrWhite getComponents:(CGFloat*)&m_vertsFrames[curFrame * 4 + 3].color];
				
		GLfloat alpha = m_animPlaying ? 0 : m_alphaInactive;
		if (curFrame == curFrameIndex)
		{
			alpha = m_alphaActive;
			m_currentFramePos = curWidth + floor(w / 2.0f);
		}
			
		m_vertsFrames[curFrame * 4 + 0].color.a =
		m_vertsFrames[curFrame * 4 + 1].color.a =
		m_vertsFrames[curFrame * 4 + 2].color.a =
		m_vertsFrames[curFrame * 4 + 3].color.a = alpha;
		
		if (curFrame == curFrameIndex)
		{
			memcpy(&m_vertsFrame, &m_vertsFrames[curFrame * 4], sizeof(OGLVertex) * 4);
			
			[clrGreen getComponents:(CGFloat*)&m_vertsFrame[0].color];
			[clrGreen getComponents:(CGFloat*)&m_vertsFrame[1].color];
			[clrGreen getComponents:(CGFloat*)&m_vertsFrame[2].color];
			[clrGreen getComponents:(CGFloat*)&m_vertsFrame[3].color];
			
			m_vertsFrame[0].color.a =
			m_vertsFrame[1].color.a =
			m_vertsFrame[2].color.a =
			m_vertsFrame[3].color.a = 0.5f;
			
			// update frame point here
			
			if (m_obsrvPoint)
			{
				m_vertsPoint[0].pos.x = l;
				m_vertsPoint[0].pos.y = t - m_obsrvPoint.propPositionY;
				m_vertsPoint[0].pos.z = 0.0f;
				
				m_vertsPoint[1].pos.x = r;
				m_vertsPoint[1].pos.y = t - m_obsrvPoint.propPositionY;
				m_vertsPoint[1].pos.z = 0.0f;
				
				m_vertsPoint[2].pos.x = l + m_obsrvPoint.propPositionX;
				m_vertsPoint[2].pos.y = t;
				m_vertsPoint[2].pos.z = 0.0f;
				
				m_vertsPoint[3].pos.x = l + m_obsrvPoint.propPositionX;
				m_vertsPoint[3].pos.y = b;
				m_vertsPoint[3].pos.z = 0.0f;
				
				NSColor* clrPoint = m_obsrvPoint.propColor;
		
				[clrPoint getComponents:(CGFloat*)&m_vertsPoint[0].color];
				[clrPoint getComponents:(CGFloat*)&m_vertsPoint[1].color];
				[clrPoint getComponents:(CGFloat*)&m_vertsPoint[2].color];
				[clrPoint getComponents:(CGFloat*)&m_vertsPoint[3].color];
			}
		}
		
		curWidth = r;
		curFrame++;
	}
	
	m_framesWidth = curWidth;
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
}

// updateNoFrameTexture: updating invalid frames texture
//-----------------------------------------------------------------------------------------------
- (void)updateNoFrameTexture
{
	NSString* texPath = [[NSBundle mainBundle] pathForResource:@"red_minus" ofType:@"png" inDirectory:@"icons"];
	
	[self updateTexture:&m_oglNoFrameTexture toSize:&m_texNoFrameSize withPath:texPath];
}

// updateCheckerTexture: updating background checker texture
//-----------------------------------------------------------------------------------------------
- (void)updateCheckerTexture
{
	NSString* texPath = [[NSBundle mainBundle] pathForResource:@"gray_checkers" ofType:@"png" inDirectory:@"icons"];
	
	[self updateTexture:&m_oglCheckerTexture toSize:&m_texCheckerSize withPath:texPath];
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

// Updates points resources
//-----------------------------------------------------------------------------------------------
- (void)updatePoints
{
	// delete this
}


//############################################
// ACTIONS
//############################################

// togglePlayAnimation: play animation button handler
//-----------------------------------------------------------------------------------------------
- (IBAction)togglePlayAnimation:(id)sender
{
	if (m_animPlaying)
	{
		m_animPlaying = NO;
		
		[m_animTimer invalidate];
		SAFE_RELEASE(m_animTimer);
		
		self.propLockControls = NO;
		
		[self updateFrames];
		[self drawRect:[self frame]];
	}
	else 
	{
		self.propLockControls = YES;
		
		NSTimeInterval interval = 1.0f / m_obsrvAnimation.propSpeed;
		
		m_animTimer = [[NSTimer scheduledTimerWithTimeInterval:interval
													   target:self
													 selector:@selector(onTimer:)
													 userInfo:nil
													  repeats:YES] retain];
		m_animPlaying = YES;
	}
}

// onTimer: animation timer handler
//-----------------------------------------------------------------------------------------------
- (void)onTimer:(NSTimer*)timer
{
	NSInteger curFrame, count;
	
	switch (m_obsrvSprite.propType) 
	{
		case eSpriteTypeSequence:
			curFrame = m_obsrvAnimation.propCurrentFrameIndex;
			count    = [m_obsrvAnimation.propFrames count];
			
			curFrame++;
			if (curFrame >= count)
				curFrame = 0;
			
			m_obsrvAnimation.propCurrentFrameIndex = curFrame;

			break;
		case eSpriteTypeMovie:
			break;
	}
}

//############################################
// MOUSE INPUT CONTROL
//############################################

// Returns true if point in current frame
//-----------------------------------------------------------------------------------------------
- (BOOL)inFrame:(NSPoint)point
{
	if (!m_obsrvFrame)
		return NO;
	
	NSRect rect = [self visibleRect];
	
	NSInteger l = floorf(rect.size.width / 2.0f  - (m_obsrvFrame.propWidth / 2.0f  * m_zoom));
	NSInteger r = floorf(rect.size.width / 2.0f  + (m_obsrvFrame.propWidth / 2.0f  * m_zoom));
	NSInteger t = floorf(rect.size.height / 2.0f - (m_obsrvFrame.propHeight / 2.0f * m_zoom));
	NSInteger b = floorf(rect.size.height / 2.0f + (m_obsrvFrame.propHeight / 2.0f * m_zoom)); 
	
	return ( (point.x >= l) && (point.x < r) && (point.y >= t) && (point.y < b) );
}


// getEventLocation: returns mouse event location transformed to view coordinates (and snapped) 
//-----------------------------------------------------------------------------------------------
- (NSPoint)getEventLocation:(NSEvent*)theEvent
{
	// Get window-relative location
	
	NSPoint winLoc = [theEvent locationInWindow];
	
	// Transform into view
	
	winLoc = [self convertPoint:winLoc fromView:nil];
	
	return winLoc;
}

// mouseDown: left mouse button pressed
//-----------------------------------------------------------------------------------------------
- (void)mouseDown:(NSEvent*)theEvent
{
	NSPoint loc = [self getEventLocation:theEvent];
	
	if ([self inFrame:loc])
	{
		m_selectPoint		= loc;
		m_currentOffset.x	= 0;
		m_currentOffset.y	= 0;
		m_movePoint			= TRUE;
	}
	
	[self drawRect:[self frame]];
}

// mouseUp: left mouse button released
//-----------------------------------------------------------------------------------------------
- (void)mouseUp:(NSEvent*)theEvent
{
	m_movePoint = NO;
	
	[self drawRect:[self frame]];
}

// rightMouseDown: right mouse button pressed
//-----------------------------------------------------------------------------------------------
- (void)rightMouseDown:(NSEvent*)theEvent
{
//	NSPoint loc = [self getEventLocation:theEvent];
	
	[self drawRect:[self frame]];
}

// rightMouseUp: right mouse button released
//-----------------------------------------------------------------------------------------------
- (void)rightMouseUp:(NSEvent*)theEvent
{
	[self drawRect:[self frame]];
}

// mouseDragged: left button pressed and mouse moves
//-----------------------------------------------------------------------------------------------
- (void)mouseDragged:(NSEvent*)theEvent
{
	NSPoint loc = [self getEventLocation:theEvent];
	
	if (m_movePoint && [self inFrame:loc] && m_obsrvPoint)
	{
		m_currentOffset.x += (loc.x - m_selectPoint.x) / m_zoom;
		m_currentOffset.y -= (loc.y - m_selectPoint.y) / m_zoom;
		
		if (m_currentOffset.x >= 1.0f)
		{
			m_obsrvPoint.propPositionX += floorf(m_currentOffset.x);
			m_currentOffset.x -= floorf(m_currentOffset.x);
		}
		if (m_currentOffset.x <= -1.0f)
		{
			m_obsrvPoint.propPositionX += ceilf(m_currentOffset.x);
			m_currentOffset.x -= ceilf(m_currentOffset.x);
		}
		
		if (m_currentOffset.y >= 1.0f)
		{
			m_obsrvPoint.propPositionY += floorf(m_currentOffset.y);
			m_currentOffset.y -= floorf(m_currentOffset.y);
		}
		
		if (m_currentOffset.y <= -1.0f)
		{
			m_obsrvPoint.propPositionY += ceilf(m_currentOffset.y);
			m_currentOffset.y -= ceilf(m_currentOffset.y);
		}
		
				
		m_selectPoint = loc;
	}
	
	[self drawRect:[self frame]];
}

// rightMoustDragger: right button pressed and mouse moves
//-----------------------------------------------------------------------------------------------
- (void)rightMouseDragged:(NSEvent*)theEvent
{
//	NSPoint loc = [self getEventLocation:theEvent];
	
	[self drawRect:[self frame]];
}


// mouseMoved: mouse moves over view frame
//-----------------------------------------------------------------------------------------------
- (void)mouseMoved:(NSEvent*)theEvent 
{
//	NSPoint loc = [self getEventLocation:theEvent];
	
	[self drawRect:[self frame]];
}

// mouseExited: mouse exited frame tracking area
//-----------------------------------------------------------------------------------------------
- (void)mouseExited:(NSEvent*)theEvent 
{
	m_movePoint = NO;
	
	[self drawRect:[self frame]];
}

@end
