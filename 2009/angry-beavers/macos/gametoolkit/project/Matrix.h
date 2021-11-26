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

#import <Foundation/Foundation.h>
#import "BaseObject.h"

// **** INTERFACE ****

@interface Matrix : BaseObject 
{
@public
	GLfloat m[4][4];
}

// **** PROPERTIES ****

@property GLfloat*	propData;

// **** INSTANCE METHODS *****

// Transformation matrices

- (Matrix*)	makeIdentity;
- (Matrix*)	makeTranslationByX:		(GLfloat)x Y:(GLfloat)y Z:(GLfloat)z;
- (Matrix*)	makeScaleByX:			(GLfloat)x Y:(GLfloat)y Z:(GLfloat)z;
- (Matrix*) makeRotationX:			(GLfloat)angle;
- (Matrix*) makeRotationY:			(GLfloat)angle;
- (Matrix*) makeRotationZ:			(GLfloat)angle;

// Projection matrices

- (Matrix*)	makeOrthoLHByWidth:		(GLfloat)width Height:(GLfloat)height Near:(GLfloat)near Far:(GLfloat)far;
- (Matrix*)	makeOrthoRHByWidth:		(GLfloat)width Height:(GLfloat)height Near:(GLfloat)near Far:(GLfloat)far;
- (Matrix*) makePerspectiveLHByFOV:	(GLfloat)fov   Aspect:(GLfloat)aspect Near:(GLfloat)near Far:(GLfloat)far;
- (Matrix*) makePerspectiveRHByFOV:	(GLfloat)fov   Aspect:(GLfloat)aspect Near:(GLfloat)near Far:(GLfloat)far;

// Mathematics

- (Matrix*) multiplyWith:			(Matrix*)mx;

@end
