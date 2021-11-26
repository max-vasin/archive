//=======================================================================================================================
//  File:		OriHexGridCellRenderable.h
//  Project:	heli
//	Desc:		Hexagonal grid cell scene renderable object class interface
//
//  Created by Max Vasin on 6/7/11.
//  Copyright 2011 Origin. All rights reserved.
//=======================================================================================================================

// ******** IMPORTS ********

#import "OriBase.h"
#import "OriSceneObject.h"
#import "OriSprite.h"
#import "OriShapeDrawer.h"

// ******** INTERFACE ********

@class OriHexGridContainer;

@interface OriHexGridCellRenderable : OriSceneObject
{
    OriShapeDrawer*		m_shapeDrawer;
	oriRect				m_cellBounds;
	oriUint				m_type;
}

// ******** PROPERTIES ********

@property (nonatomic, assign)	oglColor	propColor;

// ******** INSTANCE METHODS ********

// ** initializers **

- (id)	initWithContainer:(OriHexGridContainer*)container andType:(oriUint)type;

// ** internal use **

- (void) updateShape:(OriHexGridContainer*)container;

@end
