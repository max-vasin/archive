//=======================================================================================================================
//  File:		OriTile.h
//  Project:	heli
//	Desc:		tileset item class
//
//  Created by Max Vasin on 5/11/11.
//  Copyright 2011 Origin. All rights reserved.
//=======================================================================================================================

// ***** IMPORTS *****

#import <UIKit/UIKit.h>
#import <OpenGLES/ES1/gl.h>
#import <OpenGLES/ES1/glext.h>
#import <OpenGLES/ES2/gl.h>
#import <OpenGLES/ES2/glext.h>

#import "OriBase.h"
#import "OriXMLNode.h"
#import "OriResource.h"
#import "OriTexture.h"

// ***** INTERFACE *****

@class OriTileset;

@interface OriTile : NSObject 
{
    OriTileset*		m_tileset;
	oriIntPoint		m_index;
	oglMapVector2	m_mapMin;
	oglMapVector2	m_mapMax;
}

// ****** PROPERTIES ******

@property (nonatomic, assign)	oriIntPoint		propIndex;
@property (nonatomic, readonly)	OriTileset*		propTileset;
@property (nonatomic, readonly)	oglMapVector2	propMapMin;
@property (nonatomic, readonly)	oglMapVector2	propMapMax;
@property (nonatomic, readonly)	oriRect			propMap;

// ****** CLASS METHODS ******

+ (id)					tileWithTileset:(OriTileset*)tileset;
+ (id)					tileWithTileset:(OriTileset*)tileset	withXML:(OriXMLNode*)node;
+ (id)					tileWithTileset:(OriTileset*)tileset	X:(oriUint)x	Y:(oriUint)y;

// ****** INSTANCE METHODS ******

- (id)					initWithTileset:(OriTileset*)tileset;
- (id)					initWithTileset:(OriTileset*)tileset	withXML:(OriXMLNode*)node;
- (id)					initWithTileset:(OriTileset*)tileset	X:(oriUint)x	Y:(oriUint)y;

- (void)				updateTextureDepencies;


@end
