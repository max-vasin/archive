//=======================================================================================================================
//  File:		OriServer.h
//  Project:	heli
//	Desc:		Commands server object class
//
//  Created by Max Vasin on 4/19/11.
//  Copyright 2011 Origin. All rights reserved.
//=======================================================================================================================

// ***** IMPORTS *****

#import "OriBase.h"
#import "OriServerCommand.h"
#import "OriServerResponder.h"
#import "OriTimer.h"

// ***** INTERFACE *****

@interface OriServer : NSObject <OriTimerEntry>
{
	NSMutableArray*		m_commandQueue;
	NSMutableArray*		m_responders;
}

// ***** CLASS METHODS *****

+ (id)sharedServer;

// ***** INSTANCE METHODS *****

- (void)pushCommand:(OriServerCommand*)cmd;
- (void)dispatchCommands;

- (void)addResponder:(id<OriServerResponder>)object;
- (void)delResponder:(id)object;

@end
