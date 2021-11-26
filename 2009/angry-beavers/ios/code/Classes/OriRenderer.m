//=======================================================================================================================
//  File:		OriRenderer.m
//  Project:	heli
//	Desc:		renderer class implementation
//
//  Created by Max Vasin on 3/22/11.
//  Copyright 2011 Origin. All rights reserved.
//=======================================================================================================================

// ***** IMPORT *****

#import <QuartzCore/QuartzCore.h>
#import "OriRenderer.h"

// ***** PRIVATE METHODS *****

@interface OriRenderer(Private)

- (BOOL)makeContext;
- (BOOL)makeFrameBuffer;
- (void)deleteFrameBuffer;
- (BOOL)checkError;

@end

// ***** IMLEMENTATION *****

@implementation OriRenderer

// ***** PROPERTIES *****

@synthesize propRenderView				= m_renderView;
@synthesize propRenderViewController	= m_renderViewController;
@synthesize propTextureBPP				= m_textureBPP;
@synthesize propOrientation				= m_orientation;

// Back buffer size property
//-----------------------------------------------------------------------------------------------------------------------
- (oriIntSize)propBufferSize
{
	oriIntSize size = {m_bufferWidth, m_bufferHeight};
	
	return size;
}

// Main window size property
//-----------------------------------------------------------------------------------------------------------------------
- (oriIntSize)propWindowSize
{
	oriIntSize size = {m_mainWindowSize.width, m_mainWindowSize.height};
	
	return size;
}

// View rectangle property
//-----------------------------------------------------------------------------------------------------------------------
- (oriRect)propViewRect
{
	oriRect rect;
	
	rect.min.x	= 0;
	rect.min.y	= 0;
	
	rect.max.x	= m_bufferWidth;
	rect.max.y	= m_bufferHeight;
	
	return rect;
}

// initWithWindow: designated renderer initializer
//-----------------------------------------------------------------------------------------------------------------------
- (id)initWithWindow:(UIWindow*)window
{
	if ( (self = [super init]) )
	{
		UIScreen* mainScreen = [UIScreen mainScreen];
		
		if ([mainScreen respondsToSelector:@selector(scale)])
			m_screenScale = [mainScreen scale];
		else
			m_screenScale = 1.0f;
		
		m_mainWindow = window;
		
		CGRect windowBounds = [m_mainWindow bounds];
		
		m_mainWindowSize    = windowBounds.size;
		
		DBGLOG3A(@"OriRenderer: device window logical size (%ix%i) with scale factor (%f)", 
				 (int)m_mainWindowSize.width, 
				 (int)m_mainWindowSize.height, 
				 m_screenScale);
		
		m_orientation = UIInterfaceOrientationPortrait;
		
		m_renderView = [[RenderView alloc] initWithFrame:windowBounds andRenderer:self];
		
		m_renderView.contentScaleFactor = m_screenScale;
		
		
		m_renderViewController	= [[RenderViewController alloc] initWithRenderer:self];
		
		m_textureBPP			= 8;
		
		[m_mainWindow addSubview:m_renderView];
		
		[self makeContext];
		
		[self makeFrameBuffer];
		
		DBGLOG0A(@"OriRenderer: instance allocated and initialized successfully");
	}
	return self;
}

// dealloc: desturctor
//-----------------------------------------------------------------------------------------------------------------------
- (void)dealloc
{	
	[self deleteFrameBuffer];
	
	SAFE_RELEASE(m_oglContext);
	
	[m_renderView removeFromSuperview];
	
	SAFE_RELEASE(m_renderViewController);
	SAFE_RELEASE(m_renderView);
	
	DBGLOG0A(@"OriRenderer: instance deallocated");
	
	[super dealloc];
}

// makeContext: makes new ES context for rendering
//-----------------------------------------------------------------------------------------------------------------------
- (BOOL)makeContext
{
	m_oglContext = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES1];
	
	if (!m_oglContext)
	{
		DBGLOG0A(@"OriRenderer: failed to create ES1 context");
		return NO;
	}
	
	if (![EAGLContext setCurrentContext:m_oglContext])
	{
		DBGLOG0A(@"OriRenderer: failed to set ES1 context current");
		return NO;
	}
	
	DBGLOG0A(@"OriRenderer: OpenGL context created successfully");
	return YES;
}

// makeFrameBuffer: make framebuffer resources
//-----------------------------------------------------------------------------------------------------------------------
- (BOOL)makeFrameBuffer
{
	[EAGLContext setCurrentContext:m_oglContext];
	
	if (m_colorBuffer)
	{
		glDeleteRenderbuffers(1, &m_colorBuffer);
		m_colorBuffer = 0;
	}
	
	if (m_frameBuffer)
	{
		glDeleteFramebuffers(1, &m_frameBuffer);
		m_frameBuffer = 0;
	}
	
	
	if (m_oglContext && !m_frameBuffer)
	{
		[EAGLContext setCurrentContext:m_oglContext];
		
		glGenFramebuffers(1, &m_frameBuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, m_frameBuffer);
		
		glGenRenderbuffers(1, &m_colorBuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, m_colorBuffer);
		
		[m_oglContext renderbufferStorage:GL_RENDERBUFFER fromDrawable:(CAEAGLLayer*)m_renderView.layer];
		
		glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_WIDTH,	&m_bufferWidth);
		glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_HEIGHT,	&m_bufferHeight);
		
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, m_colorBuffer);
		
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			DBGLOG1A(@"OriRenderer: failed to make complete framebuffer object (status: %x)", glCheckFramebufferStatus(GL_FRAMEBUFFER));
			return NO;
		}
		
		glBindFramebuffer(GL_FRAMEBUFFER, m_frameBuffer);
        
        glViewport(0, 0, m_bufferWidth, m_bufferHeight);
		
		[self setScreenProjection];
		
		DBGLOG2A(@"OriRenderer: backbuffer with size (%ix%i) created successfully", m_bufferWidth, m_bufferHeight);
	}
	return YES;
}

