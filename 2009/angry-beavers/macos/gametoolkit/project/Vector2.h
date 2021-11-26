//===============================================================================================
//  File:		Vector2.h
//  Project:	Fenix
//	Desc:		2D vector utility class interface
//
//  Created by Max Vasin on 11/24/10.
//  Copyright 2010 Origin. All rights reserved.
//===============================================================================================

// **** IMPORTS ****

#import <Foundation/Foundation.h>
#import "BaseObject.h"

// Forwarding Matrix class

@class Matrix;

// **** INTERFACE ****


@interface Vector2 : BaseObject 
{
@public
	GLfloat	x, y;
}

// **** PROPERTIES ****

@property				GLfloat*	propData;
@property (readonly)	GLfloat		propLength;
@property (readonly)	GLfloat		propLengthSq;

// **** CLASS METHODS ****

// Retained makers

+ (id)			create;
+ (id)			createWithX:			(GLfloat)fx Y:(GLfloat)fy;
+ (id)			createWithPointStart:	(Vector2*)start End:(Vector2*)end;

// Autoreleased makers

+ (id)			vectorWithX:			(GLfloat)fx Y:(GLfloat)fy;
+ (id)			vectorWithPointStart:	(Vector2*)start End:(Vector2*)end;

// **** INSTANCE METHODS ****

// Initializers

- (id)			initWithX:				(GLfloat)fx Y:(GLfloat)fy;
- (id)			initWithPointsStart:	(Vector2*)start End:(Vector2*)end;

// Math

- (Vector2*)	sub:					(Vector2*)vec;
- (Vector2*)	add:					(Vector2*)vec;
- (Vector2*)	scale:					(float)scale;

- (Vector2*)	makeNormalize;
- (Vector2*)	makeInvert;

- (Vector2*)	makeScaleTo:			(GLfloat)scale;
- (Vector2*)	makeLerpTo:				(Vector2*)vector withFactor:(GLfloat)k;
- (Vector2*)	makeTransformBy:		(Matrix*)mx;
- (Vector2*)	makeTransformNormalBy:	(Matrix*)mx;
- (Vector2*)	makeRotateTo:			(GLfloat)angle;
- (Vector2*)	makeDirectionStart:		(Vector2*)start End:(Vector2*)end;

- (GLfloat)		dotWith:				(Vector2*)vector;

- (Vector2*)	tangentVector;

@end
