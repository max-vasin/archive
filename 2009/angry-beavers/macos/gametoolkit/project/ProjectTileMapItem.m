//===============================================================================================
//  File:		ProjectTileMapItem.m
//  Project:	OriginGameToolkit
//	Desc:		Tilemap project item implementation
//
//  Created by Max Vasin on 4/26/11.
//  Copyright 2011 Origin Games. All rights reserved.
//===============================================================================================

// ****** IMPORTS ******

#import "ProjectTileMapItem.h"
#import "SharedResourceManager.h"
#import "ProjectRootItems.h"

// ****** PRIVATE METHODS ******

@interface ProjectTileMapItem (Private)

- (TileItem*)					getTileAtX:(uint)x	Y:(uint)y;
- (ProjectMultiSpriteItem*)		getMultiSprite:(NSString*)name;
- (ProjectSpriteItem*)			getSprite:(NSString*)name;

@end


//###############################################################################################
// HexGridCell IMPLEMENTATION
//###############################################################################################



@implementation HexGridCell

//###############################################################################################
// PROPERTIES
//###############################################################################################

// Synthesized
//-----------------------------------------------------------------------------------------------
@synthesize propIndexX		= m_indexX;
@synthesize propIndexY		= m_indexY;
@synthesize propTypeID		= m_typeID;

//###############################################################################################
// INITIALIZERS\DESTRUCTORS
//###############################################################################################

// New hex grid cell initializer
//-----------------------------------------------------------------------------------------------
- (id)initWithMap:(ProjectTileMapItem*)map
{
	if ( (self = [self init]) )
	{
		m_map	= map;
	}
	return self;
}

// Hex grid cell initializer with XML data
//-----------------------------------------------------------------------------------------------
- (id)initWithMap:(ProjectTileMapItem*)map XML:(NSXMLElement*)node
{
	if ( (self = [self initWithMap:map]) )
	{
		m_indexX	= [node getFirstChildInteger:@"X"];
		m_indexY	= [node getFirstChildInteger:@"Y"];
		m_typeID	= [node getFirstChildInteger:@"Type"];
	}
	return self;
}

//###############################################################################################
// SAVING\BUILDING
//###############################################################################################

// Saves cell to XML
//-----------------------------------------------------------------------------------------------
- (NSXMLElement*)saveXML:(NSXMLElement*)node
{
	NSXMLElement* thisNode = [node addNewChild:@"Cell"];
	
	[thisNode addNewChild:@"X"		withInteger:m_indexX];
	[thisNode addNewChild:@"Y"		withInteger:m_indexY];
	[thisNode addNewChild:@"Type"	withInteger:m_typeID];
	
	return thisNode;
}

// Builds XML
//-----------------------------------------------------------------------------------------------
- (NSXMLElement*)buildXML:(NSXMLElement*)node
{
	NSXMLElement* thisNode = [node addNewChild:@"Cell"];
	
	[thisNode addNewChild:@"X"		withInteger:m_indexX];
	[thisNode addNewChild:@"Y"		withInteger:m_indexY];
	[thisNode addNewChild:@"Type"	withInteger:m_typeID];
	
	return thisNode;
}

@end



//###############################################################################################
// TileMapNode IMPLEMENTATION
//###############################################################################################



@implementation TileMapNode

//###############################################################################################
// PROPERTIES
//###############################################################################################

// Synthesized
//-----------------------------------------------------------------------------------------------
@synthesize propIndexX		= m_indexX;
@synthesize propIndexY		= m_indexY;
@synthesize propTile		= m_tile;

//###############################################################################################
// INITIALIZERS\DESTRUCTORS
//###############################################################################################

// New map node initializer
//-----------------------------------------------------------------------------------------------
- (id)initWithMap:(ProjectTileMapItem*)map
{
	if ( (self = [self init]) )
	{
		m_map	= map;
	}
	return self;
}

