//===============================================================================================
//  File:		FloatStepperTransformer.m
//  Project:	OriginGameToolkit
//	Desc:		Interger value clamping to -1 - 1 floating point with 10000 precision
//
//  Created by Max Vasin on 4/21/11.
//  Copyright 2011 Origin Games. All rights reserved.
//===============================================================================================

// ***** IMPORTS *****

#import "FloatStepperTransformer.h"

// ***** IMPLEMENTATION *****

@implementation FloatStepperTransformer

// ***** CLASS METHODS *****

// Transformer self-registering
//-----------------------------------------------------------------------------------------------
+ (void)registerSelf
{
	FloatStepperTransformer* trans = [[[self alloc] init] autorelease];
	
	// register it with the name that we refer to it with
	[NSValueTransformer setValueTransformer:trans
									forName:@"FloatStepperTransformer"];
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
	if (!value)
		return value;
	
	if ([value respondsToSelector: @selector(floatValue)])
	{
		float flVal = [value floatValue];
		
		return [NSNumber numberWithInt:(int)(flVal * 1000)];
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
		int intVal = [value intValue];
		
		return [NSNumber numberWithFloat:(float)intVal / 1000];
	}
	return value;
}

@end
