//=======================================================================================================================
//  File:		OriTimeSampler.h
//  Project:	heli
//	Desc:		Time range sampler
//
//  Created by Max Vasin on 5/26/11.
//  Copyright 2011 Origin. All rights reserved.
//=======================================================================================================================

// ******** IMPORTS ********

#import "OriBase.h"

// ******** INTERFACE ********

@interface OriTimeSampler : NSObject 
{
	oriFloat		m_timeFrame;			// sample frame time
	oriFloat		m_timeBuffer;			// time buffer
	oriFloat		m_currentTime;			// current time (last reset)
	oriUint			m_currentSamples;		// current sambples (last reset)
	oriUint			m_lastSamples;			// number of samples generated during last update
}

// ******** PROPERTIES ********

@property (nonatomic, assign)		oriFloat	propFrame;
@property (nonatomic, readonly)		oriFloat	propTime;
@property (nonatomic, readonly)		oriUint		propSamples;
@property (nonatomic, readonly)		oriUint		propLastSamples;

// ******** INSTANCE METHODS ********


- (oriUint)	update:(oriFloat)delta;				// updates sampler and returns new samples count

- (void)	reset;								// resets sampler time and samples count

@end
