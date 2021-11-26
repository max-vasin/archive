//===============================================================================================
//  File:		ProjectRootItems.m
//  Project:	OriginGameToolkit
//	Desc:		Project root items classes collection implementations
//
//  Created by Max Vasin on 11/24/10.
//  Copyright 2010 Origin. All rights reserved.
//===============================================================================================

// ****** IMPORTS ******

#import "ProjectRootItems.h"
#import "Project.h"
#import "SharedResourceManager.h"

//###############################################################################################
// ProjectRootItem IMPLEMENTATION
//###############################################################################################

// ****** PRIVATE METHODS *******

@interface ProjectRootItem(Private)

- (void)addRootItems:(NSXMLElement*)node;

@end

@implementation ProjectRootItem

// ****** PROPERTIES *******

// Synthesized
//-----------------------------------------------------------------------------------------------
@synthesize propRootDirectory		= m_rootDirectory;
@synthesize propBuildDirectory		= m_buildDirectory;
@synthesize propCacheDirectory		= m_cacheDirectory;
@synthesize propRootTextures		= m_rootTextures;
@synthesize propRootShapes			= m_rootShapes;
@synthesize propRootSprites			= m_rootSprites;
@synthesize propRootMultiSprites	= m_rootMultiSprites;
@synthesize propRootFonts			= m_rootFonts;
@synthesize propRootTileMaps		= m_rootTilemaps;
@synthesize propRootTilesets		= m_rootTilesets;

// ****** INSTANCE METHODS *******

// Initializes new item
//-----------------------------------------------------------------------------------------------
- (id)initWithProject:(Project*)project
{
	if ( (self = [super initWithProject:project]) )
	{
		m_itemName			= @"new project";
		m_rootDirectory		= @"/";
		m_buildDirectory	= @"/";
		
		[self addRootItems:nil];
	}
	return self;
}

// Initializes new item with project and XML data
//-----------------------------------------------------------------------------------------------
- (id)initWithProject:(Project *)project fromXML:(NSXMLElement*)node
{
	if ( (self = [super initWithProject:project fromXML:node]) )
	{
		NSXMLElement*	optsNode = [node getFirstChild:@"Preferences"];
		
		self.propRootDirectory	= [optsNode getFirstChildValue:@"RootDirectory"];
		self.propBuildDirectory	= [optsNode getFirstChildValue:@"BuildDirectory"];
		self.propCacheDirectory	= [optsNode getFirstChildValue:@"CacheDirectory"];
		
		[self addRootItems:node];
	}
	
	return self;
}

// Set item icon
//-----------------------------------------------------------------------------------------------
- (void)setIcon
{
	m_itemImage	 = [SharedResourceManager sharedIcon:@"project"];
	m_expandable = YES;
}

// Destructor
//-----------------------------------------------------------------------------------------------
- (void)dealloc
{
	SAFE_RELEASE(m_rootDirectory);
	SAFE_RELEASE(m_buildDirectory);
	SAFE_RELEASE(m_cacheDirectory);
	
	[super dealloc];
}

// Saves item as part of XML schema
//-----------------------------------------------------------------------------------------------
- (NSXMLElement*)saveAsXML:(NSXMLElement*)node
{
	NSXMLElement* thisNode = [super saveAsXML:[[[NSXMLElement alloc] initWithName:@"Project"] autorelease]];
	
	// Project preferences
	
	NSXMLElement* optsNode = [thisNode addNewChild:@"Preferences"];
	
	[optsNode addNewChild:@"RootDirectory"	withValue:m_rootDirectory];
	[optsNode addNewChild:@"BuildDirectory"	withValue:m_buildDirectory];
	[optsNode addNewChild:@"CacheDirectory"	withValue:m_cacheDirectory];
	
	NSXMLElement* itemsNode = [thisNode addNewChild:@"Items"];
	
	// Save children items
	
	for (ProjectItem* item in m_itemChildren)
		[item saveAsXML:itemsNode];
	
	return thisNode;
}

