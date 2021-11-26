//===============================================================================================
//  File:		TilesetView.m
//  Project:	OriginGameToolkit
//	Desc:		Tileset view implementation
//
//  Created by Max Vasin on 4/26/11.
//  Copyright 2011 Origin Games. All rights reserved.
//===============================================================================================

// ***** IMPORTS *****

#import "TilesetView.h"
#import "AppDelegate.h"
#import "Project.h"

// ***** PRIVATE METHODS *****

@interface TilesetView (Private)

- (void)	updateCheckerTexture;
- (void)	updateTexture:(GLuint*)pTexID  toSize:(IntSize*)pSize withPath:(NSString*)path;
- (void)	updateBackground;
- (void)	renderBackground;
- (void)	renderFrames;
- (void)	cleanResources;
- (void)	rebuildResources;
- (void)	clear;
- (void)	moveObserver:(id)oldObject toObject:(id)newObject forKey:(NSString*)key;
- (void)	observeProject:(Project*)project;
- (void)	observeTileset:(ProjectTilesetItem*)tileset;
- (void)	observeTilemap:(ProjectTileMapItem*)tilemap;
- (void)	updateBaseTexture;
- (void)	updateFrames;
- (BOOL)	isOverTile:(NSPoint)point Tile:(IntPoint*)tile TileOnView:(IntPoint*)viewtile;

@end


// ***** IMPLEMENTATION *****

@implementation TilesetView

//###############################################################################################
// PROPERTIES
//###############################################################################################

// Synthesized
//-----------------------------------------------------------------------------------------------
@synthesize propViewOffsetXMin	= m_offsetXMin;
@synthesize propViewOffsetXMax	= m_offsetXMax;
@synthesize propViewOffsetYMin	= m_offsetYMin;
@synthesize propViewOffsetYMax	= m_offsetYMax;
@synthesize propTileMapMode		= m_tilemapMode;

// X view offset
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

// Y view offset
//-----------------------------------------------------------------------------------------------
- (GLfloat)propViewOffsetY
{
	return m_offsetYMax - m_offsetY;
}

- (void)setPropViewOffsetY:(GLfloat)offset
{
	m_offsetY = m_offsetYMax - offset;
	[self drawRect:[self frame]];
}

// View zooming
//-----------------------------------------------------------------------------------------------
- (void)setPropViewZoom:(GLfloat)zoom
{
	m_zoom = zoom;
	[self updateFrames];
	[self drawRect:[self frame]];
}

- (GLfloat)propViewZoom
{
	return m_zoom;
}

// Checker color
//-----------------------------------------------------------------------------------------------
- (void)setPropCheckerColor:(NSColor*)color
{
	if (color != m_colorChecker)
	{
		[m_colorChecker release];
		
		m_colorChecker = [color copy];
		
		[self updateBackground];
		[self drawRect:[self frame]];
	}
}

- (NSColor*)propCheckerColor
{
	return m_colorChecker;
}

//###############################################################################################
// OBSERVATION CONTROL
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

// observeProject: start watching for project change
//-----------------------------------------------------------------------------------------------
- (void)observeProject:(Project*)project
{
	if (m_obsrvProject != project)
	{
		if (m_tilemapMode)
			[self moveObserver:m_obsrvProject toObject:project forKey:@"propCurrentTileMapItem"];
		else
			[self moveObserver:m_obsrvProject toObject:project forKey:@"propCurrentTilesetItem"];
		
		m_obsrvProject = project;
		
		if (m_tilemapMode)
			[self observeTilemap:m_obsrvProject.propCurrentTileMapItem];
		else
			[self observeTileset:m_obsrvProject.propCurrentTilesetItem];
	}
}

