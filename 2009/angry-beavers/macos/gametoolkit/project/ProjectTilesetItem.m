//===============================================================================================
//  File:		ProjectTilesetItem.m
//  Project:	OriginGameToolkit
//	Desc:		Tileset project item class
//
//  Created by Max Vasin on 4/26/11.
//  Copyright 2011 Origin Games. All rights reserved.
//===============================================================================================

// ****** IMPORTS ******

#import "ProjectTilesetItem.h"
#import "SharedResourceManager.h"
#import "ProjectRootItems.h"

// ****** PRIVATE METHODS ******

@interface ProjectTilesetItem (Private)

@end



//###############################################################################################
// TileItem IMPLEMENTATION
//###############################################################################################



@implementation TileItem

//###############################################################################################
// PROPERTIES
//###############################################################################################

// Synthesized
//-----------------------------------------------------------------------------------------------
@synthesize propIndexX		= m_indexX;
@synthesize propIndexY		= m_indexY;
@synthesize propIndexMem	= m_indexMem;

//###############################################################################################
// INITIALIZERS\DESTRUCTORS
//###############################################################################################

// Initializer with parent tileset
//-----------------------------------------------------------------------------------------------
- (id)initWithTileset:(ProjectTilesetItem*)tileset
{
	if ( (self = [self init]) )
	{
		m_tileset		= tileset;
	}
	return self;
}

// Initializer with parent tileset and XML data
//-----------------------------------------------------------------------------------------------
- (id)initWithTileset:(ProjectTilesetItem*)tileset XML:(NSXMLElement*)node
{
	if ( (self = [self init]) )
	{
		m_tileset		= tileset;
		m_indexX		= [node getFirstChildInteger:@"X"];
		m_indexY		= [node getFirstChildInteger:@"Y"];
	}
	return self;
}

// Destructor
//-----------------------------------------------------------------------------------------------
- (void)dealloc
{
	[super dealloc];
}

//###############################################################################################
// SAVING\BUILDING
//###############################################################################################

// Saves current item to project scheme
//-----------------------------------------------------------------------------------------------
- (NSXMLElement*)saveXML:(NSXMLElement*)node
{
	NSXMLElement* thisNode = [node addNewChild:@"Tile"];
	
	[thisNode addNewChild:@"X"		withInteger:m_indexX];
	[thisNode addNewChild:@"Y"		withInteger:m_indexY];
	
	return thisNode;
}

// Builds current item to tileset scheme
//-----------------------------------------------------------------------------------------------
- (NSXMLElement*)buildXML:(NSXMLElement*)node
{
	NSXMLElement* thisNode = [node addNewChild:@"Tile"];
	
	[thisNode addNewChild:@"X"		withInteger:m_indexX];
	[thisNode addNewChild:@"Y"		withInteger:m_indexY];
	
	return thisNode;
}

@end




//###############################################################################################
// TileItem IMPLEMENTATION
//###############################################################################################




@implementation ProjectTilesetItem

//###############################################################################################
// PROPERTIES
//###############################################################################################

// Synthesized
//-----------------------------------------------------------------------------------------------
@synthesize propType		= m_type;
@synthesize propTileWidth	= m_tileWidth;
@synthesize propTileHeight	= m_tileHeight;
@synthesize propTexture		= m_texture;
@synthesize propCurrentTile	= m_currentTile;

// Tiles by X on texture
//-----------------------------------------------------------------------------------------------
- (void)setPropTilesCountX:(uint)count
{
	//[self arrangeTilesToX:count Y:m_tilesCountY CreateTiles:YES];
	[m_tiles resizeToX:count Y:m_tilesCountY Create:YES];
	
	m_tilesCountX	= count;
}

- (uint)propTilesCountX
{
	return m_tilesCountX;
}

// Tiles by Y on texture
//-----------------------------------------------------------------------------------------------
- (void)setPropTilesCountY:(uint)count
{
	//[self arrangeTilesToX:m_tilesCountX Y:count CreateTiles:YES];
	[m_tiles resizeToX:m_tilesCountX Y:count Create:YES];
	
	m_tilesCountY	= count;
}

