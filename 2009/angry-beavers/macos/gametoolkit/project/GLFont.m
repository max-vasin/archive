//
//  GLFont.m
//  OriginGameToolkit
//
//  Created by Max Vasin on 4/26/11.
//  Copyright 2011 Origin Games. All rights reserved.
//

#import "GLFont.h"


@implementation GLFont


- (id)initWithName:(NSString*)name Size:(CGFloat)size
{
	if ( (self = [self init]) )
	{
		NSFont* font = [NSFont fontWithName:name size:size];
		
		if (!font)
		{
			[self release];
			return nil;
		}
		
		
		
		
		
	}
	return self;
}




@end
