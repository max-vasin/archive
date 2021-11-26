//===============================================================================================
//  File:		Vector2.m
//  Project:	Fenix
//	Desc:		Vector2 class implementation
//
//  Created by Max Vasin on 11/24/10.
//  Copyright 2010 Origin. All rights reserved.
//===============================================================================================

// **** IMPORTS ****

#import "Vector2.h"
#import "Matrix.h"

// **** IMPLEMENTATION ****

@implementation Vector2

// **** IMPLEMENTED PROPERTIES ****

// propData
//-----------------------------------------------------------------------------------------------

- (void)setPropData:(GLfloat *)data
{
	if (!data)
		[NSException raise:@"Invalid pointer" format:@"invalid pointer to data passed to setter of property (propData)"];
	
	memcpy(&x, data, sizeof(GLfloat) * 3);
}

- (GLfloat*)propData
{
	return (GLfloat*)&x;
}

// propLength
//-----------------------------------------------------------------------------------------------

- (GLfloat)propLength
{
	return sqrtf(x * x + y * y);
}

// propLengthSq
//-----------------------------------------------------------------------------------------------

- (GLfloat)propLengthSq
{
	return (x * x + y * y);
}

// **** CLASS METHODS IMPLEMENTATION ****

// create
// Class method - creates new instance of vector with 0,0 components
//-----------------------------------------------------------------------------------------------

+ (id)create
{
	return [[Vector2 alloc] init];
}

// createWithX:Y
// Class method - creates new instance of vector with components
//-----------------------------------------------------------------------------------------------

+ (id)createWithX:(GLfloat)fx Y:(GLfloat)fy
{
	return [(Vector2*)[Vector2 alloc] initWithX:fx Y:fy];
}

// createWithPointStart:End
// Class method - creates new instance of vector with start and end points
//-----------------------------------------------------------------------------------------------

+ (id)createWithPointStart:(Vector2*)start End:(Vector2*)end
{
	return [[Vector2 alloc] initWithPointsStart:start End:end];
}

// vectorWithX:Y
// Class method - creates new instance of vector with components (autorelease)
//-----------------------------------------------------------------------------------------------

+ (id)vectorWithX:(GLfloat)fx Y:(GLfloat)fy
{
	return [[(Vector2*)[Vector2 alloc] initWithX:fx Y:fy] autorelease];
}

// vectorWithStart:End
// Class method - creates new instance of vector with start and end points (autorelease)
//-----------------------------------------------------------------------------------------------

+ (id)vectorWithPointStart:(Vector2*)start End:(Vector2*)end
{
	return [[[Vector2 alloc] initWithPointsStart:start End:end] autorelease];
}

// **** INSTANCE METHODS IMPLEMENTATION ****

// init
// Vector default construction
//-----------------------------------------------------------------------------------------------

- (id)init
{
	if ( (self = [super init]) )
	{
		memset(&x, 0, sizeof(GLfloat) * 3);
	}
	return self;
}

// initWith
// Vector constructor with arguments
//-----------------------------------------------------------------------------------------------

- (id)initWithX:(GLfloat)fx Y:(GLfloat)fy
{
	if ( (self = [super init]) )
	{
		x = fx;
		y = fy;
	}
	return self;
}

// initWithPoints
// Initializes vector with two points
//-----------------------------------------------------------------------------------------------

- (id)initWithPointsStart:(Vector2*)start End:(Vector2*)end
{
	return [self initWithX:(end->x - start->x) Y:(end->y - start->y)]; 
}

// copyWithZone
// Coping protocol support
//-----------------------------------------------------------------------------------------------

- (id)copyWithZone:(NSZone*)zone
{
	return NSCopyObject(self, 0, zone);
}

// add
// Returns new vector as result of adding second vector
//-----------------------------------------------------------------------------------------------
- (Vector2*)add:(Vector2*)vec
{
	return [Vector2 vectorWithX:x + vec->x Y:y + vec->y];
}

// sub
// Returns new vector as result of subtracting second vector
//-----------------------------------------------------------------------------------------------
- (Vector2*)sub:(Vector2*)vec
{
	return [Vector2 vectorWithX:x - vec->x Y:y - vec->y];
}

// sub
// Returns new vector as result of scaling current
//-----------------------------------------------------------------------------------------------
- (Vector2*)scale:(float)scale
{
	return [Vector2 vectorWithX:x * scale Y:y * scale];
}

// makeNormalize
// Normalizes vector
//-----------------------------------------------------------------------------------------------

- (Vector2*)makeNormalize
{
	GLfloat length = sqrtf(x * x + y * y);
	
	if (length != 0.0f)
	{
		x /= length;
		y /= length;
	}
	
	return self;
}

// createTangent
// Creates new vector wich is tangent of current
//-----------------------------------------------------------------------------------------------

- (Vector2*)tangentVector
{
	Vector2* vec = [(Vector2*)[Vector2 alloc] initWithX:-y Y:x];
	return [vec autorelease];
}

// createInvert
// Vector inversion
//-----------------------------------------------------------------------------------------------

- (Vector2*)makeInvert
{
	x = -x;
	y = -y;
	
	return self;
}

// makeScaleTo
// Scales current vector with value
//-----------------------------------------------------------------------------------------------

- (Vector2*)makeScaleTo:(GLfloat)scale
{
	x *= scale;
	y *= scale;
	
	return self;
}

// makeLerpTo
// Linearly interpolates current vector to given vector with factor
//-----------------------------------------------------------------------------------------------

- (Vector2*)makeLerpTo:(Vector2*)vector withFactor:(GLfloat)k
{
	GLfloat lx = (vector->x - x) * k;
	GLfloat ly = (vector->y - y) * k;
	
	x += lx;
	y += ly;
	
	return self;
}

// makeTransformBy
// Transform current vector with given matrix with translation
//-----------------------------------------------------------------------------------------------

- (Vector2*)makeTransformBy:(Matrix*)mx
{
	GLfloat lx = x * mx->m[0][0] + y * mx->m[1][0] + mx->m[2][0] + mx->m[3][0];
	GLfloat ly = x * mx->m[0][1] + y * mx->m[1][1] + mx->m[2][1] + mx->m[3][1];
	
	x = lx;
	y = ly;
	
	return self;
}


// makeTransformNormalBy
// Transform current vector with given matrix without translation
//-----------------------------------------------------------------------------------------------

- (Vector2*)makeTransformNormalBy:(Matrix*)mx
{
	GLfloat lx = x * mx->m[0][0] + y * mx->m[1][0] + mx->m[2][0];
	GLfloat ly = x * mx->m[0][1] + y * mx->m[1][1] + mx->m[2][1];
	
	x = lx;
	y = ly;
	
	return self;	
}

// makeRotateTo
// Rotates current vector to given angle
//-----------------------------------------------------------------------------------------------

- (Vector2*)makeRotateTo:(GLfloat)angle
{
	GLfloat lx = x * cosf(angle) - y * sinf(angle);
	GLfloat ly = x * sinf(angle) + y * cosf(angle);
	
	x = lx;
	y = ly;
	
	return self;
}


// makeDirection
// Sets current vector as direction vector between two points
//-----------------------------------------------------------------------------------------------

- (Vector2*)makeDirectionStart:(Vector2*)start End:(Vector2*)end
{
	x = end->x - start->x;
	y = end->y - start->y;
	
	return self;
}

// dotWith
// Computes dot-product value of two vectors
//-----------------------------------------------------------------------------------------------

- (GLfloat)dotWith:(Vector2*)vector
{
	return x * vector->x + y * vector->y;
}

@end
