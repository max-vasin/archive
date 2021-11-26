//=======================================================================================================================
//  File:		OriTimer.h
//  Project:	heli
//	Desc:		Engine timer class
//
//  Created by Max Vasin on 3/24/11.
//  Copyright 2011 Origin. All rights reserved.
//=======================================================================================================================

// ***** IMPORTS *****

#import <QuartzCore/QuartzCore.h>
#import <UIKit/UIKit.h>
#import "OriBase.h"
#import "OriTimerEntry.h"

// ***** INTERFACE *****

@interface OriTimer : NSObject 
{
    CADisplayLink*		m_timer;			// CADisplayLink or NSTimer
	NSMutableArray*		m_objects;			// Updated objects
	CFTimeInterval		m_lastTime;			// Last update time
	oriFloat			m_delta;			// Current timedelta
	BOOL				m_firstHit;
}

@property (nonatomic, readonly)	oriInt	propFPS;

// ***** CLASS METHODS *****

+ (id)		sharedTimer;

// ***** INSTANCE METHODS *****

- (void)	start;
- (void)	stop;
- (void)	reset;

- (void)	attach:(id<OriTimerEntry>)object;
- (void)	detach:(id<OriTimerEntry>)object;

@end
