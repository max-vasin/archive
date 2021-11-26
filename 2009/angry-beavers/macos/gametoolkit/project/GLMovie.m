//===============================================================================================
//  File:		GLMovie.m
//  Project:	OriginGameToolkit
//	Desc:		OpenGL movie wrapper implementation
//
//  Created by Max Vasin on 5/16/11.
//  Copyright 2011 Origin Games. All rights reserved.
//===============================================================================================

// ****** IMPORTS ******

#import "GLMovie.h"

@interface GLMovie (Private)

- (void)createWorkingColorSpace;
- (void)update:(CVTimeStamp*)time;

@end

// ****** IMPLEMENTATION ******

@implementation GLMovie

// ****** INSTANCE METHODS ******

//###############################################################################################
// INITIALIZERS\DESTRUCTORS
//###############################################################################################

// Initializes new instance of movie with source string provided
//-----------------------------------------------------------------------------------------------
- (id)initWithContext:(NSOpenGLContext*)context	pixelFormat:(NSOpenGLPixelFormat*)format fromSource:(NSString*)source
{
	if ( (self = [self init]) )
	{
		m_glContext			= [context retain];
		m_glPixelFormat		= [format retain];
		m_cglContext		= (CGLContextObj)[m_glContext CGLContextObj];
		m_cglPixelFormat	= (CGLPixelFormatObj)[m_glPixelFormat CGLPixelFormatObj];
		
		QTOpenGLTextureContextCreate(kCFAllocatorDefault, m_cglContext, m_cglPixelFormat, NULL, &m_textureContext);
		
		if (m_textureContext != NULL)
        {
//			[self createWorkingColorSpace];
			
//			QTVisualContextSetAttribute(m_textureContext, kQTVisualContextWorkingColorSpaceKey, m_workingColorSpace);
			
			NSError* err;
			
			m_movie	= [[QTMovie alloc] initWithFile:source error:&err];
			
			SetMovieVisualContext([m_movie quickTimeMovie], nil);
			SetMovieVisualContext([m_movie quickTimeMovie], m_textureContext);
			
			NSValue* sizeValue = [m_movie attributeForKey:QTMovieNaturalSizeAttribute];
			
			m_movieSize = [sizeValue sizeValue];
			
			m_glVertices[0].pos.x	= 0;					m_glVertices[0].pos.y	= 0;					m_glVertices[0].pos.z = 0;
			m_glVertices[1].pos.x	= m_movieSize.width;	m_glVertices[1].pos.y	= m_movieSize.height;	m_glVertices[1].pos.z = 0;
			m_glVertices[2].pos.x	= 0;					m_glVertices[2].pos.y	= m_movieSize.height;	m_glVertices[2].pos.z = 0;
			m_glVertices[3].pos.x	= m_movieSize.width;	m_glVertices[3].pos.y	= 0;					m_glVertices[3].pos.z = 0;
		
			NSColor* color = [NSColor colorWithCalibratedRed:1.0f green:1.0f blue:1.0f alpha:1.0f];
			
			[color getComponents:(CGFloat*)&m_glVertices[0].color];
			[color getComponents:(CGFloat*)&m_glVertices[1].color];
			[color getComponents:(CGFloat*)&m_glVertices[2].color];
			[color getComponents:(CGFloat*)&m_glVertices[3].color];
		
			[m_movie stop];
			[m_movie gotoBeginning];
		}
		
		
		
		
		
		/*
		
		
		_cglContext = (CGLContextObj)[_glContext CGLContextObj];
        _cglPixelFormat = (CGLPixelFormatObj)[_glPixelFormat CGLPixelFormatObj];
        CVDisplayLinkCreateWithActiveCGDisplays(&_displayLink);
        QTOpenGLTextureContextCreate(kCFAllocatorDefault, _cglContext, _cglPixelFormat, NULL, &_textureContext);
		
        if (_textureContext != NULL && !VIDEO_VIEWER_DISABLE_VISUAL_CONTEXT)
        {
            SInt32 swapInterval = 1;
            
            // QuickTime and CoreImage should use the same working color space
            [self createWorkingColorSpace];
            // Sync with screen refresh to avoid tearing
            [_glContext setValues:&swapInterval forParameter:NSOpenGLCPSwapInterval];
            QTVisualContextSetAttribute(_textureContext, kQTVisualContextWorkingColorSpaceKey, _workingColorSpace);
			
            // Listen for changes to the displays' ColorSync profiles
            CFNotificationCenterAddObserver(CFNotificationCenterGetDistributedCenter(), self,
											&VideoVCViewCMPrefsChangedCallBack, kCMDeviceProfilesNotification, NULL, 
											CFNotificationSuspensionBehaviorDeliverImmediately);
            [self displayProfileChanged:nil];      */     

		
		
		
	}
	return self;
}

