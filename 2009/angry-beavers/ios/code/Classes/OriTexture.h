//=======================================================================================================================
//  File:		OriTexture.h
//  Project:	heli
//	Desc:		texture resource interface
//
//  Created by Max Vasin on 3/21/11.
//  Copyright 2011 Origin. All rights reserved.
//=======================================================================================================================

// ******** IMPORTS ********

#import <UIKit/UIKit.h>
#import <OpenGLES/ES1/gl.h>
#import <OpenGLES/ES1/glext.h>
#import <OpenGLES/ES2/gl.h>
#import <OpenGLES/ES2/glext.h>

#import "OriBase.h"
#import "OriResource.h"


// ***** INTERFACE *****

@class OriResourceManager;

@interface OriTexture : OriResource
{
@private
	NSString*			m_source;
	oriIntSize			m_size;
	oriUint				m_memory;
	GLuint				m_texture;
}

// ***** PROPERTIES *****

@property (nonatomic, readonly)	oriIntSize	propSize;
@property (nonatomic, copy)		NSString*	propSource;
@property (nonatomic, readonly)	GLuint		propHandle;

// ***** CLASS METHODS *****

+ (id)		textureWithManager:(OriResourceManager*)resman	withName:(NSString*)name;
+ (id)		textureWithManager:(OriResourceManager*)resman	withName:(NSString*)name	fromFile:(NSString*)file;

// ***** INSTANCE METHODS *****

- (id)		initWithManager:(OriResourceManager*)resman		withName:(NSString*)name;
- (id)		initWithManager:(OriResourceManager*)resman		withName:(NSString*)name	fromFile:(NSString*)file;


@end
