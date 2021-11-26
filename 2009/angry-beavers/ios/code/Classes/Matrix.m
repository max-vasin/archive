//===============================================================================================
//  File:		Matrix.m
//  Project:	Fenix
//  Desc:		Matrix utility class implementation
//
//  Created by Max Vasin on 11/23/10.
//  Copyright 2010 Origin. All rights reserved.
//===============================================================================================

// **** IMPORTS ****

#import "Matrix.h"

// **** IMPLEMENTATION ****

@implementation Matrix

// **** IMPLEMENTED PROPERTIES ****

// propData
//-----------------------------------------------------------------------------------------------

- (void)setPropData:(oriFloat*)data
{
	if (!data)
		[NSException raise:@"Invalid pointer" format:@"invalid pointer to data passed to setter of property (propData)"];
	
	memcpy(m, data, sizeof(oriFloat) * 16);
}

- (oriFloat*)propData
{
	return (oriFloat*)m;
}


// init
// Matrix default construction
//-----------------------------------------------------------------------------------------------

- (id)init
{
	if ( (self = [super init]) )
	{
		[self makeIdentity];
	}
	return self;
}

// makeIdentity
// Makes current matrix identity matrix
//-----------------------------------------------------------------------------------------------

- (Matrix*)makeIdentity
{
	memset(m, 0, sizeof(oriFloat) * 16);
	m[0][0] = m[1][1] = m[2][2] = m[3][3] = 1.0f;
	return self;
}

// makeTranslation
// Makes current matrix translation matrix with given axis offsets
//-----------------------------------------------------------------------------------------------

- (Matrix*)makeTranslationByX:(oriFloat)x Y:(oriFloat)y Z:(oriFloat)z
{
	[self makeIdentity];
	
	m[3][0]	= x;
	m[3][1] = y;
	m[3][2] = z;
	
	return self;
}

// makeScale
// Makes current matrix scale matrix with given axis offsets
//-----------------------------------------------------------------------------------------------

- (Matrix*)makeScaleByX:(oriFloat)x Y:(oriFloat)y Z:(oriFloat)z
{
	[self makeIdentity];
	
	m[0][0]	= x;
	m[1][1] = y;
	m[2][2] = z;
	
	return self;
}

// makeRotationX
// Makes current object with rotation matrix around X axis on given angle
//-----------------------------------------------------------------------------------------------

- (Matrix*)makeRotationX:(oriFloat)angle
{
	[self makeIdentity];
	
	oriFloat	s = sinf(angle);
	oriFloat c = cosf(angle);
	
	m[1][1] = m[2][2] = c;
	m[2][1] = -s;
	m[1][2] = s;
	
	return self;
}

// makeRotationY
// Makes current object with rotation matrix around Y axis on given angle
//-----------------------------------------------------------------------------------------------

- (Matrix*)makeRotationY:(oriFloat)angle
{
	[self makeIdentity];
	
	oriFloat	s = sinf(angle);
	oriFloat c = cosf(angle);
	
	m[0][0] = m[2][2] = c;
	m[0][2] = -s;
	m[2][0] = s;
	
	return self;
}

// makeRotationZ
// Makes current object with rotation matrix around Z axis on given angle
//-----------------------------------------------------------------------------------------------

- (Matrix*)makeRotationZ:(oriFloat)angle
{
	[self makeIdentity];
	
	oriFloat	s = sinf(angle);
	oriFloat c = cosf(angle);
	
	m[0][0] = m[1][1] = c;
	m[1][0] = -s;
	m[0][1] = s;
	
	return self;
}

// makeOrthoLH
// Makes orthogonal matrix (left-handed)
//-----------------------------------------------------------------------------------------------

- (Matrix*)makeOrthoLHByWidth:(oriFloat)width Height:(oriFloat)height Near:(oriFloat)near Far:(oriFloat)far
{
	memset(m, 0, sizeof(oriFloat) * 16);
	
	m[0][0]	= 2.0f / width;
	m[1][1] = 2.0f / height;
	m[2][2] = 1.0f / (far - near);
	m[3][2] = near / (near - far);
	m[3][3] = 1.0f;
	
	return self;
}

// makeOrthoRH
// Makes orthogonal matrix (right-handed)
//-----------------------------------------------------------------------------------------------

- (Matrix*)makeOrthoRHByWidth:(oriFloat)width Height:(oriFloat)height Near:(oriFloat)near Far:(oriFloat)far
{
	memset(m, 0, sizeof(oriFloat) * 16);
	
	m[0][0]	= 2.0f / width;
	m[1][1] = 2.0f / height;
	m[2][2] = 1.0f / (near - far);
	m[3][2] = near / (near - far);
	m[3][3] = 1.0f;
	
	return self;
}

// makePerspectiveLHByFOV
// Makes perspective matrix with field of view (left-handed)
//-----------------------------------------------------------------------------------------------

- (Matrix*)makePerspectiveLHByFOV:(oriFloat)fov Aspect:(oriFloat)aspect Near:(oriFloat)near Far:(oriFloat)far
{
	oriFloat	height	= 1.0f / tanf(fov * 0.5f);
	oriFloat width	= height / aspect;
	
	memset(m, 0, sizeof(oriFloat) * 16);
	
	m[0][0]	= width;
	m[1][1] = height;
	m[2][2] = far / (far - near);
	m[2][3] = 1.0f;
	m[3][2] = -near * far / (far - near);
	m[3][3] = 0.0f;
	
	return self;
}

// makePerspectiveLHByFOV
// Makes perspective matrix with field of view (right-handed)
//-----------------------------------------------------------------------------------------------

- (Matrix*)makePerspectiveRHByFOV:(oriFloat)fov Aspect:(oriFloat)aspect Near:(oriFloat)near Far:(oriFloat)far
{
	oriFloat	height	= 1.0f / tanf(fov * 0.5f);
	oriFloat width	= height / aspect;
	
	memset(m, 0, sizeof(oriFloat) * 16);
	
	m[0][0]	= width;
	m[1][1] = height;
	m[2][2] = far / (near - far);
	m[2][3] = -1.0f;
	m[3][2] = near * far / (near - far);
	m[3][3] = 0.0f;
	
	return self;
}

// createMultiplyWith
// Creates new matrix as result of multiplication this object with anoter matrix
//-----------------------------------------------------------------------------------------------

- (Matrix*)multiplyWith:(Matrix*)mx
{
	Matrix* resultMatrix = [[Matrix alloc] init];
	
	int i, j;
	
	for (i = 0; i < 4; i++)
		for (j = 0; j < 4; j++)
		{
			resultMatrix->m[i][j] = m[i][0] * mx->m[0][j] + 
									m[i][1] * mx->m[1][j] + 
									m[i][2] * mx->m[2][j] + 
									m[i][3] * mx->m[3][j]; 
		}
	
	return [resultMatrix autorelease];
}

@end
