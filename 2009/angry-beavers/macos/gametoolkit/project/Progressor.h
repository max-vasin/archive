//===============================================================================================
//  File:		Progressor.h
//  Project:	OriginGameToolkit
//	Desc:		Progress data model class interface
//
//  Created by Max Vasin on 12/16/10.
//  Copyright 2010 Origin. All rights reserved.
//===============================================================================================

// **** IMPORTS ****

#import <Cocoa/Cocoa.h>
#import "BaseObject.h"

// **** INTERFACE ****

@interface Progressor : BaseObject 
{
	NSInteger	m_currentMinVal;
	NSInteger	m_currentMaxVal;
	NSInteger	m_overallMinVal;
	NSInteger	m_overallMaxVal;
	NSInteger	m_currentVal;
	NSInteger	m_overallVal;
	BOOL		m_currentAnimate;
	BOOL		m_overallAnimate;
	NSString*	m_currentState;
	NSString*	m_overallState;
}

// **** PROPERTIES ****

@property (assign)	NSInteger	propCurrentMinVal;
@property (assign)	NSInteger	propCurrentMaxVal;
@property (assign)	NSInteger	propOverallMinVal;
@property (assign)	NSInteger	propOverallMaxVal;
@property (assign)	NSInteger	propCurrentVal;
@property (assign)	NSInteger	propOverallVal;
@property (assign)	BOOL		propCurrentAnimate;
@property (assign)	BOOL		propOverallAnimate;
@property (copy)	NSString*	propCurrentState;
@property (copy)	NSString*	propOverallState;

@end