// Map node initializer with XML data
//-----------------------------------------------------------------------------------------------
- (id)initWithMap:(ProjectTileMapItem*)map XML:(NSXMLElement*)node
{
	if ( (self = [self initWithMap:map]) )
	{
		m_indexX	= [node getFirstChildInteger:@"X"];
		m_indexY	= [node getFirstChildInteger:@"Y"];
		
		int tileX	= [node getFirstChildInteger:@"TileX"];
		int tileY	= [node getFirstChildInteger:@"TileY"];
		
		m_tile		= [m_map getTileAtX:tileX Y:tileY];
	}
	return self;
}

//###############################################################################################
// SAVING\BUILDING
//###############################################################################################

// Saves node to XML
//-----------------------------------------------------------------------------------------------
- (NSXMLElement*)saveXML:(NSXMLElement*)node
{
	NSXMLElement* thisNode = [node addNewChild:@"Node"];
	
	[thisNode addNewChild:@"X"		withInteger:m_indexX];
	[thisNode addNewChild:@"Y"		withInteger:m_indexY];
	[thisNode addNewChild:@"TileX"	withInteger:m_tile.propIndexX];
	[thisNode addNewChild:@"TileY"	withInteger:m_tile.propIndexY];
	
	return thisNode;
}

// Building XML data
//-----------------------------------------------------------------------------------------------
- (NSXMLElement*)buildXML:(NSXMLElement*)node
{
	NSXMLElement* thisNode = [node addNewChild:@"Node"];
	
	[thisNode addNewChild:@"X"		withInteger:m_indexX];
	[thisNode addNewChild:@"Y"		withInteger:m_indexY];
	[thisNode addNewChild:@"TileX"	withInteger:m_tile.propIndexX];
	[thisNode addNewChild:@"TileY"	withInteger:m_tile.propIndexY];
	
	return thisNode;
}

@end


//###############################################################################################
// StaticWrapper IMPLEMENTATION
//###############################################################################################




@implementation StaticWrapper

//###############################################################################################
// PROPERTIES
//###############################################################################################

// Synthesized
//-----------------------------------------------------------------------------------------------
@synthesize propItem		= m_item;
@synthesize propPosition	= m_position;

// Floating point position
//-----------------------------------------------------------------------------------------------
- (void)setPropPositionFloat:(NSPoint)point
{
	m_position.x = point.x;
	m_position.y = point.y;
	
}

- (NSPoint)propPositionFloat
{
	NSPoint pos = {m_position.x, m_position.y};
	
	return pos;
}

// Description
//-----------------------------------------------------------------------------------------------
- (void)setPropDescription:(NSString*)desc
{
	// dummy
}

- (NSString*)propDescription
{
	Class itemClass = [m_item class];
	
	if (itemClass == [ProjectSpriteItem class])
	{
		return [NSString stringWithFormat:@"Sprite: %@", m_item.propName];
	}
	else if (itemClass == [ProjectMultiSpriteItem class])
	{
		return [NSString stringWithFormat:@"Multisprite: %@", m_item.propName];
	}
	
	return @"Unknown";
}



//###############################################################################################
// INITIALIZERS\DESTRUCTORS
//###############################################################################################

// Creates new autoreleased instance of static wrapper
//-----------------------------------------------------------------------------------------------
+ (id)wrapperWithItem:(ProjectItem*)item
{
	return [[[self alloc] initWithItem:item] autorelease];
}

// Initializes new static wrapper with project item relative to
//-----------------------------------------------------------------------------------------------
- (id)initWithItem:(ProjectItem*)item
{
	if ( (self = [self init]) )
	{
		m_item = item;
	}
	return self;
}

