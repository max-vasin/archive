//=======================================================================================================================
//  File:		OriMultiSpriteKey.m
//  Project:	heli
//	Desc:		multisprite record key implementation
//
//  Created by Max Vasin on 4/15/11.
//  Copyright 2011 Origin. All rights reserved.
//=======================================================================================================================

// ***** IMPORTS *****

#import "OriMultiSpriteKey.h"
#import "OriMultiSprite.h"
#import "OriMultiSpriteRecord.h"
#import "OriSprite.h"

// ***** IMPLEMENTATION *****

@implementation OriMultiSpriteKey

// ****** PROPERTIES ******

// Synthesized
//-----------------------------------------------------------------------------------------------------------------------
@synthesize propRecord		= m_record;
@synthesize propSprite		= m_sprite;
@synthesize propAnimation	= m_animation;
@synthesize propFrame		= m_frame;
@synthesize propLayer		= m_layer;
@synthesize propLink		= m_link;
@synthesize propBindName	= m_bindName;
@synthesize propBindPoint	= m_bindPoint;

// ****** CLASS METHODS ******

//#######################################################################################################################
// CONSTRUCTION
//#######################################################################################################################

// Returns new autoreleased key with record
//-----------------------------------------------------------------------------------------------------------------------
+ (id)keyWithRecord:(OriMultiSpriteRecord*)record
{
	return [[[self alloc] initWithRecord:record] autorelease];
}

// Returns new autoreleased key with record and XML data
//-----------------------------------------------------------------------------------------------------------------------
+ (id)keyWithRecord:(OriMultiSpriteRecord*)record withXML:(OriXMLNode*)node
{
	return [[[self alloc] initWithRecord:record withXML:node] autorelease];
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
		//DBGLOG0A(@"OriMultiSpriteKey: instance allocated and initialized successfully");
	}
	return self;
}

// Initializes instance of multisprite key with record
//-----------------------------------------------------------------------------------------------------------------------
- (id)initWithRecord:(OriMultiSpriteRecord*)record
{
	if ( (self = [self init]) )
	{
		m_record = record;
	}
	return self;
}

// Initializes instance of multisprite key with record and XML data
//-----------------------------------------------------------------------------------------------------------------------
- (id)initWithRecord:(OriMultiSpriteRecord*)record withXML:(OriXMLNode*)node
{
	if ( (self = [self initWithRecord:record]) )
	{
		if (node)
		{
			// Linked sprite
			
			m_link = [node getFirstChildValueInt:@"Sprite" placeholder:NSNotFound];
			
			if (m_link == NSNotFound)
			{
				DBGLOG1A(@"OriMultiSpriteKey: (%@) sprite index not found in XML", m_record->m_multisprite.propName);
			}
			else
			{
				m_sprite = [m_record getWeakSpriteLink:m_link];
				
				if (!m_sprite)
					DBGLOG2A(@"OriMultiSpriteKey: (%@) cant get linked sprite at index (%i)", m_record->m_multisprite.propName, m_link);
			}
			
			// Animation
			
			if (m_sprite)
			{
				NSString* animationName = [node getFirstChildValue:@"Animation" placeholder:nil];
				
				if (!animationName)
				{
					DBGLOG1A(@"OriMultiSpriteKey: (%@) sprite animation name not found in XML", m_record->m_multisprite.propName);
				}
				else
				{
					m_animation = [m_sprite getAnimation:animationName];
					
					if (!m_animation)
						DBGLOG2A(@"OriMultiSpriteKey: (%@) cant get sprite animation (%@)", m_record->m_multisprite.propName, animationName);
				}
			}
			
			// Key values
			
			m_frame				= [node getFirstChildValueInt:@"Frame"		placeholder:0];
			m_layer				= [node getFirstChildValueFloat:@"Layer"	placeholder:0.0f];
			self.propBindName	= [node getFirstChildValue:@"Point"			placeholder:@""];
			
			OriSpriteAnimationPoint* bindPoint = [m_record getBasePoint:m_bindName];
			
			if (bindPoint)
				m_bindPoint = bindPoint.propPosition;
		}
	}
	return self;
}

// Destructor
//-----------------------------------------------------------------------------------------------------------------------
- (void)dealloc
{
	SAFE_RELEASE(m_bindName);
	
	//DBGLOG0A(@"OriMultiSpriteKey: instance deallocated");
	
	[super dealloc];
}

@end
