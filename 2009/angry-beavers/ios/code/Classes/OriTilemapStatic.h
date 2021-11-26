//=======================================================================================================================
//  File:		OriTilemapStatic.h
//  Project:	heli
//	Desc:		Tilemap static objects container resource
//
//  Created by Max Vasin on 5/12/11.
//  Copyright 2011 Origin. All rights reserved.
//=======================================================================================================================

// ***** IMPORTS *****

#import <UIKit/UIKit.h>
#import "OriBase.h"
#import "OriXMLNode.h"
#import "OriResource.h"

// ***** INTERFACE *****

@class OriTilemap;

@interface OriTilemapStatic : NSObject 
{
    OriTilemap*			m_tilemap;
	OriResource*		m_object;
	oriIntPoint			m_position;
}

// ****** PROPERTIES ******

@property (nonatomic, readonly)		OriResource*	propObject;
@property (nonatomic, readonly)		oriIntPoint		propPosition;

// ****** CLASS METHODS ******

+ (id)		staticWithTilemap:(OriTilemap*)tilemap;
+ (id)		staticWithTilemap:(OriTilemap*)tilemap		withXML:(OriXMLNode*)node;

// ****** INSTANCE METHODS ******

- (id)		initWithTilemap:(OriTilemap*)tilemap;
- (id)		initWithTilemap:(OriTilemap*)tilemap		withXML:(OriXMLNode*)node;

@end
