//===============================================================================================
//  File:		TilemapView.h
//  Project:	OriginGameToolkit
//	Desc:		Tilemap editor view class
//
//  Created by Max Vasin on 5/4/11.
//  Copyright 2011 Origin Games. All rights reserved.
//===============================================================================================

// ***** IMPORTS *****

#import <Cocoa/Cocoa.h>
#import "Base.h"
#import "BaseObject.h"
#import "Project.h"
#import "GLMultiSprite.h"
#import "GLHexagon.h"
#import "TextureStore.h"
#import "ProjectRootItems.h"

typedef enum
{
	eStaticEditSprite		= 0,
	eStaticEditMultiSprite	= 1,
	eStaticEditMove			= 2
	
}enStaticEditMode;

typedef enum
{
	eEditLayerTiles			= 0,
	eEditLayerStatic		= 1,
	eEditLayerRegions		= 2,
	eEditLayerHexGrid		= 3
	
}enEditLayer;

// ***** INTERFACE *****

@interface TilemapView : NSOpenGLView
{
	BOOL					m_contextReady;			// Context initialization flag
	
	OGLVertex				m_vertsBack[4];			// Background shape vertices
	GLuint					m_oglCheckerTexture;	// OpenGL checkers texture
	GLuint					m_oglTilesetTexture;	// OpenGL tileset texture
	IntSize					m_texTilesetSize;		// OpenGL tileset texture size
	IntSize					m_texCheckerSize;		// OpenGL checkers texture size
	OGLVertex*				m_vertsTiles;			// Tiles vertices
	uint					m_vertsTilesCount;		// Tiles vertices count
	GLMultiSprite*			m_glMultiSprite;
	TextureStore*			m_texStore;				// Texture common storage
	NSMutableArray*			m_static;				// Static objects graphics wrappers
	GLHexagon*				m_hexagon;				// Hexagon grid grapics cell
	float					m_mapDrawOffset;
	
	// ** preferences **
	
	NSColor*				m_colorChecker;			// Background checker texture modulation color
	
	// ** current observing objects **
	
	Project*				m_obsrvProject;			// Current project tracking
	ProjectTilesetItem*		m_obsrvTileset;			// Current tileset tracking
	ProjectTileMapItem*		m_obsrvTilemap;			// Current tilemap tracking
	enStaticEditMode		m_staticMode;			// Static layer edit mode
	enEditLayer				m_editLayer;			// Current editing layer
	
	// ** view control **
	
	GLfloat					m_offsetX;				// View X offset (slider)
	GLfloat					m_offsetY;				// View Y offset (slider)
	GLfloat					m_offsetXMin;			// View X offset min bound (slider)
	GLfloat					m_offsetXMax;			// View X offset max bound (slider)
	GLfloat					m_offsetYMin;			// View Y offset min bound (slider)
	GLfloat					m_offsetYMax;			// View Y offset max bound (slider)
	GLfloat					m_zoom;					// View zooming
	
	// ** mouse tracking **
	
	NSTrackingArea*			m_trackArea;			// Mouse tracking area
	IntPoint				m_mouseNode;			// Mouse node index
	NSPoint					m_mapDragStart;			// Right mouse map dragging start point
	NSPoint					m_objectDragStart;		// Left mouse object draggin start point
	NSPoint					m_objectDragPos;		// Object dragging start position
	NSPoint					m_mapDragOffset;		// Start map drag offset
	BOOL					m_mouseOverNode;		// Mouse over node flag
	HexGridCell*			m_currentCell;			// Current grid cell
	TilemapGridCellType*	m_currentCellType;		// Currently selected cell type to draw
	BOOL					m_bNoRedraw;
}

@property (nonatomic, assign)	GLfloat					propViewOffsetXMin;
@property (nonatomic, assign)	GLfloat					propViewOffsetXMax;
@property (nonatomic, assign)	GLfloat					propViewOffsetYMin;
@property (nonatomic, assign)	GLfloat					propViewOffsetYMax;
@property (nonatomic, assign)	GLfloat					propViewOffsetX;
@property (nonatomic, assign)	GLfloat					propViewOffsetY;
@property (nonatomic, assign)	GLfloat					propViewZoom;
@property (nonatomic, copy)		NSColor*				propCheckerColor;
@property (nonatomic, assign)	enStaticEditMode		propStaticEditMode;
@property (nonatomic, assign)	enEditLayer				propEditLayer;
@property (nonatomic, readonly)	NSArray*				propEditLayers;
@property (nonatomic, assign)	TilemapGridCellType*	propCurrentCellType;

@end
