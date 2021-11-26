//===============================================================================================
//  File:		TextureView.m
//  Project:	OriginGameToolkit
//	Desc:		Texture project item view implementation
//
//  Created by Max Vasin on 12/2/10.
//  Copyright 2010 Origin. All rights reserved.
//===============================================================================================

// **** IMPORTS ****

#import "TextureView.h"
#import <GLUT/GLUT.h>

// **** INTERFACE ****

@interface TextureView (Private)

- (void)clear;

@end

// **** IMPLEMENTATION ****

@implementation TextureView

// **** SYNTHESIZED PROPERTIES ****

@synthesize propBackColor		= m_backColor;
@synthesize propViewOffsetXMin	= m_offsetXMin;
@synthesize propViewOffsetXMax	= m_offsetXMax;
@synthesize propViewOffsetYMin	= m_offsetYMin;
@synthesize propViewOffsetYMax	= m_offsetYMax;

// **** IMPLEMENTED PROPERTIES ****

// Current texture item
//-----------------------------------------------------------------------------------------------
- (ProjectTextureItem*)propCurrentItem
{
	return m_itemTexture;
}

- (void)setPropCurrentItem:(ProjectTextureItem*)item
{
	m_itemTexture = item;
	
	// Reset info strings
	self.propMemoryString = nil;
	self.propSizeString   = nil;
	
	[self rebuildResources];
	[self drawRect:[self frame]];
}

// Packed texture view
//-----------------------------------------------------------------------------------------------
- (void)setPropViewPacked:(BOOL)packed
{
	m_viewPacked = packed;
	
	// Reset info strings
	self.propMemoryString = nil;
	self.propSizeString   = nil;

	[self rebuildResources];
	[self drawRect:[self frame]];
}

- (BOOL)propViewPacked
{
	return m_viewPacked;
}

// Texture size string
//-----------------------------------------------------------------------------------------------
- (void)setPropSizeString:(NSString*)value
{
	// dummy
}

- (NSString*)propSizeString
{
	if (!m_itemTexture)
		return @"No texture";
	
	IntSize size;
	
	if (m_viewPacked)
		size = m_itemTexture.propPackedSize;
	else
		size = m_itemTexture.propCachedSize;
	
	return [NSString stringWithFormat:@"Size: %ix%i", size.width, size.height];
}

// Texture memory string
//-----------------------------------------------------------------------------------------------
- (void)setPropMemoryString:(NSString*)value
{
	// dummy
}

- (NSString*)propMemoryString
{
	if (!m_itemTexture)
		return @"No texture";
	
	unsigned int memory;
	
	if (m_viewPacked)
		memory = m_itemTexture.propPackedMemory;
	else
		memory = m_itemTexture.propCachedMemory;
	
	return [NSString stringWithFormat:@"Memory: %#1.2f Mb", (float)memory / 1048576];
}

// propViewOffsetX
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

// propViewOffsetY
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

// View zooming
//-----------------------------------------------------------------------------------------------
- (void)setPropViewZoom:(GLfloat)zoom
{
	m_zoom = zoom;
	[self drawRect:[self frame]];
}

- (GLfloat)propViewZoom
{
	return m_zoom;
}

// **** INSTANCE METHODS ****

// awakeFromNib
// This view just finished loading from NIB - do initialization
//-----------------------------------------------------------------------------------------------

- (void)awakeFromNib
{
	m_contextReady	= NO;
	m_backGrid		= nil;
	
	[m_appPrefs addObserver:self forKeyPath:@"propTextureViewGridDashMultiplier"	options:NSKeyValueObservingOptionNew context:nil];
	[m_appPrefs addObserver:self forKeyPath:@"propTextureViewGridHSpace"			options:NSKeyValueObservingOptionNew context:nil];
	[m_appPrefs addObserver:self forKeyPath:@"propTextureViewGridVSpace"			options:NSKeyValueObservingOptionNew context:nil];
	[m_appPrefs addObserver:self forKeyPath:@"propTextureViewGridBackColor"			options:NSKeyValueObservingOptionNew context:nil];
	[m_appPrefs addObserver:self forKeyPath:@"propTextureViewGridDashColor"			options:NSKeyValueObservingOptionNew context:nil];
	[m_appPrefs addObserver:self forKeyPath:@"propTextureViewGridOriginColor"		options:NSKeyValueObservingOptionNew context:nil];
	[m_appPrefs addObserver:self forKeyPath:@"propTextureViewTextureAlpha"			options:NSKeyValueObservingOptionNew context:nil];
	
	[m_viewsTab addObserver:self forKeyPath:@"propCurrentItem"						options:NSKeyValueObservingOptionNew context:nil];
}