// Initializes new static wrapper XML node
//-----------------------------------------------------------------------------------------------
- (id)initWithXML:(NSXMLElement*)node TileMap:(ProjectTileMapItem*)map
{
	if ( (self = [self init]) )
	{
		Class class		= NSClassFromString([node getFirstChildValue:@"Class"]);
		NSString* name	= [node getFirstChildValue:@"Name"];
		
		if (class == [ProjectSpriteItem class])
		{
			m_item = [map getSprite:name];
		}
		else if (class == [ProjectMultiSpriteItem class])
		{
			m_item = [map getMultiSprite:name];
		}
		
		m_position.x = [node getFirstChildInteger:@"PosX"];
		m_position.y = [node getFirstChildInteger:@"PosY"];
	}
	return self;
}


//###############################################################################################
// SAVING\BUILDING
//###############################################################################################

// Saves static wrapper to XML
//-----------------------------------------------------------------------------------------------
- (NSXMLElement*)saveXML:(NSXMLElement*)node
{
	NSXMLElement* thisNode = [node addNewChild:@"Static"];
	
	NSString* className = NSStringFromClass([m_item class]);
	
	[thisNode addNewChild:@"Class"	withValue:className];
	[thisNode addNewChild:@"Name"	withValue:m_item.propName];
	[thisNode addNewChild:@"PosX"	withInteger:m_position.x];
	[thisNode addNewChild:@"PosY"	withInteger:m_position.y];
	
	return thisNode;
}

// Builds static wrapper
//-----------------------------------------------------------------------------------------------
- (NSXMLElement*)buildXML:(NSXMLElement*)node
{
	NSXMLElement* thisNode = [node addNewChild:@"Static"];
	
	NSString* className = @"unknown";
	
	if		([m_item class] == [ProjectMultiSpriteItem class])
	{
		className = @"multisprite";
	}
	else if	([m_item class] == [ProjectSpriteItem class])
	{
		className = @"sprite";
	}
		
	[thisNode addNewChild:@"Class"	withValue:className];
	[thisNode addNewChild:@"Name"	withValue:m_item.propName];
	[thisNode addNewChild:@"PosX"	withInteger:m_position.x];
	[thisNode addNewChild:@"PosY"	withInteger:m_position.y];
	
	return thisNode;
}


//###############################################################################################
// DATA ACCESS
//###############################################################################################

// Sorts static by X:Y position
//-----------------------------------------------------------------------------------------------
- (NSInteger)layerSort:(StaticWrapper*)entry
{
	if (m_position.y < entry->m_position.y)
		return NSOrderedAscending;
	
	if (m_position.y  == entry->m_position.y)
		return NSOrderedSame;
	
	return NSOrderedDescending;
}

@end



//###############################################################################################
// ProjectTileMapItem IMPLEMENTATION
//###############################################################################################




@implementation ProjectTileMapItem

//###############################################################################################
// PROPERTIES
//###############################################################################################

// Synthesized
//-----------------------------------------------------------------------------------------------
@synthesize propTileset				= m_tileset;
@synthesize propEditTile			= m_editTile;
@synthesize propBackColor			= m_backColor;
@synthesize propEditSprite			= m_staticEditSprite;
@synthesize propEditMultiSprite		= m_staticEditMultiSprite;
@synthesize propCurrentStatic		= m_currentStatic;
@synthesize propGridAspect			= m_gridAspect;
@synthesize propGridOffsetX			= m_gridOffsetX;
@synthesize propGridOffsetY			= m_gridOffsetY;

// Map width (in tiles)
//-----------------------------------------------------------------------------------------------
- (void)setPropMapWidth:(uint)width
{
	m_mapWidth = width;
	
	[m_map resizeToX:width Y:m_mapHeight Create:YES];
}

- (uint)propMapWidth
{
	return m_mapWidth;
}

// Map height (in tiles)
//-----------------------------------------------------------------------------------------------
- (void)setPropMapHeight:(uint)height
{
	m_mapHeight = height;
	
	[m_map resizeToX:m_mapWidth Y:height Create:YES];
}

- (uint)propMapHeight
{
	return m_mapHeight;
}

