//=======================================================================================================================
//  File:		OriRenderer.h
//  Project:	heli
//	Desc:		OpenGL renderer abstraction class
//	
//  Created by Max Vasin on 3/22/11.
//  Copyright 2011 Origin. All rights reserved.
//=======================================================================================================================

// ***** IMPORTS *****

#import <UIKit/UIKit.h>

#import "OriBase.h"
#import "RenderView.h"
#import "RenderViewController.h"

// ***** INTERFACE *****

@interface OriRenderer : NSObject 
{
@private
	
	UIWindow*				m_mainWindow;
	CGSize					m_mainWindowSize;
	EAGLContext*			m_oglContext;
	RenderView*				m_renderView;
	RenderViewController*	m_renderViewController;
	GLuint					m_frameBuffer;
	GLuint					m_colorBuffer;
	GLint					m_bufferWidth;
    GLint					m_bufferHeight;
	float					m_screenScale;
	oriUint					m_textureBPP;
	UIInterfaceOrientation	m_orientation;
}

// ***** PROPERTIES *****

@property (nonatomic, retain)	IBOutlet RenderView*			propRenderView;
@property (nonatomic, retain)	IBOutlet RenderViewController*	propRenderViewController;
@property (nonatomic, readonly)	oriUint							propTextureBPP;
@property (nonatomic, readonly)	oriIntSize						propBufferSize;
@property (nonatomic, readonly) oriIntSize						propWindowSize;
@property (nonatomic, readonly) UIInterfaceOrientation			propOrientation;
@property (nonatomic, readonly)	oriRect							propViewRect;


// ***** INSTANCE METHODS *****

- (id)		initWithWindow:(UIWindow*)window;
- (void)	present;
- (void)	clear;
- (GLuint)	createTextureW:(GLuint)width H:(GLuint)height Data:(void*)data Format:(GLenum)format Packing:(GLenum)packing;
- (void)	destroyTexture:(GLuint)texture;

- (void)	setScreenProjection;
- (void)	rotateTo:(UIInterfaceOrientation)orientation;
- (CGPoint)	projectTouch:(CGPoint)point;

@end