// dealloc
// Destructor
//-----------------------------------------------------------------------------------------------

- (void)dealloc
{
	[m_appPrefs removeObserver:self forKeyPath:@"propTextureViewGridDashMultiplier"];
	[m_appPrefs removeObserver:self forKeyPath:@"propTextureViewGridHSpace"];
	[m_appPrefs removeObserver:self forKeyPath:@"propTextureViewGridVSpace"];		
	[m_appPrefs removeObserver:self forKeyPath:@"propTextureViewGridBackColor"];		
	[m_appPrefs removeObserver:self forKeyPath:@"propTextureViewGridDashColor"];		
	[m_appPrefs removeObserver:self forKeyPath:@"propTextureViewGridOriginColor"];	
	[m_appPrefs removeObserver:self forKeyPath:@"propTextureViewTextureAlpha"];		

	[m_viewsTab removeObserver:self forKeyPath:@"propCurrentItem"];

	[self cleanResources];
	
	SAFE_RELEASE(m_backGrid);
	
	[super dealloc];
}

// Values observer
//-----------------------------------------------------------------------------------------------

- (void)observeValueForKeyPath:(NSString *)keyPath ofObject:(id)object change:(NSDictionary*)change context:(void*)context
{
	if		([keyPath isEqualToString:@"propTextureViewGridDashMultiplier"])
		m_backGrid.propDashMultiplier = [[change valueForKey:NSKeyValueChangeNewKey] intValue];
	else if ([keyPath isEqualToString:@"propTextureViewGridHSpace"])
		m_backGrid.propHSpace = [[change valueForKey:NSKeyValueChangeNewKey] intValue];
	else if ([keyPath isEqualToString:@"propTextureViewGridVSpace"])
		m_backGrid.propVSpace = [[change valueForKey:NSKeyValueChangeNewKey] intValue];
	else if ([keyPath isEqualToString:@"propTextureViewGridBackColor"])
		self.propBackColor = [change valueForKey:NSKeyValueChangeNewKey];
	else if ([keyPath isEqualToString:@"propTextureViewGridDashColor"])
		m_backGrid.propDashColor = [change valueForKey:NSKeyValueChangeNewKey];
	else if ([keyPath isEqualToString:@"propTextureViewGridOriginColor"])
		m_backGrid.propAxisColor = [change valueForKey:NSKeyValueChangeNewKey];			
	/*
	else if ([keyPath isEqualToString:@"propTextureViewTextureAlpha"])
	*/
	else if ([keyPath isEqualToString:@"propCurrentItem"])
	{
		id newItem = [change valueForKey:NSKeyValueChangeNewKey];
		if ([newItem class] == [ProjectTextureItem class])
			self.propCurrentItem = newItem; 
	}	
	[self drawRect:[self frame]];
}

// Called each time need to draw some portion of view - we redraw everything
//-----------------------------------------------------------------------------------------------
- (void)drawRect:(NSRect)dirtyRect
{
	if (m_noRedraw)
		return;
	
	[[self openGLContext] makeCurrentContext];
	
	// Our context not initialized
	if (!m_contextReady)
	{
		m_backGrid		= [[GuidlineGrid alloc] initWithContext:[self openGLContext] andRect:[self visibleRect]];
		
		m_backGrid.propDashMultiplier	= m_appPrefs.propTextureViewGridDashMultiplier;
		m_backGrid.propHSpace			= m_appPrefs.propTextureViewGridHSpace;
		m_backGrid.propVSpace			= m_appPrefs.propTextureViewGridVSpace;
		m_backGrid.propDashColor		= m_appPrefs.propTextureViewGridDashColor;
		m_backGrid.propAxisColor		= m_appPrefs.propTextureViewGridOriginColor;
		
		m_borderColor					= [NSColor redColor];
		
		self.propBackColor				= m_appPrefs.propTextureViewGridBackColor;
		
		m_contextReady	= YES;
	
		[self rebuildResources];
	}
	
	[self reshape];
	
	[self clear];
	
	[m_backGrid render];
	
	[self renderTexture];
	
	[[self openGLContext] flushBuffer];
}

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

