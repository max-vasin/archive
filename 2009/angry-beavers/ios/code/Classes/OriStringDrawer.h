//=======================================================================================================================
//  File:		OriStringDrawer.h
//  Project:	heli
//	Desc:		String drawer utility class interface
//
//  Created by Max Vasin on 6/9/11.
//  Copyright 2011 Origin. All rights reserved.
//=======================================================================================================================

// ******** IMPORTS ********

#import <UIKit/UIKit.h>
#import "OriBase.h"
#import "OriFont.h"

// ******** STRING ALIGNMENT ENUM ********

typedef enum
{
	
	eStringDrawerAlignLeft		= 0,
	eStringDrawerAlignRight		= 1,
	eStringDrawerAlignMiddle	= 2
	
}enStringDrawerAlign;

// ******** INTERFACE ********

#define VERTEX_PER_CHAR			6

@interface OriStringDrawer : NSObject
{
    NSString*				m_string;
	OriFont*				m_font;
	enStringDrawerAlign		m_align;
	oglColor				m_color;
	oriUint					m_vertexCount;
	oriInt					m_spacing;

#if defined (ORIENGINE_STRING_DRAWER_USES_VBO)
	GLuint					m_vbo;
#else
	oglVertex*				m_vertexBuffer;
#endif

}

// ******** PROPERTIES ********

@property (nonatomic, copy)		NSString*				propString;
@property (nonatomic, assign)	enStringDrawerAlign		propAlign;
@property (nonatomic, readonly)	OriFont*				propFont;
@property (nonatomic, assign)	oglColor				propColor;
@property (nonatomic, assign)	oriInt					propSpacing;

// ******** INSTANCE METHODS ********

- (id)		initWithFont:		(OriFont*)font		withCapacity:(oriInt)capacity;
- (id)		initWithFontName:	(NSString*)name		withCapacity:(oriInt)capacity;
- (void)	render;
- (void)	resetBuffers;


@end
