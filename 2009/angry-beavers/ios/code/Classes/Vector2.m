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

- (void)setPropData:(oriFloat*)data
{
	if (!data)
		[NSException raise:@"Invalid pointer" format:@"invalid pointer to data passed to setter of property (propData)"];
	
	memcpy(&x, data, sizeof(oriFloat) * 3);
}

- (oriFloat*)propData
{
	return (oriFloat*)&x;
}

// propLength
//-----------------------------------------------------------------------------------------------

- (oriFloat)propLength
{
	return sqrtf(x * x + y * y);
}

// propLengthSq
//-----------------------------------------------------------------------------------------------

- (oriFloat)propLengthSq
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

+ (id)createWithX:(oriFloat)fx Y:(oriFloat)fy
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

// vector
// Class method - creates new instance of zero vector
//-----------------------------------------------------------------------------------------------

+ (id)vector
{
	return [[[self alloc] init] autorelease];
}

// vectorWithX:Y
// Class method - creates new instance of vector with components (autorelease)
//-----------------------------------------------------------------------------------------------

+ (id)vectorWithX:(oriFloat)fx Y:(oriFloat)fy
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
		memset(&x, 0, sizeof(oriFloat) * 3);
	}
	return self;
}

// initWith
// Vector constructor with arguments
//-----------------------------------------------------------------------------------------------

- (id)initWithX:(oriFloat)fx Y:(oriFloat)fy
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

// makeNormalize
// Normalizes vector
//-----------------------------------------------------------------------------------------------

- (Vector2*)makeNormalize
{
	oriFloat length = sqrtf(x * x + y * y);
	
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

- (Vector2*)makeScaleTo:(oriFloat)scale
{
	x *= scale;
	y *= scale;
	
	return self;
}

// makeLerpTo
// Linearly interpolates current vector to given vector with factor
//-----------------------------------------------------------------------------------------------

- (Vector2*)makeLerpTo:(Vector2*)vector withFactor:(oriFloat)k
{
	oriFloat lx = (vector->x - x) * k;
	oriFloat ly = (vector->y - y) * k;
	
	x += lx;
	y += ly;
	
	return self;
}

// makeTransformBy
// Transform current vector with given matrix with translation
//-----------------------------------------------------------------------------------------------

- (Vector2*)makeTransformBy:(Matrix*)mx
{
	oriFloat lx = x * mx->m[0][0] + y * mx->m[1][0] + mx->m[2][0] + mx->m[3][0];
	oriFloat ly = x * mx->m[0][1] + y * mx->m[1][1] + mx->m[2][1] + mx->m[3][1];
	
	x = lx;
	y = ly;
	
	return self;
}


// makeTransformNormalBy
// Transform current vector with given matrix without translation
//-----------------------------------------------------------------------------------------------

- (Vector2*)makeTransformNormalBy:(Matrix*)mx
{
	oriFloat lx = x * mx->m[0][0] + y * mx->m[1][0] + mx->m[2][0];
	oriFloat ly = x * mx->m[0][1] + y * mx->m[1][1] + mx->m[2][1];
	
	x = lx;
	y = ly;
	
	return self;	
}

// makeRotateTo
// Rotates current vector to given angle
//-----------------------------------------------------------------------------------------------

- (Vector2*)makeRotateTo:(oriFloat)angle
{
	oriFloat lx = x * cosf(angle) - y * sinf(angle);
	oriFloat ly = x * sinf(angle) + y * cosf(angle);
	
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

- (oriFloat)dotWith:(Vector2*)vector
{
	return x * vector->x + y * vector->y;
}

// angleFullWith
// Computes angle bitween two vectors. Angle always poitive in range 0 - 2PI (CCW)
//-----------------------------------------------------------------------------------------------

- (oriFloat)angleFullWith:(Vector2*)vector
{
	oriFloat mag = self.propLength * vector.propLength;
	
	if (!mag)
		return 0;
	
	oriFloat dot   = [self dotWith:vector];
	oriFloat sign  = x * vector->y - y * vector->x; // 3rd component of 3D vector in Z-plane
	oriFloat angle = acosf(dot / mag);
	
	if (sign < 0)
		return 2*PI-angle;
	
	return angle;
	//return atan2(vector->y, vector->x) - atan2(y, x);
}

// angleWith
// Computes angle bitween two vectors
//-----------------------------------------------------------------------------------------------

- (oriFloat)angleWith:(Vector2*)vector
{
	oriFloat mag = self.propLength * vector.propLength;
	
	if (!mag)
		return 0;
	
	oriFloat dot   = [self dotWith:vector];
	oriFloat sign  = x * vector->y - y * vector->x; // 3rd component of 3D vector in Z-plane
	oriFloat angle = acosf(dot / mag);
	
	if (sign < 0)
		return -angle;
	
	return angle;
}

@end
