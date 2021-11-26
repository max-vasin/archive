//===============================================================================================
//  File:		ProjectSpriteItem.h
//  Project:	OriginGameToolkit
//	Desc:		Project sprite item container class interface
//
//  Created by Max Vasin on 11/24/10.
//  Copyright 2010 Origin. All rights reserved.
//===============================================================================================

// ****** IMPORTS ******

#import <Cocoa/Cocoa.h>
#import "ProjectItem.h"
#import "SpriteAnimation.h"

// Sprite type
//-----------------------------------------------------------------------------------------------
typedef enum
{
	eSpriteTypeSequence		= 0,
	eSpriteTypeMovie		= 1
}enSpriteType;

// ****** INTERFACE ******

@class ProjectTextureItem;

@interface ProjectSpriteItem : ProjectItem 
{
	NSString*			m_texture;
	NSMutableArray*		m_animations;
	SpriteAnimation*	m_currentAnim;
	enSpriteType		m_type;
}

// ****** PROPERTIES ******

@property (nonatomic, assign)		ProjectTextureItem*		propTexture;
@property (nonatomic, readonly)		NSArray*				propAnimations;
@property (nonatomic, assign)		SpriteAnimation*		propCurrentAnim;
@property (nonatomic, assign)		enSpriteType			propType;
@property (nonatomic, readonly)		NSArray*				propTypeList;

// ****** METHODS ******

- (SpriteAnimation*)	getAnimation:(NSString*)name;

- (void)				addAnimation;
- (void)				delAnimation;

- (NSArray*)			getLinkedAnimations:	(TextureAssemblyItem*)item;

@end
