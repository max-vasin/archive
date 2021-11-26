//===============================================================================================
//  File:		TilemapView.m
//  Project:	OriginGameToolkit
//	Desc:		Tilemap view class implementation
//
//  Created by Max Vasin on 5/4/11.
//  Copyright 2011 Origin Games. All rights reserved.
//===============================================================================================

// ***** IMPORTS *****

#import "TilemapView.h"
#import "Project.h"
#import "AppDelegate.h"

// ***** PRIVATE METHODS *****

@interface TilemapView (Private)

- (void)		updateCheckerTexture;
- (void)		updateTexture:(GLuint*)pTexID  toSize:(IntSize*)pSize withPath:(NSString*)path;
- (void)		updateBackground;
- (void)		renderBackground;
- (void)		renderTiles;
- (void)		renderHexGrid:(float)offset;
- (void)		cleanResources;
- (void)		rebuildResources;
- (void)		clear;
- (void)		moveObserver:(id)oldObject toObject:(id)newObject forKey:(NSString*)key;
- (void)		observeProject:(Project*)project;
- (void)		observeTilemap:(ProjectTileMapItem*)tilemap;

- (void)		updateTilesetTexture;
- (void)		updateTiles;
- (void)		updateStatic;

- (BOOL)		isOverNode:(NSPoint)point	Node:(IntPoint*)node;
- (HexGridCell*)getGridCell:(NSPoint)point;
- (float)		getDist:(NSPoint)point		Edge1:(NSPoint)a	Edge2:(NSPoint)b;
- (NSPoint)		getStaticPoint:(NSPoint)point;
- (NSPoint)		staticToView:(NSPoint)point;

@end

// ***** IMPLEMENTATION *****

@implementation TilemapView

//###############################################################################################
// PROPERTIES
//###############################################################################################

// Synthesized
//-----------------------------------------------------------------------------------------------
@synthesize propViewOffsetXMin	= m_offsetXMin;
@synthesize propViewOffsetXMax	= m_offsetXMax;
@synthesize propViewOffsetYMin	= m_offsetYMin;
@synthesize propViewOffsetYMax	= m_offsetYMax;
@synthesize propCurrentCellType	= m_currentCellType;

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
	[self updateTiles];
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

// Static layer edit mode
//-----------------------------------------------------------------------------------------------
- (void)setPropStaticEditMode:(enStaticEditMode)mode
{
	m_staticMode = mode;
	
	[self updateStatic];
}

- (enStaticEditMode)propStaticEditMode
{
	return m_staticMode;
}

// Edit layer
//-----------------------------------------------------------------------------------------------
- (void)setPropEditLayer:(enEditLayer)layer
{
	m_editLayer = layer;
	
	[self drawRect:[self frame]];
	
}

- (enEditLayer)propEditLayer
{
	return m_editLayer;
}

// Edit layers array (for list)
//-----------------------------------------------------------------------------------------------
- (NSArray*)propEditLayers
{
	return [NSArray arrayWithObjects:@"tiles", @"static", @"regions", @"hexgrid", nil];
}



//###############################################################################################
// INITIALIZATION\DESTRUCTION
//###############################################################################################

