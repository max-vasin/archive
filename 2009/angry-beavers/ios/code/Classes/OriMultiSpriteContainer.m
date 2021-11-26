//=======================================================================================================================
//  File:		OriMultiSpriteContainer.m
//  Project:	heli
//	Desc:		multisprite scene object implementation
//
//  Created by Max Vasin on 4/15/11.
//  Copyright 2011 Origin. All rights reserved.
//=======================================================================================================================

// ******** IMPORTS ********

#import "OriMultiSpriteContainer.h"
#import "OriSpriteRenderable.h"
#import "OriScene.h"

// ******** PRIVATE METHODS ********


@interface OriMultiSpriteContainer(Private)

- (void)makeSprites;
- (void)updateSprites;

@end

// ******** IMPLEMENTATION ********

@implementation OriMultiSpriteContainer

//#######################################################################################################################
// PROPERTIES
//#######################################################################################################################

//-----------------------------------------------------------------------------------------------------------------------
// PROPERTY: overriding layer property
//-----------------------------------------------------------------------------------------------------------------------

- (void)setPropLayer:(oriFloat)layer
{
	if (m_layer != layer)
	{
	
		m_baseSprite.propLayer -= m_layer;
		m_baseSprite.propLayer += layer;
	
		for (OriSpriteRenderable* linkSprite in m_linkSprites)
		{
			linkSprite.propLayer -= m_layer;
			linkSprite.propLayer += layer;
		}
	
		m_layer = layer;
		
		[m_scene autoRearrange];
	}
}

- (oriFloat)layer
{
	return m_layer;
}

//-----------------------------------------------------------------------------------------------------------------------
// PROPERTY: current multisprite resource
//-----------------------------------------------------------------------------------------------------------------------

- (OriMultiSprite*)propMultiSprite
{
	return m_multisprite;
}

//-----------------------------------------------------------------------------------------------------------------------
// PROPERTY: current multisprite resource name
//-----------------------------------------------------------------------------------------------------------------------

- (NSString*)propMultiSpriteName
{
	return m_multisprite.propName;
}

//-----------------------------------------------------------------------------------------------------------------------
// PROPERTY: current multisprite frame
//-----------------------------------------------------------------------------------------------------------------------

- (void)setPropFrame:(oriUint)frame
{
	if (m_frameIndex != frame)
	{
		m_frameIndex = frame;
		
		[self updateSprites];
	}
}

- (oriUint)propFrame
{
	return m_frameIndex;
}

//#######################################################################################################################
// INITIALIZERS\DESTRUCTORS
//#######################################################################################################################

//-----------------------------------------------------------------------------------------------------------------------
// Initializes new multisprite container with mutlisprite resource
//-----------------------------------------------------------------------------------------------------------------------

- (id)initWithMultiSprite:(OriMultiSprite*)multisprite Parent:(OriSceneObject*)parent
{
	if ( (self = [super initWithParent:parent]) )
	{
		m_multisprite = [multisprite retain];
		
		m_linkSprites = [[NSMutableArray alloc] initWithCapacity:10];
		
		[self makeSprites];
		
		[self updateSprites];
	}
	return self;
}

//-----------------------------------------------------------------------------------------------------------------------
// Initializes new multisprite container with mutlisprite resource name
//-----------------------------------------------------------------------------------------------------------------------

- (id)initWithMultiSpriteName:(NSString*)name Parent:(OriSceneObject*)parent
{
	OriMultiSprite* multisprite = [[OriResourceManager sharedManager] getMultiSprite:name];
	
	DBG_ASSERT_LOG1A(!multisprite, @"OriMultiSpriteContainer: failed to get multisprite resource with name (%@)", name);
	
	self = [self initWithMultiSprite:multisprite Parent:parent];
	
	[multisprite release];
	
	return self;
}

//-----------------------------------------------------------------------------------------------------------------------
// Destructor
//-----------------------------------------------------------------------------------------------------------------------

- (void)dealloc
{
	[m_multisprite release];
	
	[m_baseSprite release];
	
	[m_linkSprites release];
	
	[super dealloc];
}

//#######################################################################################################################
// OVERRIDINGS OF OriSceneObject
//#######################################################################################################################

//-----------------------------------------------------------------------------------------------------------------------
// We are container
//-----------------------------------------------------------------------------------------------------------------------

- (enSceneObjectType)type
{
	return eSceneObjectContainer;
}

//-----------------------------------------------------------------------------------------------------------------------
// We are mutable
//-----------------------------------------------------------------------------------------------------------------------

- (BOOL)isMutable
{
	return YES;
}

//-----------------------------------------------------------------------------------------------------------------------
// Put all tile renderables to scene and sort
//-----------------------------------------------------------------------------------------------------------------------

- (void)attachContent:(OriScene*)scene
{
	[scene attach:m_baseSprite];
	
	for (OriSceneObject* object in m_linkSprites)
		[scene attach:object];
}

//-----------------------------------------------------------------------------------------------------------------------
// Remove all renderables from scene
//-----------------------------------------------------------------------------------------------------------------------

- (void)detachContent:(OriScene*)scene
{
	[scene detach:m_baseSprite];
	
	for (OriSceneObject* object in m_linkSprites)
		[scene detach:object];
}

//#######################################################################################################################
// PRIVATE METHODS
//#######################################################################################################################

//-----------------------------------------------------------------------------------------------------------------------
// Makes new sprite renderables
//-----------------------------------------------------------------------------------------------------------------------

- (void)makeSprites
{
	if (m_multisprite)
	{
		NSString* baseSpriteName = m_multisprite.propBaseSprite.propName;
		
		m_baseSprite = [[OriSpriteRenderable alloc] initWithSpriteName:baseSpriteName Parent:self];
		m_baseSprite.propAnimation = m_multisprite.propBaseAnimation;
		
		m_layer = m_multisprite.propLayer;
		
		for (oriUint i = 0; i < m_multisprite.propLinksCount; i++)
		{
			NSString* linkName = [m_multisprite getSpriteLink:i];
			
			if (linkName)
			{
				OriSpriteRenderable* linkSprite = [[OriSpriteRenderable alloc] initWithSpriteName:linkName Parent:self];
				
				[m_linkSprites addObject:linkSprite];
				
				[linkSprite release];
			}
		}
	}
}

// Update sprite objects
//-----------------------------------------------------------------------------------------------------------------------
- (void)updateSprites
{
	m_baseSprite.propFrame = m_frameIndex;
	
	OriMultiSpriteRecord* record = [m_multisprite getRecord:m_frameIndex];
	
	if (record)
	{
		for (oriUint i = 0; i < record.propKeysCount; i++)
		{
			OriMultiSpriteKey* key = [record getKeyAtIndex:i];
			
			if (key)
			{
				OriSpriteRenderable* linkSprite = [m_linkSprites objectAtIndex:key.propLink];
				
				linkSprite.propAnimation	= key.propAnimation;
				linkSprite.propFrame		= key.propFrame;
				linkSprite.propLayer		= m_layer + key.propLayer;
				
				oriIntPoint bindPoint		= key.propBindPoint;
				oriIntPoint	baseOrigin		= [m_multisprite.propBaseAnimation getFrame:m_frameIndex].propOrigin;
				
				[linkSprite setPositionX:(bindPoint.x - baseOrigin.x) Y:-(bindPoint.y - baseOrigin.y)];
			}
		}
	}
	
	[m_scene autoRearrange];
}
 
@end
