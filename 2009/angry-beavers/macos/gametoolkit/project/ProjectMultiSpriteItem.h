//===============================================================================================
//  File:		ProjectMultiSpriteItem.h
//  Project:	OriginGameToolkit
//	Desc:		Project multisprite item class
//
//  Created by Max Vasin on 3/28/11.
//  Copyright 2011 Origin Games. All rights reserved.
//===============================================================================================

// ****** IMPORTS ******

#import <Cocoa/Cocoa.h>
#import "MultiSpriteRecord.h"

//###############################################################################################
// Sprite wrapper
//###############################################################################################

// ****** INTERFACE ******

@interface SpriteWrapper : BaseObject
{
	ProjectMultiSpriteItem*		m_multisprite;
	ProjectSpriteItem*			m_sprite;
}

// ****** PROPERTIES ******

@property (nonatomic, assign)	ProjectSpriteItem*	propSprite;
@property (nonatomic, assign)	NSString*			propName;

// ****** CLASS METHODS ******

+ (id)spriteWrapperWithMultiSprite:(ProjectMultiSpriteItem*)multisprite andSprite:(ProjectSpriteItem*)sprite;

// ****** INSTANCE METHODS ******

- (id)initWithMultiSprite:(ProjectMultiSpriteItem*)multisprite andSprite:(ProjectSpriteItem*)sprite;

@end


//###############################################################################################
// MultiSprite item
//###############################################################################################

// ****** INTERFACE ******

@interface ProjectMultiSpriteItem : ProjectItem 
{
	ProjectSpriteItem*		m_baseSprite;		// Base sprite of multisprite
	SpriteAnimation*		m_baseAnimation;	// Base animation of multisprite
	NSMutableArray*			m_linkedSprites;	// Linked sprites
	NSMutableArray*			m_records;			// Records array
	unsigned int			m_currentFrame;		// Current frame
	SpriteWrapper*			m_currentSprite;	// Current sprite (linked)
	MultiSpriteRecord*		m_currentRecord;	// Current record
	MultiSpriteRecordKey*	m_currentKey;		// Current key
	float					m_layer;			// Base sprite layer
}

// ****** PROPERTIES ******

@property (nonatomic, assign)	BOOL					propRefreshView;
@property (nonatomic, assign)	BOOL					propUpdateTextures;
@property (nonatomic, assign)	ProjectSpriteItem*		propBaseSprite;
@property (nonatomic, assign)	SpriteAnimation*		propBaseAnimation;
@property (nonatomic, assign)	NSMutableArray*			propLinkedSprites;
@property (nonatomic, assign)	NSIndexSet*				propSelectionIndexes;
@property (nonatomic, assign)	SpriteWrapper*			propCurrentSprite;
@property (nonatomic, assign)	unsigned int			propCurrentFrame;
@property (nonatomic, assign)	MultiSpriteRecord*		propCurrentRecord;
@property (nonatomic, assign)	MultiSpriteRecordKey*	propCurrentKey;
@property (nonatomic, assign)	float					propLayer;

// ****** INSTANCE METHODS ******

- (ProjectSpriteItem*)		getSprite:(NSString*)name;

- (void)					addSprite;
- (void)					delSprite;

- (unsigned int)			indexOfSprite:(SpriteWrapper*)sprite;
- (SpriteWrapper*)			spriteAtIndex:(unsigned int)index;
- (FramePoint*)				originPoint;
- (FramePoint*)				pointWithName:(NSString*)name;
- (void)					syncRecords;
- (MultiSpriteRecord*)		recordAtIndex:(uint)index;

@end
