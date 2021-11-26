//=======================================================================================================================
//  File:		OriMultiSprite.m
//  Project:	heli
//	Desc:		multisprite resource implementation
//
//  Created by Max Vasin on 4/12/11.
//  Copyright 2011 Origin. All rights reserved.
//=======================================================================================================================

// ***** IMPORTS *****

#import "OriMultiSprite.h"
#import "OriResourceManager.h"

// ***** IMPLEMENTATION *****

@implementation OriMultiSprite

// ****** PROPERTIES ******

// Synthesized
//-----------------------------------------------------------------------------------------------------------------------
@synthesize propBaseSprite		= m_baseSprite;
@synthesize propBaseAnimation	= m_baseAnimation;
@synthesize propLayer			= m_layer;

// Returns number of records
//-----------------------------------------------------------------------------------------------------------------------
- (unsigned int)propRecordsCount
{
	return [m_records count];
}

// Returns number of linked sprites
//-----------------------------------------------------------------------------------------------------------------------
- (unsigned int)propLinksCount
{
	return [m_linkedSprites count];
}

// (OriResource overriding) system memory used by this resource
//-----------------------------------------------------------------------------------------------------------------------
- (oriUint)propSysMemUsed
{
	return 0;
}

// (OriResource overriding) video memory used by thid resource
//-----------------------------------------------------------------------------------------------------------------------
- (oriUint)propVideoMemUsed
{
	return 0;
}


// ****** CLASS METHODS ******

//#######################################################################################################################
// CONSTRUCTION
//#######################################################################################################################

// Returns new autoreleased instance of multisprite with given resource manager
//-----------------------------------------------------------------------------------------------------------------------
+ (id)multiSpriteWithManager:(OriResourceManager*)resman withName:(NSString*)name
{
	return [[[self alloc] initWithManager:resman withName:name] autorelease];
}

// Returns new autoreleased instance of multisprite with given resource manager and XML data
//-----------------------------------------------------------------------------------------------------------------------
+ (id)multiSpriteWithManager:(OriResourceManager*)resman withName:(NSString*)name withXML:(OriXMLNode*)node
{
	return [[[self alloc] initWithManager:resman withName:name withXML:node] autorelease];
}

// ****** INSTANCE METHODS ******

//#######################################################################################################################
// INITIALIZERS/DESTRUCTORS
//#######################################################################################################################

// Initializes instance of sprite with given resource manager
//-----------------------------------------------------------------------------------------------------------------------
- (id)initWithManager:(OriResourceManager*)resman withName:(NSString*)name
{
	if ( (self = [super initWithManager:resman withName:name]) )
	{
		m_linkedSprites = [[NSMutableArray alloc] init];
		m_records		= [[NSMutableArray alloc] init];
		
		DBG_INIT_LOG_NAMED_0(self, m_name);
	}
	return self;
}

