//=======================================================================================================================
//  File:		OriMultiSpriteItem.h
//  Project:	heli
//	Desc:		multi sprite item class
//
//  Created by Max Vasin on 4/12/11.
//  Copyright 2011 Origin. All rights reserved.
//=======================================================================================================================

// ***** IMPORTS *****

#import <UIKit/UIKit.h>
#import "OriBase.h"
#import "OriXMLNode.h"
#import "OriMultiSpriteKey.h"

// ***** INTERFACE *****

@class OriMultiSprite;
@class OriSprite;
@class OriSpriteAnimation;

@interface OriMultiSpriteRecord : NSObject 
{
@public
	
	OriMultiSprite*		m_multisprite;
	NSMutableArray*		m_keys;
}

// ****** PROPERTIES ******
@property (nonatomic, readonly)		unsigned int	propKeysCount;

// ****** CLASS METHODS ******

+ (id)						recordWithMultiSprite:(OriMultiSprite*)multisprite;
+ (id)						recordWithMultiSprite:(OriMultiSprite*)multisprite	withXML:(OriXMLNode*)node;

// ****** INSTANCE METHODS ******

- (id)						initWithMultiSprite:(OriMultiSprite*)multisprite;
- (id)						initWithMultiSprite:(OriMultiSprite*)multisprite	withXML:(OriXMLNode*)node;


- (OriMultiSpriteKey*)		getKeyAtIndex:(unsigned int)index;
- (OriMultiSpriteKey*)		getKeyAtSprite:(OriSprite*)sprite;

- (OriSprite*)				getWeakSpriteLink:(unsigned int)index;

- (OriSpriteAnimationPoint*)getBasePoint:(NSString*)name;

@end
