//=======================================================================================================================
//  File:		GameAIController.h
//  Project:	heli
//	Desc:		Game AI controller class interface
//
//  Created by Max Vasin on 6/23/11.
//  Copyright 2011 Origin. All rights reserved.
//=======================================================================================================================

// ******** IMPORTS ********

#import "OriBase.h"
#import "GameHexGrid.h"
#import "GameUnit.h"
#import "GameHexBindedUnit.h"
#import "GameBeaverUnit.h"

// ******** INTERFACE ********

@interface GameAIController : NSObject 
{
    GameHexGrid*		m_hexgrid;
	NSMutableArray*		m_cellsSpawnBeaver;
}

// ******** INSTANCE METHODS ********


//** construction\destruction **

- (id)initWithHexGrid:(GameHexGrid*)hexgrid;

@end
