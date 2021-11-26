//=======================================================================================================================
//  File:		OriTileset.m
//  Project:	heli
//	Desc:		tileset class implementation
//
//  Created by Max Vasin on 5/11/11.
//  Copyright 2011 Origin. All rights reserved.
//=======================================================================================================================

// ***** IMPORTS *****

#import "OriTileset.h"
#import "OriResourceManager.h"

// ***** IMPLEMENTATION *****

@implementation OriTileset

// ****** PROPERTIES ******

// Synthesized
//-----------------------------------------------------------------------------------------------------------------------
@synthesize propTexture		= m_texture;
@synthesize propTileSize	= m_tileSize;

// ****** CLASS METHODS ******

//#######################################################################################################################
// CONSTRUCTION
//#######################################################################################################################

// Returns new autoreleased instance of tileset with given resource manager
//-----------------------------------------------------------------------------------------------------------------------
+ (id)tilesetWithManager:(OriResourceManager*)resman withName:(NSString*)name
{
	return [[[self alloc] initWithManager:resman withName:name] autorelease];
}

// Returns new autoreleased instance of tileset with given resource manager and XML data
//-----------------------------------------------------------------------------------------------------------------------
+ (id)tilesetWithManager:(OriResourceManager*)resman withName:(NSString*)name withXML:(OriXMLNode*)node
{
	return [[[self alloc] initWithManager:resman withName:name withXML:node] autorelease];
}

// ****** INSTANCE METHODS ******

//#######################################################################################################################
// INITIALIZERS/DESTRUCTORS
//#######################################################################################################################

// Initializes instance of sprite with given resource manager
//-----------------------------------------------------------------------------------------------------------------------
- (id)initWithManager:(OriResourceManager*)resman withName:(NSString*)name
{
	if ( (self = [super initWithManager:resman withName:name]) )
	{
		m_tiles	= [[Array2D alloc] initWithDelegate:self];
		
		DBG_INIT_LOG_NAMED_0(self, m_name);
	}
	return self;
}

// Initializes instance of sprite with given resource manager and XML data
//-----------------------------------------------------------------------------------------------------------------------
- (id)initWithManager:(OriResourceManager*)resman withName:(NSString*)name withXML:(OriXMLNode*)node
{
	if ( (self = [self initWithManager:resman withName:name]) )
	{
		if (node)
		{
			NSString* textureName = [node getFirstChildValue:@"Texture" placeholder:nil];
			
			if (textureName)
			{
				m_texture = [m_resMan getTexture:textureName];
				
				DBG_ASSERT_LOG2A(!m_texture, @"OriTileset: (%@) failed to get texture (%@)", m_name, textureName);
			}
			else
			{
				DBGLOG1A(@"OriTileset: (%@) no texture name in XML file", m_name);
			}
			
			m_dimensions.width  = [node getFirstChildValueInt:@"CountX"		placeholder:0];
			m_dimensions.height = [node getFirstChildValueInt:@"CountY"		placeholder:0];
			m_tileSize.width	= [node getFirstChildValueInt:@"TileWidth"	placeholder:0];
			m_tileSize.height	= [node getFirstChildValueInt:@"TileHeight" placeholder:0];
			
			DBG_ASSERT_LOG1A(!m_dimensions.width || !m_dimensions.height, @"OriTileset: (%@) invalid dimensions", m_name);
			DBG_ASSERT_LOG1A(!m_tileSize.width   || !m_tileSize.height,   @"OriTileset: (%@) invalid tile size", m_name);
			
			[m_tiles resizeToX:m_dimensions.width Y:m_dimensions.height Create:NO];
			
			OriXMLNode* tilesRoot = [node getFirstChild:@"Tiles"];
			
			if (tilesRoot)
			{
				for (OriXMLNode* tileNode in tilesRoot.propChildren)
				{
					OriTile* tile = [[OriTile alloc] initWithTileset:self withXML:tileNode];
					
					oriIntPoint index = tile.propIndex;
					
					[m_tiles replaceAtX:index.x Y:index.y With:tile];
					
					[tile updateTextureDepencies];
					
					[tile release];
				}
			}
		}
	}
	return self;
}

// Desctructor
//-----------------------------------------------------------------------------------------------------------------------
- (void)dealloc
{
	SAFE_RELEASE(m_texture);
	SAFE_RELEASE(m_tiles);
	
	DBG_DEALLOC_LOG_NAMED_0(self, m_name);
	
	[super dealloc];
}

//#######################################################################################################################
// DATA ACCESS
//#######################################################################################################################

// Returns tile at index
//-----------------------------------------------------------------------------------------------------------------------
- (OriTile*)tileAtIndex:(oriIntPoint)index
{
	return (OriTile*)[m_tiles getObjectAtX:index.x Y:index.y];
}

// Returns tile index
//-----------------------------------------------------------------------------------------------------------------------
- (oriIntPoint)indexOfTile:(OriTile*)tile
{
	return [m_tiles indexOfObject:tile];
}


//#######################################################################################################################
// Array2DDelegate PROTOCOL
//#######################################################################################################################

// Called by Array2D when it need to get new element
//-----------------------------------------------------------------------------------------------------------------------
- (id)createInstanceOfArray:(Array2D*)array X:(uint)x Y:(uint)y DimX:(uint)dimX DimY:(uint)dimY
{
	return [OriTile tileWithTileset:self X:x Y:y];
}

@end
