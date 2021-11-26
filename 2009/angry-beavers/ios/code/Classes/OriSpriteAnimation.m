//=======================================================================================================================
//  File:		OriSpriteAnimation.m
//  Project:	heli
//	Desc:		sprite animation class implementation
//
//  Created by Max Vasin on 3/23/11.
//  Copyright 2011 Origin. All rights reserved.
//=======================================================================================================================

// ***** IMPORTS *****

#import "OriSpriteAnimation.h"
#import "OriSprite.h"

// ***** IMPLEMENTATION *****

@implementation OriSpriteAnimation

// ****** PROPERTIES ******

@synthesize propName	= m_name;
@synthesize propOrigin	= m_origin;
@synthesize propSpeed	= m_speed;

// propFrameCount
//-----------------------------------------------------------------------------------------------------------------------
- (oriUint)propFrameCount
{
	return [m_frames count];
}

// System memory used by this resource
//-----------------------------------------------------------------------------------------------------------------------
- (oriUint)propSysMemUsed
{
	oriUint memUsed = 0;
	
	for (OriSpriteAnimationFrame* frame in m_frames)
		memUsed += frame.propSysMemUsed;
	
	return memUsed + malloc_size(self) + malloc_size(m_frames) + malloc_size(m_name) + (m_name.length * sizeof(oriUniChar));
}

// Video memory used by this resource
//-----------------------------------------------------------------------------------------------------------------------
- (oriUint)propVideoMemUsed
{
	return 0;
}


// ****** CLASS METHODS ******

//#######################################################################################################################
// CONSTRUCTION
//#######################################################################################################################

// Returns new autoreleased sprite animation instance with for given sprite
//-----------------------------------------------------------------------------------------------------------------------
+ (id)animationWithSprite:(OriSprite*)sprite
{
	return [[[self alloc] initWithSprite:sprite] autorelease];
}

// Returns new autoreleased sprite animation instance with for given sprite and initializes it from XML data
//-----------------------------------------------------------------------------------------------------------------------
+ (id)animationWithSprite:(OriSprite*)sprite withXML:(OriXMLNode*)node
{
	return [[[self alloc] initWithSprite:sprite withXML:node] autorelease];
}

// ****** INSTANCE METHODS ******

//#######################################################################################################################
// INITIALIZERS/DESTRUCTORS
//#######################################################################################################################

// Defalut initializer
//-----------------------------------------------------------------------------------------------------------------------
- (id)init
{
	if ( (self = [super init]) )
	{
		m_frames = [[NSMutableArray alloc] init];
		
		DBG_INIT_LOG_2(self);
	}
	return self;
}

// Initializes instance of sprite animation with given parent sprite
//-----------------------------------------------------------------------------------------------------------------------
- (id)initWithSprite:(OriSprite*)sprite
{
	if ( (self = [self init]) )
	{
		m_sprite = sprite;
	}
	return self;
}

// Initializes instance of sprite animation with given parent sprite and XML data
//-----------------------------------------------------------------------------------------------------------------------
- (id)initWithSprite:(OriSprite*)sprite withXML:(OriXMLNode*)node
{
	if ( (self = [self initWithSprite:sprite]) )
	{
		if (node)
		{
			m_name		= [[node getFirstChildValue:@"Name"				placeholder:@""] copy];
			m_speed		= [node getFirstChildValueInt:@"AnimationSpeed" placeholder:1];
			m_origin	= [[node getFirstChildValue:@"Origin"			placeholder:@""] copy];
			
			OriXMLNode* framesRoot = [node getFirstChild:@"Frames"];
			
			if (framesRoot)
			{
				for (OriXMLNode* frameNode in framesRoot.propChildren)
					[m_frames addObject:[OriSpriteAnimationFrame frameWithAnimation:self withXML:frameNode]];
			}
		
		}
	}
	return self;
}

// Destructor
//-----------------------------------------------------------------------------------------------------------------------
- (void)dealloc
{
	SAFE_RELEASE(m_frames);
	SAFE_RELEASE(m_name);
	SAFE_RELEASE(m_origin);
	
	DBG_DEALLOC_LOG_2(self);

	[super dealloc];
}

//#######################################################################################################################
// FRAME CONTROL
//#######################################################################################################################

// Returns frame by index
//-----------------------------------------------------------------------------------------------------------------------
- (OriSpriteAnimationFrame*)getFrame:(oriUint)index
{
	if (index >= self.propFrameCount)
		return nil;
	
	return [m_frames objectAtIndex:index];
}

// Returns frame index
//-----------------------------------------------------------------------------------------------------------------------
- (oriUint)getFrameIndex:(OriSpriteAnimationFrame*)frame
{
	return [m_frames indexOfObject:frame];
}

// Returns point by name and frame index
//-----------------------------------------------------------------------------------------------------------------------
- (OriSpriteAnimationPoint*)getPoint:(NSString*)name atFrame:(oriUint)frame
{
	if ( (frame >= [m_frames count]) || !name)
		return nil;
	
	return [[m_frames objectAtIndex:frame] getPointByName:name];
}


//#######################################################################################################################
// ANIMATION DYNAMIC CONTROL
//#######################################################################################################################

// Builds animation resources
//-----------------------------------------------------------------------------------------------------------------------
- (void)updateTextureDepencies
{
	for (OriSpriteAnimationFrame* frame in m_frames)
		[frame updateTextureDepencies:m_sprite.propTexture];
}


@end
