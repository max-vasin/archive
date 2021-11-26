//=======================================================================================================================
//  File:		OriScene.h
//  Project:	heli
//	Desc:		Base game objects collection.
//
//  Created by Max Vasin on 3/24/11.
//  Copyright 2011 Origin. All rights reserved.
//=======================================================================================================================

// ******** IMPORTS ********

#import "OriBase.h"
#import "OriTimer.h"
#import "OriSceneObject.h"
#import "OriTimeSampler.h"

// ******** INTERFACE ********

@class OriResourceManager;
@class OriRenderer;

@interface OriScene : NSObject <OriTimerEntry>
{
@private
	OriResourceManager*		m_resman;
	NSString*				m_name;
	NSMutableArray*			m_objects[eSceneObjectTypesCount];
	NSMutableArray*			m_mutables;
	NSMutableArray*			m_dynamics;
	Vector2*				m_translate;
	Vector2*				m_scale;
	oriInt					m_visibleObjectsCount;
	OriTimeSampler*			m_timeSampler;	// DEPRECATED
	BOOL					m_rearrange;
	
}

// ******** PROPERTIES ********

@property (nonatomic, copy)		NSString*			propName;
@property (nonatomic, copy)		Vector2*			propTranslate;
@property (nonatomic, copy)		Vector2*			propScale;
@property (nonatomic, assign)	oriFloat			propTimeSample;
@property (nonatomic, readonly)	oriInt				propVisibleObjectsCount;

// ******** INSTANCE METHODS ********

// ** initializers **

- (id)					initWithName:(NSString*)name;

// ** scene objects control **

- (void)				attach:(OriSceneObject*)object;
- (void)				detach:(OriSceneObject*)object;
- (oriUint)				objectsCountOfType:(enSceneObjectType)type;
- (OriSceneObject*)		objectOfType:(enSceneObjectType)type	atIndex:(oriUint)index;

// ** scene life cycle **

- (void)				resetTime;
- (void)				render;
- (void)				rearrange;
- (void)				autoRearrange;

@end
