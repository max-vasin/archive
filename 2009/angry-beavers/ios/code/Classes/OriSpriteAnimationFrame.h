//=======================================================================================================================
//  File:		OriSpriteAnimationFrame.h
//  Project:	heli
//	Desc:		sprite animation frame class interface
//
//  Created by Max Vasin on 3/23/11.
//  Copyright 2011 Origin. All rights reserved.
//=======================================================================================================================

// ******** IMPORTS ********

#import <UIKit/UIKit.h>
#import "OriBase.h"
#import "OriXMLNode.h"
#import "OriSpriteAnimationPoint.h"
#import "OriTexture.h"

// ******** INTERFACE ********

@class OriSpriteAnimation;

@interface OriSpriteAnimationFrame : NSObject 
{
@private
	OriSpriteAnimation*	m_animation;
	oriIntPoint			m_position;
	oriIntSize			m_size;
	oglMapVector2		m_mapMin;
	oglMapVector2		m_mapMax;
	NSMutableArray*		m_points;
	BOOL				m_mirrorX;
	BOOL				m_mirrorY;
}

// ******** PROPERTIES ********

@property (nonatomic, readonly)	oriUint			propSysMemUsed;
@property (nonatomic, readonly)	oriUint			propVideoMemUsed;
@property (nonatomic, readonly) oriIntPoint		propOrigin;
@property (nonatomic, readonly)	oriUint			propPointsCount;
@property (nonatomic, readonly) oglMapVector2	propMapMin;
@property (nonatomic, readonly) oglMapVector2	propMapMax;
@property (nonatomic, readonly)	oriRect			propMapRect;
@property (nonatomic, readonly) oriIntSize		propSize;

// ******** CLASS METHODS ********

+ (id)		frameWithAnimation:(OriSpriteAnimation*)animation;
+ (id)		frameWithAnimation:(OriSpriteAnimation*)animation	withXML:(OriXMLNode*)node;

// ******** INSTANCE METHODS ********

- (id)		initWithAnimation:(OriSpriteAnimation*)animation;
- (id)		initWithAnimation:(OriSpriteAnimation*)animation	withXML:(OriXMLNode*)node;

- (OriSpriteAnimationPoint*)	getPointAtIndex:(oriUint)index;
- (OriSpriteAnimationPoint*)	getPointByName:(NSString*)name;

- (void)	updateTextureDepencies:(OriTexture*)texture;

@end