// Initializes instance of sprite with given resource manager and XML data
//-----------------------------------------------------------------------------------------------------------------------
- (id)initWithManager:(OriResourceManager*)resman withName:(NSString*)name withXML:(OriXMLNode*)node
{
	if ( (self = [self initWithManager:resman withName:name]) )
	{
		if (node)
		{
			// Load base sprite
			
			NSString* baseSpriteName = [node getFirstChildValue:@"BaseSprite" placeholder:nil];
			if (!baseSpriteName)
			{
				DBGLOG1A(@"<FAIL>OriMultiSprite: (%@) cant find base sprite name in XML", self.propName);
			}
			else
			{
				m_baseSprite = [m_resMan getSprite:baseSpriteName];
				if (!m_baseSprite)
					DBGLOG2A(@"<FAIL>OriMultiSprite: (%@) cant get base sprite (%@)", self.propName, baseSpriteName);
			}
			
			// Get base animaton
			
			if (m_baseSprite)
			{
				NSString* baseAnimationName = [node getFirstChildValue:@"BaseAnimation" placeholder:nil];
				if (!baseAnimationName)
				{
					DBGLOG1A(@"<FAIL>OriMultiSprite: (%@) cant find base sprite animation name in XML", self.propName);
				}
				else
				{
					m_baseAnimation = [m_baseSprite getAnimation:baseAnimationName];
					if (!m_baseAnimation)
						DBGLOG2A(@"<FAIL>OriMultiSprite: (%@) cant get base sprite animation (%@)", self.propName, baseAnimationName);
				}
			}
			
			// Layer
			
			m_layer = [node getFirstChildValueFloat:@"Layer" placeholder:0.0f];
						
			// Load links
			
			OriXMLNode* linksRoot = [node getFirstChild:@"Links"];
			
			if (!linksRoot)
			{
				DBGLOG1A(@"<FAIL>OriMultiSprite: (%@) cant find links root node in XML", self.propName);
			}
			else
			{
				for (OriXMLNode* linkNode in linksRoot.propChildren)
				{
					NSString*	spriteName = [linkNode getFirstChildValue:@"Sprite" placeholder:nil];
					
					if (!spriteName)
						DBGLOG1A(@"<FAIL>OriMultiSprite: (%@) invalid sprite name in XML data", self.propName);
					
					
					OriSprite*	sprite = [m_resMan getSprite:spriteName];
					
					if (!sprite)
					{
						DBGLOG2A(@"<FAIL>OriMultiSprite: (%@) cant get linked sprite (%@)", self.propName, spriteName);
					}
					else
					{
						[m_linkedSprites addObject:sprite];
						
						SAFE_RELEASE(sprite);
					}
				}
			}
			
			// Load records
			
			OriXMLNode* recordsRoot = [node getFirstChild:@"Records"];
			
			if (!recordsRoot)
			{
				DBGLOG1A(@"<FAIL>OriMultiSprite: (%@) cant find records root node in XML", self.propName);
			}
			else
			{			
				for (OriXMLNode* recordNode in recordsRoot.propChildren)
				{
					OriMultiSpriteRecord* newRec = [OriMultiSpriteRecord alloc];
					
					[m_records addObject:newRec];
					
					[[newRec initWithMultiSprite:self withXML:recordNode] release];
				}
			}
		}
	}
	return self;
}

// Desctructor
//-----------------------------------------------------------------------------------------------------------------------
- (void)dealloc
{
	SAFE_RELEASE(m_baseSprite);
	SAFE_RELEASE(m_records);
	SAFE_RELEASE(m_linkedSprites);

	DBG_DEALLOC_LOG_NAMED_0(self, m_name);

	[super dealloc];
}

// Returns record by index
//-----------------------------------------------------------------------------------------------------------------------
- (OriMultiSpriteRecord*)getRecord:(unsigned int)index
{
	if (index >= [m_records count])
		return nil;
	
	return [m_records objectAtIndex:index];
}

// Returns linked sprite name at index
//-----------------------------------------------------------------------------------------------------------------------
- (NSString*)getSpriteLink:(unsigned int)index
{
	if (index >= [m_linkedSprites count])
		return nil;
	
	OriSprite* linkSprite = [m_linkedSprites objectAtIndex:index];
	
	return linkSprite.propName;
}

// Returns weak pointer to linked sprite
//-----------------------------------------------------------------------------------------------------------------------
- (OriSprite*)getWeakSpriteLink:(unsigned int)index
{
	if (index >= [m_linkedSprites count])
		return nil;
	
	return [m_linkedSprites objectAtIndex:index];
}

// Returns point position at frame
//-----------------------------------------------------------------------------------------------------------------------
- (OriSpriteAnimationPoint*)getBasePoint:(NSString*)name forRecord:(OriMultiSpriteRecord*)record
{
	if (!m_baseAnimation)
		return nil;
	
	oriUint index = [m_records indexOfObject:record];
	
	return [m_baseAnimation getPoint:name atFrame:index];
}

@end
