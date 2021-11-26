//===============================================================================================
//  File:		GLMultiSprite.m
//  Project:	OriginGameToolkit
//	Desc:		Multisprite graphics container implementation
//
//  Created by Max Vasin on 5/5/11.
//  Copyright 2011 Origin Games. All rights reserved.
//===============================================================================================

// ***** IMPORTS *****

#import "GLMultiSprite.h"
#import "ProjectMultiSpriteItem.h"

//###############################################################################################
// SPRITE ENTRY HELPER CONTAINER
//###############################################################################################

@interface msSpriteEntry : NSObject
{
@public
	GLMultiSprite*			m_multisprite;
	ProjectSpriteItem*		m_item;
	OGLVertex				m_verts[4];
	GLfloat					m_layer;
	CGSize					m_size;
	Vec2					m_origin;
	BOOL					m_valid;
	GLuint					m_texture;
}

// ****** PROPERTIES ******

@property (nonatomic, assign)	ProjectSpriteItem*		propItem;
@property (nonatomic, assign)	GLfloat					propLayer;
@property (nonatomic, assign)	CGSize					propSize;
@property (nonatomic, assign)	Vec2					propOrigin;
@property (nonatomic, assign)	BOOL					propValid;
@property (nonatomic, readonly)	ProjectTextureItem*		propTexture;

// ****** CLASS METHODS ******

+ (id)entryWithMultiSprite:(GLMultiSprite*)multisprite	SpriteItem:(ProjectSpriteItem*)item;

// ****** INSTANCE METHODS ******

- (id)initWithMultiSprite:(GLMultiSprite*)multisprite	SpriteItem:(ProjectSpriteItem*)item;

- (NSInteger)layerSort:(msSpriteEntry*)entry;

@end

//###############################################################################################
// SPRITE ENTRY HELPER CONTAINER IMPLEMENTATION
//###############################################################################################

@implementation msSpriteEntry

// ****** PROPERTIES ******

// Synthesized
//-----------------------------------------------------------------------------------------------
@synthesize propItem		= m_item;
@synthesize propLayer		= m_layer;
@synthesize propOrigin		= m_origin;
@synthesize propSize		= m_size;
@synthesize propValid		= m_valid;

- (ProjectTextureItem*)propTexture
{
	return m_item.propTexture;
}

// ****** CLASS METHODS ******

// Returns new entry with texture item
//-----------------------------------------------------------------------------------------------
+ (id)entryWithMultiSprite:(GLMultiSprite*)multisprite	SpriteItem:(ProjectSpriteItem*)item;
{
	return [[[self alloc] initWithMultiSprite:multisprite SpriteItem:item] autorelease];
}

// ****** INSTANCE METHODS ******

// Initializes new entry with texture item
//-----------------------------------------------------------------------------------------------
- (id)initWithMultiSprite:(GLMultiSprite*)multisprite	SpriteItem:(ProjectSpriteItem*)item;
{
	if ( (self = [self init]) )
	{
		m_multisprite	= multisprite;
		m_item			= item;
		m_texture		= [m_multisprite.propTextreStore getByItem:m_item.propTexture];
	}
	
	return self;
}

// Sorting selector
//-----------------------------------------------------------------------------------------------
- (NSInteger)layerSort:(msSpriteEntry*)entry
{
	if (m_layer < entry->m_layer)
		return NSOrderedAscending;
	
	if (m_layer == entry->m_layer)
		return NSOrderedSame;
	
	return NSOrderedDescending;
}

@end

//###############################################################################################
// MULTISPRITE CONTAINER PRIVATE
//###############################################################################################

@interface GLMultiSprite (Private)

- (void)updateSprite:(msSpriteEntry*)entry 
		   baseEntry:(msSpriteEntry*)base
			 atFrame:(AnimationFrame*)frame
		   bindPoint:(FramePoint*)point
		 originPoint:(FramePoint*)origin;

- (void)updateTextures;


@end


//###############################################################################################
// MULTISPRITE CONTAINER IMPLEMENTATION
//###############################################################################################

@implementation GLMultiSprite

