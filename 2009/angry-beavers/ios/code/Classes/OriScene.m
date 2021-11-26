//=======================================================================================================================
//  File:		OriScene.m
//  Project:	heli
//	Desc:		Base game objects collection container
//
//  Created by Max Vasin on 3/24/11.
//  Copyright 2011 Origin. All rights reserved.
//=======================================================================================================================

// ******** IMPORTS ********

#import "OriScene.h"
#import "OriResourceManager.h"
#import "OriTimer.h"

// ******** PRIVATE METHODS ********

@interface OriScene (Private)

- (NSMutableArray*)	listForObject:(OriSceneObject*)object;
- (BOOL)			objectVisible:(OriSceneObject*)object;

@end


// ******** IMPLEMENTATION ********

@implementation OriScene

//#######################################################################################################################
// PROPERTIES
//#######################################################################################################################

//-----------------------------------------------------------------------------------------------------------------------
// SYNTHESIZED PROPERTIES
//-----------------------------------------------------------------------------------------------------------------------

@synthesize propName				= m_name;
@synthesize propTranslate			= m_translate;
@synthesize propScale				= m_scale;
@synthesize propVisibleObjectsCount	= m_visibleObjectsCount;

//-----------------------------------------------------------------------------------------------------------------------
// PROPERTY: time sampler sample size
//-----------------------------------------------------------------------------------------------------------------------

- (void)setPropTimeSample:(oriFloat)sample
{
	m_timeSampler.propFrame		= sample;
}

- (oriFloat)propTimeSample
{
	return m_timeSampler.propFrame;
}

//#######################################################################################################################
// INITIALIZERS/DESTRUCTORS
//#######################################################################################################################

//-----------------------------------------------------------------------------------------------------------------------
// Default initializer
//-----------------------------------------------------------------------------------------------------------------------

- (id)init
{
	if ( (self = [super init]) )
	{
		m_resman		= [[OriResourceManager sharedManager] retain];
		
		for (oriUint typeIndex = 0; typeIndex < eSceneObjectTypesCount; typeIndex++)
			m_objects[typeIndex] = [[NSMutableArray alloc] initWithCapacity:50];
		
		m_mutables		= [[NSMutableArray alloc] initWithCapacity:50];
		m_dynamics		= [[NSMutableArray alloc] initWithCapacity:50];
		m_timeSampler	= [[OriTimeSampler alloc] init];
		
		self.propName	= @"unnamed scene";
		
		m_translate		= [[Vector2 alloc] initWithX:0 Y:0];
		m_scale			= [[Vector2 alloc] initWithX:1 Y:1];
		
		
		//[[OriTimer sharedTimer] attach:self];
	}
	return self;
}

//-----------------------------------------------------------------------------------------------------------------------
// Initializes new instance of scene with name
//-----------------------------------------------------------------------------------------------------------------------

- (id)initWithName:(NSString*)name
{
	if ( (self = [self init]) )
	{
		self.propName	= name;
	}
	return self;
}

//-----------------------------------------------------------------------------------------------------------------------
// Destructor
//-----------------------------------------------------------------------------------------------------------------------

- (void)dealloc
{
	//[[OriTimer sharedTimer] detach:self];
	
	for (oriUint typeIndex = 0; typeIndex < eSceneObjectTypesCount; typeIndex++)
		[m_objects[typeIndex] release];
	
	[m_mutables release];
	[m_dynamics release];
	
	[m_resman release];
	
	[m_name release];
	
	[m_translate release];
	[m_scale release];
	
	[super dealloc];
}



//#######################################################################################################################
// OBJECTS CONTROL
//#######################################################################################################################

//-----------------------------------------------------------------------------------------------------------------------
// Attaches object to scene
//-----------------------------------------------------------------------------------------------------------------------

- (void)attach:(OriSceneObject*)object
{
	DBG_ASSERT_LOG0A(!object, @"scene->attach: nil object recieved");
	
	if (!object)
		return;
	
	NSMutableArray* list = [self listForObject:object];
	
	DBG_ASSERT_LOG0A(!list, @"scene->attach: cant find list for the object");
	
	if (!list)
		return;
	
	if (![list containsObject:object])
		[list addObject:object];
	
	if ([object isMutable])
		if (![m_mutables containsObject:object])
			[m_mutables addObject:object];
	
	if ([object isDynamic])
		if (![m_dynamics containsObject:object])
			[m_dynamics addObject:object];
	
	if ([object type] == eSceneObjectContainer)
		[object attachContent:self];
	
	if (object.propScene)
		[object.propScene detach:object];
	
	object.propScene = self;
	
	[self autoRearrange];
}

//-----------------------------------------------------------------------------------------------------------------------
// Detaches object from scene
//-----------------------------------------------------------------------------------------------------------------------

- (void)detach:(OriSceneObject*)object
{
	DBG_ASSERT_LOG0A(!object, @"scene->detach: nil object recieved");
	
	NSMutableArray* list = [self listForObject:object];
	
	DBG_ASSERT_LOG0A(!list, @"scene->attach: cant find list for the object");
	
	if (!list)
		return;
	
	if ([list containsObject:object])
	{
		if ([object type] == eSceneObjectContainer)
			[object detachContent:self];

		if (object.propScene == self)
			object.propScene = nil;

		[list removeObject:object];
	}
	
	if ([object isMutable])
		[m_mutables removeObject:object];
	
	if ([object isDynamic])
		[m_dynamics removeObject:object];
	
	[self autoRearrange];
}

