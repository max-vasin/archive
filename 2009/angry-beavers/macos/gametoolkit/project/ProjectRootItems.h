//===============================================================================================
//  File:		ProjectRootItems.h
//  Project:	OriginGameToolkit
//  Desc:		Root project items classes collection
//
//  Created by Max Vasin on 11/24/10.
//  Copyright 2010 Origin. All rights reserved.
//===============================================================================================

// ****** IMPORTS ******

#import <Cocoa/Cocoa.h>
#import "ProjectItem.h"
#import "ProjectTextureItem.h"
#import "ProjectShapeItem.h"
#import "ProjectSpriteItem.h"
#import "ProjectFontItem.h"
#import "ProjectTilesetItem.h"
#import "ProjectTileMapItem.h"

//###############################################################################################
// ProjectRootItem INTERFACE
//###############################################################################################

@interface ProjectRootItem : ProjectItem 
{
	NSString*	m_rootDirectory;
	NSString*	m_buildDirectory;
	NSString*	m_cacheDirectory;
	
	ProjectItem*	m_rootTextures;
	ProjectItem*	m_rootShapes;
	ProjectItem*	m_rootSprites;
	ProjectItem*	m_rootMultiSprites;
	ProjectItem*	m_rootFonts;
	ProjectItem*	m_rootTilesets;
	ProjectItem*	m_rootTilemaps;
}

@property (nonatomic, copy)			NSString*		propRootDirectory;
@property (nonatomic, copy)			NSString*		propBuildDirectory;
@property (nonatomic, copy)			NSString*		propCacheDirectory;

@property (nonatomic, readonly)		ProjectItem*	propRootTextures;
@property (nonatomic, readonly)		ProjectItem*	propRootShapes;
@property (nonatomic, readonly)		ProjectItem*	propRootSprites;
@property (nonatomic, readonly)		ProjectItem*	propRootMultiSprites;
@property (nonatomic, readonly)		ProjectItem*	propRootFonts;
@property (nonatomic, readonly)		ProjectItem*	propRootTilesets;
@property (nonatomic, readonly)		ProjectItem*	propRootTileMaps;

@end

//###############################################################################################
// ProjectTextureRootItem INTERFACE
//###############################################################################################

@interface ProjectTextureRootItem : ProjectItem 
{

}

@property (nonatomic, assign)		unsigned int	propCachedMemory;
@property (nonatomic, assign)		unsigned int	propPackedMemory;
@property (nonatomic, assign)		NSString*		propCachedMemoryString;
@property (nonatomic, assign)		NSString*		propPackedMemoryString;

@end

//###############################################################################################
// ProjectShapeRootItem INTERFACE
//###############################################################################################

@interface ProjectShapeRootItem : ProjectItem 
{

}

@end

//###############################################################################################
// ProjectSpriteRootItem INTERFACE
//###############################################################################################

@interface ProjectSpriteRootItem : ProjectItem 
{

}

- (NSArray*)getLinkedAnimations:(TextureAssemblyItem*)item;

@end

//###############################################################################################
// ProjectFontRootItem INTERFACE
//###############################################################################################

@interface ProjectFontRootItem : ProjectItem 
{
	
}

@end

//###############################################################################################
// ProjectMultiSpriteRootItem INTERFACE
//###############################################################################################

@interface ProjectMultiSpriteRootItem : ProjectItem
{
	
}

@end

//###############################################################################################
// ProjectTilesetRootItem INTERFACE
//###############################################################################################

@interface ProjectTilesetRootItem : ProjectItem
{
	
}

@end

//###############################################################################################
// ProjectTileMapRootItem INTERFACE
//###############################################################################################

// Tilemap grid cell type interface
//-----------------------------------------------------------------------------------------------

@interface TilemapGridCellType : BaseObject
{
	uint		m_id;
	NSString*	m_name;
	NSColor*	m_color;
}

@property (nonatomic, assign)	uint			propID;
@property (nonatomic, copy)		NSString*		propName;
@property (nonatomic, copy)		NSColor*		propColor;
@property (nonatomic, readonly)	NSColor*		propTransparentColor;

- (id)				initWithID:(uint)ID;
- (id)				initWithXML:(NSXMLElement*)node;

- (NSXMLElement*)	saveXML:(NSXMLElement*)node;

@end

// Tilemap root item interface
//-----------------------------------------------------------------------------------------------

@interface ProjectTileMapRootItem : ProjectItem
{
	NSMutableArray*			m_gridCellTypes;
	TilemapGridCellType*	m_currentCellType;
}

@property (nonatomic, assign)	TilemapGridCellType*	propCurrentGridAttribute;
@property (nonatomic, assign)	NSArray*				propGridAttributes;

- (void)					addGridAttribute;
- (void)					delGridAttrubute;
- (NSArray*)				gridAttributes;
- (TilemapGridCellType*)	cellTypeByID:(uint)typeID;

@end

