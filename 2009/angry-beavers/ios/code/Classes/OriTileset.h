//=======================================================================================================================
//  File:		OriTileset.h
//  Project:	heli
//	Desc:		tileset resource class
//
//  Created by Max Vasin on 3/21/11.
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
#import "OriTile.h"
#import "Array2D.h"

// ***** INTERFACE *****

@class OriResourceManager;

@interface OriTileset : OriResource <Array2DDelegate>
{
	OriTexture*			m_texture;
	Array2D*			m_tiles;
	oriIntSize			m_dimensions;
	oriIntSize			m_tileSize;
}

// ****** PROPERTIES ******

@property (nonatomic, assign)	OriTexture*		propTexture;
@property (nonatomic, assign)	oriIntSize		propTileSize;

// ****** CLASS METHODS ******

+ (id)					tilesetWithManager:(OriResourceManager*)resman	withName:(NSString*)name;
+ (id)					tilesetWithManager:(OriResourceManager*)resman	withName:(NSString*)name	withXML:(OriXMLNode*)node;

// ****** INSTANCE METHODS ******

- (id)					initWithManager:(OriResourceManager*)resman		withName:(NSString*)name;
- (id)					initWithManager:(OriResourceManager*)resman		withName:(NSString*)name	withXML:(OriXMLNode*)node;

- (OriTile*)			tileAtIndex:(oriIntPoint)index;
- (oriIntPoint)			indexOfTile:(OriTile*)tile;

@end
