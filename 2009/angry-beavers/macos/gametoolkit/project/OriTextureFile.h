//===============================================================================================
//  File:		OriTextureFile.h
//  Project:	OriginGameToolkit
//	Desc:		Texture internal format file utility class
//
//  Created by Max Vasin on 6/10/11.
//  Copyright 2011 Origin Games. All rights reserved.
//===============================================================================================

// ****** IMPORTS ******

#import <Cocoa/Cocoa.h>
#import "Base.h"
#import "BaseObject.h"
#import "ProjectTextureItem.h"


// ****** INTERFACE ******

@interface OriTextureFile : NSObject 
{
	NSString*				m_path;
	GLuint					m_texture;
	oriTextureFileHeader	m_header;
}

@property (nonatomic, readonly)		GLuint texture;
@property (nonatomic, readonly)		unsigned int width;
@property (nonatomic, readonly)		unsigned int height;
@property (nonatomic, readonly)		IntSize size;

// ****** INSTANCE METHODS ******

- (id)		initWithPath:(NSString*)path;


@end
