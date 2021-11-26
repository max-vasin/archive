//===============================================================================================
//  File:		MultiSpriteRecord.h
//  Project:	OriginGameToolkit
//	Desc:		Multi sprite record class interface
//
//  Created by Max Vasin on 3/29/11.
//  Copyright 2011 Origin Games. All rights reserved.
//===============================================================================================

// ****** IMPORTS ******

#import <Cocoa/Cocoa.h>
#import "Base.h"
#import "BaseObject.h"
#import "ProjectSpriteItem.h"

@class SpriteWrapper;
@class ProjectMultiSpriteItem;
@class ProjectMultiSpriteRecord;

//###############################################################################################
// MULTISPRITE RECORD KEY
//###############################################################################################

// ****** INTERFACE ******

@class MultiSpriteRecord;

@interface MultiSpriteRecordKey : BaseObject
{
	MultiSpriteRecord*	m_record;
	SpriteWrapper*		m_sprite;
	SpriteAnimation*	m_animation;
	FramePoint*			m_point;
	
	unsigned int		m_frame;
	unsigned int		m_animSpeed;
	BOOL				m_animPlay;
	float				m_layer;
}

// ****** PROPERTIES ******

@property (nonatomic, assign)	SpriteWrapper*		propSprite;
@property (nonatomic, assign)	SpriteAnimation*	propAnimation;
@property (nonatomic, assign)	FramePoint*			propPoint;
@property (nonatomic, assign)	unsigned int		propFrame;
@property (nonatomic, assign)	unsigned int		propAnimationSpeed;
@property (nonatomic, assign)	BOOL				propAnimationPlay;
@property (nonatomic, assign)	float				propLayer;

// ****** CLASS METHODS ******

+ (id)				keyWithRecord:(MultiSpriteRecord*)record	andSprite:(SpriteWrapper*)sprite;
+ (id)				keyWithRecord:(MultiSpriteRecord*)record	formXML:(NSXMLElement*)node;

// ****** INSTANCE METHODS ******

- (id)				initWithRecord:(MultiSpriteRecord*)record	andSprite:(SpriteWrapper*)sprite;
- (id)				initWithRecord:(MultiSpriteRecord*)record	formXML:(NSXMLElement*)node;

- (NSXMLElement*)	saveXML:(NSXMLElement*)node;
- (NSXMLElement*)	buildXML:(NSXMLElement*)node;

@end

//###############################################################################################
// MULTISPRITE RECORD
//###############################################################################################

// ****** INTERFACE ******

@interface MultiSpriteRecord : BaseObject
{
	ProjectMultiSpriteItem*		m_multiSprite;		// Multisprite container
	NSMutableArray*				m_keys;				// Record keys
}

// ****** PROPERTIES ******

@property (nonatomic, readonly)		NSMutableArray*		propKeys;

// ****** CLASS METHODS ******

+ (id)						recordWithMultiSprite:(ProjectMultiSpriteItem*)item;
+ (id)						recordWithMultiSprite:(ProjectMultiSpriteItem*)item	fromXML:(NSXMLElement*)node;

// ****** INSTANCE METHODS ******

- (id)						initWithMultiSprite:(ProjectMultiSpriteItem*)item;
- (id)						initWithMultiSprite:(ProjectMultiSpriteItem*)item	fromXML:(NSXMLElement*)node;

- (void)					syncKeys;
- (MultiSpriteRecordKey*)	getKey:(SpriteWrapper*)sprite;

- (unsigned int)			indexOfSprite:(SpriteWrapper*)sprite;
- (SpriteWrapper*)			spriteAtIndex:(unsigned int)index;
- (FramePoint*)				originPoint;
- (FramePoint*)				pointWithName:(NSString*)name;

- (NSXMLElement*)			saveXML:(NSXMLElement*)node;
- (NSXMLElement*)			buildXML:(NSXMLElement*)node;

@end
