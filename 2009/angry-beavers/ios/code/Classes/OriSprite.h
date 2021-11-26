//=======================================================================================================================
//  File:		OriSprite.h
//  Project:	heli
//	Desc:		sprite resource interface
//
//  Created by Max Vasin on 3/21/11.
//  Copyright 2011 Origin. All rights reserved.
//=======================================================================================================================

// ***** IMPORTS *****

#import <UIKit/UIKit.h>
#import "OriBase.h"
#import "OriResource.h"
#import "OriTexture.h"
#import "OriSpriteAnimation.h"

// ***** INTERFACE *****

@class OriResourceManager;

@interface OriSprite : OriResource 
{
@private
	OriTexture*					m_texture;
	NSMutableArray*				m_animations;
}

@property (nonatomic, readonly)		oriUint			propAnimationCount;
@property (nonatomic, readonly)		OriTexture*		propTexture;

// ****** CLASS METHODS ******

+ (id)					spriteWithManager:(OriResourceManager*)resman	withName:(NSString*)name;
+ (id)					spriteWithManager:(OriResourceManager*)resman	withName:(NSString*)name	withXML:(OriXMLNode*)node;

// ****** INSTANCE METHODS ******

- (id)					initWithManager:(OriResourceManager*)resman		withName:(NSString*)name	withXML:(OriXMLNode*)node;

- (OriSpriteAnimation*)	getAnimation:(NSString*)name;
- (OriSpriteAnimation*) getAnimationByIndex:(oriUint)index;

@end
