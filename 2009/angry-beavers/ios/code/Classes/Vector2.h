//===============================================================================================
//  File:		Vector2.h
//  Project:	Fenix
//	Desc:		2D vector utility class interface
//
//  Created by Max Vasin on 11/24/10.
//  Copyright 2010 Origin. All rights reserved.
//===============================================================================================

// **** IMPORTS ****

#import "OriBase.h"

// Forwarding Matrix class

@class Matrix;

// **** INTERFACE ****

@interface Vector2 : NSObject
{
@public
	oriFloat	x, y;
}

// **** PROPERTIES ****

@property				oriFloat*	propData;
@property (readonly)	oriFloat		propLength;
@property (readonly)	oriFloat		propLengthSq;

// **** CLASS METHODS ****

// Retained makers

+ (id)			create;
+ (id)			createWithX:			(oriFloat)fx Y:(oriFloat)fy;
+ (id)			createWithPointStart:	(Vector2*)start End:(Vector2*)end;

// Autoreleased makers

+ (id)			vector;
+ (id)			vectorWithX:			(oriFloat)fx Y:(oriFloat)fy;
+ (id)			vectorWithPointStart:	(Vector2*)start End:(Vector2*)end;

// **** INSTANCE METHODS ****

// Initializers

- (id)			initWithX:				(oriFloat)fx Y:(oriFloat)fy;
- (id)			initWithPointsStart:	(Vector2*)start End:(Vector2*)end;

// Math

- (Vector2*)	makeNormalize;
- (Vector2*)	makeInvert;

- (Vector2*)	makeScaleTo:			(oriFloat)scale;
- (Vector2*)	makeLerpTo:				(Vector2*)vector withFactor:(oriFloat)k;
- (Vector2*)	makeTransformBy:		(Matrix*)mx;
- (Vector2*)	makeTransformNormalBy:	(Matrix*)mx;
- (Vector2*)	makeRotateTo:			(oriFloat)angle;
- (Vector2*)	makeDirectionStart:		(Vector2*)start End:(Vector2*)end;

- (oriFloat)	dotWith:				(Vector2*)vector;

- (Vector2*)	tangentVector;

- (oriFloat)	angleWith:				(Vector2*)vector;
- (oriFloat)	angleFullWith:			(Vector2*)vector;

@end
