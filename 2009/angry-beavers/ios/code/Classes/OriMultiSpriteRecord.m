//=======================================================================================================================
//  File:		OriMultiSpriteItem.m
//  Project:	heli
//	Desc:		multi sprite item implementation
//
//  Created by Max Vasin on 4/12/11.
//  Copyright 2011 Origin. All rights reserved.
//=======================================================================================================================

// ***** IMPORTS *****

#import "OriMultiSpriteRecord.h"
#import "OriMultiSprite.h"
#import "OriSprite.h"

// ***** IMPLEMENTATION *****

@implementation OriMultiSpriteRecord

// ****** PROPERTIES ******

// Keys number property
//-----------------------------------------------------------------------------------------------------------------------
- (unsigned int)propKeysCount
{
	return [m_keys count];
}

// ****** CLASS METHODS ******

//#######################################################################################################################
// CONSTRUCTION
//#######################################################################################################################

// Returns new autoreleased sprite animation instance with for given sprite
//-----------------------------------------------------------------------------------------------------------------------
+ (id)recordWithMultiSprite:(OriMultiSprite*)multisprite
{
	return [[[self alloc] initWithMultiSprite:multisprite] autorelease];
}

// Returns new autoreleased sprite animation instance with for given sprite and initializes it from XML data
//-----------------------------------------------------------------------------------------------------------------------
+ (id)recordWithMultiSprite:(OriMultiSprite*)multisprite withXML:(OriXMLNode*)node
{
	return [[[self alloc] initWithMultiSprite:multisprite withXML:node] autorelease];
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
		m_keys	= [[NSMutableArray alloc] init];
		
		//DBGLOG0A(@"OriMultiSpriteRecord: instance allocated and initialized successfully");
	}
	return self;
}

// Initializes instance of sprite animation with given parent sprite
//-----------------------------------------------------------------------------------------------------------------------
- (id)initWithMultiSprite:(OriMultiSprite*)multisprite
{
	if ( (self = [self init]) )
	{
		m_multisprite = multisprite;
	}
	return self;
}

// Initializes instance of sprite animation with given parent sprite and XML data
//-----------------------------------------------------------------------------------------------------------------------
- (id)initWithMultiSprite:(OriMultiSprite*)multisprite withXML:(OriXMLNode*)node
{
	if ( (self = [self initWithMultiSprite:multisprite]) )
	{
		if (node)
		{
			OriXMLNode* keysRoot = [node getFirstChild:@"Keys"];
			
			DBG_ASSERT_LOG1A(!keysRoot, @"OriMultiSpriteRecord: (%@) cant find record keys root node in XML", m_multisprite.propName);
			
			if (keysRoot)
			{
				for (OriXMLNode* keyNode in keysRoot.propChildren)
					[m_keys addObject:[OriMultiSpriteKey keyWithRecord:self withXML:keyNode]];
			}
		}
	}
	return self;
}

// Destructor
//-----------------------------------------------------------------------------------------------------------------------
- (void)dealloc
{
	SAFE_RELEASE(m_keys);
	
	//DBGLOG0A(@"OriMultiSpriteRecord: instance deallocated");
	
	[super dealloc];
}

// Returns key at index
//-----------------------------------------------------------------------------------------------------------------------
- (OriMultiSpriteKey*)getKeyAtIndex:(unsigned int)index
{
	if (index >= [m_keys count])
		return nil;
	
	return [m_keys objectAtIndex:index];
}

// Returns key of sprite
//-----------------------------------------------------------------------------------------------------------------------
- (OriMultiSpriteKey*)getKeyAtSprite:(OriSprite*)sprite
{
	return [m_keys getObjectByProp:@"propSprite" Value:sprite];
}

// Returns weak pointer to linked sprite
//-----------------------------------------------------------------------------------------------------------------------
- (OriSprite*)getWeakSpriteLink:(unsigned int)index
{
	return [m_multisprite getWeakSpriteLink:index];
}

// Returns base point
//-----------------------------------------------------------------------------------------------------------------------
- (OriSpriteAnimationPoint*)getBasePoint:(NSString*)name
{
	return [m_multisprite getBasePoint:name forRecord:self];
}


@end
