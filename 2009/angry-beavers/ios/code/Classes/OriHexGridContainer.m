//=======================================================================================================================
//  File:		OriHexGridContainer.m
//  Project:	heli
//	Desc:		Hex grid container scene object
//
//  Created by Max Vasin on 6/7/11.
//  Copyright 2011 Origin. All rights reserved.
//=======================================================================================================================

// ******** IMPORTS ********

#import "OriHexGridContainer.h"
#import "OriScene.h"
#import "OriResourceManager.h"

// ********	PRIVATE METHODS ********

@interface OriHexGridContainer (Private)

- (void)calcHexagon;

@end


// ********	IMPLEMENTATION ********

@implementation OriHexGridContainer

//#######################################################################################################################
// PROPERTIES
//#######################################################################################################################

//-----------------------------------------------------------------------------------------------------------------------
// SYNTHESIZED PROPERTIES
//-----------------------------------------------------------------------------------------------------------------------

@synthesize propTilemap		= m_tilemap;
@synthesize propSprite		= m_sprite;
@synthesize propCellSide	= m_cellSide;
@synthesize propCellAspect	= m_cellAspect;
@synthesize propCellWidth	= m_cellWidth;
@synthesize propCellHeight	= m_cellHeight;

//-----------------------------------------------------------------------------------------------------------------------
// PROPERTY: layer overriding
//-----------------------------------------------------------------------------------------------------------------------

- (void)setPropLayer:(oriFloat)layer
{
	if (m_layer != layer)
	{
		m_layer = layer;
		
		for (oriInt i = 0; i < m_cells.propCount; i++)
			[[m_cells getObjectAtIndex:i] setPropLayer:layer];
		
		[m_scene rearrange];
	}
}

//#######################################################################################################################
// INITIALIZERS\DESTRUCTORS
//#######################################################################################################################

//-----------------------------------------------------------------------------------------------------------------------
// Designated initializer
//-----------------------------------------------------------------------------------------------------------------------

- (id)initWithTilemap:(OriTilemap*)tilemap andSprite:(OriSprite*)sprite
{
	if ( (self = [self init]) )
	{
		m_tilemap	= [tilemap retain];
		m_sprite	= [sprite retain];
		m_cells		= [[Array2D alloc] initWithDelegate:self];
		
		m_cellAspect	= tilemap.propGridCellAspect;
		m_cellSide		= tilemap.propGridCellSide;
		
		[self calcHexagon];
		
		oriIntSize  gridSize = tilemap.propGridSize;
		
		[m_cells resizeToX:gridSize.width Y:gridSize.height Create:YES];
	}
	return self;
}

//-----------------------------------------------------------------------------------------------------------------------
// Designated initializer with tilemap name
//-----------------------------------------------------------------------------------------------------------------------

- (id)initWithTilemapName:(NSString*)name andSpriteName:(NSString*)spritename
{
	OriTilemap* tilemap = [[OriResourceManager sharedManager] getTilemap:name];
	
	if (!tilemap)
		return nil;
	
	OriSprite* sprite = [[OriResourceManager sharedManager] getSprite:spritename];
	
	if (!sprite)
	{
		[tilemap release];
		return nil;
	}
	
	self = [self initWithTilemap:tilemap andSprite:sprite];
	
	[tilemap release];
	[sprite release];
	
	return self;
}

//-----------------------------------------------------------------------------------------------------------------------
// Destructor
//-----------------------------------------------------------------------------------------------------------------------

- (void)dealloc
{
	[m_cells release];
	[m_tilemap release];
	[m_sprite release];
	
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
	for (oriInt i = 0; i < m_cells.propCount; i++)
	{
		OriHexGridCellRenderable* cellRenderable = [m_cells getObjectAtIndex:i];
		
		if (cellRenderable)
			[scene attach:cellRenderable];
	}
}

//-----------------------------------------------------------------------------------------------------------------------
// Remove all renderables from scene
//-----------------------------------------------------------------------------------------------------------------------

- (void)detachContent:(OriScene*)scene
{
	for (oriInt i = 0; i < m_cells.propCount; i++)
	{
		OriHexGridCellRenderable* cellRenderable = [m_cells getObjectAtIndex:i];
		
		if (cellRenderable)
			[scene detach:cellRenderable];
	}
}


//#######################################################################################################################
// <Array2DDelegate> PROTOCOL ADOPTION
//#######################################################################################################################

//-----------------------------------------------------------------------------------------------------------------------
// Must create new array item
//-----------------------------------------------------------------------------------------------------------------------

- (id)createInstanceOfArray:(Array2D*)array X:(oriUint)x Y:(oriUint)y DimX:(oriUint)dimX DimY:(oriUint)dimY
{
	oriUint flipY		= m_tilemap.propGridSize.height - y - 1;
	oriUint cellType	= [m_tilemap cellAtX:x Y:flipY];
	
	if (!cellType)
		return nil;
	
	Vector2*	pos		= [[Vector2 alloc] init];
	oriIntPoint offset	= m_tilemap.propGridOffset;
	
	pos->x = floor(x * 2 * m_cellR + (y & 1) * m_cellR + offset.x);
	pos->y = floor(y * (m_cellH + m_cellSide) * m_cellAspect + offset.y);
	
	Vector2* scale = [[Vector2 alloc] initWithX:1.0f Y:m_cellAspect];
	
	OriHexGridCellRenderable* newCell = [[OriHexGridCellRenderable alloc] initWithContainer:self andType:cellType];
	
	newCell.propPosition = pos;
	newCell.propScale	 = scale;
	
	[pos release];
	[scale release];
	
	return newCell;
}

//#######################################################################################################################
// PRIVATE METHODS
//#######################################################################################################################

//-----------------------------------------------------------------------------------------------------------------------
// Calculates hexagon cell sizes
//-----------------------------------------------------------------------------------------------------------------------

- (void)calcHexagon
{
	m_cellH			= sin(RAD_30) * m_cellSide;
	m_cellR			= cos(RAD_30) * m_cellSide;
	m_cellHeight	= m_cellSide + 2 * m_cellH;
	m_cellWidth		= m_cellR * 2;
}




@end
