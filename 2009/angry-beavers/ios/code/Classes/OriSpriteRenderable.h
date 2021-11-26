//=======================================================================================================================
//  File:		OriSpriteRenderable.h
//  Project:	heli
//	Desc:		Sprite renderable scene object class interface
//
//  Created by Max Vasin on 3/24/11.
//  Copyright 2011 Origin. All rights reserved.
//=======================================================================================================================

// ******** IMPORTS ********

#import <UIKit/UIKit.h>
#import "OriBase.h"
#import "OriSceneObject.h"
#import "OriResourceManager.h"
#import "OriTimeSampler.h"
#import "OriShapeDrawer.h"

// ******** INTERFACE ********

@interface OriSpriteRenderable : OriSceneObject 
{
	OriSprite*				m_sprite;			// sprite resource reference
	OriSpriteAnimation*		m_animation;		// sprite animation resource reference
	OriShapeDrawer*			m_shapeDrawer;		// shape drawer used to render sprite
	oriRect					m_spriteBounds;		// sprite bounding rectangle
	OriTimeSampler*			m_timeSampler;		// time quantizer used to play animation
	oriUint					m_frameIndex;		// current frame index
	BOOL					m_playing;			// animation playing flag
}

// ******** PROPERTIES ********

@property (nonatomic, assign)	NSString*				propSpriteName;
@property (nonatomic, assign)	NSString*				propAnimationName;
@property (nonatomic, assign)	OriSprite*				propSprite;
@property (nonatomic, assign)	OriSpriteAnimation*		propAnimation;
@property (nonatomic, assign)	oriInt					propFPS;
@property (nonatomic, assign)	oriUint					propFrame;

// ******** INSTANCE METHODS ********

//** initializers **

- (id)		initWithSprite:(OriSprite*)sprite	Parent:(OriSceneObject*)parent;
- (id)		initWithSpriteName:(NSString*)name	Parent:(OriSceneObject*)parent;

//** animation control **

- (void)	play;
- (void)	stop;
 
@end
