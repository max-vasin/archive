//=======================================================================================================================
//  File:		GameHexBindedUnit.h
//  Project:	heli
//	Desc:		Game unit binded to hex grid cells class interface
//
//  Created by Max Vasin on 6/23/11.
//  Copyright 2011 Origin. All rights reserved.
//=======================================================================================================================

// ******** IMPORTS ********

#import "OriBase.h"
#import "GameUnit.h"
#import "GameHexGridCell.h"

// ******** INTERFACE ********

@interface GameHexBindedUnit : GameUnit
{
    GameHexGridCell*	m_cellCurrent;
	GameHexGridCell*	m_cellTarget;
}

// ******** PROPERTIES ********

@property (nonatomic, assign)	GameHexGridCell* propCellCurrent;
@property (nonatomic, assign)	GameHexGridCell* propCellTarget;


@end
