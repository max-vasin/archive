//===============================================================================================
//  File:		MultiSpriteView.m
//  Project:	OriginGameToolkit
//	Desc:		Multi sprite OpenGL view implementation
//
//  Created by Max Vasin on 3/30/11.
//  Copyright 2011 Origin Games. All rights reserved.
//===============================================================================================

// ************* IMPORTS *************

#import "MultiSpriteView.h"
#import "Project.h"
#import "AppDelegate.h"

static NSOpenGLPixelFormatAttribute formatAttributes[] =
{
    NSOpenGLPFADoubleBuffer,
    NSOpenGLPFAColorSize, 24,
    0
};

// ************* PRIVATE METHODS *************

@interface MultiSpriteView (Private)

- (void)	clear;
- (void)	moveObserver:			(id)oldObject	toObject:(id)newObject	forKey:(NSString*)key;
- (void)	observeProject:			(Project*)project;
- (void)	observeMultiSprite:		(ProjectMultiSpriteItem*)multisprite;
- (void)	observeRecord:			(MultiSpriteRecord*)record;
- (void)	observeKey:				(MultiSpriteRecordKey*)key;
- (void)	renderMultiSprite;
@end

// ************* IMPLEMENTATION *************

@implementation MultiSpriteView

//###############################################################################################
// OBJECTS OBSERVATION CONTROL
//###############################################################################################

// Safely removes self-observer for old object and assigns to new
//-----------------------------------------------------------------------------------------------
- (void)moveObserver:(id)oldObject toObject:(id)newObject forKey:(NSString*)key
{
	if (oldObject)
		[oldObject removeObserver:self forKeyPath:key];
	
	if (newObject) 
		[newObject addObserver:self forKeyPath:key options:NSKeyValueObservingOptionNew context:nil];
}

// Observing for project change
//-----------------------------------------------------------------------------------------------
- (void)observeProject:(Project*)project
{
	if (m_obsrvProject != project)
	{
		[self moveObserver:m_obsrvProject toObject:project forKey:@"propCurrentMultiSpriteItem"];
		
		m_obsrvProject = project;
		
		[self observeMultiSprite:m_obsrvProject.propCurrentMultiSpriteItem];
	}
}

// Start watching for current multisprite change
//-----------------------------------------------------------------------------------------------
- (void)observeMultiSprite:(ProjectMultiSpriteItem*)multisprite
{
	if (m_obsrvMultiSprite != multisprite)
	{
		[self moveObserver:m_obsrvMultiSprite toObject:multisprite forKey:@"propCurrentKey"];
		[self moveObserver:m_obsrvMultiSprite toObject:multisprite forKey:@"propCurrentRecord"];
		[self moveObserver:m_obsrvMultiSprite toObject:multisprite forKey:@"propLayer"];
		[self moveObserver:m_obsrvMultiSprite toObject:multisprite forKey:@"propRefreshView"];
		[self moveObserver:m_obsrvMultiSprite toObject:multisprite forKey:@"propUpdateTextures"];
		
		m_obsrvMultiSprite = multisprite;
		
		[m_texStore clear];
		
		SAFE_RELEASE(m_glMultiSprite);
		m_glMultiSprite = [[GLMultiSprite alloc] initWithContext:[self openGLContext] Item:m_obsrvMultiSprite TextureStore:m_texStore];
				
		[self observeRecord:m_obsrvMultiSprite.propCurrentRecord];
		[self observeKey:m_obsrvMultiSprite.propCurrentKey];
	}
}

// Start watching for current multisprite record change
//-----------------------------------------------------------------------------------------------
- (void)observeRecord:(MultiSpriteRecord*)record
{
	if (m_obsrvRecord != record)
	{
		m_obsrvRecord = record;
		
		m_glMultiSprite.propRecord = m_obsrvRecord;
	}
}