// Start watching for tileset properties
//-----------------------------------------------------------------------------------------------
- (void)observeTileset:(ProjectTilesetItem*)tileset
{
	if (m_obsrvTileset != tileset)
	{
		[self moveObserver:m_obsrvTileset toObject:tileset forKey:@"propTileWidth"];
		[self moveObserver:m_obsrvTileset toObject:tileset forKey:@"propTileHeight"];
		[self moveObserver:m_obsrvTileset toObject:tileset forKey:@"propTilesCountX"];
		[self moveObserver:m_obsrvTileset toObject:tileset forKey:@"propTilesCountY"];
		[self moveObserver:m_obsrvTileset toObject:tileset forKey:@"propTexture"];
		[self moveObserver:m_obsrvTileset toObject:tileset forKey:@"propType"];
		[self moveObserver:m_obsrvTileset toObject:tileset forKey:@"propCurrentTile"];
		
		m_obsrvTileset = tileset;
		
		[self updateBaseTexture];
		[self updateFrames];
		[self drawRect:[self frame]];
	}
}

// Start watching for tilemap properties
//-----------------------------------------------------------------------------------------------
- (void)observeTilemap:(ProjectTileMapItem*)tilemap
{
	if (m_obsrvTilemap != tilemap)
	{
		[self moveObserver:m_obsrvTilemap toObject:tilemap forKey:@"propTileset"];
		[self moveObserver:m_obsrvTilemap toObject:tilemap forKey:@"propEditTile"];
		
		m_obsrvTilemap = tilemap;
		
		[self observeTileset:tilemap.propTileset];
	}
}


// Observing values changing
//-----------------------------------------------------------------------------------------------
- (void)observeValueForKeyPath:(NSString*)keyPath ofObject:(id)object change:(NSDictionary*)change context:(void*)context
{
	id newValue = [change valueForKey:NSKeyValueChangeNewKey];
	
	if (newValue == [NSNull null])
		newValue = nil;
	
	// Observing objects change
	
	if		([keyPath isEqualToString:@"propCurrentProject"])
		[self observeProject:newValue];
	
	else if	([keyPath isEqualToString:@"propCurrentTilesetItem"])
	{
		if (!m_tilemapMode)
			[self observeTileset:newValue];
	}
	
	
	else if	([keyPath isEqualToString:@"propCurrentTileMapItem"])
	{
		if (m_tilemapMode)
			[self observeTilemap:newValue];
	}
	
	else if	([keyPath isEqualToString:@"propTileset"])
	{
		if (m_tilemapMode)
			[self observeTileset:newValue];
	}
	
	// Observing preferences changings
	
	// Observing animation changings
	
	if		([keyPath isEqualToString:@"propTexture"])
		[self updateBaseTexture];
	
	
	[self updateBackground];
	[self updateFrames];
	[self drawRect:[self frame]];
}

//###############################################################################################
// INITIALIZATION\DESTRUCTION
//###############################################################################################

// Awakening from NIB
//-----------------------------------------------------------------------------------------------
- (void)awakeFromNib
{
	m_contextReady	= NO;
	
	self.propCheckerColor = [NSColor colorWithCalibratedRed:0.0f green:0.5f blue:0.5f alpha:1.0f];
	
	// Connect to application preferences container to reflect its changes immediately
	// Connect to tab view to watch items changing
	
	
	AppDelegate* appDelegate = (AppDelegate*)[NSApp delegate];
	
	[appDelegate addObserver:self forKeyPath:@"propCurrentProject" options:NSKeyValueObservingOptionNew context:nil];
	
	[self observeProject:appDelegate.propCurrentProject];
}

//###############################################################################################
// VIEW DRAWING CONTROL
//###############################################################################################

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
												   options:(NSTrackingMouseEnteredAndExited | NSTrackingMouseMoved | NSTrackingActiveAlways)
													 owner:self userInfo:nil];
		
		[self addTrackingArea:m_trackArea];
	}
	
	[self updateBackground];
	[self updateFrames];
}

// View drawing
//-----------------------------------------------------------------------------------------------
- (void)drawRect:(NSRect)dirtyRect
{
	[[self openGLContext] makeCurrentContext];
	
	// Our context not initialized
	if (!m_contextReady)
	{
		m_contextReady	= YES;
		
		self.propViewZoom	= 1.0f;
		
		m_glFrameSelect = [[GLFrame alloc] init];
		m_glFrameSelect.propBorder	= 5;
		
		m_glFrameMove	= [[GLFrame alloc] init];
		m_glFrameMove.propBorder	= 5;
		
		[self rebuildResources];
		
		[self reshape];
	}

	[self renderBackground];
	
	[self renderFrames];
	
	[[self openGLContext] flushBuffer];
}

