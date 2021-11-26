//===============================================================================================
//  File:		Project.m
//  Project:	OriginGameToolkit
//  Desc:		Project container class implementation (adopts data source protocol)
//
//  Created by Max Vasin on 11/24/10.
//  Copyright 2010 Origin. All rights reserved.
//===============================================================================================

// ***** IMPORTS *****

#import "Project.h"
#import "AppDelegate.h"
#import "ImageAndTextCell.h"

// ***** IMPLEMENTATION *****

@implementation Project

// ***** PROPERTIES *****

// Synthesized
//-----------------------------------------------------------------------------------------------
@synthesize propIsNewProject			= m_newProject;
@synthesize propModified				= m_modified;
@synthesize propFileName				= m_fileName;

@synthesize propRootItem				= m_itemRoot;

@synthesize propCurrentTextureItem		= m_itemCurrentTexture;
@synthesize propCurrentShapeItem		= m_itemCurrentShape;
@synthesize propCurrentSpriteItem		= m_itemCurrentSprite;
@synthesize propCurrentMultiSpriteItem	= m_itemCurrentMultiSprite;
@synthesize propCurrentTilesetItem		= m_itemCurrentTileset;
@synthesize propCurrentTileMapItem		= m_itemCurrentTileMap;


// Current item property
//-----------------------------------------------------------------------------------------------
- (void)setPropCurrentItem : (ProjectItem*)item
{
	m_itemCurrent = item;
	
	// Reset all current items

	self.propCurrentTextureItem		= nil;
	self.propCurrentShapeItem		= nil;
	self.propCurrentSpriteItem		= nil;
	self.propCurrentMultiSpriteItem	= nil;
	self.propCurrentTilesetItem		= nil;
	self.propCurrentTileMapItem		= nil;
	
	AppDelegate* app = [NSApp delegate];
	
	// Select appropriate current item
	
	if		([item class] == [ProjectTextureItem class])
	{
		self.propCurrentTextureItem = (ProjectTextureItem*)item;
	}
	else if ([item class] == [ProjectShapeItem class])
	{
		self.propCurrentShapeItem = (ProjectShapeItem*)item;
	}
	else if ([item class] == [ProjectSpriteItem class])
	{
		self.propCurrentSpriteItem = (ProjectSpriteItem*)item;
	}
	else if ([item class] == [ProjectMultiSpriteItem class])
	{
		self.propCurrentMultiSpriteItem = (ProjectMultiSpriteItem*)item;
	}
	else if ([item class] == [ProjectTilesetItem class])
	{
		self.propCurrentTilesetItem	= (ProjectTilesetItem*)item;
	}
	else if ([item class] == [ProjectTileMapItem class])
	{
		self.propCurrentTileMapItem = (ProjectTileMapItem*)item;
	}
	
	[app showMapToolbox:[item class] == [ProjectTileMapItem class]];
}	

- (ProjectItem*)propCurrentItem
{
	return m_itemCurrent;
}

// Texture list array
//-----------------------------------------------------------------------------------------------
- (NSArray*)propTextureList
{
	return m_itemRoot.propRootTextures.propChildren;
}

- (void)setPropTextureList:(NSArray*)list
{
	// just dummy for KVO
}

// Sprite list array
//-----------------------------------------------------------------------------------------------
- (NSArray*)propSpriteList
{
	return m_itemRoot.propRootSprites.propChildren;
}

- (void)setPropSpriteList:(NSArray*)list
{
	// just dummy for KVO
}

// Tileset list array
//-----------------------------------------------------------------------------------------------
- (NSArray*)propTilesetList
{
	return m_itemRoot.propRootTilesets.propChildren;
}

- (void)setPropTilesetList:(NSArray*)list
{
	// just dummy for KVO
}

// Multisprite list array
//-----------------------------------------------------------------------------------------------
- (NSArray*)propMultiSpriteList
{
	return m_itemRoot.propRootMultiSprites.propChildren;
}

- (void)setPropMultiSpriteList:(NSArray*)list
{
	// just dummy for KVO
}

// Project name
//-----------------------------------------------------------------------------------------------
- (NSString*)propName
{
	return [m_itemRoot propName];
}

- (void)setPropName:(NSString*)value
{
	[m_itemRoot setPropName:value];
}