// View size has been changed
//-----------------------------------------------------------------------------------------------
- (void)reshape
{
	NSRect frame = [self visibleRect];
	
	if (NSIsEmptyRect([self visibleRect]))
		glViewport(0, 0, 1, 1);
	else
		glViewport(0, 0, frame.size.width, frame.size.height);
	
	Vector2* origin = [Vector2 vectorWithX:frame.size.width / 2.0f Y:frame.size.height / 2.0f];
	
	m_backGrid.propOrigin	= origin;
	m_backGrid.propFrame	= frame;
}

// Rebuilds all resources with current texture item
//-----------------------------------------------------------------------------------------------
- (void)rebuildResources
{
	[[self openGLContext] makeCurrentContext];
	
	[self cleanResources];
	
	if (!m_itemTexture)
		return;
	
	// Try to load texture result
/*
	NSString* texPath = (m_viewPacked) ? m_itemTexture.propOutputPath : m_itemTexture.propBuildPath;
	
	NSData* fileContents = [NSData dataWithContentsOfFile:texPath];
	
	if (!fileContents)
		return;
	
	NSBitmapImageRep* img = [[[NSBitmapImageRep alloc] initWithDataAndConvert:fileContents] autorelease];
	if(!img)
		return;
	
	glGenTextures(1, &m_oglTexture);
	
	[img uploadToTexture:m_oglTexture];
	
	m_texSize = [img intSize];
	
	*/
	
	if (m_viewPacked)
	{
		NSString* texPath = (m_viewPacked) ? [m_itemTexture.propOutputPath stringByReplacingOccurrencesOfString:@"png"  withString:@"oritex"]: m_itemTexture.propBuildPath;
	
		[m_textureFile release];
	
		m_textureFile = [[OriTextureFile alloc] initWithPath:texPath];
	
		m_texSize = m_textureFile.size;
		m_oglTexture = m_textureFile.texture;
	}
	else
	{
		NSString* texPath = (m_viewPacked) ? m_itemTexture.propOutputPath : m_itemTexture.propBuildPath;
		
		NSData* fileContents = [NSData dataWithContentsOfFile:texPath];
		
		if (!fileContents)
			return;
		
		NSBitmapImageRep* img = [[[NSBitmapImageRep alloc] initWithDataAndConvert:fileContents] autorelease];
		if(!img)
			return;
		
		glGenTextures(1, &m_oglTexture);
		
		[img uploadToTexture:m_oglTexture];
		
		m_texSize = [img intSize];
	}
	
	
	
	// Rebuild vertices
	
	m_vertices[0].pos.x		= -floor((float)m_texSize.width / 2.0f);
	m_vertices[0].pos.y		= floor((float)m_texSize.height / 2.0f);
	m_vertices[0].pos.z		= 0.0f;
	m_vertices[0].color.r	= m_vertices[0].color.g = m_vertices[0].color.b = 1.0f; m_vertices[0].color.a = 1.0f;
	m_vertices[0].uv.x		= 0.0f;
	m_vertices[0].uv.y		= 0.0f;
	
	m_vertices[1].pos.x		= floor((float)m_texSize.width / 2.0f);
	m_vertices[1].pos.y		= floor((float)m_texSize.height / 2.0f);
	m_vertices[1].pos.z		= 0.0f;
	m_vertices[1].color.r	= m_vertices[1].color.g = m_vertices[1].color.b = m_vertices[1].color.a = 1.0f;
	m_vertices[1].uv.x		= 1.0f;
	m_vertices[1].uv.y		= 0.0f;
	
	m_vertices[2].pos.x		= -floor((float)m_texSize.width / 2.0f);
	m_vertices[2].pos.y		= -floor((float)m_texSize.height / 2.0f);
	m_vertices[2].pos.z		= 0.0f;
	m_vertices[2].color.r	= m_vertices[2].color.g = m_vertices[2].color.b = m_vertices[2].color.a = 1.0f;
	m_vertices[2].uv.x		= 0.0f;
	m_vertices[2].uv.y		= 1.0f;
	
	m_vertices[3].pos.x		= floor((float)m_texSize.width / 2.0f);
	m_vertices[3].pos.y		= -floor((float)m_texSize.height / 2.0f);
	m_vertices[3].pos.z		= 0.0f;
	m_vertices[3].color.r	= m_vertices[3].color.g = m_vertices[3].color.b = m_vertices[3].color.a = 1.0f;
	m_vertices[3].uv.x		= 1.0f;
	m_vertices[3].uv.y		= 1.0f;
	
	// Border
	m_borderVertices[0].pos.x = m_vertices[0].pos.x - 1;
	m_borderVertices[0].pos.y = m_vertices[0].pos.y + 1;
	m_borderVertices[0].pos.z = 0;
	
	m_borderVertices[1].pos.x = m_vertices[1].pos.x + 1;
	m_borderVertices[1].pos.y = m_vertices[1].pos.y + 1;
	m_borderVertices[1].pos.z = 0;
	
	m_borderVertices[3].pos.x = m_vertices[2].pos.x - 1;
	m_borderVertices[3].pos.y = m_vertices[2].pos.y - 1;
	m_borderVertices[3].pos.z = 0;
	
	m_borderVertices[2].pos.x = m_vertices[3].pos.x + 1;
	m_borderVertices[2].pos.y = m_vertices[3].pos.y - 1;
	m_borderVertices[2].pos.z = 0;
	
	[m_borderColor getComponents:(CGFloat*)&m_borderVertices[0].color.r];
	[m_borderColor getComponents:(CGFloat*)&m_borderVertices[1].color.r];
	[m_borderColor getComponents:(CGFloat*)&m_borderVertices[2].color.r];
	[m_borderColor getComponents:(CGFloat*)&m_borderVertices[3].color.r];
	
	
	m_noRedraw = YES;
	
	// Set offsets
	self.propViewOffsetX		= 0.0f;
	self.propViewOffsetY		= 0.0f;
	self.propViewOffsetXMin		= -m_texSize.width * 2;
	self.propViewOffsetXMax		= m_texSize.width * 2;
	self.propViewOffsetYMin		= -m_texSize.height * 2;
	self.propViewOffsetYMax		= m_texSize.height * 2;
	
	// Fit texture
	NSRect viewRect = [self visibleRect];
	
	float zoomX = (float)viewRect.size.width / (float)m_texSize.width - 0.05f;
	if (zoomX > 1.0f)
		zoomX = 1.0f;
	
	float zoomY = (float)viewRect.size.height / (float)m_texSize.height - 0.05f;
	if (zoomY > 1.0f)
		zoomY = 1.0f;
	
	self.propViewZoom = (zoomX > zoomY) ? zoomY : zoomX;
	
	m_noRedraw = NO;
	
	[self drawRect:[self frame]];
}

