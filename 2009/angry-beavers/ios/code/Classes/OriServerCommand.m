//
//  OriServerCommand.m
//  heli
//
//  Created by Max Vasin on 4/19/11.
//  Copyright 2011 Origin. All rights reserved.
//

#import "OriServerCommand.h"


@implementation OriServerCommand

@synthesize propId		= m_cmdId;
@synthesize propArgs	= m_cmdArgs;

+ (id)commandId:(enServerCommandID)cmdId Args:(NSArray *)args
{
	return [[[self alloc] initWithId:cmdId Args:args] autorelease];
}

- (id)initWithId:(enServerCommandID)cmdId Args:(NSArray *)args
{
	if ( (self = [super init]) )
	{
		m_cmdId		= cmdId;
		m_cmdArgs	= [args mutableCopy];
	}
	
	return self;
}

- (void)dealloc
{
	SAFE_RELEASE(m_cmdArgs);
	
	[super dealloc];
}

@end
