//
//  NSColorExtension.m
//  OriginGameToolkit
//
//  Created by Max Vasin on 5/17/11.
//  Copyright 2011 Origin Games. All rights reserved.
//

#import "NSColorExtension.h"


@implementation NSColor (Extension)

+ (id)randomColor
{
	float r = (float)rand() / (float)RAND_MAX;
	float g = (float)rand() / (float)RAND_MAX;
	float b = (float)rand() / (float)RAND_MAX;
	
	return [self colorWithCalibratedRed:r green:g blue:b alpha:1.0f];
}


- (void)setOpenGL
{
	glColor4f([self redComponent], [self greenComponent], [self blueComponent], [self alphaComponent]);
}

@end