// Cleans all OpenGL stuff
//-----------------------------------------------------------------------------------------------
- (void)cleanResources
{
	[[self openGLContext] makeCurrentContext];
	
	glDeleteTextures(1, &m_oglTexture);
	m_oglTexture = 0;
}

// Renders current texture
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
    glOrtho(-floor(rect.size.width / 2.0f), 
			 ceil(rect.size.width / 2.0f),
			-floor(rect.size.height / 2.0f),
			 ceil(rect.size.height / 2.0f),
			-1000.0f, 1000.0f);
	
	glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
	glLoadIdentity();
	
	// Apply scroll and zoom
	glScalef(m_zoom, m_zoom, 1.0f);
	glTranslatef(floor(-m_offsetX), floor(-m_offsetY), 0.0f);
	
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
	
	glDisable( GL_TEXTURE_2D );
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
	
	glVertexPointer(3, GL_FLOAT, sizeof(OGLColorVertex), &m_borderVertices[0]);
	glColorPointer(4, GL_FLOAT, sizeof(OGLColorVertex), &m_borderVertices[0].color);
	
	glLineWidth(1.0f);
	
	glDrawArrays(GL_LINE_LOOP, 0, 4);
	
	glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
	glMatrixMode(GL_PROJECTION);
    glPopMatrix();
}

// **** ACTIONS IMPLEMENTATION ****

// Resets current offsets
//-----------------------------------------------------------------------------------------------

- (IBAction) resetOffsets:(id)sender
{
	self.propViewOffsetX	= 0.0f;
	self.propViewOffsetY	= 0.0f;
	
	[self drawRect:[self frame]];
}

@end
