//=======================================================================================================================
//  File:		OriShapeDrawer.h
//  Project:	heli
//	Desc:		Shape OpenGL drawer utility class interface
//
//  Created by Max Vasin on 6/3/11.
//  Copyright 2011 Origin. All rights reserved.
//=======================================================================================================================

// ******** IMPORTS ********

#import <UIKit/UIKit.h>
#import "OriBase.h"
#import "OriTexture.h"

// ******** SHAPE DRAWER SHAPE TYPE ********

typedef enum 
{

	eShapeDrawerRectangle	= 0

}enShapeDrawerType;

// ******** INTERFACE ********

@interface OriShapeDrawer : NSObject 
{
@private
	enShapeDrawerType	m_type;
	oglVertex*			m_vertexBuffer;
	oriUint				m_vertexCount;
	OriTexture*			m_texture;
	oglColor			m_color;
	oriVector2			m_offset;
}

// ******** PROPERTIES ********

@property (nonatomic, assign)	OriTexture*		propTexture;
@property (nonatomic, assign)	oglColor		propColor;
@property (nonatomic, assign)	oriVector2		propOffset;


// ******** INSTANCE METHODS ********

// ** initializers **

- (id)		initRectangle;

// ** all types ** //

- (void)	setAlpha:(GLubyte)alpha;

// ** rectangle related ** //

- (void)	rectangleSetSize:(oriIntSize)size;
- (void)	rectangleSetMapping:(oriRect)map;

// ** drawing **

- (void)	render;

@end
