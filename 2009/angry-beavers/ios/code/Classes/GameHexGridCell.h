//=======================================================================================================================
//  File:		GameHexGridCell.h
//  Project:	heli
//	Desc:		Game hexagonal grid cell class interface
//
//  Created by Max Vasin on 5/30/11.
//  Copyright 2011 Origin. All rights reserved.
//=======================================================================================================================

// ******** IMPORTS ********

#import "OriBase.h"
#import "GameUnit.h"

// ******** CELL TYPES ********

typedef enum
{
	eGameCellNull			= 0,	// null cell
	eGameCellBlocked		= 3,	// blocked cell
	eGameCellWater			= 6,	// water cell
	eGameCellSpawnBeaver	= 7,	// beaver spawning cell
	
	eGameCellUndefinedType	= 0xFFFF

}enGameCellType;


// ******** INTERFACE ********

@class GameHexGrid;

@interface GameHexGridCell : NSObject 
{
	GameHexGrid*		m_grid;
	enGameCellType		m_type;
	GameUnit*			m_unit;
	oriIntPoint			m_index;
}

// ******** PROPERTIES ********

@property (nonatomic, assign)	enGameCellType		propType;
@property (nonatomic, assign)	GameUnit*			propUnit;
@property (nonatomic, readonly)	GameHexGrid*		propGrid;
@property (nonatomic, readonly)	oriIntPoint			propIndex;

// ******** INSTANCE METHODS ********

- (id)	initWithGrid:(GameHexGrid*)grid X:(oriUint)x Y:(oriUint)y Type:(enGameCellType)type;

@end