// deleteFrameBuffer: delete framebuffer resources
//-----------------------------------------------------------------------------------------------------------------------
- (void)deleteFrameBuffer
{
	if (m_oglContext)
    {
        [EAGLContext setCurrentContext:m_oglContext];
        
        if (m_frameBuffer)
        {
            glDeleteFramebuffers(1, &m_frameBuffer);
            m_frameBuffer = 0;
        }
        
        if (m_colorBuffer)
        {
            glDeleteRenderbuffers(1, &m_colorBuffer);
            m_colorBuffer = 0;
        }
    }
}

// present: presents current content of backbuffer
//-----------------------------------------------------------------------------------------------------------------------
- (void)present
{
	if (m_oglContext)
    {
        [EAGLContext setCurrentContext:m_oglContext];
        
        glBindRenderbuffer(GL_RENDERBUFFER, m_colorBuffer);
		
        [m_oglContext presentRenderbuffer:GL_RENDERBUFFER];
	}
}

// clear: clears backbuffer with specified color
//-----------------------------------------------------------------------------------------------------------------------
- (void)clear
{
	if (m_oglContext)
	{
		[EAGLContext setCurrentContext:m_oglContext];
		
		glBindRenderbuffer(GL_RENDERBUFFER, m_colorBuffer);
		
		glClearColor(0.0f, 1.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
	}
}

// Creates new texture associated with this renderer
//-----------------------------------------------------------------------------------------------------------------------
- (GLuint)createTextureW:(GLuint)width H:(GLuint)height Data:(void*)data Format:(GLenum)format Packing:(GLenum)packing;
{
	[EAGLContext setCurrentContext:m_oglContext];
	
	GLuint	 texture;
	GLubyte* buffer = (GLubyte*)data;
		
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, packing, buffer);
	
	if ([self checkError])
	{
		DBGLOG0A(@"OriRenderer: failed to create new texture");
		glDeleteTextures(1, &texture);
		return 0;
	}
	
	return texture;
}

// Destroys given texture
//-----------------------------------------------------------------------------------------------------------------------
- (void)destroyTexture:(GLuint)texture
{
	if (texture)
		glDeleteTextures(1, &texture);
}

// Sets projection matrix to screen-size
//-----------------------------------------------------------------------------------------------------------------------
- (void)setScreenProjection
{
	[EAGLContext setCurrentContext:m_oglContext];
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
    glOrthof(0.0f, m_bufferWidth, 0.0f, m_bufferHeight, -1000.0f, 1000.0f);
	
	glMatrixMode(GL_MODELVIEW);
}

// Recreates backbuffer when view rotates to new orientation
//-----------------------------------------------------------------------------------------------------------------------
- (void)rotateTo:(UIInterfaceOrientation)orientation
{
	m_orientation = orientation;
	
	[self makeFrameBuffer];
}

// Returns touch point projected to renderer coordinate system
//-----------------------------------------------------------------------------------------------------------------------
- (CGPoint)projectTouch:(CGPoint)point
{
	switch (m_orientation) 
	{
		case UIInterfaceOrientationPortrait:
		case UIInterfaceOrientationPortraitUpsideDown:
			
			point.y = m_mainWindowSize.height - point.y;
			
			break;
			
		case UIInterfaceOrientationLandscapeLeft:
		case UIInterfaceOrientationLandscapeRight:
			
			point.y = m_mainWindowSize.width - point.y;
			
			break;
	}
	
	point.x *= m_screenScale;
	point.y *= m_screenScale;
	
	return point;
}


// Check current ES state and generates error info (in debug version)
//-----------------------------------------------------------------------------------------------------------------------
- (BOOL)checkError
{
	GLenum err = glGetError();
	
	if (err == GL_NO_ERROR)
		return NO;

#ifdef DEBUG

	switch (err)
	{
		case GL_INVALID_ENUM:
			DBGLOG0A(@"OriRenderer: ES error - An unacceptable value is specified for an enumerated argument. The offending command is ignored, and has no other side effect than to set the error flag.");
			break;
		case GL_INVALID_VALUE:
			DBGLOG0A(@"OriRenderer: ES error - A numeric argument is out of range. The offending command is ignored, and has no other side effect than to set the error flag.");
			break;
		case GL_INVALID_OPERATION:
			DBGLOG0A(@"OriRenderer: ES error - The specified operation is not allowed in the current state. The offending command is ignored, and has no other side effect than to set the error flag.");
			break;
		case GL_STACK_OVERFLOW:
			DBGLOG0A(@"OriRenderer: ES error - This command would cause a stack overflow. The offending command is ignored, and has no other side effect than to set the error flag.");
			break;
		case GL_STACK_UNDERFLOW:
			DBGLOG0A(@"OriRenderer: ES error - This command would cause a stack underflow. The offending command is ignored, and has no other side effect than to set the error flag.");
			break;
		case GL_OUT_OF_MEMORY:
			DBGLOG0A(@"OriRenderer: ES error - There is not enough memory left to execute the command. The state of the GL is undefined, except for the state of the error flags, after this error is recorded.");
			break;
	}

#endif

	return YES;
}

@end
