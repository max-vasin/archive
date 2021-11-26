//=======================================================================================================================
//  File:		OriTilemapNode.h
//  Project:	heli
//	Desc:		Tilemap node class interface
//
//  Created by Max Vasin on 5/11/11.
//  Copyright 2011 Origin. All rights reserved.
//=======================================================================================================================

// ***** IMPORTS *****

#import <UIKit/UIKit.h>
#import "OriBase.h"
#import "Array2D.h"
#import "OriXMLNode.h"
#import "OriTile.h"

// ***** INTERFACE *****

@class OriTilemap;

@interface OriTilemapNode : NSObject 
{
    OriTilemap*		m_tilemap;
	oriIntPoint		m_nodeIndex;
	oriIntPoint		m_tileIndex;
	OriTile*		m_tile;
}

// ****** PROPERTIES ******

@property (nonatomic, assign)	oriIntPoint		propNodeIndex;
@property (nonatomic, assign)	oriIntPoint		propTileIndex;
@property (nonatomic, assign)	OriTile*		propTile;

// ****** CLASS METHODS ******

+ (id)	nodeWithTilemap:(OriTilemap*)tilemap;
+ (id)	nodeWithTilemap:(OriTilemap*)tilemap	withXML:(OriXMLNode*)node;
+ (id)	nodeWithTilemap:(OriTilemap*)tilemap	X:(oriUint)x	Y:(oriUint)y;

// ****** INSTANCE METHODS ******

- (id)	initWithTilemap:(OriTilemap*)tilemap;
- (id)	initWithTilemap:(OriTilemap*)tilemap	withXML:(OriXMLNode*)node;
- (id)	initWithTilemap:(OriTilemap*)tilemap	X:(oriUint)x	Y:(oriUint)y;

@end
