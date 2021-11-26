//
//  OriServerCommand.h
//  heli
//
//  Created by Max Vasin on 4/19/11.
//  Copyright 2011 Origin. All rights reserved.
//

#import "OriBase.h"

typedef enum
{
	eServerCmdNone			= 0,
	eServerCmdPlayerMove	= 1,
	eServerCmdPlayerLook	= 2
	
}enServerCommandID;


@interface OriServerCommand : NSObject 
{
	enServerCommandID	m_cmdId;
	NSArray*			m_cmdArgs;
}


@property (nonatomic, readonly)	enServerCommandID	propId;
@property (nonatomic, readonly)	NSArray*			propArgs;

+ (id)commandId:(enServerCommandID)cmdId	Args:(NSArray*)args;

- (id)initWithId:(enServerCommandID)cmdId	Args:(NSArray*)args;


@end