//###############################################################################################
// OPENGL RENDERING CONTROL
//###############################################################################################

// Clears OpenGL backbuffer
//-----------------------------------------------------------------------------------------------
- (void)clear
{
	glClearColor(0.0f, 1.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

// Background checker texture rendering
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

// Tiles frames rendering
//-----------------------------------------------------------------------------------------------
- (void)renderFrames
{
	if (!m_oglTexture || !m_numFramesVerts)
		return;
	
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
	
	// Center view by Y axis
	glTranslatef((float)m_glFrameSelect.propBorder, rect.size.height - (float)m_glFrameSelect.propBorder, 0.0f);
	
	// Apply zoom
	
	glTranslatef(-m_offsetX, m_offsetY, 0.0f);

	glScalef(m_zoom, m_zoom, 1.0f);
	
	glTranslatef(m_fullOffset / 2.0f, 0.0f, 0.0f);
	
	// Preparing sources
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
	
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
		glBindTexture( GL_TEXTURE_2D, m_oglTexture );
		
		glDrawArrays(GL_TRIANGLE_STRIP, i*4, 4);
	}
	
	[m_glFrameSelect render];
	[m_glFrameMove render];
	
	// Restore transformations
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
    glPopMatrix();
}

//###############################################################################################
// RESOURCES BUILDING
//###############################################################################################

// All resources rebuilding
//-----------------------------------------------------------------------------------------------
- (void)rebuildResources
{
	[[self openGLContext] makeCurrentContext];
	
	[self cleanResources];
	
	[self updateBaseTexture];
	[self updateCheckerTexture];
	[self updateFrames];
}

// Cleans all OpenGL stuff
//-----------------------------------------------------------------------------------------------
- (void)cleanResources
{
	[[self openGLContext] makeCurrentContext];
	
	SAFE_FREE(m_vertsFrames);
	m_numFramesVerts = 0;
	m_numFrames		 = 0;
	
	glDeleteTextures(1, &m_oglTexture);
	glDeleteTextures(1, &m_oglCheckerTexture);
	
	m_oglTexture		= 0;
	m_oglCheckerTexture = 0;
}

// Recreating tiles frames vertices
//-----------------------------------------------------------------------------------------------
- (void)updateFrames
{
	SAFE_FREE(m_vertsFrames);
	
	m_numFramesVerts = 0;
	m_numFrames		 = 0;
	
	if (!m_texSize.width || !m_texSize.height)
		return;
	
	m_glFrameSelect.propWidth		= m_obsrvTileset.propTileWidth;
	m_glFrameSelect.propHeight		= m_obsrvTileset.propTileHeight;
	
	m_glFrameMove.propWidth			= m_obsrvTileset.propTileWidth;
	m_glFrameMove.propHeight		= m_obsrvTileset.propTileHeight;
	
	// How much tiles we can fit in frame width
	
	NSRect frame	   = [self visibleRect];
	
	frame.size.width  -= (float)m_glFrameSelect.propBorder;
	frame.size.height -= (float)m_glFrameSelect.propBorder;
	
	
	int tilesByX		= floor(frame.size.width / m_obsrvTileset.propTileWidth);
	
	IntPoint curTile;
	
	if (m_tilemapMode)
		curTile = m_obsrvTilemap.propEditTile;
	else
		curTile = m_obsrvTileset.propCurrentTile;
	
	// Find location of current tileset tile
	
	int selOffset = curTile.y * m_obsrvTileset.propTilesCountX + curTile.x;
	
	int selY	  = floor(selOffset / tilesByX);
	int selX	  = selOffset - (selY * tilesByX);
	
	m_glFrameSelect.propPositionX = selX * m_obsrvTileset.propTileWidth;
	m_glFrameSelect.propPositionY = - ( (selY + 1) * m_obsrvTileset.propTileHeight);
	m_glFrameSelect.propVisible   = YES;
	
	
	// Allocating space
	
	m_numFrames			= m_obsrvTileset.propTilesCount;
	m_numFramesVerts	= m_numFrames * 4;
	
	m_vertsFrames    = calloc(m_numFramesVerts, sizeof(OGLVertex));
	
	// Precalc
	
	float du = (float)m_obsrvTileset.propTileWidth / (float)m_texSize.width;
	float dv = (float)m_obsrvTileset.propTileHeight / (float)m_texSize.height;
	NSColor* clrWhite = [[NSColor colorWithCalibratedRed:1.0f green:1.0f blue:1.0f alpha:1.0f] autorelease];
	
	// Start frames building
	
	uint indX = 0, indY = 0;
	uint texX = 0, texY = 0;
	OGLVertex* v0, *v1, *v2, *v3;
	
	for (uint tileIndex = 0; tileIndex < m_obsrvTileset.propTilesCount; tileIndex++)
	{
		v0 = &m_vertsFrames[tileIndex * 4 + 0];
		v1 = &m_vertsFrames[tileIndex * 4 + 1];
		v2 = &m_vertsFrames[tileIndex * 4 + 2];
		v3 = &m_vertsFrames[tileIndex * 4 + 3];
		
		float L		= indX * (float)m_obsrvTileset.propTileWidth;
		float R		= L + (float)m_obsrvTileset.propTileWidth;
		float T		= -(float)(indY * m_obsrvTileset.propTileHeight);
		float B		= T - (float)m_obsrvTileset.propTileHeight;
		
		float midW	= floor(indX * m_obsrvTileset.propTileWidth + (m_obsrvTileset.propTileWidth / 2.0f));
		float midH	= -floor(indY * m_obsrvTileset.propTileHeight + (m_obsrvTileset.propTileHeight / 2.0f));
		
		float umin	= texX * du;
		float vmin	= texY * dv;
		float umax	= umin + du;
		float vmax	= vmin + dv;
		float midu	= umin + du / 2.0f;
		float midv	= vmin + dv / 2.0f;
		
		switch (m_obsrvTileset.propType)
		{
			case eTilesetTypeQuadAlpha:
				v0->pos.x = L; v0->pos.y = T; v0->pos.z = 0.0f; v0->uv.x = umin; v0->uv.y = vmin;
				v1->pos.x = R; v1->pos.y = T; v1->pos.z = 0.0f; v1->uv.x = umax; v1->uv.y = vmin;
				v2->pos.x = L; v2->pos.y = B; v2->pos.z = 0.0f; v2->uv.x = umin; v2->uv.y = vmax;
				v3->pos.x = R; v3->pos.y = B; v3->pos.z = 0.0f; v3->uv.x = umax; v3->uv.y = vmax;
				break;
				
			case eTilesetTypeDiamond:
				v0->pos.x = L; v0->pos.y = midH; v0->pos.z = 0.0f; v0->uv.x = umin; v0->uv.y = midv;
				v1->pos.x = midW; v1->pos.y = T; v1->pos.z = 0.0f; v1->uv.x = midu; v1->uv.y = vmin;
				v2->pos.x = midW; v2->pos.y = B; v2->pos.z = 0.0f; v2->uv.x = midu; v2->uv.y = vmax;
				v3->pos.x = R; v3->pos.y = midH; v3->pos.z = 0.0f; v3->uv.x = umax; v3->uv.y = midv;
				break;
		}
		
		[clrWhite getComponents:(CGFloat*)&v0->color];
		[clrWhite getComponents:(CGFloat*)&v1->color];
		[clrWhite getComponents:(CGFloat*)&v2->color];
		[clrWhite getComponents:(CGFloat*)&v3->color];
		
		texX++;
		
		if (texX >= m_obsrvTileset.propTilesCountX)
		{
			texX = 0;
			texY++;
		}
		
		indX++;
		
		if (indX >= tilesByX)
		{
			indX = 0;
			indY++;
		}
	}
	
	self.propViewOffsetYMin = 0.0f;
	
	int diff = (indY * m_obsrvTileset.propTileHeight * m_zoom) - frame.size.height;
	
	self.propViewOffsetYMax = (diff > 0) ? diff + m_glFrameSelect.propBorder : 0;
	
	if (self.propViewOffsetY > self.propViewOffsetYMax)
		self.propViewOffsetY = self.propViewOffsetYMax;
	
	int dimX = (tilesByX > m_obsrvTileset.propTilesCount) ? m_obsrvTileset.propTilesCount : tilesByX;
	
	
	diff = (dimX * m_obsrvTileset.propTileWidth * m_zoom) - frame.size.width;
	
	m_fullOffset = (diff < 0) ? -diff : 0;
	
	self.propViewOffsetXMax = (diff > 0) ? diff + m_glFrameSelect.propBorder : 0;
	
	if (self.propViewOffsetX > self.propViewOffsetXMax)
		self.propViewOffsetX = self.propViewOffsetXMax;
	
}

// Recreating background sprite
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
	m_vertsBack[0].uv.y  = v;
	
	m_vertsBack[1].pos.x = w;
	m_vertsBack[1].pos.y = 0.0f;
	m_vertsBack[1].pos.z = 0.0f;
	m_vertsBack[1].uv.x  = u;
	m_vertsBack[1].uv.y  = v;
	
	m_vertsBack[2].pos.x = 0.0f;
	m_vertsBack[2].pos.y = h;
	m_vertsBack[2].pos.z = 0.0f;
	m_vertsBack[2].uv.x  = 0.0f;
	m_vertsBack[2].uv.y  = 0.0f;
	
	m_vertsBack[3].pos.x = w;
	m_vertsBack[3].pos.y = h;
	m_vertsBack[3].pos.z = 0.0f;
	m_vertsBack[3].uv.x  = u;
	m_vertsBack[3].uv.y  = 0.0f;
	
	[m_colorChecker getComponents:(CGFloat*)&m_vertsBack[0].color];
	[m_colorChecker getComponents:(CGFloat*)&m_vertsBack[1].color];
	[m_colorChecker getComponents:(CGFloat*)&m_vertsBack[2].color];
	[m_colorChecker getComponents:(CGFloat*)&m_vertsBack[3].color];
}

