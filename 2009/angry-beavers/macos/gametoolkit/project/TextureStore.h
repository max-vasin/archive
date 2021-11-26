//===============================================================================================
//  File:		TextureStore.h
//  Project:	OriginGameToolkit
//	Desc:		Texture store class
//
//  Created by Max Vasin on 5/5/11.
//  Copyright 2011 Origin Games. All rights reserved.
//===============================================================================================

// ***** IMPORTS *****

#import <Cocoa/Cocoa.h>
#import "Base.h"
#import "BaseObject.h"

// ***** INTERFACE *****

@class ProjectTextureItem;

@interface TextureStore : BaseObject
{
	NSOpenGLContext*	m_context;
	NSMutableArray*		m_store;
}

// ***** PROPERTIES *****

@property (nonatomic, readonly)	NSOpenGLContext* propContext;

// ***** INSTANCE METHODS *****

- (id)		initWithContext:	(NSOpenGLContext*)context;
- (GLuint)	getByItem:			(ProjectTextureItem*)item;
- (IntSize)	sizeByItem:			(ProjectTextureItem*)item;
- (void)	delByItem:			(ProjectTextureItem*)item;
- (void)	delById:			(GLuint)texture;
- (void)	clear;

@end
