//=======================================================================================================================
//  File:		EAGLView.m
//  Project:	heli
//	Desc:		EAGL surface wrapper interface
//
//  Created by Max Vasin on 3/21/11.
//  Copyright 2011 Origin Games. All rights reserved.
//=======================================================================================================================

// ***** IMPORTS *****

#import <UIKit/UIKit.h>
#import <OpenGLES/EAGL.h>
#import <OpenGLES/ES1/gl.h>
#import <OpenGLES/ES1/glext.h>
#import <OpenGLES/ES2/gl.h>
#import <OpenGLES/ES2/glext.h>

#import "OriBase.h"


// ***** INTERFACE *****

@class OriRenderer;

@interface RenderView : UIView
{
	OriRenderer*	m_renderer;
}

// ***** PROPERTIES *****


// ***** METHODS *****

- (id) initWithFrame:(CGRect)frame andRenderer:(OriRenderer*)renderer;

@end
