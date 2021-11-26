//=======================================================================================================================
//  File:		GameHexGrid.h
//  Project:	heli
//	Desc:		Game hexagonal grid class interface
//
//  Created by Max Vasin on 5/30/11.
//  Copyright 2011 Origin. All rights reserved.
//=======================================================================================================================

// ******** IMPORTS ********

#import "OriBase.h"
#import "OriResourceManager.h"
#import "Array2D.h"
#import "GameHexGridCell.h"

// ******** INTERFACE ********

@interface GameHexGrid : NSObject <Array2DDelegate>
{
	OriTilemap*		m_tilemap;		// Tilemap resource link
	Array2D*		m_cells;		// Grid cells 2D resizeable array object
}

// ******** PROPERTIES ********

@property (nonatomic, readonly)		OriTilemap*		propTilemap;

// ******** INSTANCE METHODS ********

- (id)		initWithTilemap:(OriTilemap*)tilemap;
- (id)		initWithTilemapName:(NSString*)name;

- (void)	resetCells;
- (void)	enumerateCellOfType:(oriUint)type	Object:(id)object	Selector:(SEL)selector;

@end