// Map area (in tiles)
//-----------------------------------------------------------------------------------------------
- (uint)propMapArea
{
	return m_mapWidth * m_mapHeight;
}

// Map type
//-----------------------------------------------------------------------------------------------
- (void)setPropMapType:(enTileMapType)type
{
	m_type	= type;
}

- (enTileMapType)propMapType
{
	return m_type;
}

// Map types array
//-----------------------------------------------------------------------------------------------
- (NSArray*)propMapTypes
{
	return [NSArray arrayWithObjects:@"quads", @"diamonds", nil];
}

// Grid width (in cells)
//-----------------------------------------------------------------------------------------------
- (void)setPropGridWidth:(uint)width
{
	m_gridWidth	= width;
	
	[m_grid resizeToX:m_gridWidth Y:m_gridHeight Create:YES];
}

- (uint)propGridWidth
{
	return m_gridWidth;
}

// Grid height (in cells)
//-----------------------------------------------------------------------------------------------
- (void)setPropGridHeight:(uint)height
{
	m_gridHeight = height;
	
	[m_grid resizeToX:m_gridWidth Y:m_gridHeight Create:YES];
}

- (uint)propGridHeight
{
	return m_gridHeight;
}

// Grid area (in cells)
//-----------------------------------------------------------------------------------------------
- (uint)propGridArea
{
	return m_gridWidth * m_gridHeight;
}

// Grid cell edge (pixels)
//-----------------------------------------------------------------------------------------------
- (void)setPropGridCellEdge:(uint)edge
{
	m_gridCellEdge	= edge;
}

- (uint)propGridCellEdge
{
	return m_gridCellEdge;
}

// Staic objects count
//-----------------------------------------------------------------------------------------------
- (uint)propStaticCount
{
	return [m_static count];
}

// Staic objects list
//-----------------------------------------------------------------------------------------------
- (void)setPropStatic:(NSMutableArray*)list
{
	// dummy
}

- (NSMutableArray*)propStatic
{
	return m_static;
}

// Selection indexes
//-----------------------------------------------------------------------------------------------
- (void)setPropStaticIndexes:(NSIndexSet*)indexes
{
	if (!indexes || ![indexes count])
	{
		self.propCurrentStatic = nil;
	}
	else
	{
		self.propCurrentStatic	= [m_static objectAtIndex:[indexes firstIndex]];
	}
}

- (NSIndexSet*)propStaticIndexes
{
	if (!m_currentStatic)
		return nil;
	
	return [NSIndexSet indexSetWithIndex:[m_static indexOfObject:self.propCurrentStatic]];
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
		m_map		= [[Array2D alloc] initWithDelegate:self];
		m_grid		= [[Array2D alloc] initWithDelegate:self];
		m_static	= [[NSMutableArray alloc] initWithCapacity:10];
	}
	return self;
}

