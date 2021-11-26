//=======================================================================================================================
//  File:		OriMapStaticContainer.h
//  Project:	heli
//	Desc:		Map static objects container class interface
//
//  Created by Max Vasin on 6/16/11.
//  Copyright 2011 Origin. All rights reserved.
//=======================================================================================================================

// ******** IMPORTS ********

#import "OriBase.h"
#import "OriTimer.h"
#import "OriSceneObject.h"
#import "OriMapStaticRenderable.h"
#import "OriTilemap.h"

// ******** INTERFACE ********

@interface OriMapStaticContainer : OriSceneObject
{
	OriTilemap*			m_tilemap;
	NSMutableArray*		m_objects;
}

// ******** INSTANCE METHODS ********

- (id)		initWithTilemap:(OriTilemap*)tilemap	Parent:(OriSceneObject*)parent;
- (id)		initWithTilemapName:(NSString*)name		Parent:(OriSceneObject*)parent;


@end
