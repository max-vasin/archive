//
//  OriServerResponder.h
//  heli
//
//  Created by Max Vasin on 4/19/11.
//  Copyright 2011 Origin. All rights reserved.
//

#import "OriBase.h"
#import "OriServerCommand.h"

@protocol OriServerResponder <NSObject>

- (void)handleCommand:(OriServerCommand*)cmd;
- (BOOL)isWeakLinkToServer;

@end
