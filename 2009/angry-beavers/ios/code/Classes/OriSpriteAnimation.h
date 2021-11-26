//=======================================================================================================================
//  File:		OriSpriteAnimation.h
//  Project:	heli
//	Desc:		sprite animation class iterface
//
//  Created by Max Vasin on 3/23/11.
//  Copyright 2011 Origin. All rights reserved.
//=======================================================================================================================

// ***** IMPORTS *****

#import <UIKit/UIKit.h>
#import "OriBase.h"
#import "OriXMLNode.h"
#import "OriSpriteAnimationFrame.h"

// ***** INTERFACE *****

@class OriSprite;

@interface OriSpriteAnimation : NSObject 
{
@public
	
	OriSprite*			m_sprite;
	NSString*			m_name;
	NSMutableArray*		m_frames;
	oriInt				m_speed;
	NSString*			m_origin;
}

@property (nonatomic, readonly)	oriUint		propFrameCount;
@property (nonatomic, readonly)	oriUint		propSysMemUsed;
@property (nonatomic, readonly)	oriUint		propVideoMemUsed;
@property (nonatomic, readonly)	oriInt		propSpeed;
@property (nonatomic, readonly)	NSString*	propName;
@property (nonatomic, readonly)	NSString*	propOrigin;

// ****** CLASS METHODS ******

+ (id)							animationWithSprite:(OriSprite*)sprite;
+ (id)							animationWithSprite:(OriSprite*)sprite	withXML:(OriXMLNode*)node;

// ****** INSTANCE METHODS ******

- (id)							initWithSprite:(OriSprite*)sprite;
- (id)							initWithSprite:(OriSprite*)sprite		withXML:(OriXMLNode*)node;

- (OriSpriteAnimationFrame*)	getFrame:(oriUint)index;
- (oriUint)						getFrameIndex:(OriSpriteAnimationFrame*)frame;
- (OriSpriteAnimationPoint*)	getPoint:(NSString*)name	atFrame:(oriUint)frame;

- (void)						updateTextureDepencies;

@end