// Project root directory
//-----------------------------------------------------------------------------------------------
- (NSString*)propRootDirectory
{
	return [m_itemRoot propRootDirectory];
}

- (void)setPropRootDirectory:(NSString*)value
{
	[m_itemRoot setPropRootDirectory:value];
}

// Project build directory (bundle)
//-----------------------------------------------------------------------------------------------
- (NSString*)propBuildDirectory
{
	return [m_itemRoot propBuildDirectory];
}

- (NSString*)propCacheDirectory
{
	return [m_itemRoot propCacheDirectory];
}

// ***** INSTANCE METHODS *****

//###############################################################################################
// INITIALIZIZERS\DESTRUCTORS
//###############################################################################################

// Initializes new project container with associated UI objects
//-----------------------------------------------------------------------------------------------
- (id)initWithTree:(NSOutlineView*)tree andController:(NSObjectController*)ctrl andPropPanel:(PropPanel*)panel andViewsTab:(ViewsTab*)tab
{
	if ((self = [super init]))
	{
		m_tree			= tree;
		m_controller	= ctrl;
		m_panel			= panel;
		m_viewsTab		= tab;
		m_newProject	= YES;
		m_modified		= NO;
		
		m_itemRoot = [[ProjectRootItem alloc] initWithProject:self];
		
		self.propCurrentItem = m_itemRoot;
		
		[m_tree setDataSource:self];
		[m_tree setDelegate:self];
		[m_tree setTarget:self];
		[m_tree setDoubleAction:@selector(treeDoubleClick)];
		[m_tree expandItem:m_itemRoot];
		
		[m_controller setContent:self];
		
		[self log:"$Cobject created\n"];
	}
	return self;
}

// Desturctor
//-----------------------------------------------------------------------------------------------
- (void)dealloc
{

	SAFE_RELEASE(m_itemRoot);
	
	[self log:"$Cobject destoryed\n"];
	
	[super dealloc];
}

// Unbind any UI components
//-----------------------------------------------------------------------------------------------
- (void)unbindAll
{
	[m_tree setDataSource:nil];
	[m_tree setDelegate:nil];
	[m_tree setTarget:nil];
	[m_tree setDoubleAction:nil];
	[m_tree reloadData];
	
	[m_controller setContent:nil];
}

//###############################################################################################
// ADOPTING <NSOutlineViewDataSource>
//###############################################################################################

// Must return number of children of given item (takes nil for root item)
//-----------------------------------------------------------------------------------------------
- (NSInteger)outlineView:(NSOutlineView*)outlineView numberOfChildrenOfItem:(id)item 
{
    return (item == nil) ? 1 : ((ProjectItem*)item).propChildrenCount;
}

// Must return YES if item wants to be expandable by user
//-----------------------------------------------------------------------------------------------
- (BOOL)outlineView:(NSOutlineView*)outlineView isItemExpandable:(id)item 
{
	return ((ProjectItem*)item).propExpandable;
}

// Must return child of taken item by index
//-----------------------------------------------------------------------------------------------
- (id)outlineView:(NSOutlineView *)outlineView child:(NSInteger)index ofItem:(id)item 
{
	
    return (item == nil) ? m_itemRoot : [item getChildAtIndex:index];
}

// Must return actual cell value depending on item (cell a TextCells)
//-----------------------------------------------------------------------------------------------
- (id)outlineView:(NSOutlineView *)outlineView objectValueForTableColumn:(NSTableColumn*)tableColumn byItem:(id)item 
{
    if (!item)
		return nil;
	
	return [item propName];
}

//###############################################################################################
// ADOPTING <NSOutlineViewDelegate>
//###############################################################################################

// View wants to display cell - we use it to setup cell image
//-----------------------------------------------------------------------------------------------
- (void)outlineView:(NSOutlineView *)outlineView willDisplayCell:(id)cell forTableColumn:(NSTableColumn *)tableColumn item:(id)item
{
	[cell setImage:[item propImage]];
}

// Tracking items selection
//-----------------------------------------------------------------------------------------------
- (BOOL)outlineView:(NSOutlineView *)outlineView shouldSelectItem:(id)item
{
	m_panel.propCurrentItem		= item;
	m_viewsTab.propCurrentItem	= item;
	self.propCurrentItem		= item;
	
	return YES;
}

