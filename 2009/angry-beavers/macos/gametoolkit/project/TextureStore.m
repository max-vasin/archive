//===============================================================================================
//  File:		TextureStore.m
//  Project:	OriginGameToolkit
//	Desc:		Texture store class implementation
//
//  Created by Max Vasin on 5/5/11.
//  Copyright 2011 Origin Games. All rights reserved.
//===============================================================================================

// ***** IMPORTS *****

#import "TextureStore.h"
#import "ProjectTextureItem.h"

//###############################################################################################
// STORE ENTRY PRIVATE CLASS INTERFACE
//###############################################################################################

@interface tsEntry : BaseObject
{
@public
	TextureStore*			m_store;
	GLuint					m_texture;
	ProjectTextureItem*		m_item;
	IntSize					m_size;
}

// Properties
//-----------------------------------------------------------------------------------------------
@property (nonatomic, assign)	ProjectTextureItem*		propItem;
@property (nonatomic, assign)	GLuint					propID;

// Class methods
//-----------------------------------------------------------------------------------------------
+ (id)entryWithStore:(TextureStore*)store;

// Instance methods
//-----------------------------------------------------------------------------------------------
- (id)initWithStore:(TextureStore*)store;

@end

//###############################################################################################
// STORE ENTRY PRIVATE CLASS IMPLEMENTATION
//###############################################################################################

@implementation tsEntry

// Sythesized properties
//-----------------------------------------------------------------------------------------------
@synthesize propItem	= m_item;
@synthesize propID		= m_texture;

// Creates autoreleased instance of entry
//-----------------------------------------------------------------------------------------------
+ (id)entryWithStore:(TextureStore*)store
{
	return [[[self alloc] initWithStore:store] autorelease];
}

// Initializes store entry
//-----------------------------------------------------------------------------------------------
- (id)initWithStore:(TextureStore*)store
{
	if ( (self = [self init]) )
	{
		m_store = store;
	}
	return self;
}

// Destructor
//-----------------------------------------------------------------------------------------------
- (void)dealloc
{
	[m_store.propContext makeCurrentContext];
	
	glDeleteTextures(1, &m_texture);
	
	[super dealloc];
}

@end

//###############################################################################################
// PRIVATE METHODS OF TEXTURE STORE
//###############################################################################################

@interface TextureStore (Private)

- (tsEntry*)	getEntry:		(ProjectTextureItem*)item;
- (void)		updateTexture:	(GLuint*)pTexID  toSize:(IntSize*)pSize withPath:(NSString*)path;

@end

//###############################################################################################
// TEXTURE STORE IMPLEMENTATION
//###############################################################################################

@implementation TextureStore

// Synthesized properties
//-----------------------------------------------------------------------------------------------
@synthesize propContext	= m_context;

// Designated initializer with OpenGL context
//-----------------------------------------------------------------------------------------------
- (id)initWithContext:(NSOpenGLContext*)context
{
	if ( (self = [self init]) )
	{
		m_context = [context retain];
		m_store   = [[NSMutableArray alloc] initWithCapacity:10];
	}
	return self;
}

// Destructor
//-----------------------------------------------------------------------------------------------
- (void)dealloc
{
	SAFE_RELEASE(m_store);
	SAFE_RELEASE(m_context);
	
	[super dealloc];
}

// Returns texture by project item (load new or already loaded)
//-----------------------------------------------------------------------------------------------
- (GLuint)getByItem:(ProjectTextureItem*)item
{
	tsEntry* entry = [self getEntry:item];
	
	if (!entry)
		return 0;
	
	return entry->m_texture;
}

// Returns texture size by project item
//-----------------------------------------------------------------------------------------------
- (IntSize)sizeByItem:(ProjectTextureItem*)item
{
	tsEntry* entry = [self getEntry:item];
	
	
	if (!entry)
	{
		IntSize zeroSize = {0, 0};
		return zeroSize;	
	}
	
	return entry->m_size;
}

// Deletes texture by project item
//-----------------------------------------------------------------------------------------------
- (void)delByItem:(ProjectTextureItem*)item
{
	tsEntry* entry = [m_store getObjectByProp:@"propItem" Value:item];
	
	if (!entry)
		return;
	
	[m_store removeObject:entry];
}

// Deletes texture by OpenGL identifier
//-----------------------------------------------------------------------------------------------
- (void)delById:(GLuint)texture
{
	tsEntry* entry;
	
	BOOL bOk = NO;
	
	for (tsEntry* entry in m_store)
	{
		if (entry->m_texture == texture)
		{
			bOk = YES;
			break;
		}
	}
	
	if (!bOk)
		return;
	
	[m_store removeObject:entry];
}

// Removes all stored textures
//-----------------------------------------------------------------------------------------------
- (void)clear
{
	[m_store removeAllObjects];
}


// Private: returns entry by item
//-----------------------------------------------------------------------------------------------
- (tsEntry*)getEntry:(ProjectTextureItem*)item
{
	if (!item)
		return nil;
	
	tsEntry* entry = [m_store getObjectByProp:@"propItem" Value:item];
	
	if (entry)
		return entry;
	
	entry = [tsEntry entryWithStore:self];
	
	entry->m_item = item;
	
	[self updateTexture:&entry->m_texture toSize:&entry->m_size withPath:item.propBuildPath];
	
	[m_store addObject:entry];
	
	return entry;
}

// Updates OpenGL texture from image source
//-----------------------------------------------------------------------------------------------
- (void)updateTexture:(GLuint*)pTexID toSize:(IntSize*)pSize withPath:(NSString*)path
{
	[m_context makeCurrentContext];
	
	glDeleteTextures(1, pTexID);
	*pTexID = 0;
	
	NSData* fileContents = [NSData dataWithContentsOfFile:path];
	
	if (!fileContents)
		return;
	
	NSBitmapImageRep* img = [[[NSBitmapImageRep alloc] initWithDataAndConvert:fileContents] autorelease];
	if(!img)
		return;
	
	glGenTextures(1, pTexID);
	
	[img uploadToTexture:*pTexID];
	
	*pSize = [img intSize];
}


@end