// Awakening from NIB
//-----------------------------------------------------------------------------------------------
- (void)awakeFromNib
{
	m_contextReady	= NO;
	
	//self.propCheckerColor = [NSColor colorWithCalibratedRed:1.0f green:1.0f blue:1.0f alpha:1.0f];
	
	self.propCheckerColor = [NSColor cyanColor];
	
	m_static	= [[NSMutableArray alloc] initWithCapacity:10];
	m_hexagon	= [[GLHexagon alloc] init];
	
	// Connect to application preferences container to reflect its changes immediately
	// Connect to tab view to watch items changing
	
	AppDelegate* appDelegate = (AppDelegate*)[NSApp delegate];
	
	[appDelegate addObserver:self forKeyPath:@"propCurrentProject" options:NSKeyValueObservingOptionNew context:nil];
	
	[self observeProject:appDelegate.propCurrentProject];
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

// Start watching for project change
//-----------------------------------------------------------------------------------------------
- (void)observeProject:(Project*)project
{
	if (m_obsrvProject != project)
	{
		[self moveObserver:m_obsrvProject toObject:project forKey:@"propCurrentTileMapItem"];
		
		m_obsrvProject = project;
		
		[self observeTilemap:m_obsrvProject.propCurrentTileMapItem];
	}
}

// Start watching for tilemap change
//-----------------------------------------------------------------------------------------------
- (void)observeTilemap:(ProjectTileMapItem*)tilemap
{
	if (m_obsrvTilemap != tilemap)
	{
		[self moveObserver:m_obsrvTilemap toObject:tilemap forKey:@"propMapWidth"];
		[self moveObserver:m_obsrvTilemap toObject:tilemap forKey:@"propMapHeight"];
		[self moveObserver:m_obsrvTilemap toObject:tilemap forKey:@"propTileset"];
		[self moveObserver:m_obsrvTilemap toObject:tilemap forKey:@"propBackColor"];
		[self moveObserver:m_obsrvTilemap toObject:tilemap forKey:@"propEditMultiSprite"];
		[self moveObserver:m_obsrvTilemap toObject:tilemap forKey:@"propCurrentStatic"];
		[self moveObserver:m_obsrvTilemap toObject:tilemap forKey:@"propGridWidth"];
		[self moveObserver:m_obsrvTilemap toObject:tilemap forKey:@"propGridHeight"];
		[self moveObserver:m_obsrvTilemap toObject:tilemap forKey:@"propGridCellEdge"];
		[self moveObserver:m_obsrvTilemap toObject:tilemap forKey:@"propGridAspect"];
		[self moveObserver:m_obsrvTilemap toObject:tilemap forKey:@"propGridOffsetX"];
		[self moveObserver:m_obsrvTilemap toObject:tilemap forKey:@"propGridOffsetY"];
		
		m_obsrvTilemap	= tilemap;
		self.propCheckerColor	= m_obsrvTilemap.propBackColor;
		m_hexagon.propSize		= m_obsrvTilemap.propGridCellEdge;
		m_hexagon.propAspect	= m_obsrvTilemap.propGridAspect;
		
		[m_texStore clear];
		
		[self updateTilesetTexture];
		[self updateTiles];
		[self updateStatic];
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
	{
		[self observeProject:newValue];
	}
	else if	([keyPath isEqualToString:@"propCurrentTileMapItem"])
	{
		[self observeTilemap:newValue];
	}
	
	
	if		([keyPath isEqualToString:@"propTileset"])
	{
		[self updateTilesetTexture];
	}
	else if ([keyPath isEqualToString:@"propBackColor"])
	{
		self.propCheckerColor = newValue;
	}
	else if ([keyPath isEqualToString:@"propEditMultiSprite"])
	{
		SAFE_RELEASE(m_glMultiSprite);
		m_glMultiSprite = [[GLMultiSprite alloc] initWithContext:[self openGLContext] Item:newValue TextureStore:m_texStore];
		m_glMultiSprite.propColor = [NSColor colorWithCalibratedRed:1.0f green:1.0f blue:1.0f alpha:0.5f];
	}
	else if ([keyPath isEqualToString:@"propCurrentStatic"])
	{
		[self updateStatic];
	}
	else if ([keyPath isEqualToString:@"propGridWidth"])
	{
	
	}
	else if ([keyPath isEqualToString:@"propGridHeight"])
	{
	
	}
	else if ([keyPath isEqualToString:@"propGridCellEdge"])
	{
		m_hexagon.propSize	= [newValue floatValue];
	}
	else if ([keyPath isEqualToString:@"propGridAspect"])
	{
		m_hexagon.propAspect = [newValue floatValue];
	}
	
	[self updateBackground];
	[self updateTiles];
	[self drawRect:[self frame]];
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
	//[self updateTiles];
}

// View drawing
//-----------------------------------------------------------------------------------------------
- (void)drawRect:(NSRect)dirtyRect
{
	if (m_bNoRedraw)
		return;
	
	[[self openGLContext] makeCurrentContext];
	
	// Our context not initialized
	if (!m_contextReady)
	{
		m_contextReady	= YES;
		
		m_texStore = [[TextureStore alloc] initWithContext:[self openGLContext]];
		
		self.propViewZoom	= 1.0f;
				
		[self rebuildResources];
		
		[self reshape];
	}
	
	[self renderBackground];
	
	[self renderTiles];
	
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

// Tiles rendering
//-----------------------------------------------------------------------------------------------
- (void)renderTiles
{
	if (!m_oglTilesetTexture || !m_vertsTilesCount)
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
	ProjectTilesetItem* tileset = m_obsrvTilemap.propTileset;
	
	float offsx = (float)(tileset.propTileWidth * m_obsrvTilemap.propMapHeight) / 2.0f - ((float)tileset.propTileWidth / 2.0f);
	
	// Apply zoom
	glTranslatef(-m_offsetX, rect.size.height + m_offsetY, 0.0f);
	
	glScalef(m_zoom, m_zoom, 1.0f);
	
	glTranslatef(offsx, 0.0f, 0.0f);
	
	
		
	// Preparing sources
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
	
	// Set source
	glVertexPointer(	3, GL_FLOAT, sizeof(OGLVertex), &m_vertsTiles[0].pos);
	glColorPointer(		4, GL_FLOAT, sizeof(OGLVertex), &m_vertsTiles[0].color);
	glTexCoordPointer(	2, GL_FLOAT, sizeof(OGLVertex), &m_vertsTiles[0].uv);
	
	glEnable(GL_TEXTURE_2D);
	
	// Set blending ops
	glEnable( GL_BLEND );
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
	glEnable( GL_TEXTURE_2D );
	
	// Draw frames sequence
	
	for (NSInteger i = 0; i < m_obsrvTilemap.propMapArea; i++)
	{
		glBindTexture( GL_TEXTURE_2D, m_oglTilesetTexture );
		
		glDrawArrays(GL_TRIANGLE_STRIP, i*4, 4);
	}
	
	/*glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	glPushMatrix();
	
	// Apply zoom
	glTranslatef(-m_offsetX, m_offsetY, 0.0f);*/
	
	// Render grid
	
	[self renderHexGrid:offsx];
	
	
	// Render static layer
	
	for (id object in m_static)
	{
		Class objClass = [object class];
		
		if (objClass == [GLMultiSprite class])
		{
			GLMultiSprite* msprite = (GLMultiSprite*)object;
			
			NSColor* color = msprite.propColor;
			
			if (m_editLayer != eEditLayerStatic)
			{
				color = [NSColor colorWithCalibratedRed:[color redComponent]
												  green:[color greenComponent]
												   blue:[color blueComponent]
												  alpha:0.5f];
				
			}
			else
			{
				color = [NSColor colorWithCalibratedRed:[color redComponent]
												  green:[color greenComponent]
												   blue:[color blueComponent]
												  alpha:1.0f];
			}
			msprite.propColor = color;
			
			[msprite render];
		}
	}
	
	if (m_editLayer == eEditLayerStatic && m_staticMode != eStaticEditMove)
	{
		[m_glMultiSprite render];
	}
	
	
	// Restore transformations
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
    glPopMatrix();
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

//###############################################################################################
// RESOURCES BUILDING
//###############################################################################################

// All resources rebuilding
//-----------------------------------------------------------------------------------------------
- (void)rebuildResources
{
	[[self openGLContext] makeCurrentContext];
	
	[self cleanResources];
	
	[self updateCheckerTexture];
	[self updateTilesetTexture];
}

// Cleans all OpenGL stuff
//-----------------------------------------------------------------------------------------------
- (void)cleanResources
{
	[[self openGLContext] makeCurrentContext];
	
	glDeleteTextures(1, &m_oglCheckerTexture);
	glDeleteTextures(1, &m_oglTilesetTexture);
	
	m_oglCheckerTexture = 0;
	m_oglTilesetTexture = 0;
	
	SAFE_FREE(m_vertsTiles);
}

// Recreating tiles vertices
//-----------------------------------------------------------------------------------------------
- (void)updateTiles
{
	SAFE_FREE(m_vertsTiles);
	
	m_vertsTilesCount = 0;
	
	if (!m_obsrvTilemap)
		return;
	
	if (!m_texTilesetSize.width || !m_texTilesetSize.height)
		return;
	
	// Realloc vertices
	
	m_vertsTilesCount = m_obsrvTilemap.propMapArea * 4;
	
	m_vertsTiles = (OGLVertex*)calloc(m_vertsTilesCount, sizeof(OGLVertex));
	
	
	// Precalc
	
	ProjectTilesetItem* tileset = m_obsrvTilemap.propTileset;
	
	float tw  = (float)tileset.propTileWidth;
	float th  = (float)tileset.propTileHeight;
	float dtw = tw / 2.0f;
	float dth = th / 2.0f;
	float du  = tw / (float)m_texTilesetSize.width;
	float dv  = th / (float)m_texTilesetSize.height;
	
	NSColor* clrWhite = [NSColor colorWithCalibratedRed:1.0f green:1.0f blue:1.0f alpha:1.0f];
	NSColor* clrRed   = [NSColor colorWithCalibratedRed:1.0f green:0.5f blue:0.5f alpha:1.0f];
	
	int startY = 0, startX = 0, index = 0;
	OGLVertex* v0, *v1, *v2, *v3;
	int bl = 0, br = 0, bt = 0, bb = 0;
	
	while (startX < m_obsrvTilemap.propMapWidth)
	{
		int curY = startY;
		int curX = startX;
		
		while ( (curY >= 0) && (curX < m_obsrvTilemap.propMapWidth) )
		{
			v0 = &m_vertsTiles[index * 4 + 0];
			v1 = &m_vertsTiles[index * 4 + 1];
			v2 = &m_vertsTiles[index * 4 + 2];
			v3 = &m_vertsTiles[index * 4 + 3];
			
			TileMapNode* node = [m_obsrvTilemap getNodeAtX:curX Y:curY];
			
			float umin = node.propTile.propIndexX * du;
			float vmin = node.propTile.propIndexY * dv;
			float umax = umin + du;
			float vmax = vmin + dv;
			
			float l	= curX * dtw - curY * dtw;
			float r = l + tw;
			float t = -curY * dth - curX * dth;
			float b = t - th;
			
			if (l < bl)
				bl = l;
			
			if (r > br)
				br = r;
			
			if (t > bt)
				bt = t;
			
			if (b < bb)
				bb = b;
					
			v0->pos.x = l;	v0->pos.y = t; v0->pos.z = 0; v0->uv.x = umin; v0->uv.y = vmin;
			v1->pos.x = r;	v1->pos.y = t; v1->pos.z = 0; v1->uv.x = umax; v1->uv.y = vmin;
			v2->pos.x = l;	v2->pos.y = b; v2->pos.z = 0; v2->uv.x = umin; v2->uv.y = vmax;
			v3->pos.x = r;	v3->pos.y = b; v3->pos.z = 0; v3->uv.x = umax; v3->uv.y = vmax;
			
			if ( (curX == m_mouseNode.x) && (curY == m_mouseNode.y) && m_mouseOverNode && (m_editLayer == eEditLayerTiles))
			{
				[clrRed getComponents:(CGFloat*)&v0->color];
				[clrRed getComponents:(CGFloat*)&v1->color];
				[clrRed getComponents:(CGFloat*)&v2->color];
				[clrRed getComponents:(CGFloat*)&v3->color];
				
			}
			else
			{
				[clrWhite getComponents:(CGFloat*)&v0->color];
				[clrWhite getComponents:(CGFloat*)&v1->color];
				[clrWhite getComponents:(CGFloat*)&v2->color];
				[clrWhite getComponents:(CGFloat*)&v3->color];
			}
			
			curY--;
			curX++;
			index++;
		}
		
		if (startY == (m_obsrvTilemap.propMapHeight - 1))
		{
			startX++;
		}
		else
			startY++;
	}
	
	NSRect frame = [self visibleRect];
	
	self.propViewOffsetYMin = 0.0f;
	
	int diff = (abs(bb - bt) * m_zoom) - frame.size.height;
	
	self.propViewOffsetYMax = (diff > 0) ? diff : 0;
	
	if (self.propViewOffsetY > self.propViewOffsetYMax)
		self.propViewOffsetY = self.propViewOffsetYMax;
	
	diff = (abs(br - bl) * m_zoom) - frame.size.width;
	
	self.propViewOffsetXMax = (diff > 0) ? diff : 0;
	
	if (self.propViewOffsetX > self.propViewOffsetXMax)
		self.propViewOffsetX = self.propViewOffsetXMax;
	
	m_mapDrawOffset = (float)(tileset.propTileWidth * m_obsrvTilemap.propMapHeight) / 2.0f - ((float)tileset.propTileWidth / 2.0f);
}

// Rebuilding static objects
//-----------------------------------------------------------------------------------------------
- (void)updateStatic
{
	[m_static removeAllObjects];
	
	for (uint index = 0; index < m_obsrvTilemap.propStaticCount; index++)
	{
		StaticWrapper* wrapper = [m_obsrvTilemap getStaticAtIndex:index];
		
		Class	itemClass	= [wrapper.propItem class];
		
		if (itemClass == [ProjectMultiSpriteItem class])
		{
			GLMultiSprite* graph = [[GLMultiSprite alloc] initWithContext:[self openGLContext] Item:(ProjectMultiSpriteItem*)wrapper.propItem TextureStore:m_texStore];
			
			if (wrapper == m_obsrvTilemap.propCurrentStatic && m_editLayer == eEditLayerStatic && m_staticMode == eStaticEditMove)
				graph.propColor = [NSColor colorWithCalibratedRed:1.0f green:0.0f blue:0.0f alpha:1.0f];
			else
				graph.propColor = [NSColor colorWithCalibratedRed:1.0f green:1.0f blue:1.0f alpha:1.0f];
			
			graph.propPosition = [self staticToView:wrapper.propPositionFloat];
			
			[m_static addObject:graph];
			
			[graph release];
		}
	}
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

// Updating tileset texture
//-----------------------------------------------------------------------------------------------
- (void)updateTilesetTexture
{
	[[self openGLContext] makeCurrentContext];
	
	glDeleteTextures(1, &m_oglTilesetTexture);
	m_oglTilesetTexture = 0;
	
	if (!m_obsrvTilemap.propTileset)
		return;
	
	ProjectTextureItem* itemTexture = m_obsrvTilemap.propTileset.propTexture;
	
	if (!itemTexture)
		return;
	
	NSString* texPath = [itemTexture propBuildPath];
	
	[self updateTexture:&m_oglTilesetTexture toSize:&m_texTilesetSize withPath:texPath];
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
// HEXAGONAL GRID
//############################################

// Updates hexagon shape vertices
//-----------------------------------------------------------------------------------------------
- (void)renderHexGrid:(float)offset
{
	if (!m_obsrvTilemap)
		return;
	
	ProjectTileMapRootItem* rootTilemaps = (ProjectTileMapRootItem*)m_obsrvTilemap.propParentItem;
	
	NSRect frame = [self frame];
	
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	
	// Apply zoom
	glTranslatef(-m_offsetX, m_offsetY, 0.0f);
	
	glScalef(m_zoom, m_zoom, 1.0f);
	
	glTranslatef(m_obsrvTilemap.propGridOffsetX, -m_obsrvTilemap.propGridOffsetY, 0.0f);
	
	//glTranslatef(offset, 0.0f, 0.0f);
	
	
	
	for (int y = 0; y < m_obsrvTilemap.propGridHeight; y++)
		for (int x = 0; x < m_obsrvTilemap.propGridWidth; x++)
		{
			float posX = x * 2 * m_hexagon.propProjectionX + (y & 1) * m_hexagon.propProjectionX;
			float posY = (frame.size.height / m_zoom) - (y * (m_hexagon.propProjectionY + m_hexagon.propSize) * m_hexagon.propAspect);
			
			HexGridCell* cell = [m_obsrvTilemap cellAtX:x Y:y];
			
			TilemapGridCellType* cellType = nil;
			
			if (cell == m_currentCell)
			{
				cellType = m_currentCellType;
			}
			else
			{
				cellType = [rootTilemaps cellTypeByID:cell.propTypeID];
			}
						
			if (cellType.propID)
			{
				m_hexagon.propColorFill		= cellType.propTransparentColor;
				m_hexagon.propRenderSolid	= YES;
			}
			else
				m_hexagon.propRenderSolid	= NO;
			
			
			glPushMatrix();
			glTranslatef(posX, posY, 0.0f);
			
			[m_hexagon render];
			
			glPopMatrix();
		}
	
	glPopMatrix();
}


//############################################
// MOUSE INPUT CONTROL
//############################################

// Returns hexagonal grid cell from point coordinates
//-----------------------------------------------------------------------------------------------
- (HexGridCell*)getGridCell:(NSPoint)point
{
	// Project point to grid space
	
	NSRect frame = [self frame];
	
	point.x += m_offsetX - m_obsrvTilemap.propGridOffsetX;
	point.y = (frame.size.height - point.y + m_offsetY - m_obsrvTilemap.propGridOffsetY) / m_hexagon.propAspect;
	
	// Apply zoom
	
	if (m_zoom)
	{
		point.y /= m_zoom;
		point.x /= m_zoom;
	}
	

	// Find grid section
	
	int secX	= (int)floor(point.x / m_hexagon.propBoundWidth);
	int secY	= (int)floor(point.y / (m_hexagon.propSize + m_hexagon.propProjectionY));
	
	if (secX < 0 || secY < 0)
		return nil;
	
	point.x		-= secX * m_hexagon.propBoundWidth;
	point.y		-= secY * (m_hexagon.propSize + m_hexagon.propProjectionY);
	
	float grad	= m_hexagon.propProjectionY / m_hexagon.propProjectionX;
	
	
	if (!(secY & 1))
	{
		// type A section
		if (point.y < (m_hexagon.propProjectionY - point.x * grad))
		{
			secX--;
			secY--;
		}
		if (point.y < (- m_hexagon.propProjectionY + point.x * grad))
		{
			secY--;
		}
	}
	else
	{
		// type B section
		
		// right
		if (point.x >= m_hexagon.propProjectionX)
		{
			if (point.y < (2 * m_hexagon.propProjectionY - point.x * grad))
			{
				secY--;
			}
		}
		// left
		else
		{
			if (point.y < (point.x * grad))
			{
				secY--;
			}
			else
			{
				secX--;
			}
		}
	}
	
	if (secX < 0 || secY < 0)
		return nil;
	
	if (secX >= m_obsrvTilemap.propGridWidth || secY >= m_obsrvTilemap.propGridHeight)
		return nil;
	
	return [m_obsrvTilemap cellAtX:secX Y:secY];
}

// Returns distance from point to line
//-----------------------------------------------------------------------------------------------
- (float)getDist:(NSPoint)point Edge1:(NSPoint)a Edge2:(NSPoint)b
{
	float A = - (b.y - a.y);
	float B = b.x - a.x;
	float C = - (A * a.x + B * a.y);
	
	return A * point.x + B * point.y + C;
}


// Returns YES if point over tilemap node
//-----------------------------------------------------------------------------------------------
- (BOOL)isOverNode:(NSPoint)point Node:(IntPoint*)node
{
	NSRect frame = [self visibleRect];
	
	if (!m_obsrvTilemap.propMapWidth || !m_obsrvTilemap.propMapHeight)
		return FALSE;
	
	// Offset correction
	
	ProjectTilesetItem* tileset = m_obsrvTilemap.propTileset;
	
	point.x += m_offsetX;
	point.y = -(frame.size.height - point.y + m_offsetY);
	
	// Apply zoom
	
	if (m_zoom)
	{
		point.y /= m_zoom;
		point.x /= m_zoom;
	}
	
	point.x -= m_mapDrawOffset;
	
	float tw  = (float)tileset.propTileWidth;
	float th  = (float)tileset.propTileHeight;
	float dtw = tw / 2.0f;
	float dth = th / 2.0f;
	
	int startY = 0, startX = 0, index = 0;
	
	NSPoint a, b;
	
	while (startX < m_obsrvTilemap.propMapWidth)
	{
		int curY = startY;
		int curX = startX;
		
		while ( (curY >= 0) && (curX < m_obsrvTilemap.propMapWidth) )
		{
			float l	= curX * dtw - curY * dtw;
			float t = -curY * dth - curX * dth;
			
			a.x = l + dtw;	a.y = t - th;
			b.x = l + tw;	b.y = t - dth;
				
			float D1 = [self getDist:point Edge1:a Edge2:b];
			
			a.x = l + tw;	a.y = t - dth;
			b.x = l + dtw;	b.y = t;
			
			float D2 = [self getDist:point Edge1:a Edge2:b];
			
			a.x = l + dtw;	a.y = t;
			b.x = l;		b.y = t - dth;
			
			float D3 = [self getDist:point Edge1:a Edge2:b];
			
			a.x = l;		a.y = t - dth;
			b.x = l + dtw;	b.y = t - th;
			
			float D4 = [self getDist:point Edge1:a Edge2:b];
			
			if ( (D1 > 0) && (D2 > 0) && (D3 > 0) && (D4 > 0) )
			{
				if (node)
				{
					node->x = curX;
					node->y = curY;
				}
				return YES;
			}

			curY--;
			curX++;
			index++;
		}
		
		if (startY == (m_obsrvTilemap.propMapHeight - 1))
		{
			startX++;
		}
		else
			startY++;
	}
	return NO;
}

// Returns drawing static point
//-----------------------------------------------------------------------------------------------
- (NSPoint)staticToView:(NSPoint)point
{
	point.x -= m_mapDrawOffset;
	point.y = -point.y;
	
	return point;
}

// Returns map-related static element position
//-----------------------------------------------------------------------------------------------
- (NSPoint)getStaticPoint:(NSPoint)point
{
	// Offset correction
	
	NSRect frame = [self frame];
	
	point.x += m_offsetX;

	point.y = frame.size.height - point.y + m_offsetY;
	
	// Apply zoom
	
	if (m_zoom)
	{
		point.y /= m_zoom;
		point.x /= m_zoom;
	}
	
	//point.y = frame.size.height + point.y;
	
	//IntPoint result = {floor(point.x), floor(point.y)};
	
	return point;
}

// Returns screen-related static element position
//-----------------------------------------------------------------------------------------------
/*- (NSPoint)getScreenStaticPoint:(IntPoint)point
{
	NSPoint result = {point.x, point.y};
	

	
}*/



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
	m_objectDragStart = [self getEventLocation:theEvent];
	
	switch (m_editLayer)
	{
		case eEditLayerTiles:
			break;
			
		case eEditLayerStatic:
			switch (m_staticMode)
			{
				case eStaticEditSprite:
					break;
				case eStaticEditMultiSprite:
					break;
				case eStaticEditMove:
					m_objectDragPos = m_obsrvTilemap.propCurrentStatic.propPositionFloat;
					break;
			}
			break;
			
		case eEditLayerRegions:
			break;
			
		case eEditLayerHexGrid:
			break;
	}
	
	[self drawRect:[self frame]];
}

// mouseUp: left mouse button released
//-----------------------------------------------------------------------------------------------
- (void)mouseUp:(NSEvent*)theEvent
{
	NSPoint loc = [self getEventLocation:theEvent];
	
	BOOL over;
	
	NSPoint staticPos = [self getStaticPoint:loc];
	
	switch (m_editLayer)
	{
		case eEditLayerTiles:
			over = [self isOverNode:loc Node:&m_mouseNode];
	
			if (over)
			{
				[m_obsrvTilemap placeEditTileAtX:m_mouseNode.x Y:m_mouseNode.y];
		
				[self updateTiles];
			}
			break;
			
		case eEditLayerStatic:
			
			switch (m_staticMode)
			{
				case eStaticEditMultiSprite:
					[m_obsrvTilemap placeEditMultiSpriteAtX:staticPos.x Y:staticPos.y];
					break;
					
				case eStaticEditSprite:
					[m_obsrvTilemap placeEditSpriteAtX:staticPos.x Y:staticPos.y];
					break;
					
				case eStaticEditMove:
					break;
			}
			
			[self updateStatic];
			
			break;
			
		case eEditLayerRegions:
			
			break;
			
		case eEditLayerHexGrid:
			if (m_currentCell)
				m_currentCell.propTypeID = m_currentCellType.propID;
			break;
	}
	
	[self drawRect:[self frame]];
}

// rightMouseDown: right mouse button pressed
//-----------------------------------------------------------------------------------------------
- (void)rightMouseDown:(NSEvent*)theEvent
{
	m_mapDragStart = [self getEventLocation:theEvent];
	
	m_mapDragOffset.x = self.propViewOffsetX;
	m_mapDragOffset.y = self.propViewOffsetY;
	
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
	
	
	BOOL over;
	
	//NSPoint statPos = [self getStaticPoint:loc];
	
	NSPoint drag;
	
	switch (m_editLayer)
	{
		case eEditLayerTiles:
			over = [self isOverNode:loc Node:&m_mouseNode];
	
			if (over)
			{
				[m_obsrvTilemap placeEditTileAtX:m_mouseNode.x Y:m_mouseNode.y];
		
				[self updateTiles];
			}
			break;
			
		case eEditLayerStatic:
			
			switch (m_staticMode)
			{
				case eStaticEditMultiSprite:
					break;
				
				case eStaticEditSprite:
					break;
				
				case eStaticEditMove:
					drag.x = m_objectDragPos.x + (loc.x - m_objectDragStart.x) / m_zoom;
					drag.y = m_objectDragPos.y - (loc.y - m_objectDragStart.y) / m_zoom;
					
					m_obsrvTilemap.propCurrentStatic.propPositionFloat = drag;
					
					[m_obsrvTilemap sortStatic];
					
					[self updateStatic];
					
					break;
			}

			break;
			
		case eEditLayerRegions:
			break;
			
		case eEditLayerHexGrid:
			m_currentCell = [self getGridCell:loc];
			if (m_currentCell)
				m_currentCell.propTypeID = m_currentCellType.propID;
			break;
	}
	
	[self drawRect:[self frame]];
}

// rightMoustDragger: right button pressed and mouse moves
//-----------------------------------------------------------------------------------------------
- (void)rightMouseDragged:(NSEvent*)theEvent
{
	NSPoint loc = [self getEventLocation:theEvent];
	
	float offsetX = m_mapDragOffset.x + m_mapDragStart.x - loc.x;
	float offsetY = m_mapDragOffset.y + m_mapDragStart.y - loc.y;
	
	if (offsetX < 0)
		offsetX = 0;
	
	if (offsetX > self.propViewOffsetXMax)
		offsetX = self.propViewOffsetXMax;
	
	if (offsetY < 0)
		offsetY = 0;
	
	if (offsetY > self.propViewOffsetYMax)
		offsetY = self.propViewOffsetYMax;
	
	m_bNoRedraw = YES;
	
	self.propViewOffsetX = offsetX;
	self.propViewOffsetY = offsetY;

	m_bNoRedraw = NO;
	
	[self drawRect:[self frame]];
}


// mouseMoved: mouse moves over view frame
//-----------------------------------------------------------------------------------------------
- (void)mouseMoved:(NSEvent*)theEvent 
{
	NSPoint loc = [self getEventLocation:theEvent];
	
	NSPoint statPos = [self getStaticPoint:loc];
	
	switch (m_editLayer)
	{
		case eEditLayerTiles:
			m_mouseOverNode = [self isOverNode:loc Node:&m_mouseNode];
			[self updateTiles];
			break;
			
		case eEditLayerStatic:
			
			m_glMultiSprite.propPosition = [self staticToView:statPos];
			break;
			
		case eEditLayerRegions:
			break;
		
		case eEditLayerHexGrid:
			m_currentCell = [self getGridCell:loc];
			break;
	}
	
	[self drawRect:[self frame]];
}

// mouseExited: mouse exited frame tracking area
//-----------------------------------------------------------------------------------------------
- (void)mouseExited:(NSEvent*)theEvent 
{
	m_mouseOverNode = NO;
	m_currentCell	= nil;
	
	[self drawRect:[self frame]];
}

// scrollWheel: scrolling
//-----------------------------------------------------------------------------------------------
//- (void)scrollWheel:(NSEvent*)theEvent
//{
	
//	[self drawRect:[self frame]];
//}

// Accepting first mouse click
//-----------------------------------------------------------------------------------------------
- (BOOL)acceptsFirstMouse:(NSEvent*)theEvent
{
	return YES;
}


@end
