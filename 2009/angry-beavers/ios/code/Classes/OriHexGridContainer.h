//=======================================================================================================================
//  File:		OriHexGridContainer.h
//  Project:	heli
//	Desc:		Hex grid scene container class interface
//
//  Created by Max Vasin on 6/7/11.
//  Copyright 2011 Origin. All rights reserved.
//=======================================================================================================================

// ******** IMPORTS ********

#import "OriBase.h"
#import "OriTimer.h"
#import "OriSceneObject.h"
#import "OriTilemap.h"
#import "OriHexGridCellRenderable.h"
#import "OriSprite.h"
#import "Array2D.h"

// ******** INTERFACE ********

@interface OriHexGridContainer : OriSceneObject <Array2DDelegate>
{
	OriTilemap*		m_tilemap;
	OriSprite*		m_sprite;
	Array2D*		m_cells;
	oriFloat		m_cellSide;
	oriFloat		m_cellAspect;
	oriFloat		m_cellH;
	oriFloat		m_cellR;
	oriFloat		m_cellWidth;
	oriFloat		m_cellHeight;
}

// ********	PROPERTIES ********

@property (nonatomic, readonly)		OriTilemap*		propTilemap;
@property (nonatomic, readonly)		OriSprite*		propSprite;
@property (nonatomic, assign)		oriFloat		propCellSide;
@property (nonatomic, assign)		oriFloat		propCellAspect;
@property (nonatomic, readonly)		oriFloat		propCellWidth;
@property (nonatomic, readonly)		oriFloat		propCellHeight;

// ********	INSTANCE METHODS ********

// ** initializers **

- (id)	initWithTilemap:		(OriTilemap*)tilemap	andSprite:(OriSprite*)sprite;
- (id)	initWithTilemapName:	(NSString*)name			andSpriteName:(NSString*)spritename;

@end