// Adds root items
//-----------------------------------------------------------------------------------------------
- (void)addRootItems:(NSXMLElement*)node;
{
	NSXMLElement* rootNode;
	NSXMLElement* itemsNode = [node getFirstChild:@"Items"];
	
	[m_itemChildren removeAllObjects];
		
	// Textures
	
	if ( (rootNode = [itemsNode getFirstChild:@"Textures"]) )
		m_rootTextures = [[[ProjectTextureRootItem alloc]		initWithParent:self fromXML:rootNode] autorelease];
	else
		m_rootTextures = [[[ProjectTextureRootItem alloc]		initWithParent:self] autorelease];
	
	
	// Shapes
	
	if ( (rootNode = [itemsNode getFirstChild:@"Shapes"]) )
		m_rootShapes = [[[ProjectShapeRootItem alloc]		initWithParent:self fromXML:rootNode] autorelease];
	else
		m_rootShapes = [[[ProjectShapeRootItem alloc]		initWithParent:self] autorelease];
	
	// Sprites
	
	if ( (rootNode = [itemsNode getFirstChild:@"Sprites"]) )
		m_rootSprites = [[[ProjectSpriteRootItem alloc]		initWithParent:self fromXML:rootNode] autorelease];
	else
		m_rootSprites = [[[ProjectSpriteRootItem alloc]		initWithParent:self] autorelease];
	
	// Multisprites
	
	if ( (rootNode = [itemsNode getFirstChild:@"MultiSprites"]) )
		m_rootMultiSprites = [[[ProjectMultiSpriteRootItem alloc] initWithParent:self fromXML:rootNode] autorelease];
	else
		m_rootMultiSprites = [[[ProjectMultiSpriteRootItem alloc] initWithParent:self] autorelease];
	
	// Fonts
	
	if ( (rootNode = [itemsNode getFirstChild:@"Fonts"]) )
		m_rootFonts = [[[ProjectFontRootItem alloc] initWithParent:self fromXML:rootNode] autorelease];
	else
		m_rootFonts = [[[ProjectFontRootItem alloc] initWithParent:self] autorelease];


	// Tilesets
	
	if ( (rootNode = [itemsNode getFirstChild:@"Tilesets"]) )
		m_rootTilesets = [[[ProjectTilesetRootItem alloc] initWithParent:self fromXML:rootNode] autorelease];
	else
		m_rootTilesets = [[[ProjectTilesetRootItem alloc] initWithParent:self] autorelease];

	// Tilemaps
	
	if ( (rootNode = [itemsNode getFirstChild:@"TileMaps"]) )
		m_rootTilemaps = [[[ProjectTileMapRootItem alloc] initWithParent:self fromXML:rootNode] autorelease];
	else
		m_rootTilemaps = [[[ProjectTileMapRootItem alloc] initWithParent:self] autorelease];
}


@end

//###############################################################################################
// ProjectTextureRootItem IMPLEMENTATION
//###############################################################################################

@implementation ProjectTextureRootItem

// ****** PROPERTIES *******

// Total amout of memory used by cached textures
//-----------------------------------------------------------------------------------------------
- (unsigned int)propCachedMemory
{
	unsigned int total = 0;
	
	for (ProjectTextureItem* item in m_itemChildren)
		total += item.propCachedMemory;
	
	return total;
}

- (void)setPropCachedMemory:(unsigned int)value
{
	// dummy
}

// Total amout of memory used by packed textures
//-----------------------------------------------------------------------------------------------
- (unsigned int)propPackedMemory
{
	unsigned int total = 0;
	
	for (ProjectTextureItem* item in m_itemChildren)
		total += item.propPackedMemory;
	
	return total;
}

- (void)setPropPackedMemory:(unsigned int)value
{
	// dummy
}

