//=======================================================================================================================
//  File:		OriTilemapNode.m
//  Project:	heli
//	Desc:		Tilemap node class implementation
//
//  Created by Max Vasin on 5/11/11.
//  Copyright 2011 Origin. All rights reserved.
//=======================================================================================================================

// ***** IMPORTS *****

#import "OriTilemapNode.h"
#import "OriTilemap.h"

// ***** IMPLEMENTATION *****

@implementation OriTilemapNode

// ****** PROPERTIES ******

// Synthesized
//-----------------------------------------------------------------------------------------------------------------------
@synthesize propNodeIndex		= m_nodeIndex;

// Tile index property
//-----------------------------------------------------------------------------------------------------------------------
- (void)setPropTileIndex:(oriIntPoint)index
{
	m_tileIndex = index;
	
	m_tile = [m_tilemap.propTileset tileAtIndex:index];
}

- (oriIntPoint)propTileIndex
{
	return m_tileIndex;
}

// Tile property
//-----------------------------------------------------------------------------------------------------------------------
- (void)setPropTile:(OriTile*)tile
{
	if (m_tile != tile)
	{
		m_tile		= tile;
		m_tileIndex = [m_tilemap.propTileset indexOfTile:tile];
	}
}

- (OriTile*)propTile
{
	return m_tile;
}

// ****** CLASS METHODS ******

// Returns new autoreleased node with tilemap binding
//-----------------------------------------------------------------------------------------------------------------------
+ (id)nodeWithTilemap:(OriTilemap*)tilemap
{
	return [[[self alloc] initWithTilemap:tilemap] autorelease];
}

// Returns new autoreleased node with tilemap binding from XML data
//-----------------------------------------------------------------------------------------------------------------------
+ (id)nodeWithTilemap:(OriTilemap*)tilemap withXML:(OriXMLNode*)node
{
	return [[[self alloc] initWithTilemap:tilemap withXML:node] autorelease];
}

// Returns new autoreleased node with tilemap binding with node index
//-----------------------------------------------------------------------------------------------------------------------
+ (id)nodeWithTilemap:(OriTilemap*)tilemap X:(oriUint)x Y:(oriUint)y
{
	return [[[self alloc] initWithTilemap:tilemap X:x Y:y] autorelease];
}

// ****** INSTANCE METHODS ******

// Initializes new node with tilemap binding
//-----------------------------------------------------------------------------------------------------------------------
- (id)initWithTilemap:(OriTilemap*)tilemap
{
	if ( (self = [self init]) )
	{
		m_tilemap	= tilemap;
	}
	return self;
}

// Initializes new node with tilemap binding and XML data
//-----------------------------------------------------------------------------------------------------------------------
- (id)initWithTilemap:(OriTilemap*)tilemap withXML:(OriXMLNode*)node
{
	if ( (self = [self initWithTilemap:tilemap]) )
	{
		m_nodeIndex.x	= [node getFirstChildValueInt:@"X"		placeholder:0];
		m_nodeIndex.y	= [node getFirstChildValueInt:@"Y"		placeholder:0];
		m_tileIndex.x	= [node getFirstChildValueInt:@"TileX"	placeholder:0];
		m_tileIndex.y	= [node getFirstChildValueInt:@"TileY"	placeholder:0];
		
		m_tile			= [m_tilemap.propTileset tileAtIndex:m_tileIndex];
	}
	return self;
}

// Initializes new node with tilemap binding and node index
//-----------------------------------------------------------------------------------------------------------------------
- (id)initWithTilemap:(OriTilemap*)tilemap X:(oriUint)x Y:(oriUint)y
{
	if ( (self = [self initWithTilemap:tilemap]) )
	{
		m_nodeIndex.x	= x;
		m_nodeIndex.y	= y;
	}
	return self;
}

@end
