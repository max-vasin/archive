//
//  ProjectOutlineView.m
//  OriginGameToolkit
//
//  Created by Max Vasin on 5/5/11.
//  Copyright 2011 Origin Games. All rights reserved.
//

#import "ProjectOutlineView.h"


@implementation ProjectOutlineView


- (BOOL)acceptsFirstMouse:(NSEvent*)theEvent
{
	[self mouseDown:theEvent];
	
	return YES;
}

@end
