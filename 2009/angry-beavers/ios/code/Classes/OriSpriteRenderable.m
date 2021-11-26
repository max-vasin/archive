//=======================================================================================================================
//  File:		OriSpriteRenderable.m
//  Project:	heli
//	Desc:		Sprite renderable scene object class implementation
//
//  Created by Max Vasin on 3/24/11.
//  Copyright 2011 Origin. All rights reserved.
//=======================================================================================================================

// ******** IMPORTS ******** 

#import "OriSpriteRenderable.h"

// ******** PRIVATE METHODS ******** 

@interface OriSpriteRenderable(Private)

- (void)updateFrame;

@end

// ******** IMPLEMENTATION ********

@implementation OriSpriteRenderable

//#######################################################################################################################
// PROPERTIES
//#######################################################################################################################

//-----------------------------------------------------------------------------------------------------------------------
// PROPERTY: current sprite resource
//-----------------------------------------------------------------------------------------------------------------------

- (void)setPropSprite:(OriSprite*)sprite
{
	if (m_sprite != sprite)
	{
		[m_sprite release];
		
		m_sprite = [sprite retain];
		
		m_shapeDrawer.propTexture = m_sprite.propTexture;
		
		self.propAnimation = [m_sprite getAnimationByIndex:0];
	}
}

- (OriSprite*)propSprite
{
	return m_sprite;
}

//-----------------------------------------------------------------------------------------------------------------------
// PROPERTY: current sprite resource name
//-----------------------------------------------------------------------------------------------------------------------

- (void)setPropSpriteName:(NSString*)name
{
	OriSprite* sprite = [[OriResourceManager sharedManager] getSprite:name];
	
	DBG_ASSERT_LOG1A(!sprite, @"OriSpriteRenderable: failed to get sprite with name (%@)", name);
	
	self.propSprite = sprite;
	
	[sprite release];
}

- (NSString*)propSpriteName
{
	return m_sprite.propName;
}

//-----------------------------------------------------------------------------------------------------------------------
// PROPERTY: current sprite animation
//-----------------------------------------------------------------------------------------------------------------------

- (void)setPropAnimation:(OriSpriteAnimation*)animation
{
	if (m_animation != animation)
	{
		m_animation = animation;
		
		self.propFPS = m_animation.propSpeed;
		
		if (m_frameIndex >= m_animation.propFrameCount)
			m_frameIndex = m_animation.propFrameCount - 1;
	
		[self updateFrame];
	}
}

- (OriSpriteAnimation*)propAnimation
{
	return m_animation;
}

//-----------------------------------------------------------------------------------------------------------------------
// PROPERTY: current sprite animation name
//-----------------------------------------------------------------------------------------------------------------------

- (void)setPropAnimationName:(NSString*)name
{
	OriSpriteAnimation* animation = [m_sprite getAnimation:name];
	
	DBG_ASSERT_LOG1A(!animation, @"OriSpriteRenderable: failed to get animation with name (%@)", name);
	
	self.propAnimation = animation;
}

- (NSString*)propAnimationName
{
	return m_animation.propName;
}

//-----------------------------------------------------------------------------------------------------------------------
// PROPERTY: current animation FPS
//-----------------------------------------------------------------------------------------------------------------------

- (void)setPropFPS:(oriInt)fps
{
	if (!fps)
		m_timeSampler.propFrame = 65535;
	else
		m_timeSampler.propFrame = 1.0f / (oriFloat)fps;
}

- (oriInt)propFPS
{
	oriFloat time = m_timeSampler.propFrame;
	
	if (!time)
		return 0;
	
	return (oriInt)(1.0f / time);
}

//-----------------------------------------------------------------------------------------------------------------------
// PROPERTY: current animation frame index
//-----------------------------------------------------------------------------------------------------------------------

- (void)setPropFrame:(oriUint)frame
{
	if (m_frameIndex != frame)
	{
		m_frameIndex = frame;
		
		if (m_frameIndex >= m_animation.propFrameCount)
			m_frameIndex = m_animation.propFrameCount - 1;
		
		[self updateFrame];
	}
}

- (oriUint)propFrame
{
	return m_frameIndex;
}


//#######################################################################################################################
// INITIALIZERS/DESTRUCTORS
//#######################################################################################################################

//-----------------------------------------------------------------------------------------------------------------------
// Initializes new sprite renderable with resource
//-----------------------------------------------------------------------------------------------------------------------

