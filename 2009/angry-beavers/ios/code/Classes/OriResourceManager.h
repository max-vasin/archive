//=======================================================================================================================
//  File:		ResourceManager.h
//  Project:	heli
//  Desc:		engine resources manager interface
//
//  Created by Max Vasin on 3/21/11.
//  Copyright 2011 Origin. All rights reserved.
//=======================================================================================================================

// ***** IMPORTS *****

#import <UIKit/UIKit.h>

#import "OriBase.h"
#import "OriXMLNode.h"
#import "OriRenderer.h"
#import "OriResource.h"
#import "OriTexture.h"
#import "OriSprite.h"
#import "OriMultiSprite.h"
#import "OriTileset.h"
#import "OriTilemap.h"
#import "OriFont.h"

// ***** INTERFACE *****

@interface OriResourceManager : NSObject 
{
	OriRenderer*		m_renderer;
	
	NSString*			m_pathTextures;
	NSString*			m_pathSprites;
	NSString*			m_pathMultiSprites;
	NSString*			m_pathTilesets;
	NSString*			m_pathTilemaps;
	NSString*			m_pathFonts;
	
	NSMutableArray*		m_listTextures;
	NSMutableArray*		m_listSprites;
	NSMutableArray*		m_listMultiSprites;
	NSMutableArray*		m_listTilesets;
	NSMutableArray*		m_listTilemaps;
	NSMutableArray*		m_listFonts;
}

@property (nonatomic, readonly, assign)	OriRenderer*	propRenderer;
@property (nonatomic, readonly, assign) oriUint			propTextureBPP;


+ (id)				sharedManager;

- (id)				initWithRenderer:(OriRenderer*)renderer;

- (OriTexture*)		getTexture:		(NSString*)name;
- (OriSprite*)		getSprite:		(NSString*)name;
- (OriMultiSprite*)	getMultiSprite:	(NSString*)name;
- (OriTileset*)		getTileset:		(NSString*)name;
- (OriTilemap*)		getTilemap:		(NSString*)name;
- (OriFont*)		getFont:		(NSString*)name;

- (void)			removeResource:(OriResource*)resource;


@end