// Total amout of memory used by packed textures (string representation)
//-----------------------------------------------------------------------------------------------
- (NSString*)propPackedMemoryString
{
	return [NSString stringWithFormat:@"%#1.2f Mb", (float)self.propPackedMemory / MB];
}

- (void)setPropPackedMemoryString:(NSString*)value
{
	// dummy
}

// Total amout of memory used by cached textures (string representation)
//-----------------------------------------------------------------------------------------------
- (NSString*)propCachedMemoryString
{
	return [NSString stringWithFormat:@"%#1.2f Mb", (float)self.propCachedMemory / MB];
}

- (void)setPropCachedMemoryString:(NSString*)value
{
	// dummy
}

// ****** INSTANCE METHODS *******

// Initializes new item with parent
//-----------------------------------------------------------------------------------------------
- (id)initWithParent:(ProjectItem*)parent
{
	if ( (self = [super initWithParent:parent]) )
	{
		self.propName			= @"textures";
		self.propChildName		= @"texture";
	}
	return self;
}


// Initializes new item with parent and XML data
//-----------------------------------------------------------------------------------------------
- (id)initWithParent:(ProjectItem*)item fromXML:(NSXMLElement*)node
{
	if ( (self = [super initWithParent:item fromXML:node]) )
	{
		self.propName			= @"textures";
		self.propChildName		= @"texture";
		
		NSXMLElement*	optsNode = [node getFirstChild:@"Preferences"];
		
		self.propLocalBuildDirectory = [optsNode getFirstChildValue:@"LocalBuildDirectory"];
				
		NSArray* itemsNode = [[node getFirstChild:@"Items"] children];
		
		for (NSXMLElement* itemNode in itemsNode)
			[[[ProjectTextureItem alloc] initWithParent:self fromXML:itemNode] autorelease];
	}
	
	return self;
}

// Saves item as part of XML schema
//-----------------------------------------------------------------------------------------------
- (NSXMLElement*)saveAsXML:(NSXMLElement*)node
{
	NSXMLElement* thisNode = [node addNewChild:@"Textures"];

	NSXMLElement* optsNode = [thisNode addNewChild:@"Preferences"];

	[optsNode addNewChild:@"LocalBuildDirectory" withValue:m_localBuildDirectory];
	
	NSXMLElement* itemsNode = [thisNode addNewChild:@"Items"];
	
	for (ProjectItem* item in m_itemChildren)
		[item saveAsXML:itemsNode];

	return thisNode;
}

// Updates item name value
//-----------------------------------------------------------------------------------------------
- (void)updateName
{
	[super updateName];
	
	self.propName = [NSString stringWithFormat:@"textures (%i)", [m_itemChildren count]];
	
	self.propCachedMemory		= 0;
	self.propPackedMemory		= 0;
	self.propCachedMemoryString = nil;
	self.propPackedMemoryString = nil;
}

// Set item icon
//-----------------------------------------------------------------------------------------------
- (void)setIcon
{
	m_itemImage = [SharedResourceManager sharedIcon:@"texture_root"];
	m_expandable = YES;
}

@end

//###############################################################################################
// ProjectShapeRootItem IMPLEMENTATION
//###############################################################################################

@implementation ProjectShapeRootItem

// ****** INSTANCE METHODS *******

// Initializes new item with parent
//-----------------------------------------------------------------------------------------------
- (id)initWithParent:(ProjectItem*)parent
{
	if ( (self = [super initWithParent:parent]) )
	{
		self.propName			= @"shapes";
		self.propChildName		= @"shape";
	}
	return self;
}

