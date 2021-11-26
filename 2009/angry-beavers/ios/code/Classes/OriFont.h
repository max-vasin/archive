//=======================================================================================================================
//  File:		OriFont.h
//  Project:	heli
//	Desc:		Font resource class interface
//
//  Created by Max Vasin on 6/8/11.
//  Copyright 2011 Origin. All rights reserved.
//=======================================================================================================================

// ******** IMPORTS ********

#import <UIKit/UIKit.h>
#import "OriBase.h"
#import "OriResource.h"
#import "OriTexture.h"
#import "OriSpriteAnimation.h"

// ******** FONT GLYPH STRUCTURE ********

typedef struct
{
	oriIntSize		size;
	oglMapVector2	mapmin;
	oglMapVector2	mapmax;

}oriFontGlyph;

// ******** INTERFACE ********

@interface OriFont : OriResource
{
	OriTexture*		m_texture;			// base font texture
	oriFontGlyph	m_glyphs[256];		// font glyphs array
	oriInt			m_space;
}

// ******** PROPERTIES ********

@property (nonatomic, readonly)		OriTexture*		propTexture;
@property (nonatomic, readonly)		oriInt			propSpace;

// ******** INSTANCE METHODS ********

- (oriFontGlyph*)	glyph:(oriChar)code;

@end
