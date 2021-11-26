//=======================================================================================================================
//  File:		OriTimeSampler.m
//  Project:	heli
//	Desc:		Time sampler class implementation
//
//  Created by Max Vasin on 5/26/11.
//  Copyright 2011 Origin. All rights reserved.
//=======================================================================================================================

// ******** IMPORTS ********

#import "OriTimeSampler.h"

// ******** IMPLEMENTATION ********

@implementation OriTimeSampler

//#######################################################################################################################
// PROPERTIES
//#######################################################################################################################

//-----------------------------------------------------------------------------------------------------------------------
// SYNTHESIZED PROPERTIES
//-----------------------------------------------------------------------------------------------------------------------

@synthesize propTime		= m_currentTime;
@synthesize propSamples		= m_currentSamples;
@synthesize propFrame		= m_timeFrame;
@synthesize propLastSamples	= m_lastSamples;

//#######################################################################################################################
// CONSTRUCTION/DESTRUCTION
//#######################################################################################################################

//-----------------------------------------------------------------------------------------------------------------------
// Default initializer
//-----------------------------------------------------------------------------------------------------------------------

- (id)init
{
	if ( (self = [super init]) )
	{
		m_timeFrame		= 0.1f;
	}
	return self;
}

//-----------------------------------------------------------------------------------------------------------------------
// Destructor
//-----------------------------------------------------------------------------------------------------------------------

- (void)dealloc
{
	[super dealloc];
}

//#######################################################################################################################
// SAMPLER ACCESS
//#######################################################################################################################

//-----------------------------------------------------------------------------------------------------------------------
// Updates sampler time and returns number of new samples
//-----------------------------------------------------------------------------------------------------------------------

- (oriUint)update:(oriFloat)delta
{
	if (!m_timeFrame)
		return 0;
	
	m_timeBuffer += delta;
	
	m_lastSamples = floor(m_timeBuffer / m_timeFrame);
	
	if (m_lastSamples > 0)
	{
		m_currentSamples += m_lastSamples;
		m_timeBuffer	 -= m_lastSamples * m_timeFrame;
	}
	
	m_currentTime += delta;
	
	return m_lastSamples;
}

//-----------------------------------------------------------------------------------------------------------------------
// Resets sampler internal buffers
//-----------------------------------------------------------------------------------------------------------------------

- (void)reset
{
	m_timeBuffer		= 0.0f;
	m_currentTime		= 0.0f;
	m_currentSamples	= 0;
	m_lastSamples		= 0;
}

@end