//=======================================================================================================================
//  File:		OriTileRenderable.h
//  Project:	heli
//	Desc:		Tile renderable scene object class interface
//
//  Created by Max Vasin on 6/3/11.
//  Copyright 2011 Origin. All rights reserved.
//=======================================================================================================================

// ******** IMPORTS ********

#import "OriBase.h"
#import "OriSceneObject.h"
#import "OriTile.h"
#import "OriShapeDrawer.h"

// ******** INTERFACE ********

@class OriTilemapContainer;

@interface OriTileRenderable : OriSceneObject
{
	OriShapeDrawer*		m_shapeDrawer;
	oriRect				m_tileBounds;
}

// ******** PROPERTIES ********

@property (nonatomic, assign)	oglColor	propColor;

// ******** INSTANCE METHODS ********

// ** initializers **

- (id)	initWithContainer:(OriTilemapContainer*)container andTile:(OriTile*)tile;

@end