// Tracking doubleclick to view properties panel
//-----------------------------------------------------------------------------------------------
- (void)treeDoubleClick
{
	NSInteger selRow = [m_tree selectedRow];
	if (selRow < 0)
		return;
	
	ProjectItem* selItem = (ProjectItem*)[m_tree itemAtRow:selRow];
	
	if (!selItem)
		return;
	
	m_panel.propCurrentItem		= selItem;
	m_viewsTab.propCurrentItem	= selItem;
	self.propCurrentItem		= selItem;
	
	[m_panel makeKeyAndOrderFront:self];
}

//###############################################################################################
// ITEMS OPERATIONS
//###############################################################################################

// Adding new texture item
//-----------------------------------------------------------------------------------------------
- (void)addTexture
{
	[[[ProjectTextureItem alloc] initWithParent:m_itemRoot.propRootTextures] autorelease];
	
	[m_tree reloadData];
	
	self.propTextureList = nil; // just to start KVO mechanincs to update controllers
}

// Adding new shape item
//-----------------------------------------------------------------------------------------------
- (void)addShape
{
	[[[ProjectShapeItem alloc] initWithParent:m_itemRoot.propRootShapes] autorelease];
	
	[m_tree reloadData];
}

// Adding new sprite item
//-----------------------------------------------------------------------------------------------
- (void)addSprite
{
	[[[ProjectSpriteItem alloc] initWithParent:m_itemRoot.propRootSprites] autorelease];
	
	[m_tree reloadData];
	
	self.propSpriteList = nil; // just to start KVO mechanics
}

// Adding new font item
//-----------------------------------------------------------------------------------------------
- (void)addFont
{
	[[[ProjectFontItem alloc] initWithParent:m_itemRoot.propRootFonts] autorelease];
	
	[m_tree reloadData];
}

// Adding new multisprite item
//-----------------------------------------------------------------------------------------------
- (void)addMultiSprite
{
	[[[ProjectMultiSpriteItem alloc] initWithParent:m_itemRoot.propRootMultiSprites] autorelease];
	
	[m_tree reloadData];
	
	self.propMultiSpriteList = nil; // just for KVO
}

// Adding new tileset item
//-----------------------------------------------------------------------------------------------
- (void)addTileset
{
	[[[ProjectTilesetItem alloc] initWithParent:m_itemRoot.propRootTilesets] autorelease];
	
	[m_tree reloadData];
}

// Adding new tilemap item
//-----------------------------------------------------------------------------------------------
- (void)addTileMap
{
	[[[ProjectTileMapItem alloc] initWithParent:m_itemRoot.propRootTileMaps] autorelease];
	
	[m_tree reloadData];
}


// Deletes current selection item
//-----------------------------------------------------------------------------------------------
- (void)deleteCurrentItem
{
	if (!m_itemCurrent)
		return;
	
	[m_itemCurrent removeSelf];
	
	[m_tree reloadData];
	
	NSInteger selRow = [m_tree selectedRow];
	
	if (selRow < 0)
		return;
	
	ProjectItem* selItem = (ProjectItem*)[m_tree itemAtRow:selRow];
	
	if (!selItem)
		return;
	
	m_panel.propCurrentItem		= selItem;
	m_viewsTab.propCurrentItem	= selItem;
	self.propCurrentItem		= selItem;
}


//###############################################################################################
// PROJECT I\O
//###############################################################################################

// Saves whole project to XML file
//-----------------------------------------------------------------------------------------------
- (result)save:(NSString*)file
{
	if (![file length])
	{
		DBG_START	[self log:"$C[save] - ERROR: empty file name passed as argument\n"];	DBG_END
		return RES_ERR_INVALID_ARGS;
	}
	
	NSXMLDocument* doc = [[[NSXMLDocument alloc] init] autorelease];
	
	doc.version				= @"1.0";
	doc.characterEncoding	= @"UTF-8";
	
	NSXMLElement* rootProject	= [m_itemRoot saveAsXML:nil];
	
	doc.rootElement = rootProject;
	
	NSData* output = [doc XMLDataWithOptions:NSXMLDocumentIncludeContentTypeDeclaration | NSXMLNodePrettyPrint];
	
	if (![output writeToFile:file atomically:YES])
	{
		DBG_START	[self log:"$C[save] - ERROR: cannot write XML schema to [%s]\n", [file UTF8String]];	DBG_END
		return RES_ERR_FILESYSTEM;
	}
	
	m_fileName		= [file copy];
	m_newProject	= NO;
	m_modified		= NO;
	
	return RES_OK;
}

