//===============================================================================================
//  File:		SliderZoomValueTransformer.m
//  Project:	OriginGameToolkit
//	Desc:		Zoom slider balanced value transformer implementation
//
//  Created by Max Vasin on 4/21/11.
//  Copyright 2011 Origin Games. All rights reserved.
//===============================================================================================

// ***** IMPORTS *****

#import "SliderZoomValueTransformer.h"

// ***** IMPLEMENTATION *****

@implementation SliderZoomValueTransformer

// ***** CLASS METHODS *****

// Transformer self-registering
//-----------------------------------------------------------------------------------------------
+ (void)registerSelf
{
	SliderZoomValueTransformer* trans = [[[self alloc] init] autorelease];
	
	// register it with the name that we refer to it with
	[NSValueTransformer setValueTransformer:trans
									forName:@"SliderZoomValueTransformer"];
}

// Required: transformed value class
//-----------------------------------------------------------------------------------------------
+ (Class)transformedValueClass
{
    return [NSNumber class];
}

// Required: allow reverse transformation flag
//-----------------------------------------------------------------------------------------------
+ (BOOL)allowsReverseTransformation
{
    return YES;
}

// ***** INSTANCE METHODS *****

// Required: model-control transformation
//-----------------------------------------------------------------------------------------------
- (id)transformedValue:(id)value
{
	// slider in range 0 - 16
	if (!value)
		return value;
	
	if ([value respondsToSelector: @selector(floatValue)])
	{
		float zoom = [value floatValue];
		
		if (zoom < 1)
			return [NSNumber numberWithFloat:zoom * 8];
		
		return [NSNumber numberWithFloat:(zoom - 1) * 4 + 8];
	}
	return value;
}

// Required: control-model transformation
//-----------------------------------------------------------------------------------------------
- (id)reverseTransformedValue:(id)value
{
	if (!value)
		return value;
	
	if ([value respondsToSelector: @selector(floatValue)])
	{
		float zoom = [value floatValue];
		if (zoom < 8)
			return [NSNumber numberWithFloat:zoom / 8];
		else
			return [NSNumber numberWithFloat:1 + ((zoom - 8) / 4)];
	}
	return value;
}

@end
