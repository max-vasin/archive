//=======================================================================================================================
//  File:		OriTilemapStatic.m
//  Project:	heli
//	Desc:		Tilemap static container object
//
//  Created by Max Vasin on 5/12/11.
//  Copyright 2011 Origin. All rights reserved.
//=======================================================================================================================

// ***** IMPORTS *****

#import "OriTilemapStatic.h"
#import "OriTilemap.h"
#import "OriResourceManager.h"

// ***** IMPLEMENTATION *****

@implementation OriTilemapStatic

// ****** PROPERTIES ******

// Synthesized
//-----------------------------------------------------------------------------------------------------------------------
@synthesize propObject		= m_object;
@synthesize propPosition	= m_position;


// ****** CLASS METHODS ******

// Returns new autoreleased static wrapper with tilemap binding
//-----------------------------------------------------------------------------------------------------------------------
+ (id)staticWithTilemap:(OriTilemap*)tilemap
{
	return [[[self alloc] initWithTilemap:tilemap] autorelease];
}

// Returns new autoreleased static wrapper with tilemap binding and XML data
//-----------------------------------------------------------------------------------------------------------------------
+ (id)staticWithTilemap:(OriTilemap*)tilemap withXML:(OriXMLNode*)node
{
	return [[[self alloc] initWithTilemap:tilemap withXML:node] autorelease];
}

// ****** INSTANCE METHODS ******

// Initializes new static wrapper with tilemap binding
//-----------------------------------------------------------------------------------------------------------------------
- (id)initWithTilemap:(OriTilemap*)tilemap
{
	if ( (self = [self init]) )
	{
		m_tilemap = tilemap;
	}
	return self;
}

// Initializes new static wrapper with tilemap binding and XML data
//-----------------------------------------------------------------------------------------------------------------------
- (id)initWithTilemap:(OriTilemap*)tilemap withXML:(OriXMLNode*)node
{
	if ( (self = [self initWithTilemap:tilemap]) )
	{
		m_position.x	= [node getFirstChildValueInt:@"PosX" placeholder:0];
		m_position.y	= [node getFirstChildValueInt:@"PosY" placeholder:0];
		
		NSString* objectClass = [node getFirstChildValue:@"Class" placeholder:@""];
		NSString* objectName  = [node getFirstChildValue:@"Name"  placeholder:@""];
		
		if		([objectClass isEqualToString:@"multisprite"])
		{
			m_object	= [[OriResourceManager sharedManager] getMultiSprite:objectName];
		}
		else if	([objectClass isEqualToString:@"sprite"])
		{
			m_object	= [[OriResourceManager sharedManager] getSprite:objectName];
		}
	}
	return self;
}

// Destructor
//-----------------------------------------------------------------------------------------------------------------------
- (void)dealloc
{
	SAFE_RELEASE(m_object);	
	
	[super dealloc];
}

@end
