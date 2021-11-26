//=======================================================================================================================
//  File:		OriTilemapContainer.m
//  Project:	heli
//	Desc:		Tilemap scene container class implementation
//
//  Created by Max Vasin on 6/3/11.
//  Copyright 2011 Origin. All rights reserved.
//=======================================================================================================================

// ******** IMPORTS ********

#import "OriTilemapContainer.h"
#import "OriScene.h"
#import "OriResourceManager.h"

// ******** IMPLEMENTATION ********

@implementation OriTilemapContainer

//#######################################################################################################################
// PROPERTIES
//#######################################################################################################################

//-----------------------------------------------------------------------------------------------------------------------
// SYNTHESIZED PROPERTIES
//-----------------------------------------------------------------------------------------------------------------------

@synthesize propTilemap		= m_tilemap;

//-----------------------------------------------------------------------------------------------------------------------
// PROPERTY: layer overriding
//-----------------------------------------------------------------------------------------------------------------------

- (void)setPropLayer:(oriFloat)layer
{
	if (m_layer != layer)
	{
		m_layer = layer;
	
		for (oriInt i = 0; i < m_nodes.propCount; i++)
			[[m_nodes getObjectAtIndex:i] setPropLayer:layer];
		
		[m_scene rearrange];
	}
}

//#######################################################################################################################
// INITIALIZERS\DESTRUCTORS
//#######################################################################################################################

//-----------------------------------------------------------------------------------------------------------------------
// Designated initializer
//-----------------------------------------------------------------------------------------------------------------------

- (id)initWithTilemap:(OriTilemap*)tilemap
{
	if ( (self = [self init]) )
	{
		m_tilemap	= [tilemap retain];
		m_nodes		= [[Array2D alloc] initWithDelegate:self];
		
		oriIntSize  mapSize = tilemap.propSize;
		
		[m_nodes resizeToX:mapSize.width Y:mapSize.height Create:YES];
	}
	return self;
}

//-----------------------------------------------------------------------------------------------------------------------
// Designated initializer with tilemap name
//-----------------------------------------------------------------------------------------------------------------------

- (id)initWithTilemapName:(NSString*)name
{
	OriTilemap* tilemap = [[OriResourceManager sharedManager] getTilemap:name];
	
	if (!tilemap)
		return nil;
	
	self = [self initWithTilemap:tilemap];
	
	[tilemap release];
	
	return self;
}

//-----------------------------------------------------------------------------------------------------------------------
// Destructor
//-----------------------------------------------------------------------------------------------------------------------

- (void)dealloc
{
	[m_nodes release];
	[m_tilemap release];
	
	[super dealloc];
}


//#######################################################################################################################
// OVERRIDINGS OF OriSceneObject
//#######################################################################################################################

//-----------------------------------------------------------------------------------------------------------------------
// We are container
//-----------------------------------------------------------------------------------------------------------------------

- (enSceneObjectType)type
{
	return eSceneObjectContainer;
}

//-----------------------------------------------------------------------------------------------------------------------
// We are immutable
//-----------------------------------------------------------------------------------------------------------------------

- (BOOL)isMutable
{
	return NO;
}

//-----------------------------------------------------------------------------------------------------------------------
// Container can move
//-----------------------------------------------------------------------------------------------------------------------

- (BOOL)isDynamic
{
	return YES;
}

//-----------------------------------------------------------------------------------------------------------------------
// Put all tile renderables to scene and sort
//-----------------------------------------------------------------------------------------------------------------------

- (void)attachContent:(OriScene*)scene
{
	for (oriInt i = 0; i < m_nodes.propCount; i++)
		[scene attach:[m_nodes getObjectAtIndex:i]];
}

//-----------------------------------------------------------------------------------------------------------------------
// Remove all renderables from scene
//-----------------------------------------------------------------------------------------------------------------------

- (void)detachContent:(OriScene*)scene
{
	for (oriInt i = 0; i < m_nodes.propCount; i++)
		[scene detach:[m_nodes getObjectAtIndex:i]];
}

//#######################################################################################################################
// TILES ACCESS
//#######################################################################################################################