- (uint)propTilesCountY
{
	return m_tilesCountY;
}

// Total tiles count
//-----------------------------------------------------------------------------------------------
- (uint)propTilesCount
{
	return m_tilesCountX * m_tilesCountY;
}

// Tileset elements types array
//-----------------------------------------------------------------------------------------------
- (NSArray*)propTilesetTypes
{
	return [NSArray arrayWithObjects:@"quad", @"diamond", nil];
}

// Tile aspect (distortion)
//-----------------------------------------------------------------------------------------------
- (float)propTileAspect
{
	if (!m_tileWidth)
		return 0;
	
	return (float)m_tileHeight / (float)m_tileWidth;
}


//###############################################################################################
// INITIALIZERS\DESTRUCTORS
//###############################################################################################

// Overriding (ProjectItem) initializer
//-----------------------------------------------------------------------------------------------
- (id)initWithParent:(ProjectItem*)parent
{
	if ((self = [super initWithParent:parent]))
	{
		m_tilesCountX	= 1;
		m_tilesCountY	= 1;
		m_tileWidth		= 128;
		m_tileHeight	= 64;
		m_type			= eTilesetTypeQuadAlpha;
		
		m_tiles			= [[Array2D alloc] initWithDelegate:self];
		[m_tiles resizeToX:1 Y:1 Create:YES];
	}
	return self;
}

// Overriding (ProjectItem) initializer
//-----------------------------------------------------------------------------------------------
- (id)initWithParent:(ProjectItem*)parent fromXML:(NSXMLElement*)node
{
	if ( (self = [super initWithParent:parent fromXML:node]) )
	{
		if (!m_tiles)
			m_tiles	= [[Array2D alloc] initWithDelegate:self];
		
		ProjectRootItem* root = (ProjectRootItem*)m_itemParent->m_itemParent;
		
		m_texture		= (ProjectTextureItem*)[(ProjectTextureRootItem*)root.propRootTextures getChildByName:[node getFirstChildValue:@"Texture"]];
		
		m_tilesCountX	= [node getFirstChildInteger:@"CountX"];
		m_tilesCountY	= [node getFirstChildInteger:@"CountY"];
		m_tileWidth		= [node getFirstChildInteger:@"TileWidth"];
		m_tileHeight	= [node getFirstChildInteger:@"TileHeight"];
		m_type			= [node getFirstChildInteger:@"Type"];
		
		[m_tiles resizeToX:m_tilesCountX Y:m_tilesCountY Create:NO];
		
		NSXMLElement* tilesNode = [node getFirstChild:@"Tiles"];
		
		//IntSize memSize = {m_tilesCountX, m_tilesCountY};
		
		for (NSXMLElement* tileNode in [tilesNode children])
		{
			TileItem* newItem = [[TileItem alloc] initWithTileset:self XML:tileNode];
			
			//[self replaceTile:newItem InMemory:m_tiles MemSize:memSize];
			
			[m_tiles replaceAtX:newItem.propIndexX Y:newItem.propIndexY With:newItem];
			
			[newItem release];
		}
			
	}
	return self;
}

// Destructor
//-----------------------------------------------------------------------------------------------
- (void)dealloc
{
	SAFE_RELEASE(m_tiles);
	
	[super dealloc];
}

//###############################################################################################
// TILES ACCESS
//###############################################################################################

// Returns tile by XY indexes
//-----------------------------------------------------------------------------------------------
- (TileItem*)getTileAtX:(uint)x Y:(uint)y
{
	return [m_tiles getObjectAtX:x Y:y];
}

//###############################################################################################
// MISC
//###############################################################################################

// Sets item icon
//-----------------------------------------------------------------------------------------------
- (void)setIcon
{
	m_itemImage = [SharedResourceManager sharedIcon:@"tileset"];
}

