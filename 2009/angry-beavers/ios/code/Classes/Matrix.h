//===============================================================================================
//  File:		Matrix.h
//  Project:	Fenix
//  Desc:		4x4 Matrix utility class. Provides methods to work with 3D transformation
//				matrices.
//
//  Created by Max Vasin on 11/23/10.
//  Copyright 2010 Origin. All rights reserved.
//===============================================================================================

// **** IMPORTS ****

#import "OriBase.h"

// **** INTERFACE ****

@interface Matrix : NSObject
{
@public
	oriFloat m[4][4];
}

// **** PROPERTIES ****

@property GLfloat*	propData;

// **** INSTANCE METHODS *****

// Transformation matrices

- (Matrix*)	makeIdentity;
- (Matrix*)	makeTranslationByX:		(oriFloat)x Y:(oriFloat)y Z:(oriFloat)z;
- (Matrix*)	makeScaleByX:			(oriFloat)x Y:(oriFloat)y Z:(oriFloat)z;
- (Matrix*) makeRotationX:			(oriFloat)angle;
- (Matrix*) makeRotationY:			(oriFloat)angle;
- (Matrix*) makeRotationZ:			(oriFloat)angle;

// Projection matrices

- (Matrix*)	makeOrthoLHByWidth:		(oriFloat)width Height:(oriFloat)height Near:(oriFloat)near Far:(oriFloat)far;
- (Matrix*)	makeOrthoRHByWidth:		(oriFloat)width Height:(oriFloat)height Near:(oriFloat)near Far:(oriFloat)far;
- (Matrix*) makePerspectiveLHByFOV:	(oriFloat)fov   Aspect:(oriFloat)aspect Near:(oriFloat)near Far:(oriFloat)far;
- (Matrix*) makePerspectiveRHByFOV:	(oriFloat)fov   Aspect:(oriFloat)aspect Near:(oriFloat)near Far:(oriFloat)far;

// Mathematics

- (Matrix*) multiplyWith:			(Matrix*)mx;

@end