// Updating tileset base texture
//-----------------------------------------------------------------------------------------------
- (void)updateBaseTexture
{
	[[self openGLContext] makeCurrentContext];
	
	glDeleteTextures(1, &m_oglTexture);
	m_oglTexture = 0;
	
	if (!m_obsrvTileset)
		return;
	
	ProjectTextureItem* itemTexture = m_obsrvTileset.propTexture;
	
	if (!itemTexture)
		return;
	
	NSString* texPath = [itemTexture propBuildPath];
	
	[self updateTexture:&m_oglTexture toSize:&m_texSize withPath:texPath];
	
	
	glDeleteTextures(1, &m_oglFrameTexture);
	
	IntSize texSize;
	
	texPath = [[NSBundle mainBundle] pathForResource:@"border_texture" ofType:@"png" inDirectory:@"icons"];
	
	[self updateTexture:&m_oglFrameTexture toSize:&texSize withPath:texPath];
	
	m_glFrameSelect.propTextureSize = texSize;
	m_glFrameSelect.propTexture		= m_oglFrameTexture;
	m_glFrameSelect.propColor		= [NSColor colorWithCalibratedRed:1.0f green:1.0f blue:0.0f alpha:1.0f];
	
	m_glFrameMove.propTextureSize	= texSize;
	m_glFrameMove.propTexture		= m_oglFrameTexture;
	m_glFrameMove.propColor			= [NSColor colorWithCalibratedRed:1.0f green:1.0f blue:0.0f alpha:0.5f];
}

