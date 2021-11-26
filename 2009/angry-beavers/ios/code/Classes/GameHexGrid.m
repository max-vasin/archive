//=======================================================================================================================
//  File:		GameHexGrid.m
//  Project:	heli
//	Desc:		Game hexagonal grid class implementation
//
//  Created by Max Vasin on 5/30/11.
//  Copyright 2011 Origin. All rights reserved.
//=======================================================================================================================

// ******** IMPORTS ********

#import "GameHexGrid.h"

// ******** PRIVATE METHODS ********

@interface GameHexGrid (Private)


@end

// ******** IMPLEMENTATION ********

@implementation GameHexGrid

//#######################################################################################################################
// PROPERTIES
//#######################################################################################################################

//-----------------------------------------------------------------------------------------------------------------------
// SYNTHESIZED
//-----------------------------------------------------------------------------------------------------------------------

@synthesize propTilemap	= m_tilemap;

//#######################################################################################################################
// CONSTRUCTION
//#######################################################################################################################

//-----------------------------------------------------------------------------------------------------------------------
// Designated initializer
//-----------------------------------------------------------------------------------------------------------------------

- (id)initWithTilemap:(OriTilemap*)tilemap
{
	if ( (self = [self init]) )
	{
		m_tilemap	= [tilemap retain];
		m_cells		= [[Array2D alloc] initWithDelegate:self];
		
		[m_cells resizeToX:m_tilemap.propGridSize.width Y:m_tilemap.propGridSize.height Create:YES];
	}
	
	return self;
}

//-----------------------------------------------------------------------------------------------------------------------
// Initializer with tilemap name
//-----------------------------------------------------------------------------------------------------------------------

- (id)initWithTilemapName:(NSString*)name
{
	OriTilemap* tilemap = [[OriResourceManager sharedManager] getTilemap:name];
	
	DBG_ASSERT_LOG1A(!tilemap, @"GameHexGrid: failed to get tilemap resource with name (%@)", name);
	
	self = [self initWithTilemap:tilemap];
	
	return self;
}

//-----------------------------------------------------------------------------------------------------------------------
// Destructor
//-----------------------------------------------------------------------------------------------------------------------

- (void)dealloc
{
	[m_cells release];
	[m_tilemap release];
	
	[super dealloc];
}

//-----------------------------------------------------------------------------------------------------------------------
// Resets all cells values to linked tilemap resource hexgrid data
//-----------------------------------------------------------------------------------------------------------------------

- (void)resetCells
{
	if (!m_tilemap)
	{
		[m_cells resizeToX:0 Y:0 Create:NO];
		return;
	}
	
	oriIntSize gridSize = m_tilemap.propGridSize;
	
	[m_cells resizeToX:gridSize.width Y:gridSize.height Create:YES];
	
	for (oriInt y = 0; y < gridSize.height; y++)
		for (oriInt x = 0; x < gridSize.width; x++)
		{
			GameHexGridCell* cell = [m_cells getObjectAtX:x Y:y];
			
			cell.propType	= [m_tilemap cellAtX:x Y:y];
			cell.propUnit	= nil;
		}
}

//#######################################################################################################################
// DATA ACCESS
//#######################################################################################################################

//-----------------------------------------------------------------------------------------------------------------------
// Enumerates cells of given type
//-----------------------------------------------------------------------------------------------------------------------

- (void)enumerateCellOfType:(oriUint)type Object:(id)object Selector:(SEL)selector
{
	for (oriUint index = 0; index < m_cells.propCount; index++)
	{
		GameHexGridCell* cell = [m_cells getObjectAtIndex:index];
		
		
		if (cell.propType == type || type == eGameCellUndefinedType)
			[object performSelector:selector withObject:cell];
	}
}

//#######################################################################################################################
// Array2DDelegate PROTOCOL SUPPORT
//#######################################################################################################################

//-----------------------------------------------------------------------------------------------------------------------
// Must return new instance of array at given X:Y index
//-----------------------------------------------------------------------------------------------------------------------

- (id)createInstanceOfArray:(Array2D*)array X:(oriUint)x Y:(oriUint)y DimX:(oriUint)dimX DimY:(oriUint)dimY
{
	GameHexGridCell* cell = [[GameHexGridCell alloc] initWithGrid:self X:x Y:y Type:[m_tilemap cellAtX:x Y:y]];
	
	return cell;
}



@end