// Initializes new item with parent and XML data
//-----------------------------------------------------------------------------------------------
- (id)initWithParent:(ProjectItem*)parent fromXML:(NSXMLElement*)node
{
	if ( (self = [super initWithParent:parent fromXML:node]) )
	{
		self.propName		= @"shapes";
		self.propChildName	= @"shape";
		
		NSXMLElement*	optsNode = [node getFirstChild:@"Preferences"];
		
		self.propLocalBuildDirectory = [optsNode getFirstChildValue:@"LocalBuildDirectory"];

		NSArray* itemsNode = [[node getFirstChild:@"Items"] children];
		
		for (NSXMLElement* itemNode in itemsNode)
			[[[ProjectShapeItem alloc] initWithParent:self fromXML:itemNode] autorelease];
	}
	return self;
}


// Saves item as part of XML schema
//-----------------------------------------------------------------------------------------------
- (NSXMLElement*)saveAsXML:(NSXMLElement*)node
{
	NSXMLElement* thisNode = [node addNewChild:@"Shapes"];
	
	NSXMLElement* optsNode = [thisNode addNewChild:@"Preferences"];
	
	[optsNode addNewChild:@"LocalBuildDirectory" withValue:m_localBuildDirectory];
	
	NSXMLElement* itemsNode = [thisNode addNewChild:@"Items"];
	
	for (ProjectItem* item in m_itemChildren)
		[item saveAsXML:itemsNode];

	return thisNode;
}

// Updates item name value
//-----------------------------------------------------------------------------------------------
- (void)updateName
{
	[super updateName];
	
	self.propName = [NSString stringWithFormat:@"shapes (%i)", [m_itemChildren count]];
}

// Set item icon
//-----------------------------------------------------------------------------------------------
- (void)setIcon
{
	m_itemImage = [SharedResourceManager sharedIcon:@"shape_root"];
	m_expandable = YES;
}

@end

//###############################################################################################
// ProjectSpriteRootItem IMPLEMENTATION
//###############################################################################################

@implementation ProjectSpriteRootItem 

// ****** INSTANCE METHODS *******

// Initializes new item with parent
//-----------------------------------------------------------------------------------------------
- (id)initWithParent:(ProjectItem*)parent
{
	if ( (self = [super initWithParent:parent]) )
	{
		self.propName			= @"sprites";
		self.propChildName		= @"sprite";
	}
	return self;
}

// Initializes new item with parent and XML data
//-----------------------------------------------------------------------------------------------
- (id)initWithParent:(ProjectItem*)parent fromXML:(NSXMLElement*)node
{
	if ( (self = [super initWithParent:parent fromXML:node]) )
	{
		self.propName		= @"sprites";
		self.propChildName	= @"sprite";
		
		NSXMLElement*	optsNode = [node getFirstChild:@"Preferences"];
		
		self.propLocalBuildDirectory = [optsNode getFirstChildValue:@"LocalBuildDirectory"];
		
		// Shapes
		
		NSArray* itemsNode = [[node getFirstChild:@"Items"] children];
		
		for (NSXMLElement* itemNode in itemsNode)
			[[[ProjectSpriteItem alloc] initWithParent:self fromXML:itemNode] autorelease];
	}
	return self;
}


// Saves item as part of XML schema
//-----------------------------------------------------------------------------------------------
- (NSXMLElement*)saveAsXML:(NSXMLElement*)node
{
	NSXMLElement* thisNode = [node addNewChild:@"Sprites"];
	
	NSXMLElement* optsNode = [thisNode addNewChild:@"Preferences"];
	
	[optsNode addNewChild:@"LocalBuildDirectory" withValue:m_localBuildDirectory];
	
	NSXMLElement* itemsNode = [thisNode addNewChild:@"Items"];
	
	// Save shapes
	
	for (ProjectItem* item in m_itemChildren)
		[item saveAsXML:itemsNode];
	
	return thisNode;
}

// Updates item name value
//-----------------------------------------------------------------------------------------------
- (void)updateName
{
	[super updateName];
	
	self.propName = [NSString stringWithFormat:@"sprites (%i)", [m_itemChildren count]];
}