// Updating background checker texture
//-----------------------------------------------------------------------------------------------
- (void)updateCheckerTexture
{
	NSString* texPath = [[NSBundle mainBundle] pathForResource:@"gray_checkers" ofType:@"png" inDirectory:@"icons"];
	
	[self updateTexture:&m_oglCheckerTexture toSize:&m_texCheckerSize withPath:texPath];
}

// Updating texture by path
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
// MOUSE INPUT CONTROL
//############################################

// Returns YES if point over any tile and returns tile coordinates
//-----------------------------------------------------------------------------------------------
- (BOOL)isOverTile:(NSPoint)point Tile:(IntPoint*)tile TileOnView:(IntPoint*)viewtile;
{
	NSRect frame = [self visibleRect];
	
	// Offset correction
	
	point.x += m_offsetX - (float)m_glFrameSelect.propBorder;
	point.y = frame.size.height - point.y + m_offsetY - (float)m_glFrameSelect.propBorder;
	
	
	// Apply zoom
	
	if (m_zoom)
	{
		point.y /= m_zoom;
		point.x /= m_zoom;
	}
	
	point.x -= (m_fullOffset / 2.0f);
	
	if ( (point.x < 0) || (point.y < 0) )
		return NO;
	
	// check X
	
	int tilesByX = floor(frame.size.width / m_obsrvTileset.propTileWidth);
	
	if (point.x >= tilesByX * m_obsrvTileset.propTileWidth)
		return NO;
	
	// check Y
	
	int tilesByY = floor(m_obsrvTileset.propTilesCount / tilesByX);
	
	if ((tilesByY * tilesByX) < m_obsrvTileset.propTilesCount)
		tilesByY++;
	
	if (point.y >= tilesByY * m_obsrvTileset.propTileHeight)
		return NO;
	
	// find tile
	
	int indX = floor(point.x / m_obsrvTileset.propTileWidth);
	int indY = floor(point.y / m_obsrvTileset.propTileHeight);
	int offset = indY * tilesByX + indX;

	if (offset >= m_obsrvTileset.propTilesCount)
		return NO;
	
	if (viewtile)
	{
		viewtile->x = indX;
		viewtile->y = indY;
	}
		
	if (tile)
	{
		tile->y	 = floor(offset/ m_obsrvTileset.propTilesCountX);
		tile->x  = offset - tile->y * m_obsrvTileset.propTilesCountX;
	}
	
	return YES;
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
	//NSPoint loc = [self getEventLocation:theEvent];
	
	[self drawRect:[self frame]];
}

