//=======================================================================================================================
//  File:		OriMapStaticContainer.m
//  Project:	heli
//	Desc:		Map static objects container implementation
//
//  Created by Max Vasin on 6/16/11.
//  Copyright 2011 Origin. All rights reserved.
//=======================================================================================================================

// ******** IMPORTS ********

#import "OriMapStaticContainer.h"
#import "OriScene.h"

// ******** PRIVATE METHODS ********

@interface OriMapStaticContainer (Private)

- (void)makeObjects;

@end

// ******** IMPLEMENTATION ********

@implementation OriMapStaticContainer

//#######################################################################################################################
// PROPERTIES
//#######################################################################################################################

//-----------------------------------------------------------------------------------------------------------------------
// SYNTHESIZED PROPERTIES
//-----------------------------------------------------------------------------------------------------------------------

//#######################################################################################################################
// INITIALIZERS\DESTRUCTORS
//#######################################################################################################################

//-----------------------------------------------------------------------------------------------------------------------
// Designated initializer
//-----------------------------------------------------------------------------------------------------------------------

- (id)initWithTilemap:(OriTilemap*)tilemap Parent:(OriSceneObject*)parent
{
	if ( (self = [super initWithParent:parent]) )
	{
		m_tilemap	= [tilemap retain];
		m_objects	= [[NSMutableArray alloc] initWithCapacity:50];
		
		[self makeObjects];
	}
	return self;
}

//-----------------------------------------------------------------------------------------------------------------------
// Initializer with tilemap name
//-----------------------------------------------------------------------------------------------------------------------

- (id)initWithTilemapName:(NSString*)name Parent:(OriSceneObject*)parent
{
	OriTilemap* tilemap = [[OriResourceManager sharedManager] getTilemap:name];
	
	DBG_ASSERT_LOG1A(!tilemap, @"OriMapStaticContainer: failed to get tilemap with name (%@)", name);
	
	self = [self initWithTilemap:tilemap Parent:parent];
	
	[tilemap release];
	
	return self;
}

//-----------------------------------------------------------------------------------------------------------------------
// Destructor
//-----------------------------------------------------------------------------------------------------------------------

- (void)dealloc
{
	[m_objects release];
	
	[m_tilemap release];
	
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
	return NO;
}

//-----------------------------------------------------------------------------------------------------------------------
// We are dynamic
//-----------------------------------------------------------------------------------------------------------------------

- (BOOL)isDynamic
{
	return YES;
}

//-----------------------------------------------------------------------------------------------------------------------
// Put all tile renderables to scene and sort
//-----------------------------------------------------------------------------------------------------------------------

- (void)attachContent:(OriScene*)scene
{
	for (OriSceneObject* object in m_objects)
		[scene attach:object];
}

//-----------------------------------------------------------------------------------------------------------------------
// Remove all renderables from scene
//-----------------------------------------------------------------------------------------------------------------------

- (void)detachContent:(OriScene*)scene
{
	for (OriSceneObject* object in m_objects)
		[scene detach:object];
}

//#######################################################################################################################
// PRIVATE METHODS
//#######################################################################################################################

//-----------------------------------------------------------------------------------------------------------------------
// Makes objects from resource
//-----------------------------------------------------------------------------------------------------------------------

- (void)makeObjects
{
	[m_objects removeAllObjects];
	
	oriIntSize mapDrawSize = m_tilemap.propDrawSize;
	
	for (oriUint index = 0; index < m_tilemap.propStaticObjectsCount; index++)
	{
		OriTilemapStatic* source = [m_tilemap staticAtIndex:index];
		oriIntPoint position	 = source.propPosition;
		
		OriResource* resource	 = source.propObject;
		
		if ([resource class] == [OriMultiSprite class])
		{
			OriMultiSprite* multisprite = (OriMultiSprite*)resource;
			
			OriMapStaticRenderable* object = [[OriMapStaticRenderable alloc] initWithMultiSprite:multisprite Parent:self];
			
			[object setPositionX:position.x Y:mapDrawSize.height - position.y];
			
			[m_objects addObject:object];
			
			[object release];
		}
	}
}


@end
