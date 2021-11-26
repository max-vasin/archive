//===============================================================================================
//  File:		GLFrame.h
//  Project:	OriginGameToolkit
//	Desc:		OpenGL bordered frame
//
//  Created by Max Vasin on 4/27/11.
//  Copyright 2011 Origin Games. All rights reserved.
//===============================================================================================

// ***** IMPORTS *****

#import <Cocoa/Cocoa.h>
#import "Base.h"
#import "BaseObject.h"

// ***** INTERFACE *****

@interface GLFrame : NSObject 
{
	BOOL		m_rebuild;
	
	GLuint		m_texture;
	uint		m_border;
	uint		m_width;
	uint		m_height;
	IntSize		m_texSize;
	IntPoint	m_position;
	NSColor*	m_color;
	BOOL		m_visible;
	
	OGLVertex	m_rectL[4];
	OGLVertex	m_rectR[4];
	OGLVertex	m_rectT[4];
	OGLVertex	m_rectB[4];
}

// ***** PROPERTIES *****

@property (nonatomic, assign)	uint		propWidth;
@property (nonatomic, assign)	uint		propHeight;
@property (nonatomic, assign)	int			propPositionX;
@property (nonatomic, assign)	int			propPositionY;
@property (nonatomic, assign)	uint		propBorder;
@property (nonatomic, copy)		NSColor*	propColor;
@property (nonatomic, assign)	GLuint		propTexture;
@property (nonatomic, assign)	IntSize		propTextureSize;
@property (nonatomic, assign)	BOOL		propVisible;

// ***** INSTANCE METHODS *****

- (void)render;

@end
