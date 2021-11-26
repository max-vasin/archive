//=======================================================================================================================
//  File:		OriSceneObject.h
//  Project:	heli
//	Desc:		Base game object container class.
//
//  Created by Max Vasin on 3/24/11.
//  Copyright 2011 Origin. All rights reserved.
//=======================================================================================================================

// ******** IMPORTS ********

#import "OriBase.h"

// ******** SCENE OBJECT TYPE ********

typedef enum
{
	
	eSceneObjectDummy = 0,
	eSceneObjectRenderable,
	eSceneObjectContainer,
	
	eSceneObjectTypesCount
	
}enSceneObjectType;


// ******** INTERFACE ********

@class OriScene;

@interface OriSceneObject : NSObject 
{
	OriScene*			m_scene;		// parent scene
	OriSceneObject*		m_parent;		// parent object
	Vector2*			m_position;		// current object position to parent object
	Vector2*			m_velocity;		// current object velocity relative to parent object
	Vector2*			m_scale;		// current object scale
	oriFloat			m_layer;		// current object layer
}

// ******** PROPERTIES ********

@property (nonatomic, assign)	oriFloat			propLayer;
@property (nonatomic, copy)		Vector2*			propVelocity;
@property (nonatomic, copy)		Vector2*			propPosition;
@property (nonatomic, copy)		Vector2*			propScale;
@property (nonatomic, assign)	OriScene*			propScene;
@property (nonatomic, assign)	OriSceneObject*		propParent;

// ******** INSTANCE METHODS ********

- (id)					initWithParent:(OriSceneObject*)parent;

// ** determining object type **

- (enSceneObjectType)	type;
- (BOOL)				isMutable;
- (BOOL)				isDynamic;
- (NSArray*)			watchForPropsToRearrange;

// ** container related **

- (void)				attachContent:(OriScene*)scene;
- (void)				detachContent:(OriScene*)scene;

// ** renderable related **

- (void)				render;
- (oriRect)				bounds;

// ** mutable related **

- (void)				update:(oriFloat)delta;

// ** direct access **

- (void)				setPositionX:(oriFloat)x			Y:(oriFloat)y;
- (void)				setVelocityX:(oriFloat)x			Y:(oriFloat)y;
- (void)				updatePosition:(oriFloat)time;
- (void)				absoluteTranslationX:(oriFloat*)x	Y:(oriFloat*)y;
- (void)				sceneTranslationX:(oriFloat*)x		Y:(oriFloat*)y;
- (void)				absoluteScaleX:(oriFloat*)x			Y:(oriFloat*)y;
- (void)				sceneScaleX:(oriFloat*)x			Y:(oriFloat*)y;

// ** internal use **

- (NSInteger)			layerSort:(OriSceneObject*)object;

@end
