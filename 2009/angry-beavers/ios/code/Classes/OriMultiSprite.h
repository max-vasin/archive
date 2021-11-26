//=======================================================================================================================
//  File:		OriMultiSprite.h
//  Project:	heli
//	Desc:		multisprite resource iterface
//
//  Created by Max Vasin on 4/12/11.
//  Copyright 2011 Origin. All rights reserved.
//=======================================================================================================================

// ***** IMPORTS *****

#import <UIKit/UIKit.h>
#import "OriBase.h"
#import "OriResource.h"
#import "OriXMLNode.h"
#import "OriMultiSpriteRecord.h"

// ***** INTERFACE *****

@class OriResourceManager;

@interface OriMultiSprite : OriResource 
{
@public

	OriSprite*				m_baseSprite;
	OriSpriteAnimation*		m_baseAnimation;
	float					m_layer;
	NSMutableArray*			m_linkedSprites;
	NSMutableArray*			m_records;
}

@property	(nonatomic, readonly)	OriSprite*			propBaseSprite;
@property	(nonatomic, readonly)	OriSpriteAnimation*	propBaseAnimation;
@property	(nonatomic, readonly)	float				propLayer;
@property	(nonatomic, readonly)	unsigned int		propRecordsCount;
@property	(nonatomic, readonly)	unsigned int		propLinksCount;

// ****** CLASS METHODS ******

+ (id)						multiSpriteWithManager:(OriResourceManager*)resman	withName:(NSString*)name;
+ (id)						multiSpriteWithManager:(OriResourceManager*)resman	withName:(NSString*)name	withXML:(OriXMLNode*)node;

// ****** INSTANCE METHODS ******

- (id)						initWithManager:(OriResourceManager*)resman		withName:(NSString*)name;
- (id)						initWithManager:(OriResourceManager*)resman		withName:(NSString*)name	withXML:(OriXMLNode*)node;


- (OriMultiSpriteRecord*)	getRecord:(unsigned int)index;

- (NSString*)				getSpriteLink:(unsigned int)index;
- (OriSprite*)				getWeakSpriteLink:(unsigned int)index;

- (OriSpriteAnimationPoint*)getBasePoint:(NSString*)name forRecord:(OriMultiSpriteRecord*)record;

@end
