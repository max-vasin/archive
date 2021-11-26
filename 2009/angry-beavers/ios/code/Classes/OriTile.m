//=======================================================================================================================
//  File:		OriTile.m
//  Project:	heli
//	Desc:		tileset tile item implementation
//
//  Created by Max Vasin on 5/11/11.
//  Copyright 2011 Origin. All rights reserved.
//=======================================================================================================================

// ***** IMPORTS *****

#import "OriTile.h"
#import "OriTileset.h"

// ***** IMPLEMENTATION *****

@implementation OriTile

// ****** PROPERTIES ******

// Synthesized
//-----------------------------------------------------------------------------------------------------------------------
@synthesize propIndex	= m_index;
@synthesize propTileset	= m_tileset;
@synthesize propMapMin	= m_mapMin;
@synthesize propMapMax	= m_mapMax;

//-----------------------------------------------------------------------------------------------------------------------
// PROPERTY: texture coordinates rectangle
//-----------------------------------------------------------------------------------------------------------------------

- (oriRect)propMap
{
	oriRect rect;
	
	rect.min.x	= m_mapMin.u;
	rect.min.y	= m_mapMin.v;
	rect.max.x	= m_mapMax.u;
	rect.max.y	= m_mapMax.v;
	
	return rect;
}

// ****** CLASS METHODS ******

// Returns new autoreleased tile with tileset binding
//-----------------------------------------------------------------------------------------------------------------------
+ (id)tileWithTileset:(OriTileset*)tileset
{
	return [[[self alloc] initWithTileset:tileset] autorelease];
}

// Returns new autoreleased tile with tileset binding and XML data
//-----------------------------------------------------------------------------------------------------------------------
+ (id)tileWithTileset:(OriTileset *)tileset withXML:(OriXMLNode *)node
{
	return [[[self alloc] initWithTileset:tileset withXML:node] autorelease];
}

// Returns new autoreleased tile with tileset binding and X:Y indexes
//-----------------------------------------------------------------------------------------------------------------------
+ (id)tileWithTileset:(OriTileset*)tileset X:(oriUint)x Y:(oriUint)y
{
	return [[[self alloc] initWithTileset:tileset X:x Y:y] autorelease];
}

// ****** INSTANCE METHODS ******

// Initializes new tile with tileset
//-----------------------------------------------------------------------------------------------------------------------
- (id)initWithTileset:(OriTileset*)tileset
{
	if ( (self = [self init]) )
	{
		m_tileset = tileset;
	}
	return self;
}

// Initializes new tile with tileset and XML data
//-----------------------------------------------------------------------------------------------------------------------
- (id)initWithTileset:(OriTileset*)tileset withXML:(OriXMLNode*)node
{
	if ( (self = [self initWithTileset:tileset]) )
	{
		m_index.x	= [node getFirstChildValueInt:@"X" placeholder:0];
		m_index.y	= [node getFirstChildValueInt:@"Y" placeholder:0];
	}
	return self;
}

// Initializes new tile with tileset and indexes
//-----------------------------------------------------------------------------------------------------------------------
- (id)initWithTileset:(OriTileset*)tileset X:(oriUint)x Y:(oriUint)y
{
	if ( (self = [self initWithTileset:tileset]) )
	{
		m_index.x	= x;
		m_index.y	= y;
	}
	return self;
}

// Updates texture related data
//-----------------------------------------------------------------------------------------------------------------------
- (void)updateTextureDepencies
{
	OriTexture* texture = m_tileset.propTexture;
	
	oriIntSize texSize	= texture.propSize;
	oriIntSize tileSize	= m_tileset.propTileSize;
	
	float du = 0, dv = 0;
	
	if (texSize.width)
		du = (float)tileSize.width / (float)texSize.width;
	
	if (texSize.height)
		dv = (float)tileSize.height / (float)texSize.height;
	
	m_mapMin.u	= m_index.x * du;
	m_mapMin.v	= m_index.y * dv;
	
	m_mapMax.u	= m_mapMin.u + du;
	m_mapMax.v	= m_mapMin.v + dv;
}

@end