// Loads whole project from taken file
//-----------------------------------------------------------------------------------------------
- (result)load:(NSString*)file
{
	if (![file length])
	{
		DBG_START	[self log:"$C[load] - ERROR: empty file name passed as argument\n"];	DBG_END
		return RES_ERR_INVALID_ARGS;
	}
	
	NSData*	data = [NSData dataWithContentsOfFile:file];
	
	if (!data)
	{
		DBG_START	[self log:"$C[load] - ERROR: cant read data from [%s]\n", [file UTF8String]];	DBG_END
		return RES_ERR_FILESYSTEM;
	}
	
	NSError* parseError = nil;
	
	NSXMLDocument* doc = [[[NSXMLDocument alloc] initWithData:data options:0 error:&parseError] autorelease];
	
	if (!doc)
	{
		DBG_START	
		[self log:"$C[load] - ERROR: cant parse XML in [%s]\n", [file UTF8String]];	
		[self log:"$CXML parser: %s\n", [[parseError description] UTF8String]];
		DBG_END
		return RES_ERR_FILESYSTEM;
	}
	
	// XML schema loaded successfully, now create objects
	NSXMLElement* rootProject = doc.rootElement;
	
	if (!rootProject)
	{
		DBG_START	[self log:"$C[load] - ERROR: no root element in XML document\n"];	DBG_END
		return RES_ERR_INVALID_ARGS;
	}
	
	SAFE_RELEASE(m_itemRoot);
	
	@try 
	{
		m_itemRoot = [[ProjectRootItem alloc] initWithProject:self fromXML:rootProject];
		
	}
	@catch (NSException* e) 
	{
		DBG_START
		[self log:"$C[load] - ERROR: exception raised during project awakening from XML [%s]\n", [file UTF8String]];
		[self log:"$C[load] - Name: %s\n", [[e name] UTF8String]];
		[self log:"$C[load] - Description: %s\n", [[e description] UTF8String]];
		DBG_END
		return RES_ERR_FILESYSTEM;
	}
	
	m_fileName		= [file copy];
	m_newProject	= NO;
	m_modified		= NO;
	
	[m_tree reloadData];
	[m_tree expandItem:m_itemRoot];
	
	[m_controller setContent:nil];
	[m_controller setContent:self];
	
	self.propCurrentItem		= m_itemRoot;
	m_panel.propCurrentItem		= m_itemRoot;
	m_viewsTab.propCurrentItem	= m_itemRoot;
	
	return RES_OK;
}

- (void)updateTree:(ProjectItem*)item
{
	if (!item)
		[m_tree reloadData];
	else
		[m_tree reloadItem:item];
}

// Project building
//-----------------------------------------------------------------------------------------------
- (result)build:(Progressor*)progress
{
	// Check paths
	
	BOOL isDir;
	
	if (![[NSFileManager defaultManager] fileExistsAtPath:[m_itemRoot propBuildDirectory] isDirectory:&isDir] || !isDir)
		[NSException raise:@"Prebuild check failed" format:@"Invalid project build directory. You must create this directory if you are using SVN or set new directory in project properties panel."];
	
	if (![[NSFileManager defaultManager] fileExistsAtPath:[m_itemRoot propCacheDirectory] isDirectory:&isDir] || !isDir)
		[NSException raise:@"Prebuild check failed" format:@"Invalid project cache directory. You must create this directory if you are using SVN or set new directory in project properties panel."];
	
	
	progress.propOverallMinVal  = 0;
	progress.propOverallMaxVal  = 1;
	progress.propOverallVal		= 1;
	
	progress.propOverallState	= @"Overall: building project";
	
	result res = [m_itemRoot build:progress];
	
	if (FAIL(res))
		return res;
	
	progress.propOverallState	= @"Overall: current stage - exporting sprites";
	
	m_panel.propCurrentItem		= m_panel.propCurrentItem;
	m_viewsTab.propCurrentItem	= m_viewsTab.propCurrentItem;
	self.propCurrentItem		= self.propCurrentItem;
	
	return res;
}

@end
