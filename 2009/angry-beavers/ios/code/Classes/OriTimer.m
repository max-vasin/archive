//=======================================================================================================================
//  File:		OriTimer.m
//  Project:	heli
//	Desc:		Engine timer class
//
//  Created by Max Vasin on 3/24/11.
//  Copyright 2011 Origin. All rights reserved.
//=======================================================================================================================

// ***** IMPORTS *****

#import <QuartzCore/QuartzCore.h>
#import "OriTimer.h"

// ***** PRIVATE METHODS *****

@interface OriTimer(Private)

- (void)update;

@end

// ***** IMPLEMENTATION *****

static OriTimer*	g_sharedTimer = NULL;

@implementation OriTimer

- (oriInt)propFPS
{
	if (!m_delta)
		return 65535;
	
	return (oriInt)(round((1 / m_delta) / 5) * 5);
}


// ***** CLASS METHODS *****

// Returns shared timer instance
//-----------------------------------------------------------------------------------------------------------------------
+ (id)sharedTimer
{
	return g_sharedTimer;
}

// ***** INSTANCE METHODS *****

//#######################################################################################################################
// INITIALIZERS/DESTRUCTORS
//#######################################################################################################################

// Initializes timer
//-----------------------------------------------------------------------------------------------------------------------
- (id)init
{
	if ( (self = [super init]) )
	{
		if (!g_sharedTimer)
			g_sharedTimer = self;
		
		m_objects = [[NSMutableArray alloc] init];
		
		m_timer = [[CADisplayLink displayLinkWithTarget:self selector:@selector(update:)] retain];
		
		// display link retain timer, so release it (it should be retained on dealloc before display link release)
		[self release];
		
		m_timer.frameInterval	= 1;
		m_timer.paused			= TRUE;
		
		[m_timer addToRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];	
		
		m_firstHit = TRUE;

		DBG_INIT_LOG_0(self);
	}
	return self;
}

// Destructor
//-----------------------------------------------------------------------------------------------------------------------
- (void)dealloc
{
	// we release timer from display link during init, so retain it now to avoid recursive dealloc
	[self retain];
	
	[m_timer invalidate];
	
	SAFE_RELEASE(m_timer);
	SAFE_RELEASE(m_objects);
	
	if (g_sharedTimer == self)
		g_sharedTimer = nil;
	
	DBG_DEALLOC_LOG_0(self);
	
	[super dealloc];
}

//#######################################################################################################################
// TIMER CONTROL
//#######################################################################################################################

// Starts timer
//-----------------------------------------------------------------------------------------------------------------------
- (void)start
{
	if (m_timer.paused)
	{
		m_timer.paused = FALSE;
		m_lastTime = CFAbsoluteTimeGetCurrent();
	}
}

// Pauses timer
//-----------------------------------------------------------------------------------------------------------------------
- (void)stop
{
	if (!m_timer.paused)
	{
		m_timer.paused = TRUE;
		m_delta = 0.0f;
	}
}

// Resets timer
//-----------------------------------------------------------------------------------------------------------------------
- (void)reset
{
	m_firstHit = YES;
	m_delta	   = 0.0f;
}

//#######################################################################################################################
// ENTRIES CONTROL
//#######################################################################################################################

// Attaches new timer update entry
//-----------------------------------------------------------------------------------------------------------------------
- (void)attach:(id<OriTimerEntry>)object
{
	[m_objects addObject:object];
	
	BOOL weakLink = NO;
	
	if ([object respondsToSelector:@selector(isTimerWeakLink)])
		weakLink = [object isTimerWeakLink];
	
	if (weakLink)
		[object release];
}

// Detaches timer entry
//-----------------------------------------------------------------------------------------------------------------------
- (void)detach:(id<OriTimerEntry>)object
{
	BOOL weakLink = NO;
	
	if ([object respondsToSelector:@selector(isTimerWeakLink)])
		weakLink = [object isTimerWeakLink];
	
	if (weakLink)
		[object retain];
	
	[m_objects removeObject:object];
}

// Timer hit handler
//-----------------------------------------------------------------------------------------------------------------------
- (void)update:(CADisplayLink*)timer
{
	CFTimeInterval curTime = timer.timestamp;
	
	if (m_firstHit)
	{
		m_lastTime = curTime;
		m_firstHit = NO;
		return;
	}
	
	m_delta = curTime - m_lastTime;
	
	for (id object in m_objects)
	{
		[object update:m_delta];
	}
		
	m_lastTime = curTime;
}


@end