// Overriding (ProjectItem) initializer
//-----------------------------------------------------------------------------------------------
- (id)initWithParent:(ProjectItem*)parent fromXML:(NSXMLElement*)node
{
	if ( (self = [super initWithParent:parent fromXML:node]) )
	{
		if (!m_map)
			m_map	= [[Array2D alloc] initWithDelegate:self];
		
		if (!m_grid)
			m_grid	= [[Array2D alloc] initWithDelegate:self];
		
		if (!m_static)
			m_static = [[NSMutableArray alloc] initWithCapacity:10];
		
		ProjectRootItem* root = (ProjectRootItem*)m_itemParent->m_itemParent;
		
		m_tileset		= (ProjectTilesetItem*)[(ProjectTilesetRootItem*)root.propRootTilesets getChildByName:[node getFirstChildValue:@"Tileset"]];
		
		m_mapWidth			= [node getFirstChildInteger:@"Width"];
		m_mapHeight			= [node getFirstChildInteger:@"Height"];
		m_gridWidth			= [node getFirstChildInteger:@"GridWidth"];
		m_gridHeight		= [node getFirstChildInteger:@"GridHeight"];
		m_gridCellEdge		= [node getFirstChildInteger:@"GridCellEdge"];
		m_gridAspect		= [node getFirstChildFloat:@"GridAspect"];
		m_gridOffsetX		= [node getFirstChildInteger:@"GridOffsetX"];
		m_gridOffsetY		= [node getFirstChildInteger:@"GridOffsetY"];
		m_type				= [node getFirstChildInteger:@"Type"];
		self.propBackColor	= [node getFirstChildObject:@"BackColor" placeholder:[NSColor whiteColor]];
		
		[m_map	resizeToX:m_mapWidth Y:m_mapHeight Create:NO];
		[m_grid	resizeToX:m_gridWidth Y:m_gridHeight Create:NO];
		
		NSXMLElement* nodesNode = [node getFirstChild:@"Nodes"];
		
		for (NSXMLElement* nodeNode in [nodesNode children])
		{
			TileMapNode* newNode = [[TileMapNode alloc] initWithMap:self XML:nodeNode];
			
			[m_map replaceAtX:newNode.propIndexX Y:newNode.propIndexY With:newNode];
			
			[newNode release];
		}
		
		NSXMLElement* staticNode = [node getFirstChild:@"StaticLayer"];
		
		for (NSXMLElement* stNode in [staticNode children])
		{
			StaticWrapper* newWrapper = [[StaticWrapper alloc] initWithXML:stNode TileMap:self];
			
			[m_static addObject:newWrapper];
			
			[newWrapper release];
		}
	
		NSXMLElement* gridNode = [node getFirstChild:@"Grid"];
		
		for (NSXMLElement* cellNode in [gridNode children])
		{
			HexGridCell* newCell = [[HexGridCell alloc] initWithMap:self XML:cellNode];
			
			[m_grid replaceAtX:newCell.propIndexX Y:newCell.propIndexY With:newCell];
		
			[newCell release];
		}

	}
	return self;
}

// Destructor
//-----------------------------------------------------------------------------------------------
- (void)dealloc
{
	SAFE_RELEASE(m_map);
	SAFE_RELEASE(m_grid);
	SAFE_RELEASE(m_backColor);
	SAFE_RELEASE(m_static);
	
	[super dealloc];
}

//###############################################################################################
// MISC
//###############################################################################################

// Sets item icon
//-----------------------------------------------------------------------------------------------
- (void)setIcon
{
	m_itemImage = [SharedResourceManager sharedIcon:@"tilemap"];
}

//###############################################################################################
// SAVING\BUILDING
//###############################################################################################

// Makes XML representation fo current item and attach it to node
//-----------------------------------------------------------------------------------------------
- (NSXMLElement*)saveAsXML:(NSXMLElement*)node
{
	NSXMLElement* thisNode = [node addNewChild:@"TileMap"];
	
	[thisNode addNewChild:@"Name"				withValue:m_itemName];
	[thisNode addNewChild:@"Width"				withInteger:m_mapWidth];
	[thisNode addNewChild:@"Height"				withInteger:m_mapHeight];
	[thisNode addNewChild:@"Tileset"			withValue:m_tileset.propName];
	[thisNode addNewChild:@"Type"				withInteger:m_type];
	[thisNode addNewChild:@"BackColor"			withObject:m_backColor];
	[thisNode addNewChild:@"GridWidth"			withInteger:m_gridWidth];
	[thisNode addNewChild:@"GridHeight"			withInteger:m_gridHeight];
	[thisNode addNewChild:@"GridCellEdge"		withInteger:m_gridCellEdge];
	[thisNode addNewChild:@"GridAspect"			withFloat:m_gridAspect];
	[thisNode addNewChild:@"GridOffsetX"		withInteger:m_gridOffsetX];
	[thisNode addNewChild:@"GridOffsetY"		withInteger:m_gridOffsetY];
	
	NSXMLElement* nodesNode = [thisNode addNewChild:@"Nodes"];
	
	for (int i = 0; i < self.propMapArea; i++)
		[(TileMapNode*)[m_map getObjectAtIndex:i] saveXML:nodesNode];
	
	NSXMLElement* staticNode = [thisNode addNewChild:@"StaticLayer"];
	
	for (StaticWrapper* wrapper in m_static)
		[wrapper saveXML:staticNode];
	
	NSXMLElement* gridNode = [thisNode addNewChild:@"Grid"];
	
	for (int i = 0; i < self.propGridArea; i++)
		[(HexGridCell*)[m_grid getObjectAtIndex:i] saveXML:gridNode];
	
	return thisNode;
}