- (void)dealloc
{
	[m_movie release];
	[m_glPixelFormat release];
	[m_glContext release];
	
	//CGColorSpaceRelease(m_workingColorSpace);
	
    [super dealloc];
}


- (void)update:(CVTimeStamp*)time
{
	CVOpenGLTextureRef newTexture = nil;
	
	bool newImage = QTVisualContextIsNewImageAvailable(m_textureContext, time);
	
	if (m_textureContext != NULL && newImage) 
	{
		
		QTVisualContextCopyImageForTime(m_textureContext, nil, time, &newTexture); // get the texture for the frame
		
		CVOpenGLTextureRelease(m_texture);
        
		m_texture = newTexture;
	}
	
	// give time to the Visual Context so it can release internally held resources for later re-use
	// this function should be called in every rendering pass, after old images have been released, new
	// images have been used and all rendering has been flushed to the screen.
	
	QTVisualContextTask(m_textureContext);
}

- (void)render
{
	[m_glContext makeCurrentContext];
	
	GLenum textureTarget = CVOpenGLTextureGetTarget(m_texture);
	
	// Make sure the correct texture target is enabled
	
	if (textureTarget != m_glTextureTarget)
	{
		glDisable(m_glTextureTarget);
		
		m_glTextureTarget = textureTarget;
		
		glEnable(m_glTextureTarget);
	}
	
	// Get the current texture's coordinates, bind the texture, and draw our rectangle
	
	CVOpenGLTextureGetCleanTexCoords(m_texture, (GLfloat*)&m_glVertices[0].uv,
												(GLfloat*)&m_glVertices[1].uv,
												(GLfloat*)&m_glVertices[2].uv,
												(GLfloat*)&m_glVertices[3].uv);
	
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
	
	glVertexPointer(3,		GL_FLOAT, sizeof(OGLVertex), &m_glVertices);
	glColorPointer(4,		GL_FLOAT, sizeof(OGLVertex), &m_glVertices[0].color);
	glTexCoordPointer(2,	GL_FLOAT, sizeof(OGLVertex), &m_glVertices[0].uv);
	
	glBindTexture(m_glTextureTarget, CVOpenGLTextureGetName(m_texture));
	
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

- (void)nextFrame
{
	[m_movie stepForward];

	
//	QTVisualContextCopyImageForTime(m_textureContext, nil, time, &newTexture); // get the texture for the frame
	
	CVOpenGLTextureRef newTexture = nil;
	
	CVOpenGLTextureRelease(m_texture);
	
	m_texture = newTexture;
	
	QTVisualContextTask(m_textureContext);

}

- (void)prevFrame
{
	[m_movie stepBackward];

//	QTVisualContextCopyImageForTime(m_textureContext, nil, time, &newTexture); // get the texture for the frame
	
	CVOpenGLTextureRef newTexture = nil;
	
	CVOpenGLTextureRelease(m_texture);
	
	m_texture = newTexture;
	
	QTVisualContextTask(m_textureContext);

	
}



/*
- (void)createWorkingColorSpace
{
    // An RGB color space with linear gamma curves will make a good working space
    CMProfileRef workingProfile = NULL;
    NSNumber* one = [NSNumber numberWithFloat:1.0f];
    NSDictionary* workingProfileSpec = [NSDictionary dictionaryWithObjectsAndKeys:
										@"displayRGB",                 @"profileType",
										one,                           @"gammaR",
										one,                           @"gammaG",
										one,                           @"gammaB",
										@"HDTV",                       @"phosphorSet",
										[NSNumber numberWithInt:6500], @"whiteTemp",
										nil];
	
    CMNewProfile(&workingProfile, NULL);
    CMMakeProfile(workingProfile, (CFDictionaryRef)workingProfileSpec);
    m_workingColorSpace = CGColorSpaceCreateWithPlatformColorSpace(workingProfile);
    CMCloseProfile(workingProfile);
}
*/

@end
