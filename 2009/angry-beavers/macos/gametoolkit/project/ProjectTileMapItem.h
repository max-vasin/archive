//===============================================================================================
//  File:		ProjectTileMapItem.h
//  Project:	OriginGameToolkit
//	Desc:		Tilemap project item
//
//  Created by Max Vasin on 4/26/11.
//  Copyright 2011 Origin Games. All rights reserved.
//===============================================================================================

// ****** IMPORTS ******

#import "BaseObject.h"
#import "ProjectItem.h"
#import "ProjectTilesetItem.h"
#import "Array2D.h"
#import "ProjectSpriteItem.h"
#import "ProjectMultiSpriteItem.h"

// Map type
//-----------------------------------------------------------------------------------------------
typedef enum
{
	eTileMapQuads		= 0,
	eTileMapDiamonds	= 1
}enTileMapType;


// Forward classes declaration
//-----------------------------------------------------------------------------------------------
@class ProjectTileMapItem;

//###############################################################################################
// HexGridCell INTERFACE
//###############################################################################################

@interface HexGridCell : BaseObject
{
	ProjectTileMapItem*		m_map;
	uint					m_indexX;
	uint					m_indexY;
	uint					m_typeID;
}

// ***** PROPERTIES *****

@property (nonatomic, assign)	uint	propIndexX;
@property (nonatomic, assign)	uint	propIndexY;
@property (nonatomic, assign)	uint	propTypeID;

// ***** INSTANCE METHODS *****

- (id)				initWithMap:(ProjectTileMapItem*)map;
- (id)				initWithMap:(ProjectTileMapItem*)map	XML:(NSXMLElement*)node;

- (NSXMLElement*)	saveXML:(NSXMLElement*)node;
- (NSXMLElement*)	buildXML:(NSXMLElement*)node;

@end

//###############################################################################################
// TileMapNode INTERFACE
//###############################################################################################

@interface TileMapNode : BaseObject 
{
	ProjectTileMapItem*		m_map;
	TileItem*				m_tile;
	uint					m_indexX;
	uint					m_indexY;
}

// ***** PROPERTIES *****

@property (nonatomic, assign)	TileItem*	propTile;
@property (nonatomic, assign)	uint		propIndexX;
@property (nonatomic, assign)	uint		propIndexY;

// ***** INSTANCE METHODS *****

- (id)				initWithMap:(ProjectTileMapItem*)map;
- (id)				initWithMap:(ProjectTileMapItem*)map	XML:(NSXMLElement*)node;

- (NSXMLElement*)	saveXML:(NSXMLElement*)node;
- (NSXMLElement*)	buildXML:(NSXMLElement*)node;

@end

//###############################################################################################
// StaticWrapper INTERFACE
//###############################################################################################

@interface StaticWrapper : BaseObject
{
	ProjectItem*	m_item;
	IntPoint		m_position;
}

// ***** PROPERTIES *****

@property (nonatomic, assign)	ProjectItem*	propItem;
@property (nonatomic, assign)	IntPoint		propPosition;
@property (nonatomic, assign)	NSPoint			propPositionFloat;
@property (nonatomic, assign)	NSString*		propDescription;

// ***** CLASS METHODS *****

+ (id)	wrapperWithItem:(ProjectItem*)item;

// ***** INSTANCE METHODS *****

- (id)				initWithItem:(ProjectItem*)item;
- (id)				initWithXML:(NSXMLElement*)node TileMap:(ProjectTileMapItem*)map;

- (NSXMLElement*)	saveXML:(NSXMLElement*)node;

- (NSInteger)		layerSort:(StaticWrapper*)entry;

@end


//###############################################################################################
// ProjectTileMapItem INTERFACE
//###############################################################################################

@interface ProjectTileMapItem : ProjectItem <Array2DDelegate>
{
	ProjectTilesetItem*		m_tileset;
	uint					m_mapWidth;
	uint					m_mapHeight;
	Array2D*				m_map;
	enTileMapType			m_type;
	IntPoint				m_editTile;
	NSColor*				m_backColor;
	NSMutableArray*			m_static;
	
	// ** hexagonal grid **
	
	Array2D*				m_grid;
	uint					m_gridWidth;
	uint					m_gridHeight;
	uint					m_gridCellEdge;
	float					m_gridAspect;
	int						m_gridOffsetX;
	int						m_gridOffsetY;
	
	ProjectSpriteItem*		m_staticEditSprite;
	ProjectMultiSpriteItem*	m_staticEditMultiSprite;
	StaticWrapper*			m_currentStatic;
}

// ***** PROPERTIES *****

@property (nonatomic, assign)	ProjectTilesetItem*		propTileset;
@property (nonatomic, assign)	enTileMapType			propMapType;
@property (nonatomic, assign)	uint					propMapWidth;
@property (nonatomic, assign)	uint					propMapHeight;
@property (nonatomic, readonly)	uint					propMapArea;
@property (nonatomic, readonly)	NSArray*				propMapTypes;
@property (nonatomic, assign)	IntPoint				propEditTile;
@property (nonatomic, assign)	ProjectSpriteItem*		propEditSprite;
@property (nonatomic, assign)	ProjectMultiSpriteItem*	propEditMultiSprite;
@property (nonatomic, copy)		NSColor*				propBackColor;
@property (nonatomic, readonly)	uint					propStaticCount;
@property (nonatomic, assign)	NSMutableArray*			propStatic;
@property (nonatomic, assign)	StaticWrapper*			propCurrentStatic;
@property (nonatomic, assign)	NSIndexSet*				propStaticIndexes;
@property (nonatomic, assign)	uint					propGridWidth;
@property (nonatomic, assign)	uint					propGridHeight;
@property (nonatomic, assign)	uint					propGridCellEdge;
@property (nonatomic, readonly)	uint					propGridArea;
@property (nonatomic, assign)	float					propGridAspect;
@property (nonatomic, assign)	int						propGridOffsetX;
@property (nonatomic, assign)	int						propGridOffsetY;

// ***** INSTANCE METHODS *****

- (TileMapNode*)	getNodeAtX:					(uint)x		Y:(uint)y;
- (void)			placeEditTileAtX:			(uint)x		Y:(uint)y;
- (StaticWrapper*)	getStaticAtIndex:			(uint)index;
- (void)			placeEditSpriteAtX:			(int)x		Y:(int)y;
- (void)			placeEditMultiSpriteAtX:	(int)x		Y:(int)y;
- (void)			delCurrentStatic;
- (HexGridCell*)	cellAtX:					(uint)x		Y:(uint)y;
- (void)			sortStatic;

@end