// Set item icon
//-----------------------------------------------------------------------------------------------
- (void)setIcon
{
	m_itemImage = [SharedResourceManager sharedIcon:@"sprite_root"];
	m_expandable = YES;
}

// Returns array of linked sprite animations
//-----------------------------------------------------------------------------------------------
- (NSArray*)getLinkedAnimations:(TextureAssemblyItem*)item
{
	NSMutableArray* temp = [[NSMutableArray alloc] initWithCapacity:10];
	
	for (ProjectSpriteItem* sprite in m_itemChildren)
	{
		NSArray* links = [sprite getLinkedAnimations:item];
		
		for (SpriteAnimation* anim in links)
			[temp addObject:anim];
	}
	
	NSArray* result = [NSArray arrayWithArray:temp];
	
	SAFE_RELEASE(temp);
	
	return result;
}

@end

//###############################################################################################
// ProjectFontRootItem IMPLEMENTATION
//###############################################################################################

@implementation ProjectFontRootItem

// ****** INSTANCE METHODS *******

// Initializes new item with parent
//-----------------------------------------------------------------------------------------------
- (id)initWithParent:(ProjectItem*)parent
{
	if ( (self = [super initWithParent:parent]) )
	{
		self.propName			= @"fonts";
		self.propChildName		= @"font";
	}
	return self;
}

// Initializes new item with parent and XML data
//-----------------------------------------------------------------------------------------------
- (id)initWithParent:(ProjectItem*)parent fromXML:(NSXMLElement*)node
{
	if ( (self = [super initWithParent:parent fromXML:node]) )
	{
		self.propName		= @"fonts";
		self.propChildName	= @"font";
		
		NSXMLElement*	optsNode = [node getFirstChild:@"Preferences"];
		
		self.propLocalBuildDirectory = [optsNode getFirstChildValue:@"LocalBuildDirectory"];
		
		// Shapes
		
		NSArray* itemsNode = [[node getFirstChild:@"Items"] children];
		
		for (NSXMLElement* itemNode in itemsNode)
			[[[ProjectFontItem alloc] initWithParent:self fromXML:itemNode] autorelease];
	}
	return self;
}


// Saves item as part of XML schema
//-----------------------------------------------------------------------------------------------
- (NSXMLElement*)saveAsXML:(NSXMLElement*)node
{
	NSXMLElement* thisNode = [node addNewChild:@"Fonts"];
	
	NSXMLElement* optsNode = [thisNode addNewChild:@"Preferences"];
	
	[optsNode addNewChild:@"LocalBuildDirectory" withValue:m_localBuildDirectory];
	
	NSXMLElement* itemsNode = [thisNode addNewChild:@"Items"];
	
	// Save shapes
	
	for (ProjectItem* item in m_itemChildren)
		[item saveAsXML:itemsNode];
	
	return thisNode;
}

// Updates item name value
//-----------------------------------------------------------------------------------------------
- (void)updateName
{
	[super updateName];
	
	self.propName = [NSString stringWithFormat:@"fonts (%i)", [m_itemChildren count]];
}

// Set item icon
//-----------------------------------------------------------------------------------------------
- (void)setIcon
{
	m_itemImage = [SharedResourceManager sharedIcon:@"font_root"];
	m_expandable = YES;
}

@end

//###############################################################################################
// ProjectMultiSpriteRootItem IMPLEMENTATION
//###############################################################################################

@implementation ProjectMultiSpriteRootItem

// ****** INSTANCE METHODS *******

// Initializes new item with parent
//-----------------------------------------------------------------------------------------------
- (id)initWithParent:(ProjectItem*)parent
{
	if ( (self = [super initWithParent:parent]) )
	{
		self.propName			= @"multisprites";
		self.propChildName		= @"multisprite";
	}
	return self;
}