//###############################################################################################
// SAVING\BUILDING
//###############################################################################################

// Makes XML representation fo current item and attach it to node
//-----------------------------------------------------------------------------------------------
- (NSXMLElement*)saveAsXML:(NSXMLElement*)node
{
	NSXMLElement* thisNode = [node addNewChild:@"Tileset"];
	
	[thisNode	addNewChild:@"Name"			withValue:m_itemName];
	[thisNode	addNewChild:@"Texture"		withValue:m_texture.propName];
	[thisNode	addNewChild:@"CountX"		withInteger:m_tilesCountX];
	[thisNode	addNewChild:@"CountY"		withInteger:m_tilesCountY];
	[thisNode	addNewChild:@"TileWidth"	withInteger:m_tileWidth];
	[thisNode	addNewChild:@"TileHeight"	withInteger:m_tileHeight];
	[thisNode	addNewChild:@"Type"			withInteger:m_type];
	
	NSXMLElement* tilesNode = [thisNode addNewChild:@"Tiles"];
	
	for (int i = 0; i < self.propTilesCount; i++)
		[(TileItem*)[m_tiles getObjectAtIndex:i] saveXML:tilesNode];
	
	return thisNode;
}

// Builds final tileset output
//-----------------------------------------------------------------------------------------------
- (result)build:(Progressor*)progress
{
	result res = [super build:progress];
	if (FAIL(res))
		return res;
	
	progress.propCurrentState = [NSString stringWithFormat:@"Exporting: %@", m_itemName];
	
	// Make XML scheme of sprite
	
	NSXMLElement* rootNode = [[[NSXMLElement alloc] initWithName:@"Tileset"] autorelease];
	
	[rootNode addNewChild:@"Name"			withValue:m_itemName];
	[rootNode addNewChild:@"Texture"		withValue:m_texture.propName];
	[rootNode addNewChild:@"CountX"			withInteger:m_tilesCountX];
	[rootNode addNewChild:@"CountY"			withInteger:m_tilesCountY];
	[rootNode addNewChild:@"TileWidth"		withInteger:m_tileWidth];
	[rootNode addNewChild:@"TileHeight"		withInteger:m_tileHeight];
	[rootNode addNewChild:@"Type"			withInteger:m_type];
	
	// Tiles
	
	NSXMLElement* tilesNode = [rootNode addNewChild:@"Tiles"];
	
	for (int i = 0; i < self.propTilesCount; i++)
		[(TileItem*)[m_tiles getObjectAtIndex:i] buildXML:tilesNode];
	
	
	NSString* outFile = [NSString stringWithFormat:@"%@/%@.xml", self.propItemBuildDirectory, m_itemName];
	
	NSXMLDocument* doc = [[[NSXMLDocument alloc] init] autorelease];
	
	doc.version				= @"1.0";
	doc.characterEncoding	= @"UTF-8";
	
	doc.rootElement			= rootNode;
	
	NSData* output = [doc XMLDataWithOptions:NSXMLDocumentIncludeContentTypeDeclaration | NSXMLNodePrettyPrint];
	
	if (![output writeToFile:outFile atomically:YES])
	{
		DBG_START	[self log:"$C[build] - ERROR: cannot write XML schema to [%s]\n", [outFile UTF8String]];	DBG_END
		return RES_ERR_FILESYSTEM;
	}
	
	return res;

}
 
//###############################################################################################
// Array2DDelegate PROTOCOL SUPPORT
//###############################################################################################

// Required: creates new tile instance for 2D array
//-----------------------------------------------------------------------------------------------
- (id)createInstanceOfArray:(Array2D*)array X:(uint)x Y:(uint)y DimX:(uint)dimX DimY:(uint)dimY
{
	TileItem* newTile = [[TileItem alloc] initWithTileset:self];
	
	newTile.propIndexX		= x;
	newTile.propIndexY		= y;
	newTile.propIndexMem	= y * dimX + x;
	
	return newTile;
}


@end