// mouseUp: left mouse button released
//-----------------------------------------------------------------------------------------------
- (void)mouseUp:(NSEvent*)theEvent
{
	NSPoint loc = [self getEventLocation:theEvent];
	
	IntPoint tile, viewtile;
	
	if ([self isOverTile:loc Tile:&tile TileOnView:&viewtile])
	{
		if (m_tilemapMode)
			m_obsrvTilemap.propEditTile	   = tile;
		else
			m_obsrvTileset.propCurrentTile = tile;
	}
	
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
	//NSPoint loc = [self getEventLocation:theEvent];
	
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
	NSPoint loc = [self getEventLocation:theEvent];
	
	IntPoint tile, viewtile;
	
	if ([self isOverTile:loc Tile:&tile TileOnView:&viewtile])
	{
		m_glFrameMove.propVisible = YES;
		
		m_glFrameMove.propPositionX	= viewtile.x * m_obsrvTileset.propTileWidth;
		m_glFrameMove.propPositionY = -((viewtile.y + 1) * m_obsrvTileset.propTileHeight);
	}
	else
		m_glFrameMove.propVisible = NO;
	
	[self drawRect:[self frame]];
}

// mouseExited: mouse exited frame tracking area
//-----------------------------------------------------------------------------------------------
- (void)mouseExited:(NSEvent*)theEvent 
{
	m_glFrameMove.propVisible = NO;
	
	[self drawRect:[self frame]];
}

// scrollWheel: scrolling
//-----------------------------------------------------------------------------------------------
- (void)scrollWheel:(NSEvent*)theEvent
{
	CGFloat offset = self.propViewOffsetY + [theEvent deltaY] * 10;
	
	if (offset < 0)
		offset = 0;
	
	if (offset > self.propViewOffsetYMax)
		offset = self.propViewOffsetYMax;
		
	self.propViewOffsetY = offset;

	
	NSPoint loc = [self getEventLocation:theEvent];
	
	IntPoint tile, viewtile;
	
	if ([self isOverTile:loc Tile:&tile TileOnView:&viewtile])
	{
		m_glFrameMove.propVisible = YES;
		
		m_glFrameMove.propPositionX	= viewtile.x * m_obsrvTileset.propTileWidth;
		m_glFrameMove.propPositionY = -((viewtile.y + 1) * m_obsrvTileset.propTileHeight);
	}
	else
		m_glFrameMove.propVisible = NO;

	[self drawRect:[self frame]];
}

// Accepts first moust (click-through)
//-----------------------------------------------------------------------------------------------
- (BOOL)acceptsFirstMouse:(NSEvent*)theEvent
{
	return YES;
}



@end
