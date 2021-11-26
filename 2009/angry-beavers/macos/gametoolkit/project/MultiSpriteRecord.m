//===============================================================================================
//  File:		MultiSpriteRecord.m
//  Project:	OriginGameToolkit
//	Desc:		Multi sprite record class interface
//
//  Created by Max Vasin on 3/29/11.
//  Copyright 2011 Origin Games. All rights reserved.
//===============================================================================================

// ****** IMPORTS ******

#import "MultiSpriteRecord.h"
#import "ProjectMultiSpriteItem.h"
#import "SharedResourceManager.h"

//###############################################################################################
// MULTISPRITE RECORD KEY IMPLEMENTATION
//###############################################################################################

@implementation MultiSpriteRecordKey

// ****** PROPERTIES ******

// Synthesized
//-----------------------------------------------------------------------------------------------
@synthesize propSprite			= m_sprite;
@synthesize propFrame			= m_frame;
@synthesize propPoint			= m_point;
@synthesize propAnimationPlay	= m_animPlay;
@synthesize propAnimationSpeed	= m_animSpeed;
@synthesize propLayer			= m_layer;

// Animation property
//-----------------------------------------------------------------------------------------------
- (void)setPropAnimation:(SpriteAnimation*)animation
{
	if (m_animation != animation)
	{
		m_animation = animation;
		
		self.propAnimationSpeed	= m_animation.propSpeed;
	}
}

- (SpriteAnimation*)propAnimation
{
	return m_animation;
}

// ****** CLASS METHODS ******

// Returns new autoreleased key witn record and sprite
//-----------------------------------------------------------------------------------------------
+ (id)keyWithRecord:(MultiSpriteRecord*)record andSprite:(SpriteWrapper*)sprite
{
	return [[[MultiSpriteRecordKey alloc] initWithRecord:record andSprite:sprite] autorelease];
}

// Returns new autoreleased key witn record and XML data
//-----------------------------------------------------------------------------------------------
+ (id)keyWithRecord:(MultiSpriteRecord*)record formXML:(NSXMLElement*)node
{
	return [[[MultiSpriteRecordKey alloc] initWithRecord:record formXML:node] autorelease];
}

// ****** INSTANCE METHODS ******

// Initializes new key with record and sprite
//-----------------------------------------------------------------------------------------------
- (id)initWithRecord:(MultiSpriteRecord*)record andSprite:(SpriteWrapper*)sprite
{
	if ( (self = [self init]) )
	{
		m_record	= record;
		m_sprite	= sprite;
		
		// try to set any animation
		if (m_sprite.propSprite && [m_sprite.propSprite.propAnimations count])
		{
			self.propAnimation = [m_sprite.propSprite.propAnimations objectAtIndex:0];
		}
		
		self.propPoint = [m_record originPoint];
	}
	
	return self;
}

// Initializes new key with record and XML data
//-----------------------------------------------------------------------------------------------
- (id)initWithRecord:(MultiSpriteRecord*)record formXML:(NSXMLElement*)node
{
	if ( (self = [self init]) )
	{
		m_record = record;
		
		self.propSprite			= [m_record spriteAtIndex:[node getFirstChildInteger:@"Sprite"]];
		self.propAnimation		= [m_sprite.propSprite getAnimation:[node getFirstChildValue:@"Animation"]];
		self.propFrame			= [node getFirstChildInteger:@"Frame"];
		self.propAnimationSpeed	= [node getFirstChildInteger:@"AnimSpeed"];
		self.propAnimationPlay	= [node getFirstChildInteger:@"AnimPlay"];
		self.propLayer			= [node getFirstChildFloat:@"Layer"];
		self.propPoint			= [m_record pointWithName:[node getFirstChildValue:@"Point"]];
	}
	
	return self;
}

// Saves key to XML node
//-----------------------------------------------------------------------------------------------
- (NSXMLElement*)saveXML:(NSXMLElement*)node
{
	NSXMLElement* thisNode = [node addNewChild:@"Key"];
	
	[thisNode addNewChild:@"Sprite"		withInteger:[m_record indexOfSprite:m_sprite]];
	[thisNode addNewChild:@"Animation"	withValue:m_animation.propName];
	[thisNode addNewChild:@"Frame"		withInteger:m_frame];
	[thisNode addNewChild:@"AnimSpeed"	withInteger:m_animSpeed];
	[thisNode addNewChild:@"AnimPlay"	withInteger:m_animPlay];
	[thisNode addNewChild:@"Layer"		withFloat:m_layer];
	[thisNode addNewChild:@"Point"		withValue:m_point.propName];
	
	return thisNode;
}

// Build XML node
//-----------------------------------------------------------------------------------------------
- (NSXMLElement*)buildXML:(NSXMLElement*)node
{
	NSXMLElement* thisNode = [node addNewChild:@"Key"];
	
	[thisNode addNewChild:@"Sprite"		withInteger:[m_record indexOfSprite:m_sprite]];
	[thisNode addNewChild:@"Animation"	withValue:m_animation.propName];
	[thisNode addNewChild:@"Frame"		withInteger:m_frame];
	[thisNode addNewChild:@"AnimSpeed"	withInteger:m_animSpeed];
	[thisNode addNewChild:@"AnimPlay"	withInteger:m_animPlay];
	[thisNode addNewChild:@"Layer"		withFloat:m_layer];
	[thisNode addNewChild:@"Point"		withValue:m_point.propName];
	
	return thisNode;
}

@end