// Synthesized properties
//-----------------------------------------------------------------------------------------------
@synthesize propContext		= m_context;
@synthesize propTextreStore = m_store;
@synthesize propItem		= m_item;
@synthesize propPosition	= m_position;
@synthesize propVisible		= m_visible;

// Current multisprite record property
//-----------------------------------------------------------------------------------------------
- (void)setPropRecord:(MultiSpriteRecord*)record
{
	if (m_record != record)
	{
		m_record = record;
		[self updateSprites];
	}
}

- (MultiSpriteRecord*)propRecord
{
	return m_record;
}

// Color property
//-----------------------------------------------------------------------------------------------
- (void)setPropColor:(NSColor*)color
{
	if (m_color != color)
	{
		[m_color release];
		
		m_color = [color copy];
		
		[self updateSprites];
	}
}

- (NSColor*)propColor
{
	return m_color;
}


// Designated initializer
//-----------------------------------------------------------------------------------------------
- (id)initWithContext:(NSOpenGLContext*)context Item:(ProjectMultiSpriteItem *)item TextureStore:(TextureStore*)store
{
	if ( (self = [self init]) )
	{
		m_context	= [context retain];
		m_store		= [store retain];
		m_item		= item;
		m_record	= [m_item recordAtIndex:0];
		m_visible	= YES;
		
		m_sprites	= [[NSMutableArray alloc] initWithCapacity:10];
		
		self.propColor = [NSColor colorWithCalibratedRed:1.0f green:1.0f blue:1.0f alpha:1.0f];
		
		[self updateTextures];
		[self updateSprites];
	}
	return self;
}

// Rendres multisprite
//-----------------------------------------------------------------------------------------------
- (void)dealloc
{
	SAFE_RELEASE(m_sprites);
	SAFE_RELEASE(m_store);
	SAFE_RELEASE(m_context);
	
    [super dealloc];
}

// Rendres multisprite
//-----------------------------------------------------------------------------------------------
- (void)render
{
	if (!m_visible)
		return;
	
	[m_context makeCurrentContext];
	
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	
	glTranslatef(m_position.x, m_position.y, 0.0f);
	
	
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
	
	glEnable( GL_TEXTURE_2D );
	glEnable( GL_BLEND );
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
	
	for (msSpriteEntry* entry in m_sprites)
	{
		glVertexPointer(3, GL_FLOAT,	sizeof(OGLVertex), &entry->m_verts[0].pos);
		glColorPointer(4, GL_FLOAT,		sizeof(OGLVertex), &entry->m_verts[0].color);
		glTexCoordPointer(2, GL_FLOAT,	sizeof(OGLVertex), &entry->m_verts[0].uv);
		
		glBindTexture( GL_TEXTURE_2D, [m_store getByItem:entry.propTexture]);
		
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	}
	
	glPopMatrix();
}

//###############################################################################################
// OpenGL RESOURCES BUILDING
//###############################################################################################

// Update sprites
//-----------------------------------------------------------------------------------------------
- (void)updateSprites
{
	[m_sprites removeAllObjects];
	
	if (!m_item)
		return;
	
	// Base sprite
	
	ProjectSpriteItem*	baseSprite		= m_item.propBaseSprite;
	SpriteAnimation*	baseAnimation	= m_item.propBaseAnimation;
	
	if (!baseSprite || !baseAnimation)
		return;
	
	msSpriteEntry* baseEntry = [msSpriteEntry entryWithMultiSprite:self SpriteItem:baseSprite];
	
	baseEntry.propLayer	= m_item.propLayer;
	
	AnimationFrame* baseFrame	= [baseAnimation.propFrames objectAtIndex:m_item.propCurrentFrame];
	
	[self updateSprite:baseEntry baseEntry:nil atFrame:baseFrame bindPoint:nil originPoint:baseFrame.propOriginPoint];
	
	// Linked sprites
	
	for (MultiSpriteRecordKey* key in m_record.propKeys)
	{
		ProjectSpriteItem*	linkSprite		= key.propSprite.propSprite;
		SpriteAnimation*	linkAnimation	= key.propAnimation;
		
		if (!linkSprite || !linkAnimation)
			continue;
		
		msSpriteEntry* linkEntry = [msSpriteEntry entryWithMultiSprite:self SpriteItem:linkSprite];
		
		linkEntry.propLayer	= key.propLayer;
		
		AnimationFrame* linkFrame = [linkAnimation.propFrames objectAtIndex:key.propFrame];
		FramePoint*		bindPoint = [baseFrame getPoint:key.propPoint.propName];
		
		[self updateSprite:linkEntry baseEntry:baseEntry atFrame:linkFrame bindPoint:bindPoint originPoint:linkFrame.propOriginPoint];
	}
	
	[m_sprites sortUsingSelector:@selector(layerSort:)];
}

