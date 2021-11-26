//=======================================================================================================================
//  File:		OriTilemapContainer.h
//  Project:	heli
//	Desc:		Tilemap	scene container class interface
//
//  Created by Max Vasin on 6/3/11.
//  Copyright 2011 Origin. All rights reserved.
//=======================================================================================================================

// ******** IMPORTS ********

#import "OriBase.h"
#import "OriTimer.h"
#import "OriSceneObject.h"
#import "OriTilemap.h"
#import "OriTileRenderable.h"
#import "Array2D.h"

// ******** INTERFACE ********

@interface OriTilemapContainer : OriSceneObject <Array2DDelegate>
{
	OriTilemap*		m_tilemap;
	Array2D*		m_nodes;
}

// ********	PROPERTIES ********

@property (nonatomic, readonly)		OriTilemap*		propTilemap;

// ********	INSTANCE METHODS ********

// ** initializers **

- (id)initWithTilemap:(OriTilemap*)tilemap;
- (id)initWithTilemapName:(NSString*)name;

// ** tiles access **

- (OriTileRenderable*)	pickTileAtPoint:(oriVector2)point;
- (OriTileRenderable*)	pickTileAtIndex:(oriIntPoint)index;

@end
