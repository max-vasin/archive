//=======================================================================================================================
//  File:		OriTilemap.h
//  Project:	heli
//	Desc:		Tilemap resource class
//
//  Created by Max Vasin on 5/11/11.
//  Copyright 2011 Origin. All rights reserved.
//=======================================================================================================================

// ***** IMPORTS *****

#import <UIKit/UIKit.h>
#import "OriBase.h"
#import "Array2D.h"
#import "IntegerArray2D.h"
#import "OriXMLNode.h"
#import "OriResource.h"
#import "OriTileset.h"
#import "OriTilemapNode.h"
#import "OriTilemapStatic.h"

// ***** INTERFACE *****

@interface OriTilemap : OriResource <Array2DDelegate>
{
	OriTileset*			m_tileset;
	oriIntSize			m_size;
	Array2D*			m_nodes;
	IntegerArray2D*		m_cells;
	NSMutableArray*		m_static;
	oriInt				m_gridCellSide;
	oriFloat			m_gridCellAspect;
	oriIntPoint			m_gridOffset;
	oriIntSize			m_drawSize;
}

// ****** PROPERTIES ******

@property (nonatomic, assign)	OriTileset*		propTileset;
@property (nonatomic, assign)	oriIntSize		propSize;
@property (nonatomic, readonly)	oriIntSize		propDrawSize;
@property (nonatomic, readonly)	oriUint			propStaticObjectsCount;
@property (nonatomic, readonly)	oriIntSize		propGridSize;
@property (nonatomic, readonly)	oriInt			propGridCellSide;
@property (nonatomic, readonly)	oriFloat		propGridCellAspect;
@property (nonatomic, readonly) oriIntPoint		propGridOffset;

// ****** CLASS METHODS ******

+ (id)					tilemapWithManager:(OriResourceManager*)resman	withName:(NSString*)name;
+ (id)					tilemapWithManager:(OriResourceManager*)resman	withName:(NSString*)name	withXML:(OriXMLNode*)node;

// ****** INSTANCE METHODS ******

- (id)					initWithManager:(OriResourceManager*)resman		withName:(NSString*)name;
- (id)					initWithManager:(OriResourceManager*)resman		withName:(NSString*)name	withXML:(OriXMLNode*)node;

- (OriTilemapNode*)		nodeAtX:(oriUint)x	Y:(oriUint)y;
- (OriTilemapStatic*)	staticAtIndex:(oriUint)index;
- (oriUint)				cellAtX:(oriUint)x	Y:(oriUint)y;

@end
