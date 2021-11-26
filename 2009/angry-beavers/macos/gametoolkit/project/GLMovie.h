//===============================================================================================
//  File:		GLMovie.h
//  Project:	OriginGameToolkit
//	Desc:		OpenGL movie utility object
//
//  Created by Max Vasin on 5/16/11.
//  Copyright 2011 Origin Games. All rights reserved.
//===============================================================================================

// ****** IMPORTS ******

#import <Cocoa/Cocoa.h>
//#import <OpenGL/OpenGL.h>
//#import <OpenGL/CGLMacro.h>
//#import <QuartzCore/QuartzCore.h>
#import <QuickTime/QuickTime.h>
#import <QTKit/QTKit.h>
#import "Base.h"
#import "BaseObject.h"

// ***** INTERFACE *****

@interface GLMovie : BaseObject 
{
	// OpenGL-specific
 
	NSOpenGLPixelFormat*	m_glPixelFormat;			// Cocoa-based OpenGL pixel format and context
	NSOpenGLContext*		m_glContext;
	CGLPixelFormatObj		m_cglPixelFormat;			// Handy references to the underlying CGL objects
	CGLContextObj			m_cglContext;
	NSSize					m_movieSize;
	GLenum					m_glTextureTarget;			// The currently-enabled OpenGL texture target
	OGLVertex				m_glVertices[4];			// Vertices
	
	// QuickTime/CoreVideo-specific
    
	QTVisualContextRef		m_textureContext;			// Visual context for OpenGL textures
	QTMovie*				m_movie;
	
	
//	CGColorSpaceRef      m_workingColorSpace;		// Working color space for both QuickTime and CoreImage
	CVOpenGLTextureRef   m_texture;					// Most recent frame of video from visual context
//	CVDisplayLinkRef     m_displayLink;				// Display link for managing rendering thread

}

- (id)		initWithContext:(NSOpenGLContext*)context	pixelFormat:(NSOpenGLPixelFormat*)format fromSource:(NSString*)source;
- (void)	render;
- (void)	nextFrame;
- (void)	prevFrame;

@end
