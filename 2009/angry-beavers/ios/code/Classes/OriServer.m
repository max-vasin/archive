//=======================================================================================================================
//  File:		OriServer.m
//  Project:	heli
//	Desc:		Commands server implmentation
//
//  Created by Max Vasin on 4/19/11.
//  Copyright 2011 Origin. All rights reserved.
//=======================================================================================================================

// ***** IMPORTS *****

#import "OriServer.h"

// ***** SHARED SERVER INSTANCE *****

static OriServer*	g_sharedServer;

// ***** PRIVATE METHODS *****

@interface OriServer (Private)

- (void)sleep:(oriUint)msec;

@end

// ***** IMPLEMENTATION *****

@implementation OriServer

// ***** CLASS METHODS *****

// Returns shared server instance
//-----------------------------------------------------------------------------------------------------------------------
+ (id)sharedServer
{
	return g_sharedServer;
}

// ***** INSTANCE METHODS *****

//#######################################################################################################################
// INITIALIZERS/DESTRUCTORS
//#######################################################################################################################

// Initialies new server instance or return shared instance if it exists
//-----------------------------------------------------------------------------------------------------------------------
- (id)init
{
	if (g_sharedServer)
	{
		[self release];
		return g_sharedServer;
	}
	
	if ( (self = [super init]) )
	{
		m_commandQueue	= [[NSMutableArray alloc] init];
		m_responders	= [[NSMutableArray alloc] init];
		g_sharedServer	= self;
		
		[[OriTimer sharedTimer] attach:self];
		
		DBGLOG0A(@"OriServer: instance allocated");
	}
	return self;
}

// Destructor
//-----------------------------------------------------------------------------------------------------------------------
- (void)dealloc
{
	[[OriTimer sharedTimer] detach:self];
	
	SAFE_RELEASE(m_responders);
	SAFE_RELEASE(m_commandQueue);
	
	if (self == g_sharedServer)
		g_sharedServer = nil;
	
	DBGLOG0A(@"OriServer: instance deallocated");
	
	[super dealloc];
}

//#######################################################################################################################
// COMMANDS/DISPATCH CONTROL
//#######################################################################################################################

// Pushes new command to queue directly
//-----------------------------------------------------------------------------------------------------------------------
- (void)pushCommand:(OriServerCommand*)cmd
{
	[m_commandQueue addObject:cmd];
}

// Dispatches all queued commands to responders
//-----------------------------------------------------------------------------------------------------------------------
- (void)dispatchCommands
{
	for (OriServerCommand* cmd in m_commandQueue)
	{
		for (id responder in m_responders)
		{
			[responder handleCommand:cmd];
		}
	}
	[m_commandQueue removeAllObjects];
}

// Adds new responder
//-----------------------------------------------------------------------------------------------------------------------
- (void)addResponder:(id<OriServerResponder>)object
{
	if (![m_responders containsObject:object])
	{
		[m_responders addObject:object];
		BOOL weakLink = NO;
		
		if ([object respondsToSelector:@selector(isWeakLinkToServer)])
			weakLink = [object isWeakLinkToServer];
		
		if (weakLink)
			[object release];
	}
}

// Remove responder
//-----------------------------------------------------------------------------------------------------------------------
- (void)delResponder:(id)object
{
	BOOL weakLink = NO;
	
	if ([object respondsToSelector:@selector(isWeakLinkToServer)])
		weakLink = [object isWeakLinkToServer];
	
	if (weakLink)
		[object retain];

	[m_responders removeObject:object];
}

//#######################################################################################################################
// OriTimerEntry PROTOCOL SUPPORT
//#######################################################################################################################

// Timer update
//-----------------------------------------------------------------------------------------------------------------------
- (BOOL)isTimerWeakLink
{
	return YES;
}

// Timer update
//-----------------------------------------------------------------------------------------------------------------------
- (void)update:(oriFloat)delta
{
	[self dispatchCommands];
}

//#######################################################################################################################
// PRIVATE METHODS
//#######################################################################################################################

// Correct sleep routine (multithreading)
//-----------------------------------------------------------------------------------------------------------------------
- (void)sleep:(oriUint)msec
{
	struct timespec req = {0};
	time_t sec = (int)(msec/1000);
	msec = msec - (sec*1000);
	req.tv_sec  = sec;
	req.tv_nsec = msec * 1000000L;
	while(nanosleep(&req, &req) == -1)
		continue;
}

@end
