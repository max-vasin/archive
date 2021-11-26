//=======================================================================================================================
//  File:		OriMultiSpriteKey.h
//  Project:	heli
//	Desc:		multisprite record key
//
//  Created by Max Vasin on 4/15/11.
//  Copyright 2011 Origin. All rights reserved.
//=======================================================================================================================

// ***** IMPORTS *****

#import <UIKit/UIKit.h>
#import "OriBase.h"
#import "OriXMLNode.h"

// ***** INTERFACE *****

@class OriMultiSpriteRecord;
@class OriSprite;
@class OriSpriteAnimation;
@class OriSpriteAnimationPoint;

@interface OriMultiSpriteKey : NSObject 
{
@public
	
	OriMultiSpriteRecord*		m_record;
	OriSprite*					m_sprite;
	OriSpriteAnimation*			m_animation;
	NSString*					m_bindName;
	oriIntPoint					m_bindPoint;
	unsigned int				m_frame;
	unsigned int				m_link;
	float						m_layer;
}

// ***** PROPERTIES *****

@property (nonatomic, readonly)	OriMultiSpriteRecord*	propRecord;
@property (nonatomic, readonly)	OriSprite*				propSprite;
@property (nonatomic, readonly)	OriSpriteAnimation*		propAnimation;
@property (nonatomic, readonly)	unsigned int			propFrame;
@property (nonatomic, readonly) unsigned int			propLink;
@property (nonatomic, readonly) float					propLayer;
@property (nonatomic, copy)		NSString*				propBindName;
@property (nonatomic, readonly)	oriIntPoint				propBindPoint;

// ***** CLASS METHODS *****

+ (id)		keyWithRecord:(OriMultiSpriteRecord*)record;
+ (id)		keyWithRecord:(OriMultiSpriteRecord*)record		withXML:(OriXMLNode*)node;

// ***** INSTANCE METHODS *****

- (id)		initWithRecord:(OriMultiSpriteRecord*)record;
- (id)		initWithRecord:(OriMultiSpriteRecord*)record	withXML:(OriXMLNode*)node;

@end