// Building final tilemap output
//-----------------------------------------------------------------------------------------------
- (result)build:(Progressor*)progress
{
	result res = [super build:progress];
	if (FAIL(res))
		return res;
	
	progress.propCurrentState = [NSString stringWithFormat:@"Exporting: %@", m_itemName];
	
	// Make XML scheme of sprite
	
	NSXMLElement* rootNode = [[[NSXMLElement alloc] initWithName:@"TileMap"] autorelease];
	
	[rootNode addNewChild:@"Name"		withValue:m_itemName];
	[rootNode addNewChild:@"Width"		withInteger:m_mapWidth];
	[rootNode addNewChild:@"Height"		withInteger:m_mapHeight];
	[rootNode addNewChild:@"Tileset"	withValue:m_tileset.propName];
	[rootNode addNewChild:@"Type"		withInteger:m_type];
	
	// Nodes
	
	NSXMLElement* nodesNode = [rootNode addNewChild:@"Tiles"];
	
	for (int i = 0; i < self.propMapArea; i++)
		[(TileMapNode*)[m_map getObjectAtIndex:i] buildXML:nodesNode];
	
	// Static
	
	NSXMLElement* staticNode = [rootNode addNewChild:@"StaticLayer"];
	
	for (StaticWrapper* wrapper in m_static)
		[wrapper buildXML:staticNode];
	
	// Grid
	
	NSXMLElement* gridNode = [rootNode addNewChild:@"Grid"];
	
	NSXMLElement* sizeNode = [gridNode addNewChild:@"Size"];
	
	[sizeNode addNewChild:@"W"	withInteger:m_grid.propDimX];
	[sizeNode addNewChild:@"H"	withInteger:m_grid.propDimY];
	
	NSXMLElement* cellSizeNode = [gridNode addNewChild:@"CellSize"];
	
	[cellSizeNode addNewChild:@"S"	withInteger:m_gridCellEdge];
	[cellSizeNode addNewChild:@"A"	withFloat:m_gridAspect];
	
	NSXMLElement* offsetNode = [gridNode addNewChild:@"Offset"];
	
	[offsetNode addNewChild:@"X"	withInteger:m_gridOffsetX];
	[offsetNode addNewChild:@"Y"	withInteger:m_gridOffsetY];
	
	NSXMLElement* cellsNode = [gridNode addNewChild:@"Cells"];
	
	for (int i = 0; i < self.propGridArea; i++)
		[(HexGridCell*)[m_grid getObjectAtIndex:i] buildXML:cellsNode];

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
// DATA ACCESS
//###############################################################################################

// Returns map node at given X:Y position
//-----------------------------------------------------------------------------------------------
- (TileMapNode*)getNodeAtX:(uint)x Y:(uint)y
{
	return [m_map getObjectAtX:x Y:y];
}

// Returns static at given index
//-----------------------------------------------------------------------------------------------
- (StaticWrapper*)getStaticAtIndex:(uint)index
{
	if (index < [m_static count])
		return [m_static objectAtIndex:index];
	
	return nil;
}

// Places current tile at given position
//-----------------------------------------------------------------------------------------------
- (void)placeEditTileAtX:(uint)x Y:(uint)y
{
	TileMapNode*	node = [self getNodeAtX:x Y:y];
	TileItem*		tile = [m_tileset getTileAtX:m_editTile.x Y:m_editTile.y];
	
	if (node && tile)
		node.propTile = tile;
}

// Places current sprite at given static position
//-----------------------------------------------------------------------------------------------
- (void)placeEditSpriteAtX:(int)x Y:(int)y
{
	
}

// Places current multisprite at given static position
//-----------------------------------------------------------------------------------------------
- (void)placeEditMultiSpriteAtX:(int)x Y:(int)y
{
	if (!m_staticEditMultiSprite)
		return;
	
	IntPoint pos = {x, y};
	
	StaticWrapper* wrapper = [StaticWrapper wrapperWithItem:m_staticEditMultiSprite];
	
	wrapper.propPosition = pos;
	
	[m_static addObject:wrapper];
	
	[m_static sortUsingSelector:@selector(layerSort:)];

	self.propStatic = nil;
}

// Deletes current static object
//-----------------------------------------------------------------------------------------------
- (void)delCurrentStatic
{
	if (!m_currentStatic)
		return;
	
	int index = [m_static indexOfObject:m_currentStatic];
	
	[m_static removeObject:m_currentStatic];
	
	if ([m_static count])
	{
		if (index >= [m_static count])
			index = [m_static count] - 1;
		
		self.propCurrentStatic = [m_static objectAtIndex:index];
	}
	
	[m_static sortUsingSelector:@selector(layerSort:)];

	self.propStatic = nil;
}

// Returns grid cell at given coordinates
//-----------------------------------------------------------------------------------------------
- (HexGridCell*)cellAtX:(uint)x Y:(uint)y
{
	return [m_grid getObjectAtX:x Y:y];
}


//###############################################################################################
// Array2DDelegate PROTOCOL SUPPORT
//###############################################################################################

// Required: creates new map node instance for 2D array
//-----------------------------------------------------------------------------------------------
- (id)createInstanceOfArray:(Array2D*)array X:(uint)x Y:(uint)y DimX:(uint)dimX DimY:(uint)dimY
{
	id object = nil;
	
	if (array == m_map)
	{
		TileMapNode* newNode = [[TileMapNode alloc] initWithMap:self];
	
		newNode.propIndexX	= x;
		newNode.propIndexY	= y;
	
		object = newNode;
	}
	else if (array == m_grid)
	{
		HexGridCell* newCell = [[HexGridCell alloc] initWithMap:self];
		
		newCell.propIndexX	= x;
		newCell.propIndexY	= y;
		
		object = newCell;
	}
	
	return object;
}

// Sorts static objects
//-----------------------------------------------------------------------------------------------
- (void)sortStatic
{
	[m_static sortUsingSelector:@selector(layerSort:)];
	
	self.propStatic = nil;
}

//###############################################################################################
// PRIVATE
//###############################################################################################

// Retruns base tileset tile with XY indexes
//-----------------------------------------------------------------------------------------------
- (TileItem*)getTileAtX:(uint)x Y:(uint)y
{
	return [m_tileset getTileAtX:x Y:y];
}

// Retruns multisprite item
//-----------------------------------------------------------------------------------------------
- (ProjectMultiSpriteItem*)getMultiSprite:(NSString*)name
{
	ProjectRootItem* root = (ProjectRootItem*)m_itemParent->m_itemParent;
	
	return (ProjectMultiSpriteItem*)[root.propRootMultiSprites getChildByName:name];
}

// Retruns sprite item
//-----------------------------------------------------------------------------------------------
- (ProjectSpriteItem*)getSprite:(NSString*)name
{
	ProjectRootItem* root = (ProjectRootItem*)m_itemParent->m_itemParent;
	
	return (ProjectSpriteItem*)[root.propRootSprites getChildByName:name];
}



@end