// Initializes new item with parent and XML data
//-----------------------------------------------------------------------------------------------
- (id)initWithParent:(ProjectItem*)parent fromXML:(NSXMLElement*)node
{
	if ( (self = [super initWithParent:parent fromXML:node]) )
	{
		self.propName		= @"multisprites";
		self.propChildName	= @"multisprite";
		
		NSXMLElement*	optsNode = [node getFirstChild:@"Preferences"];
		
		self.propLocalBuildDirectory = [optsNode getFirstChildValue:@"LocalBuildDirectory"];
		
		// Shapes
		
		NSArray* itemsNode = [[node getFirstChild:@"Items"] children];
		
		for (NSXMLElement* itemNode in itemsNode)
			[[[ProjectMultiSpriteItem alloc] initWithParent:self fromXML:itemNode] autorelease];
	}
	return self;
}


// Saves item as part of XML schema
//-----------------------------------------------------------------------------------------------
- (NSXMLElement*)saveAsXML:(NSXMLElement*)node
{
	NSXMLElement* thisNode = [node addNewChild:@"MultiSprites"];
	
	NSXMLElement* optsNode = [thisNode addNewChild:@"Preferences"];
	
	[optsNode addNewChild:@"LocalBuildDirectory" withValue:m_localBuildDirectory];
	
	NSXMLElement* itemsNode = [thisNode addNewChild:@"Items"];
	
	// Save shapes
	
	for (ProjectItem* item in m_itemChildren)
		[item saveAsXML:itemsNode];
	
	return thisNode;
}

// Updates item name value
//-----------------------------------------------------------------------------------------------
- (void)updateName
{
	[super updateName];
	
	self.propName = [NSString stringWithFormat:@"multisprites (%i)", [m_itemChildren count]];
}

// Set item icon
//-----------------------------------------------------------------------------------------------
- (void)setIcon
{
	m_itemImage = [SharedResourceManager sharedIcon:@"multisprite_root"];
	m_expandable = YES;
}

@end

//###############################################################################################
// ProjectTilesetRootItem IMPLEMENTATION
//###############################################################################################

@implementation ProjectTilesetRootItem

// ****** INSTANCE METHODS *******

// Initializes new item with parent
//-----------------------------------------------------------------------------------------------
- (id)initWithParent:(ProjectItem*)parent
{
	if ( (self = [super initWithParent:parent]) )
	{
		self.propName			= @"tilesets";
		self.propChildName		= @"tileset";
	}
	return self;
}

// Initializes new item with parent and XML data
//-----------------------------------------------------------------------------------------------
- (id)initWithParent:(ProjectItem*)parent fromXML:(NSXMLElement*)node
{
	if ( (self = [super initWithParent:parent fromXML:node]) )
	{
		self.propName		= @"tilesets";
		self.propChildName	= @"tileset";
		
		NSXMLElement*	optsNode = [node getFirstChild:@"Preferences"];
		
		self.propLocalBuildDirectory = [optsNode getFirstChildValue:@"LocalBuildDirectory"];
		
		// Tilesets
		
		NSArray* itemsNode = [[node getFirstChild:@"Items"] children];
		
		for (NSXMLElement* itemNode in itemsNode)
			[[[ProjectTilesetItem alloc] initWithParent:self fromXML:itemNode] autorelease];
	}
	return self;
}


// Saves item as part of XML schema
//-----------------------------------------------------------------------------------------------
- (NSXMLElement*)saveAsXML:(NSXMLElement*)node
{
	NSXMLElement* thisNode = [node addNewChild:@"Tilesets"];
	
	NSXMLElement* optsNode = [thisNode addNewChild:@"Preferences"];
	
	[optsNode addNewChild:@"LocalBuildDirectory" withValue:m_localBuildDirectory];
	
	NSXMLElement* itemsNode = [thisNode addNewChild:@"Items"];
	
	// Save shapes
	
	for (ProjectItem* item in m_itemChildren)
		[item saveAsXML:itemsNode];
	
	return thisNode;
}

