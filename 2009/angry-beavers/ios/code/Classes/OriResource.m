//=======================================================================================================================
//  OriResource.m
//  heli
//
//  Created by Max Vasin on 3/23/11.
//  Copyright 2011 Origin. All rights reserved.
//=======================================================================================================================

// ***** IMPORTS *****

#import "OriResource.h"
#import "OriResourceManager.h"

// ***** IMPLEMENTATION *****

@implementation OriResource

// ***** PROPERTIES *****

@synthesize propName	= m_name;

// System memory used by resource
//-----------------------------------------------------------------------------------------------------------------------
- (oriUint)propSysMemUsed
{
	return 0;
}

// Video memory used by resource
//-----------------------------------------------------------------------------------------------------------------------
- (oriUint)propVideoMemUsed
{
	return 0;
}

// ***** METHODS *****

// Overriding default initializer
//-----------------------------------------------------------------------------------------------------------------------
- (id)init
{
	if ( (self = [super init]) )
	{
		m_refs = 1;
	}
	return self;
}

// Destructor
//-----------------------------------------------------------------------------------------------------------------------
- (void)dealloc
{
	SAFE_RELEASE(m_name);
}


// Designated initializer
//-----------------------------------------------------------------------------------------------------------------------
- (id)initWithManager:(OriResourceManager*)resman withName:(NSString*)name
{
	if ( (self = [self init]) )
	{
		m_resMan = resman;
		self.propName = name;
	}
	
	return self;
}

// Overriding retain
//-----------------------------------------------------------------------------------------------------------------------
- (id)retain
{
	m_refs++;
	return self;
}

// Overriding retainCount
//-----------------------------------------------------------------------------------------------------------------------
- (NSUInteger)retainCount
{
	return m_refs;
}

// Overriding release
//-----------------------------------------------------------------------------------------------------------------------
- (oneway void)release
{
	// Something want drop last "external" reference, retain count 1 - manager NSMutableArray reference
	// So we autmatically drop resource from manager
	
	if (m_refs == 2 && !m_removing)
	{
		m_removing = YES;
		m_refs--;
		[m_resMan removeResource:self];
		return;
	}
	
	m_refs--;
	if (!m_refs)
		[self dealloc];
}

@end
