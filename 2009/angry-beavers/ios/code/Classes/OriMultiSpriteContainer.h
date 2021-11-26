//=======================================================================================================================
//  File:		OriMultiSpriteContainer.h
//  Project:	heli
//	Desc:		Scene multisprite container class interface
//
//  Created by Max Vasin on 4/15/11.
//  Copyright 2011 Origin. All rights reserved.
//=======================================================================================================================

// ******** IMPORTS ********

#import "OriBase.h"
#import "OriMultiSprite.h"
#import "OriResource.h"
#import "OriSpriteRenderable.h"
#import "OriSceneObject.h"

// ******** INTERFACE ********

@interface OriMultiSpriteContainer : OriSceneObject 
{
	OriMultiSprite*			m_multisprite;
	OriSpriteRenderable*	m_baseSprite;
	NSMutableArray*			m_linkSprites;
	oriUint					m_frameIndex;
	oriRect					m_bounds;
}

// ******** PROPERTIES ********

@property (nonatomic, readonly)	NSString*			propMultiSpriteName;
@property (nonatomic, readonly)	OriMultiSprite*		propMultiSprite;
@property (nonatomic, assign)	oriUint				propFrame;


// ******** INSTANCE METHODS ********

- (id)		initWithMultiSprite:(OriMultiSprite*)multisprite	Parent:(OriSceneObject*)parent;
- (id)		initWithMultiSpriteName:(NSString*)name				Parent:(OriSceneObject*)parent;

@end
