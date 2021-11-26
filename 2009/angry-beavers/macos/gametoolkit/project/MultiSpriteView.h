//===============================================================================================
//  File:		MultiSpriteView.h
//  Project:	OriginGameToolkit
//	Desc:		Multi sprite OpenGL view
//
//  Created by Max Vasin on 3/30/11.
//  Copyright 2011 Origin Games. All rights reserved.
//===============================================================================================

// ************* IMPORTS *************

#import <Cocoa/Cocoa.h>
#import "BaseObject.h"
#import "ProjectTextureItem.h"
#import "ProjectSpriteItem.h"
#import "ProjectMultiSpriteItem.h"
#import "GuidlineGrid.h"
#import "TextureStore.h"
#import "GLMultiSprite.h"

//###############################################################################################
// TEXTURE ENTRY HELPER CONTAINER
//###############################################################################################

@interface msTextureEntry : NSObject
{
@public
	ProjectTextureItem*		m_item;
	GLuint					m_texture;
	IntSize					m_size;
}

// ****** PROPERTIES ******

@property (nonatomic, assign)	ProjectTextureItem* propItem;
@property (nonatomic, assign)	GLuint				propTexture;
@property (nonatomic, assign)	IntSize				propSize;

// ****** CLASS METHODS ******

+ (id)	entryWithItem:(ProjectTextureItem*)item;

// ****** INSTANCE METHODS ******

- (id)	initWithItem:(ProjectTextureItem*)item;


@end

//###############################################################################################
// SPRITE ENTRY HELPER CONTAINER
//###############################################################################################

@interface msSpriteEntry : NSObject
{
@public
	ProjectSpriteItem*		m_item;
	msTextureEntry*			m_texEntry;
	OGLVertex				m_verts[4];
	GLfloat					m_layer;
	CGSize					m_size;
	Vec2					m_origin;
	BOOL					m_valid;
}

// ****** PROPERTIES ******

@property (nonatomic, assign)	ProjectSpriteItem*		propItem;
@property (nonatomic, assign)	msTextureEntry*			propTexture;
@property (nonatomic, assign)	GLfloat					propLayer;
@property (nonatomic, assign)	CGSize					propSize;
@property (nonatomic, assign)	Vec2					propOrigin;
@property (nonatomic, assign)	BOOL					propValid;

// ****** CLASS METHODS ******

+ (id)entryWithItem:(ProjectSpriteItem*)item andTexture:(msTextureEntry*)entry;


// ****** INSTANCE METHODS ******

- (id)initWithItem:(ProjectSpriteItem*)item  andTexture:(msTextureEntry*)entry;

- (NSInteger)layerSort:(msSpriteEntry*)entry;

@end


//###############################################################################################
// MULTISPRITE VIEW
//###############################################################################################

@interface MultiSpriteView : NSOpenGLView 
{
	// ** iternal data **
	
	BOOL						m_contextReady;
	TextureStore*				m_texStore;

	
	// ** OpenGL data **
	GLMultiSprite*				m_glMultiSprite;
	GuidlineGrid*				m_backGrid;
	
	
	// ** current observing objects **
	
	Project*					m_obsrvProject;
	ProjectMultiSpriteItem*		m_obsrvMultiSprite;
	MultiSpriteRecord*			m_obsrvRecord;
	MultiSpriteRecordKey*		m_obsrvKey;
}


@end