//-----------------------------------------------------------------------------------------------------------------------
// Returns tile at untransformed point
//-----------------------------------------------------------------------------------------------------------------------

- (OriTileRenderable*)pickTileAtPoint:(oriVector2)point
{
	// get container translation
	
	oriVector2 translation;
	
	[self absoluteTranslationX:&translation.x Y:&translation.y];
	
	// translate point
	
	point.x -= translation.x;
	point.y -= translation.y;
	
	// get screen-related indexes
	
	oriVector2	screenIndexes;
	
	oriIntSize  tileSize	= m_tilemap.propTileset.propTileSize;
	oriIntSize  mapSize		= m_tilemap.propSize;
	
	oriFloat h2	= (oriFloat)tileSize.height / 2.0f;
	oriFloat w2 = (oriFloat)tileSize.width / 2.0f;
	
	screenIndexes.x = floor(point.x / w2);
	screenIndexes.y = floor(point.y / h2);
	
	// get screen-related piece coordinates
	
	oriVector2 screenCoords;
	
	screenCoords.x = point.x - (screenIndexes.x * w2);
	screenCoords.y = point.y - (screenIndexes.y * h2);
	
	// invert Y
	
	screenIndexes.y = mapSize.width + mapSize.height - 1 - screenIndexes.y;
	
	// get tilemap related indexes
	
	oriVector2 mapIndexes;
	
	mapIndexes.x	= (screenIndexes.x + screenIndexes.y - mapSize.height) / 2.0f;
	mapIndexes.y	= (mapSize.height + screenIndexes.y - screenIndexes.x + 1) / 2.0f;
	
	if (mapIndexes.x < 0.0f || mapIndexes.y < 0.0f || mapIndexes.x > mapSize.width || mapIndexes.y > mapSize.height)
		return nil;
	
	// check piece
	
	oriFloat aspect = h2 / w2;
	
	if (mapIndexes.x == floor(mapIndexes.x))
	{
		if (screenCoords.y > screenCoords.x * aspect)
			mapIndexes.x--;
	}
	else
	{
		if (screenCoords.y > h2 - screenCoords.x * aspect)
			mapIndexes.y--;
	}
	
	oriIntPoint indexes;
	
	indexes.x = (oriInt)floor(mapIndexes.x);
	indexes.y = (oriInt)floor(mapIndexes.y);
	
	return [self pickTileAtIndex:indexes];
}

//-----------------------------------------------------------------------------------------------------------------------
// Returns tile at given tilemap indexes
//-----------------------------------------------------------------------------------------------------------------------

- (OriTileRenderable*)pickTileAtIndex:(oriIntPoint)index
{
	return [m_nodes getObjectAtX:index.x Y:index.y];
}

//#######################################################################################################################
// <Array2DDelegate> PROTOCOL ADOPTION
//#######################################################################################################################

//-----------------------------------------------------------------------------------------------------------------------
// Must create new array item
//-----------------------------------------------------------------------------------------------------------------------

- (id)createInstanceOfArray:(Array2D*)array X:(oriUint)x Y:(oriUint)y DimX:(oriUint)dimX DimY:(oriUint)dimY
{
	oriIntSize tileSize = m_tilemap.propTileset.propTileSize;
	oriIntSize mapSize	= m_tilemap.propSize;
	
	oriFloat h2	= (oriFloat)tileSize.height / 2.0f;
	oriFloat w2 = (oriFloat)tileSize.width / 2.0f;
	
	Vector2* position = [[Vector2 alloc] init];
	
	position->x	= floor(w2 * ((oriInt)mapSize.height - 1 - (oriInt)y + (oriInt)x));
	position->y = floor(h2 * ((oriInt)(mapSize.width + mapSize.height - 2) - (oriInt)x - (oriInt)y));
	
	OriTileRenderable* newTile = [[OriTileRenderable alloc] initWithContainer:self andTile:[m_tilemap nodeAtX:x Y:y].propTile];
	
	newTile.propPosition = position;
	
	[position release];
	
	return newTile;
}

@end
