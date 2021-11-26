//=======================================================================================================================
//  File:		OriTimerEntry.h
//  Project:	heli
//	Desc:		Timer entry base class
//
//  Created by Max Vasin on 3/24/11.
//  Copyright 2011 Origin. All rights reserved.
//=======================================================================================================================

// ***** IMPORTS *****

#import "OriBase.h"

// ***** INTERFACE *****

@protocol OriTimerEntry <NSObject>

- (void)update:(oriFloat)delta;
- (BOOL)isTimerWeakLink;

@end