- (id)initWithSprite:(OriSprite*)sprite Parent:(OriSceneObject*)parent
{
	if ( (self = [super initWithParent:parent]) )
	{
		m_timeSampler	= [[OriTimeSampler alloc] init];
		m_shapeDrawer	= [[OriShapeDrawer alloc] initRectangle];
		
		self.propSprite	= sprite;
	}
	return self;
}

//-----------------------------------------------------------------------------------------------------------------------
// Initializes new sprite renderable with resource name
//-----------------------------------------------------------------------------------------------------------------------

- (id)initWithSpriteName:(NSString*)name Parent:(OriSceneObject*)parent
{
	OriSprite* sprite = [[OriResourceManager sharedManager] getSprite:name];
	
	DBG_ASSERT_LOG1A(!sprite, @"OriSpriteRenderable: failed to get sprite with name (%@)", name);
	
	if (!sprite)
	{
		[self release];
		return nil;
	}
	
	self = [self initWithSprite:sprite Parent:parent];
	
	[sprite release];
	
	return self;
}

//-----------------------------------------------------------------------------------------------------------------------
// Destructor
//-----------------------------------------------------------------------------------------------------------------------

- (void)dealloc
{
	[m_sprite release];
	[m_timeSampler release];
	[m_shapeDrawer release];
	
	[super dealloc];
}

//#######################################################################################################################
// OVERRIDINGS OF OriSceneObject
//#######################################################################################################################

//-----------------------------------------------------------------------------------------------------------------------
// We are renderable object
//-----------------------------------------------------------------------------------------------------------------------

- (enSceneObjectType)type
{
	return eSceneObjectRenderable;
}

//-----------------------------------------------------------------------------------------------------------------------
// We are immutable
//-----------------------------------------------------------------------------------------------------------------------

- (BOOL)isMutable
{
	return YES;
}

//-----------------------------------------------------------------------------------------------------------------------
// Returns sprite bounds
//-----------------------------------------------------------------------------------------------------------------------

- (oriRect)bounds
{
	return m_spriteBounds;
}


//-----------------------------------------------------------------------------------------------------------------------
// Rendering sprite
//-----------------------------------------------------------------------------------------------------------------------

- (void)render
{
	[m_shapeDrawer render];
}

//-----------------------------------------------------------------------------------------------------------------------
// Updating sprite
//-----------------------------------------------------------------------------------------------------------------------

- (void)update:(oriFloat)delta
{
	if (m_playing)
	{
		oriInt framesToGo = [m_timeSampler update:delta];
		
		if (framesToGo)
		{
			m_frameIndex += framesToGo;
		
			while (m_frameIndex >= m_animation.propFrameCount) 
				m_frameIndex -= m_animation.propFrameCount;
			
			[self updateFrame];
		}
	}
}

//#######################################################################################################################
// ANIMATION CONTROL
//#######################################################################################################################

//-----------------------------------------------------------------------------------------------------------------------
// Starts animation playing
//-----------------------------------------------------------------------------------------------------------------------

- (void)play
{
	m_playing = YES;
}

//-----------------------------------------------------------------------------------------------------------------------
// Stops animation playing
//-----------------------------------------------------------------------------------------------------------------------

- (void)stop
{
	m_playing = NO;
}

//#######################################################################################################################
// PRIVATE METHODS
//#######################################################################################################################

//-----------------------------------------------------------------------------------------------------------------------
// Updates current frame geometry
//-----------------------------------------------------------------------------------------------------------------------

- (void)updateFrame
{
	OriSpriteAnimationFrame* frame = [m_animation getFrame:m_frameIndex];
	
	if (!frame)
		return;
	
	oriIntPoint origin	= frame.propOrigin;
	oriIntSize  size    = frame.propSize;
	
	oriVector2 offset = {-origin.x, - (size.height - origin.y)};
	
	m_shapeDrawer.propOffset = offset;
	
	[m_shapeDrawer rectangleSetSize:size];
	[m_shapeDrawer rectangleSetMapping:frame.propMapRect];
	
	m_spriteBounds.min.x = -origin.x;
	m_spriteBounds.min.y = -(size.height - origin.y);
	m_spriteBounds.max.x = m_spriteBounds.min.x + size.width;
	m_spriteBounds.max.y = m_spriteBounds.min.y + size.height;
}

@end
