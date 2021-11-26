//=======================================================================================================================
//  File:		OriSprite.m
//  Project:	heli
//	Desc:		sprite resource implementation
//
//  Created by Max Vasin on 3/21/11.
//  Copyright 2011 Origin. All rights reserved.
//=======================================================================================================================

// ***** IMPORTS *****

#import "OriSprite.h"
#import "OriResourceManager.h"

// ***** IMPLEMENTATION *****

@implementation OriSprite

// ****** PROPERTIES ******

//-----------------------------------------------------------------------------------------------------------------------
// SYNTHESIZED
//-----------------------------------------------------------------------------------------------------------------------
@synthesize propTexture		= m_texture;

//-----------------------------------------------------------------------------------------------------------------------
// PROPERTY: propAnimationCount
//-----------------------------------------------------------------------------------------------------------------------

- (oriUint)propAnimationCount
{
	return [m_animations count];
}

//-----------------------------------------------------------------------------------------------------------------------
// PROPERTY: system memory used by this resource
//-----------------------------------------------------------------------------------------------------------------------

- (oriUint)propSysMemUsed
{
	oriUint memUsed = 0;
	
	for (OriSpriteAnimation* anim in m_animations)
		memUsed += anim.propSysMemUsed;
	
	return memUsed + malloc_size(self) + malloc_size(m_animations) + malloc_size(m_name) + (m_name.length * sizeof(oriUniChar));
}

//-----------------------------------------------------------------------------------------------------------------------
// PROPERTY: video memory used by this resource
//-----------------------------------------------------------------------------------------------------------------------

- (oriUint)propVideoMemUsed
{
	return 0;
}

// ****** CLASS METHODS ******

//#######################################################################################################################
// CONSTRUCTION
//#######################################################################################################################

//-----------------------------------------------------------------------------------------------------------------------
// Returns new autoreleased instance of sprite with given resource manager
//-----------------------------------------------------------------------------------------------------------------------

+ (id)spriteWithManager:(OriResourceManager*)resman withName:(NSString*)name
{
	return [[[self alloc] initWithManager:resman withName:name] autorelease];
}

//-----------------------------------------------------------------------------------------------------------------------
// Returns new autoreleased instance of sprite with given resource manager and XML data
//-----------------------------------------------------------------------------------------------------------------------

+ (id)spriteWithManager:(OriResourceManager*)resman withName:(NSString*)name withXML:(OriXMLNode*)node
{
	return [[[self alloc] initWithManager:resman withName:name withXML:node] autorelease];
}

// ****** INSTANCE METHODS ******

//#######################################################################################################################
// INITIALIZERS/DESTRUCTORS
//#######################################################################################################################


//-----------------------------------------------------------------------------------------------------------------------
// Initializes instance of sprite with given resource manager
//-----------------------------------------------------------------------------------------------------------------------

- (id)initWithManager:(OriResourceManager*)resman withName:(NSString*)name
{
	if ( (self = [super initWithManager:resman withName:name]) )
	{
		m_animations	= [[NSMutableArray alloc] init];
	
		DBG_INIT_LOG_NAMED_0(self, m_name);
	}
	return self;
}


//-----------------------------------------------------------------------------------------------------------------------
// Initializes instance of sprite with given resource manager and XML data
//-----------------------------------------------------------------------------------------------------------------------

- (id)initWithManager:(OriResourceManager*)resman withName:(NSString*)name withXML:(OriXMLNode*)node
{
	if ( (self = [self initWithManager:resman withName:name]) )
	{
		if (node)
		{
			NSString* textureName = [node getFirstChildValue:@"Texture" placeholder:nil];
			
			if (textureName)
			{
				m_texture = [m_resMan getTexture:textureName];
				if (!m_texture)
					DBGLOG2A(@"OriSprite: sprite (%@) failed to get texture (%@)", m_name, textureName);
			}
			else
			{
				DBGLOG1A(@"OriSprite: sprite (%@) no texture name in XML file", m_name);
			}
							
			OriXMLNode* animRoot = [node getFirstChild:@"Animations"];
			
			if (animRoot)
			{
				for (OriXMLNode* animNode in animRoot.propChildren)
					[m_animations addObject:[OriSpriteAnimation animationWithSprite:self withXML:animNode]];
			}
			
			for (OriSpriteAnimation* anim in m_animations)
				[anim updateTextureDepencies];
		}
	}
	return self;
}

// Desctructor
//-----------------------------------------------------------------------------------------------------------------------
- (void)dealloc
{
	SAFE_RELEASE(m_texture);
	SAFE_RELEASE(m_animations);
	
	DBG_DEALLOC_LOG_NAMED_0(self, m_name);

	[super dealloc];
}

//#######################################################################################################################
// ANIMATIONS COLLECTION ACCESS
//#######################################################################################################################

// Returns animation by name
//-----------------------------------------------------------------------------------------------------------------------
- (OriSpriteAnimation*)getAnimation:(NSString*)name
{
	for (OriSpriteAnimation* anim in m_animations)
		if ([anim->m_name isEqualToString:name])
			return anim;
	
	return nil;
}

// Returns animation by index
//-----------------------------------------------------------------------------------------------------------------------
- (OriSpriteAnimation*)getAnimationByIndex:(oriUint)index
{
	if (index >= self.propAnimationCount)
		return nil;
	
	return [m_animations objectAtIndex:index];
}

@end