//-----------------------------------------------------------------------------------------------------------------------
// Returns count of attached object of given type
//-----------------------------------------------------------------------------------------------------------------------

- (oriUint)objectsCountOfType:(enSceneObjectType)type
{
	if (type >= eSceneObjectTypesCount)
		return 0;
	
	return [m_objects[type] count];
}

//-----------------------------------------------------------------------------------------------------------------------
// Returns object by type and index
//-----------------------------------------------------------------------------------------------------------------------

- (OriSceneObject*)objectOfType:(enSceneObjectType)type atIndex:(oriUint)index
{
	if (type >= eSceneObjectTypesCount)
		return nil;
	
	NSMutableArray* list = m_objects[type];
	
	if (index >= [list count])
		return nil;
	
	return [list objectAtIndex:index];
}


//#######################################################################################################################
// OriTimerEntry PROTOCOL SUPPORT
//#######################################################################################################################

// Using weak link
//-----------------------------------------------------------------------------------------------------------------------
- (BOOL)isTimerWeakLink
{
	return YES;
}

// Updates all scene objects with given timedelta
//-----------------------------------------------------------------------------------------------------------------------
- (void)update:(oriFloat)delta
{
	for (OriSceneObject* object in m_dynamics)
		[object updatePosition:delta];
	
	for (OriSceneObject* object in m_mutables)
		[object update:delta];
	
	if (m_rearrange)
		[self rearrange];
	
	/*
	for (oriUint typeIndex = 0; typeIndex < eSceneObjectTypesCount; typeIndex++)
		for (OriSceneObject* object in m_objects[typeIndex])
			[object updatePosition:delta];
	
	// call object self-updating
	
	for (oriUint typeIndex = 0; typeIndex < eSceneObjectTypesCount; typeIndex++)
		for (OriSceneObject* object in m_objects[typeIndex])
			if ([object isMutable])
				[object update:delta];
	
	 */
//	for (OriSceneObject* object in m_objects[eSceneObjectContainer])
//		[object updatePosition:delta];
}

//#######################################################################################################################
// LIFECYCLE
//#######################################################################################################################

//-----------------------------------------------------------------------------------------------------------------------
// Resets internal time sampler
//-----------------------------------------------------------------------------------------------------------------------

- (void)resetTime
{
	[m_timeSampler reset];
}

//-----------------------------------------------------------------------------------------------------------------------
// Renders all scene objects
//-----------------------------------------------------------------------------------------------------------------------

- (void)render
{
	glMatrixMode(GL_MODELVIEW);
	
	if (m_translate || m_scale)
	{
		glPushMatrix();
		if (m_translate)
			glTranslatef(m_translate->x, m_translate->y, 0.0f);
		
		if (m_scale)
			glScalef(m_scale->x, m_scale->y, 1.0f);
	}
	
	m_visibleObjectsCount = 0;
	
	for (OriSceneObject* object in m_objects[eSceneObjectRenderable])
	{
		if (![self objectVisible:object])
			continue;
		
		oriVector2 translation	= {0, 0};
		oriVector2 scale		= {1, 1};
		
		[object sceneTranslationX:&translation.x Y:&translation.y];
		[object sceneScaleX:&scale.x Y:&scale.y];
		
		glPushMatrix();
		glTranslatef(floor(translation.x), floor(translation.y), 0.0f);
		glScalef(scale.x, scale.y, 1.0f);
		
		[object render];
		
		glPopMatrix();
		
		m_visibleObjectsCount++;
	}
	
	if (m_translate || m_scale)
		glPopMatrix();
}

//-----------------------------------------------------------------------------------------------------------------------
// Rearrange objects by layer
//-----------------------------------------------------------------------------------------------------------------------

- (void)rearrange
{
	[m_objects[eSceneObjectRenderable] sortUsingSelector:@selector(layerSort:)];
	
	m_rearrange = NO;
}

//-----------------------------------------------------------------------------------------------------------------------
// Autorearrange objects by layer
//-----------------------------------------------------------------------------------------------------------------------

- (void)autoRearrange
{
	m_rearrange = YES;
}


//#######################################################################################################################
// PRIVATE METHODS
//#######################################################################################################################

//-----------------------------------------------------------------------------------------------------------------------
// Returns appropriate list for object by type
//-----------------------------------------------------------------------------------------------------------------------

- (NSMutableArray*)listForObject:(OriSceneObject*)object
{
	if (!object)
		return nil;
	
	enSceneObjectType type = [object type];
	
	return m_objects[type];
}

//-----------------------------------------------------------------------------------------------------------------------
// Returns YES if object bounding rectangle in renderer view rectangle
//-----------------------------------------------------------------------------------------------------------------------

- (BOOL)objectVisible:(OriSceneObject*)object
{
	oriRect viewRect	= m_resman.propRenderer.propViewRect;
	oriRect objRect		= [object bounds];
	
	// translate object rectangle
	
	oriVector2	translate = {0, 0};
	
	[object absoluteTranslationX:&translate.x Y:&translate.y];
	
	objRect.min.x += translate.x;
	objRect.min.y += translate.y;
	objRect.max.x += translate.x;
	objRect.max.y += translate.y;
	
	return ! (objRect.min.x > viewRect.max.x || objRect.max.x < viewRect.min.x || objRect.min.y > viewRect.max.y || objRect.max.y < viewRect.min.y);
}



@end
