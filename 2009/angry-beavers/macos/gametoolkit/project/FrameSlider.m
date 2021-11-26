//
//  FrameSlider.m
//  OriginGameToolkit
//
//  Created by Max Vasin on 3/16/11.
//  Copyright 2011 Origin Games. All rights reserved.
//

#import "FrameSlider.h"


@implementation FrameSlider

-(void)setMaxValue:(double)value
{
	[super setMaxValue:value-1];
	
	[self setNumberOfTickMarks:((NSInteger)value)];
}

-(double)maxValue
{
	return [super maxValue];
}

@end
