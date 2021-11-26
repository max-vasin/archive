//===============================================================================================
//  File:		Project.h
//  Project:	OriginGameToolkit
//  Desc:		Project container class interface (adopts data source protocol)
//
//  Created by Max Vasin on 11/24/10.
//  Copyright 2010 Origin. All rights reserved.
//===============================================================================================

// ***** IMPORTS *****

#import <Cocoa/Cocoa.h>
#import "BaseObject.h"
#import "PropPanel.h"
#import "ViewsTab.h"
#import "Progressor.h"
#import "ProjectItem.h"
#import "ProjectRootItems.h"
#import "ProjectTextureItem.h"
#import "ProjectShapeItem.h"
#import "ProjectSpriteItem.h"
#import "ProjectFontItem.h"
#import "ProjectMultiSpriteItem.h"
#import "ProjectTilesetItem.h"
#import "ProjectTileMapItem.h"

// ***** INTERFACE *****

@interface Project : BaseObject < NSOutlineViewDataSource, NSOutlineViewDelegate > 
{
	NSOutlineView*				m_tree;
	NSObjectController*			m_controller;
	PropPanel*					m_panel;
	ViewsTab*					m_viewsTab;
	
	ProjectRootItem*			m_itemRoot;
	
	ProjectItem*				m_itemCurrent;
	ProjectTextureItem*			m_itemCurrentTexture;
	ProjectShapeItem*			m_itemCurrentShape;
	ProjectSpriteItem*			m_itemCurrentSprite;
	ProjectMultiSpriteItem*		m_itemCurrentMultiSprite;
	ProjectTilesetItem*			m_itemCurrentTileset;
	ProjectTileMapItem*			m_itemCurrentTileMap;
	
	BOOL						m_newProject;
	BOOL						m_modified;
	NSString*					m_fileName;
}

// ***** PROPERTIES *****

@property (readonly)	ProjectItem*			propRootItem;
@property (readonly)	BOOL					propIsNewProject;
@property (assign)		BOOL					propModified;
@property (copy)		NSString*				propFileName;

@property (assign)		ProjectItem*			propCurrentItem;
@property (assign)		ProjectTextureItem*		propCurrentTextureItem;
@property (assign)		ProjectShapeItem*		propCurrentShapeItem;
@property (assign)		ProjectSpriteItem*		propCurrentSpriteItem;
@property (assign)		ProjectMultiSpriteItem* propCurrentMultiSpriteItem;
@property (assign)		ProjectTilesetItem*		propCurrentTilesetItem;
@property (assign)		ProjectTileMapItem*		propCurrentTileMapItem;
@property (assign)		NSArray*				propTextureList;
@property (assign)		NSArray*				propSpriteList;
@property (assign)		NSArray*				propTilesetList;
@property (assign)		NSArray*				propMultiSpriteList;

// This properties maps to ProjectRootItem
@property (copy)		NSString*				propName;
@property (copy)		NSString*				propRootDirectory;
@property (readonly)	NSString*				propBuildDirectory;
@property (readonly)	NSString*				propCacheDirectory;

// ***** METHODS *****

- (id)		initWithTree:(NSOutlineView*)tree andController:(NSObjectController*)ctrl andPropPanel:(PropPanel*)panel andViewsTab:(ViewsTab*)tab;
- (void)	treeDoubleClick;

- (void)	addTexture;
- (void)	addShape;
- (void)	addSprite;
- (void)	addFont;
- (void)	deleteCurrentItem;
- (void)	addMultiSprite;
- (void)	addTileset;
- (void)	addTileMap;

- (result)save:(NSString*)file;
- (result)load:(NSString*)file;

- (void)updateTree:(ProjectItem*)item;

- (result)build:(Progressor*)progress;

- (void)unbindAll;

@end