//###############################################################################################
// MULTISPRITE RECORD IMPLEMENTATION
//###############################################################################################

// ****** PRIVATE METHODS ******

@interface MultiSpriteRecord (Private)

//- (void) onTimer:(NSTimer*)timer;

@end


@implementation MultiSpriteRecord

// ****** PROPERTIES ******

// Synthesized
//-----------------------------------------------------------------------------------------------
@synthesize propKeys	= m_keys;

// ****** CLASS METHODS ******

// Returns new records with multisprite container
//-----------------------------------------------------------------------------------------------
+ (id)recordWithMultiSprite:(ProjectMultiSpriteItem*)item
{
	return [[[MultiSpriteRecord alloc] initWithMultiSprite:item] autorelease];
}

// Returns new records with multisprite container and XML data
//-----------------------------------------------------------------------------------------------
+ (id)recordWithMultiSprite:(ProjectMultiSpriteItem*)item fromXML:(NSXMLElement*)node
{
	return [[[MultiSpriteRecord alloc] initWithMultiSprite:item fromXML:node] autorelease];
}

// ****** INSTANCE METHODS ******

// Initializes new instance with multisprite container
//-----------------------------------------------------------------------------------------------
- (id)initWithMultiSprite:(ProjectMultiSpriteItem*)item
{
	if ( (self = [self init]) )
	{
		m_multiSprite	= item;
		m_keys			= [[NSMutableArray alloc] init];
	}
	return self;
}

// Initializes new instance with multisprite container and XML data
//-----------------------------------------------------------------------------------------------
- (id)initWithMultiSprite:(ProjectMultiSpriteItem*)item fromXML:(NSXMLElement*)node
{
	if ( (self = [self initWithMultiSprite:item]) )
	{
		NSXMLElement* keysNode = [node getFirstChild:@"Keys"];
		
		for (NSXMLElement* keyNode in [keysNode children])
			[m_keys addObject:[MultiSpriteRecordKey keyWithRecord:self formXML:keyNode]];
		
		[self syncKeys];
	}
	return self;
}

// Destructor
//-----------------------------------------------------------------------------------------------
- (void)dealloc
{
	SAFE_RELEASE(m_keys);
	
	[super dealloc];
}

// Synchronizes keys array with current multisprite linked sprites
//-----------------------------------------------------------------------------------------------
- (void)syncKeys
{
	NSMutableArray* linkedSprites = m_multiSprite.propLinkedSprites;
	
	// clean deleted keys
	
	NSMutableArray* toDelete = [[NSMutableArray alloc] init];
	
	for (MultiSpriteRecordKey* key in m_keys)
	{
		if (![linkedSprites containsObject:key.propSprite])
			[toDelete addObject:key];
	}
	
	for (MultiSpriteRecordKey* key in toDelete)
		[m_keys removeObject:key];
	
	SAFE_RELEASE(toDelete);
	
	// add new keys
	
	for (SpriteWrapper* link in linkedSprites)
	{
		MultiSpriteRecordKey* key = [m_keys getObjectByProp:@"propSprite" Value:link];
		
		if (!key)
		{
			key = [MultiSpriteRecordKey keyWithRecord:self andSprite:link];
			[m_keys addObject:key];
		}
		
		if (key.propAnimation.propSprite != key.propSprite.propSprite)
		{
			key.propFrame		= 0;
			key.propAnimation	= nil;
			
			if ([key.propSprite.propSprite.propAnimations count])
				key.propAnimation = [key.propSprite.propSprite.propAnimations objectAtIndex:0];
		}
	}
}

// Returns multisprite named point
//-----------------------------------------------------------------------------------------------
- (FramePoint*)pointWithName:(NSString*)name
{
	return [m_multiSprite pointWithName:name];
}

// Returns multisprite origin point
//-----------------------------------------------------------------------------------------------
- (FramePoint*)originPoint
{
	return [m_multiSprite originPoint];
}

// Returns key by sprite
//-----------------------------------------------------------------------------------------------
- (MultiSpriteRecordKey*)getKey:(SpriteWrapper*)sprite
{
	return [m_keys getObjectByProp:@"propSprite" Value:sprite];
}

// Returns index of linked sprite
//-----------------------------------------------------------------------------------------------
- (unsigned int)indexOfSprite:(SpriteWrapper*)sprite
{
	return [m_multiSprite indexOfSprite:sprite];
}

// Returns linked sprite by index
//-----------------------------------------------------------------------------------------------
- (SpriteWrapper*)spriteAtIndex:(unsigned int)index
{
	return [m_multiSprite spriteAtIndex:index];
}

// Saves multisprite record to XML schema
//-----------------------------------------------------------------------------------------------
- (NSXMLElement*)saveXML:(NSXMLElement*)node
{
	NSXMLElement* thisNode = [node addNewChild:@"Record"];
	
	NSXMLElement* keysNode = [thisNode addNewChild:@"Keys"];
	
	for (MultiSpriteRecordKey* key in m_keys)
		[key saveXML:keysNode];

	return thisNode;
}

// Builds multisprite record
//-----------------------------------------------------------------------------------------------
- (NSXMLElement*)buildXML:(NSXMLElement*)node
{
	NSXMLElement* thisNode = [node addNewChild:@"Record"];
	
	NSXMLElement* keysNode = [thisNode addNewChild:@"Keys"];
	
	for (MultiSpriteRecordKey* key in m_keys)
		[key buildXML:keysNode];
	
	return thisNode;
}

@end
