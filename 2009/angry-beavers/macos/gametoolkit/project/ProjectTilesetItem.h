//===============================================================================================
//  File:		ProjectTilesetItem.h
//  Project:	OriginGameToolkit
//	Desc:		Tileset project item class
//
//  Created by Max Vasin on 4/26/11.
//  Copyright 2011 Origin Games. All rights reserved.
//===============================================================================================

// ****** IMPORTS ******

#import <Foundation/Foundation.h>
#import "BaseObject.h"
#import "ProjectItem.h"
#import "ProjectTextureItem.h"
#import "Array2D.h"

// Tileset elements type
//-----------------------------------------------------------------------------------------------
typedef enum
{
	eTilesetTypeQuadAlpha	= 0,
	eTilesetTypeDiamond		= 1
}enTilesetType;

// Forward classed declaration
//-----------------------------------------------------------------------------------------------
@class ProjectTilesetItem;

//###############################################################################################
// TileItem INTERFACE
//###############################################################################################

@interface TileItem : BaseObject
{
	ProjectTilesetItem*		m_tileset;
	uint					m_indexX;
	uint					m_indexY;
	uint					m_indexMem;
}

// ****** PROPERTIES ******

@property (nonatomic, assign)	uint		propIndexX;
@property (nonatomic, assign)	uint		propIndexY;
@property (nonatomic, assign)	uint		propIndexMem;

// ****** INSTANCE METHODS ******

- (id)	initWithTileset:(ProjectTilesetItem*)tileset;
- (id)	initWithTileset:(ProjectTilesetItem*)tileset	XML:(NSXMLElement*)node;

- (NSXMLElement*)	saveXML:(NSXMLElement*)node;
- (NSXMLElement*)	buildXML:(NSXMLElement*)node;

@end

//###############################################################################################
// ProjectTilesetItem INTERFACE
//###############################################################################################

@interface ProjectTilesetItem : ProjectItem <Array2DDelegate>
{
    enTilesetType		m_type;
	uint				m_tileWidth;
	uint				m_tileHeight;
	uint				m_tilesCountX;
	uint				m_tilesCountY;
	IntPoint			m_currentTile;
	
	Array2D*			m_tiles;
	ProjectTextureItem*	m_texture;

}

// ****** PROPERTIES ******

@property (nonatomic, assign)	enTilesetType		propType;
@property (nonatomic, assign)	uint				propTileWidth;
@property (nonatomic, assign)	uint				propTileHeight;
@property (nonatomic, assign)	uint				propTilesCountX;
@property (nonatomic, assign)	uint				propTilesCountY;
@property (nonatomic, readonly)	uint				propTilesCount;
@property (nonatomic, assign)	ProjectTextureItem*	propTexture;
@property (nonatomic, readonly)	NSArray*			propTilesetTypes;
@property (nonatomic, assign)	IntPoint			propCurrentTile;
@property (nonatomic, readonly)	float				propTileAspect;

// ****** INSTANCE METHODS ******

- (TileItem*)	getTileAtX:(uint)x	Y:(uint)y;

@end
