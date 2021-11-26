//===============================================================================================
//  File:		FloatStepperFormatter.h
//  Project:	OriginGameToolkit
//	Desc:		Interger value clamping to -1 - 1 floating point with 10000 precision
//
//  Created by Max Vasin on 4/21/11.
//  Copyright 2011 Origin Games. All rights reserved.
//===============================================================================================

// ***** IMPORTS *****

#import <Foundation/Foundation.h>

// ***** INTERFACE *****

@interface FloatStepperTransformer : NSValueTransformer 

// ***** CLASS METHODS *****

+ (void)registerSelf;

@end