// Update single sprite entry
//-----------------------------------------------------------------------------------------------
- (void)updateSprite:(msSpriteEntry*)entry 
		   baseEntry:(msSpriteEntry*)base
			 atFrame:(AnimationFrame*)frame
		   bindPoint:(FramePoint*)point
		 originPoint:(FramePoint*)origin;
{
	if (entry.propTexture && frame)
	{
		GLfloat	u0, v0, u1, v1;
		GLfloat l, r, t, b;
		GLfloat pX = 0.0f, pY = 0.0f;
		
		if (point)
		{
			pX = point.propPositionX;
			pY = point.propPositionY;
		}
		
		if (base)
		{
			pX -= base->m_origin.x;
			pY -= base->m_origin.y;
		}
		
		if (origin)
		{
			entry->m_origin.x = origin.propPositionX - pX;
			entry->m_origin.y = origin.propPositionY - pY;
		}
		
		IntSize texSize = [m_store sizeByItem:entry.propTexture];
				
		u0 = (GLfloat)frame.propPositionX   / (GLfloat)texSize.width;
		v0 = (GLfloat)frame.propPositionY   / (GLfloat)texSize.height;
		u1 = u0 + (GLfloat)frame.propWidth  / (GLfloat)texSize.width;
		v1 = v0 + (GLfloat)frame.propHeight / (GLfloat)texSize.height;
		
		if (frame.propMirrorX)
			SWAPFLOAT(u0, u1);
		
		if (frame.propMirrorY)
			SWAPFLOAT(v0, v1);
		
		entry->m_size.width		= frame.propWidth;
		entry->m_size.height	= frame.propHeight;
		
		l = 0.0f - entry->m_origin.x;
		t = 0.0f + entry->m_origin.y;
		r = entry->m_size.width - entry->m_origin.x;
		b = -entry->m_size.height + entry->m_origin.y;
		
		entry->m_verts[0].pos.x = l;	entry->m_verts[0].pos.y = t;		entry->m_verts[0].pos.z = 0.0f;
		entry->m_verts[0].uv.x  = u0;	entry->m_verts[0].uv.y  = v0;
		[m_color getComponents:(CGFloat*)&entry->m_verts[0].color];
		
		
		entry->m_verts[1].pos.x = r;	entry->m_verts[1].pos.y = t;		entry->m_verts[1].pos.z = 0.0f;
		entry->m_verts[1].uv.x  = u1;	entry->m_verts[1].uv.y  = v0;
		[m_color getComponents:(CGFloat*)&entry->m_verts[1].color];
		
		entry->m_verts[2].pos.x = l;	entry->m_verts[2].pos.y = b;		entry->m_verts[2].pos.z = 0.0f;
		entry->m_verts[2].uv.x  = u0;	entry->m_verts[2].uv.y  = v1;
		[m_color getComponents:(CGFloat*)&entry->m_verts[2].color];
		
		entry->m_verts[3].pos.x = r;	entry->m_verts[3].pos.y = b;		entry->m_verts[3].pos.z = 0.0f;
		entry->m_verts[3].uv.x  = u1;	entry->m_verts[3].uv.y  = v1;
		[m_color getComponents:(CGFloat*)&entry->m_verts[3].color];
		
		entry.propValid = YES;
	}
	
	[m_sprites addObject:entry];
}

// Updates textures of current multisprite
//-----------------------------------------------------------------------------------------------
- (void)updateTextures
{
	if (!m_item)
		return;
	
	[m_store getByItem:m_item.propBaseSprite.propTexture];
	
	for (SpriteWrapper* linkSprite in m_item.propLinkedSprites)
		[m_store getByItem:linkSprite.propSprite.propTexture];
}


@end