// Start watching for current multisprite record key change
//-----------------------------------------------------------------------------------------------
- (void)observeKey:(MultiSpriteRecordKey*)key
{
	if (m_obsrvKey != key)
	{
		[self moveObserver:m_obsrvKey toObject:key forKey:@"propLayer"];
		[self moveObserver:m_obsrvKey toObject:key forKey:@"propAnimation"];
		[self moveObserver:m_obsrvKey toObject:key forKey:@"propPoint"];
		[self moveObserver:m_obsrvKey toObject:key forKey:@"propSprite"];
		[self moveObserver:m_obsrvKey toObject:key forKey:@"propFrame"];
		
		m_obsrvKey = key;
		
		[m_glMultiSprite updateSprites];
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
	
	else if	([keyPath isEqualToString:@"propCurrentMultiSpriteItem"])
		[self observeMultiSprite:newValue];
	
	else if	([keyPath isEqualToString:@"propCurrentKey"])
		[self observeKey:newValue];
	
	else if	([keyPath isEqualToString:@"propCurrentRecord"])
		[self observeRecord:newValue];
	
	else if ([keyPath isEqualToString:@"propLayer"])
		[m_glMultiSprite updateSprites];
	
	else if ([keyPath isEqualToString:@"propAnimation"])
		[m_glMultiSprite updateSprites];
	
	else if ([keyPath isEqualToString:@"propPoint"])
		[m_glMultiSprite updateSprites];

	else if ([keyPath isEqualToString:@"propFrame"])
		[m_glMultiSprite updateSprites];

	else if ([keyPath isEqualToString:@"propUpdateTextures"])
	{
		[m_glMultiSprite updateSprites];
	}

	else if ([keyPath isEqualToString:@"propRefreshView"])
	{
		[m_glMultiSprite updateSprites];
	}

	[self drawRect:[self frame]];
}

//###############################################################################################
// INITIALIZERS\DESTRUCTORS
//###############################################################################################

// View initializing
//-----------------------------------------------------------------------------------------------
- (void)awakeFromNib
{
	m_contextReady		= NO;
	
	[self setPixelFormat:[[[NSOpenGLPixelFormat alloc] initWithAttributes:formatAttributes] autorelease]];
	
	// Connect to data
	
	AppDelegate* appDelegate = (AppDelegate*)[NSApp delegate];
	[appDelegate addObserver:self forKeyPath:@"propCurrentProject"	options:NSKeyValueObservingOptionNew context:nil];
	[self observeProject:appDelegate.propCurrentProject];
}

// Destructor
//-----------------------------------------------------------------------------------------------
- (void)dealloc
{
	SAFE_RELEASE(m_backGrid);
	
	[super dealloc];
}

//###############################################################################################
// VIEW ACTIONS
//###############################################################################################

// View size change handler
//-----------------------------------------------------------------------------------------------
- (void)reshape
{
	[[self openGLContext] makeCurrentContext];
	
	NSRect frame = [self visibleRect];
	
	if (NSIsEmptyRect([self visibleRect]))
	{
		glViewport(0, 0, 1, 1);
	}
	else
	{
		glViewport(0, 0, frame.size.width, frame.size.height);
		
		Vector2* origin = m_backGrid.propOrigin;
		
		origin->x = frame.size.width / 2.0f;
		origin->y = frame.size.height / 2.0f;
		
		m_backGrid.propOrigin = origin;
		m_backGrid.propFrame = frame;
	}
}

// View drawing
//-----------------------------------------------------------------------------------------------
- (void)drawRect:(NSRect)dirtyRect
{
	
	[[self openGLContext] makeCurrentContext];
	
	if (!m_contextReady)
	{
		m_backGrid		= [[GuidlineGrid alloc] initWithContext:[self openGLContext] andRect:[self visibleRect]];
		
		m_backGrid.propVSpace = 25;
		m_backGrid.propHSpace = 25;
		
		m_texStore		= [[TextureStore alloc] initWithContext:[self openGLContext]];

		m_contextReady	= YES;
	}
	
	[self reshape];
		
	[self clear];
	
	[m_backGrid render];
	
	[self renderMultiSprite];
	
	[[self openGLContext] flushBuffer];
}

//###############################################################################################
// DRAWING METHODS
//###############################################################################################

// Clears view
//-----------------------------------------------------------------------------------------------
- (void)clear
{
	glClearColor(0.0f, 1.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}

// Rendres multisprite
//-----------------------------------------------------------------------------------------------
- (void)renderMultiSprite
{
	NSRect rect = [self frame];
	
	glDisable(GL_DEPTH_TEST);
	
	glMatrixMode(GL_PROJECTION);
    glPushMatrix();
	glLoadIdentity();
    glOrtho(-ceil(rect.size.width / 2.0f), 
			floor(rect.size.width / 2.0f),
			-ceil(rect.size.height / 2.0f),
			floor(rect.size.height / 2.0f),
			-1000.0f, 1000.0f);
	
	glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
	glLoadIdentity();
	
	[m_glMultiSprite render];
	
	glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
	glMatrixMode(GL_PROJECTION);
    glPopMatrix();

}

@end