// Updates item name value
//-----------------------------------------------------------------------------------------------
- (void)updateName
{
	[super updateName];
	
	self.propName = [NSString stringWithFormat:@"tilesets (%i)", [m_itemChildren count]];
}

// Set item icon
//-----------------------------------------------------------------------------------------------
- (void)setIcon
{
	m_itemImage = [SharedResourceManager sharedIcon:@"tileset_root"];
	m_expandable = YES;
}

@end

//###############################################################################################
// TilemapGridAttribute IMPLEMENTATION
//###############################################################################################

@implementation TilemapGridCellType

// ****** PROPERTIES *******

// Synthesized
//-----------------------------------------------------------------------------------------------
@synthesize propID		= m_id;
@synthesize propName	= m_name;
@synthesize propColor	= m_color;

// Generic transparent color
//-----------------------------------------------------------------------------------------------
- (NSColor*)propTransparentColor
{
	return [NSColor colorWithCalibratedRed:[m_color redComponent]
									 green:[m_color greenComponent]
									  blue:[m_color blueComponent]
									 alpha:0.5f];
}


// ****** INSTANCE METHODS *******

// Initializes new attribute with identifier
//-----------------------------------------------------------------------------------------------
- (id)initWithID:(uint)ID
{
	if ( (self = [self init]) )
	{
		self.propID		= ID;
		self.propName	= [NSString stringWithFormat:@"cell type %i", m_id];
		self.propColor	= [NSColor	randomColor];
	}
	return self;
}


// Initializes new attribute with XML data
//-----------------------------------------------------------------------------------------------
- (id)initWithXML:(NSXMLElement*)node
{
	if ( (self = [self init]) )
	{
		self.propID		= [node getFirstChildInteger:@"ID"];
		self.propName	= [node getFirstChildValue:@"Name"];
		self.propColor	= [node getFirstChildObject:@"Color"];
	}
	return self;
}

// Saves attrubute to XML
//-----------------------------------------------------------------------------------------------
- (NSXMLElement*)saveXML:(NSXMLElement*)node
{
	NSXMLElement* thisNode = [node addNewChild:@"CellType"];
	
	[thisNode addNewChild:@"ID"			withInteger:m_id];
	[thisNode addNewChild:@"Name"		withValue:m_name];
	[thisNode addNewChild:@"Color"		withObject:m_color];
	
	return thisNode;
}


@end

//###############################################################################################
// ProjectTileMapRootItem IMPLEMENTATION
//###############################################################################################

// Private methods
//-----------------------------------------------------------------------------------------------
@interface ProjectTileMapRootItem (Private)

- (uint)	uniqueCellTypeID;

@end

@implementation ProjectTileMapRootItem

// ****** PROPERTIES *******

// Synthesized
//-----------------------------------------------------------------------------------------------
@synthesize propCurrentGridAttribute = m_currentCellType;

// Grid attrbutes array
//-----------------------------------------------------------------------------------------------
- (void)setPropGridAttributes:(NSArray*)list
{
	// dummy
}

- (NSArray*)propGridAttributes
{
	return m_gridCellTypes;
}

// ****** INSTANCE METHODS *******

// Initializes new item with parent
//-----------------------------------------------------------------------------------------------
- (id)initWithParent:(ProjectItem*)parent
{
	if ( (self = [super initWithParent:parent]) )
	{
		self.propName			= @"tilemaps";
		self.propChildName		= @"tilemap";
		m_gridCellTypes			= [[NSMutableArray alloc] init];
	}
	return self;
}

