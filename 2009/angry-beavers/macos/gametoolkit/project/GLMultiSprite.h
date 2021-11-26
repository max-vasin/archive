//===============================================================================================
//  File:		GLMultiSprite.h
//  Project:	OriginGameToolkit
//	Desc:		Multisprite graphics container
//
//  Created by Max Vasin on 5/5/11.
//  Copyright 2011 Origin Games. All rights reserved.
//===============================================================================================

// ***** IMPORTS *****

#import <Cocoa/Cocoa.h>
#import "Base.h"
#import "BaseObject.h"
#import "TextureStore.h"

// ***** INTERFACE *****

@class ProjectMultiSpriteItem;
@class MultiSpriteRecord;

@interface GLMultiSprite : BaseObject 
{
	NSOpenGLContext*			m_context;
	TextureStore*				m_store;
	ProjectMultiSpriteItem*		m_item;
	MultiSpriteRecord*			m_record;
	NSMutableArray*				m_sprites;
	NSPoint						m_position;
	BOOL						m_visible;
	NSColor*					m_color;
}

@property (nonatomic, readonly)	 NSOpenGLContext*			propContext;
@property (nonatomic, readonly)	 TextureStore*				propTextreStore;
@property (nonatomic, readonly)	 ProjectMultiSpriteItem*	propItem;
@property (nonatomic, assign)	 MultiSpriteRecord*			propRecord;
@property (nonatomic, assign)	 NSPoint					propPosition;
@property (nonatomic, assign)	 BOOL						propVisible;
@property (nonatomic, copy)		 NSColor*					propColor;


- (id)initWithContext:(NSOpenGLContext*)context	 Item:(ProjectMultiSpriteItem*)item TextureStore:(TextureStore*)store;
- (void)updateSprites;
- (void)render;

@end
