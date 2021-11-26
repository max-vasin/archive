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

- (void)setPropData:(GLfloat *)data
{
	if (!data)
		[NSException raise:@"Invalid pointer" format:@"invalid pointer to data passed to setter of property (propData)"];
	
	memcpy(m, data, sizeof(GLfloat) * 16);
}

- (GLfloat*)propData
{
	return (GLfloat*)m;
}


// init
// Matrix default construction
//-----------------------------------------------------------------------------------------------

- (id)init
{
	if ( (self = [super init]) )
	{
		[self makeIdentity];
		
		DBG_START	[self log:"$Cobject created\n"];	DBG_END
	}
	return self;
}

// makeIdentity
// Makes current matrix identity matrix
//-----------------------------------------------------------------------------------------------

- (Matrix*)makeIdentity
{
	memset(m, 0, sizeof(GLfloat) * 16);
	m[0][0] = m[1][1] = m[2][2] = m[3][3] = 1.0f;
	return self;
}

// makeTranslation
// Makes current matrix translation matrix with given axis offsets
//-----------------------------------------------------------------------------------------------

- (Matrix*)makeTranslationByX:(GLfloat)x Y:(GLfloat)y Z:(GLfloat)z
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

- (Matrix*)makeScaleByX:(GLfloat)x Y:(GLfloat)y Z:(GLfloat)z
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

- (Matrix*)makeRotationX:(GLfloat)angle
{
	[self makeIdentity];
	
	GLfloat	s = sinf(angle);
	GLfloat c = cosf(angle);
	
	m[1][1] = m[2][2] = c;
	m[2][1] = -s;
	m[1][2] = s;
	
	return self;
}

// makeRotationY
// Makes current object with rotation matrix around Y axis on given angle
//-----------------------------------------------------------------------------------------------

- (Matrix*)makeRotationY:(GLfloat)angle
{
	[self makeIdentity];
	
	GLfloat	s = sinf(angle);
	GLfloat c = cosf(angle);
	
	m[0][0] = m[2][2] = c;
	m[0][2] = -s;
	m[2][0] = s;
	
	return self;
}

// makeRotationZ
// Makes current object with rotation matrix around Z axis on given angle
//-----------------------------------------------------------------------------------------------

- (Matrix*)makeRotationZ:(GLfloat)angle
{
	[self makeIdentity];
	
	GLfloat	s = sinf(angle);
	GLfloat c = cosf(angle);
	
	m[0][0] = m[1][1] = c;
	m[1][0] = -s;
	m[0][1] = s;
	
	return self;
}

// makeOrthoLH
// Makes orthogonal matrix (left-handed)
//-----------------------------------------------------------------------------------------------

- (Matrix*)makeOrthoLHByWidth:(GLfloat)width Height:(GLfloat)height Near:(GLfloat)near Far:(GLfloat)far
{
	memset(m, 0, sizeof(GLfloat) * 16);
	
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

- (Matrix*)makeOrthoRHByWidth:(GLfloat)width Height:(GLfloat)height Near:(GLfloat)near Far:(GLfloat)far
{
	memset(m, 0, sizeof(GLfloat) * 16);
	
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

- (Matrix*)makePerspectiveLHByFOV:(GLfloat)fov Aspect:(GLfloat)aspect Near:(GLfloat)near Far:(GLfloat)far
{
	GLfloat	height	= 1.0f / tanf(fov * 0.5f);
	GLfloat width	= height / aspect;
	
	memset(m, 0, sizeof(GLfloat) * 16);
	
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

- (Matrix*)makePerspectiveRHByFOV:(GLfloat)fov Aspect:(GLfloat)aspect Near:(GLfloat)near Far:(GLfloat)far
{
	GLfloat	height	= 1.0f / tanf(fov * 0.5f);
	GLfloat width	= height / aspect;
	
	memset(m, 0, sizeof(GLfloat) * 16);
	
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