// Initializes new item with parent and XML data
//-----------------------------------------------------------------------------------------------
- (id)initWithParent:(ProjectItem*)parent fromXML:(NSXMLElement*)node
{
	if ( (self = [super initWithParent:parent fromXML:node]) )
	{
		self.propName		= @"tilemaps";
		self.propChildName	= @"tilemap";
		
		NSXMLElement*	optsNode = [node getFirstChild:@"Preferences"];
		
		self.propLocalBuildDirectory = [optsNode getFirstChildValue:@"LocalBuildDirectory"];
		
		NSXMLElement*	cellTypesNode = [optsNode getFirstChild:@"GridCellTypes"];
		
		for (NSXMLElement* cellTypeNode in [cellTypesNode children])
		{
			TilemapGridCellType* type = [[TilemapGridCellType alloc] initWithXML:cellTypeNode];
			
			[m_gridCellTypes addObject:type];
			
			[type release];
		}
		
		// Tilemaps
		
		NSArray* itemsNode = [[node getFirstChild:@"Items"] children];
		
		for (NSXMLElement* itemNode in itemsNode)
			[[[ProjectTileMapItem alloc] initWithParent:self fromXML:itemNode] autorelease];
	}
	return self;
}


// Saves item as part of XML schema
//-----------------------------------------------------------------------------------------------
- (NSXMLElement*)saveAsXML:(NSXMLElement*)node
{
	NSXMLElement* thisNode = [node addNewChild:@"TileMaps"];
	
	NSXMLElement* optsNode = [thisNode addNewChild:@"Preferences"];
	
	[optsNode addNewChild:@"LocalBuildDirectory" withValue:m_localBuildDirectory];
	
	NSXMLElement* cellTypesNode = [optsNode addNewChild:@"GridCellTypes"];
	
	for (TilemapGridCellType* cellType in m_gridCellTypes)
		[cellType saveXML:cellTypesNode];
	
	NSXMLElement* itemsNode = [thisNode addNewChild:@"Items"];
	
	// Save tilemaps
	
	for (ProjectItem* item in m_itemChildren)
		[item saveAsXML:itemsNode];
	
	return thisNode;
}

// Updates item name value
//-----------------------------------------------------------------------------------------------
- (void)updateName
{
	[super updateName];
	
	self.propName = [NSString stringWithFormat:@"tilemaps (%i)", [m_itemChildren count]];
}

// Set item icon
//-----------------------------------------------------------------------------------------------
- (void)setIcon
{
	m_itemImage = [SharedResourceManager sharedIcon:@"tilemap_root"];
	m_expandable = YES;
}

// Returns unique attribute ID
//-----------------------------------------------------------------------------------------------
- (uint)uniqueCellTypeID
{
	uint curID = 0;
	
	while (YES)
	{
		BOOL founded = NO;
		
		for (TilemapGridCellType* cellType in m_gridCellTypes)
		{
			if (cellType.propID == curID)
			{
				founded = YES;
				break;
			}
		}
		
		if (!founded)
			break;
		
		curID++;
	}
	return curID;
}

// Returns all grid attributes in safe array
//-----------------------------------------------------------------------------------------------
- (NSArray*)gridAttributes
{
	return [NSArray arrayWithArray:m_gridCellTypes];
}

// Adds new tilemap grid attribute
//-----------------------------------------------------------------------------------------------
- (void)addGridAttribute
{
	TilemapGridCellType* cellType = [[TilemapGridCellType alloc] initWithID:[self uniqueCellTypeID]];
	
	[m_gridCellTypes addObject:cellType];
	
	[cellType release];
	
	self.propGridAttributes = nil;
}

// Deletes current grid attribute
//-----------------------------------------------------------------------------------------------
- (void)delGridAttrubute
{
	self.propCurrentGridAttribute = [m_gridCellTypes removeAndGet:self.propCurrentGridAttribute];
	
	self.propGridAttributes = nil;
}

// Returns cell type object by identifier
//-----------------------------------------------------------------------------------------------
- (TilemapGridCellType*)cellTypeByID:(uint)typeID
{
	for (TilemapGridCellType* type in m_gridCellTypes)
		if (type.propID == typeID)
			return type;
	
	return nil;
}


@end
