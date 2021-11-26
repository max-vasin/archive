//=======================================================================================================================
//  File:		GameAIController.m
//  Project:	heli
//	Desc:		Game AI controller class implementation
//
//  Created by Max Vasin on 6/23/11.
//  Copyright 2011 Origin. All rights reserved.
//=======================================================================================================================

// ******** IMPORTS ********

#import "GameAIController.h"

// ******** PRIVATE METHODS ********

@interface GameAIController (Private)

- (void)cellsEnumerator:(GameHexGridCell*)cell;

@end

// ******** IMPLEMENTATION ********

@implementation GameAIController

//#######################################################################################################################
// CONSTRUCTION\DESTRUCTION
//#######################################################################################################################

//-----------------------------------------------------------------------------------------------------------------------
// Designated initializer
//-----------------------------------------------------------------------------------------------------------------------

- (id)initWithHexGrid:(GameHexGrid*)hexgrid
{
	if ( (self = [self init]) )
	{
		m_hexgrid			= [hexgrid retain];
		m_cellsSpawnBeaver	= [[NSMutableArray alloc] initWithCapacity:10];
		
		// Startup grid analysis
		
		[m_hexgrid enumerateCellOfType:eGameCellUndefinedType Object:self Selector:@selector(cellsEnumerator:)];
	}
	return self;
}

//-----------------------------------------------------------------------------------------------------------------------
// Destructor
//-----------------------------------------------------------------------------------------------------------------------

- (void)dealloc
{
	[m_hexgrid release];
	[m_cellsSpawnBeaver release];
	
	[super dealloc];
}

//#######################################################################################################################
// PRIVATE METHODS
//#######################################################################################################################

//-----------------------------------------------------------------------------------------------------------------------
// Hex grid valuable cells extraction
//-----------------------------------------------------------------------------------------------------------------------

- (void)cellsEnumerator:(GameHexGridCell*)cell
{
	if (cell.propType == eGameCellSpawnBeaver)
		[m_cellsSpawnBeaver addObject:cell];
}


@end
