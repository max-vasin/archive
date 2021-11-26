//=======================================================================================================================
//  File:		IntegerArray2D.m
//  Project:	heli
//	Desc:		Integers 2-dimensional array class implementation
//
//  Created by Max Vasin on 5/30/11.
//  Copyright 2011 Origin. All rights reserved.
//=======================================================================================================================

// ******** IMPORTS ********

#import "IntegerArray2D.h"

// ******** IMPLEMENTATION ********

@implementation IntegerArray2D

// ******** PROPERTIES ********

//-----------------------------------------------------------------------------------------------
// SYNTHESIZED
//-----------------------------------------------------------------------------------------------

@synthesize propDimX	= m_dimX;
@synthesize propDimY	= m_dimY;

//-----------------------------------------------------------------------------------------------
// PROPERTY: total array elements count
//-----------------------------------------------------------------------------------------------

- (oriUint)propTotal
{
	return m_dimX * m_dimY;
}

//-----------------------------------------------------------------------------------------------
// PROPERTY: array memory consumption
//-----------------------------------------------------------------------------------------------

- (oriUint)propMemory
{
	return m_dimX * m_dimY * sizeof(oriUint);
}


//###############################################################################################
// ARRAY CONTROL
//###############################################################################################

//-----------------------------------------------------------------------------------------------
// Resizes array to given dimensions
//-----------------------------------------------------------------------------------------------

- (void)resizeToX:(oriUint)x Y:(oriUint)y
{
	if ( (x == m_dimX) && (y == m_dimY) )
		return;
	
	oriInt totalNew		= y * x;
	oriUint* newBlock	= (totalNew) ? calloc(totalNew, sizeof(oriUint)) : NULL;
	
	if (newBlock)
		memset(newBlock, 0, sizeof(oriUint) * totalNew);
	
	if (m_memory && newBlock)
	{
		oriInt borderX = MIN(m_dimX, x);
		oriInt borderY = MIN(m_dimY, y);
		
		for (int iy = 0; iy < borderX; iy++)
			for (int ix = 0; ix < borderY; ix++)
				newBlock[iy * x + ix] = m_memory[iy * m_dimX + ix];
	}
	
	SAFE_FREE(m_memory);
	m_memory = newBlock;

	m_dimX	 = x;
	m_dimY	 = y;
}

//-----------------------------------------------------------------------------------------------
// Returns value at given X:Y index
//-----------------------------------------------------------------------------------------------

- (oriUint)valueAtX:(oriUint)x Y:(oriUint)y
{
	if ( (x >= m_dimX) || (y >= m_dimY) || !m_memory )
		return NSNotFound;
	
	return m_memory[y * m_dimX + x];
}

//-----------------------------------------------------------------------------------------------
// Sets value at give X:Y index
//-----------------------------------------------------------------------------------------------

- (void)setValue:(oriUint)value AtX:(oriUint)x Y:(oriUint)y
{
	if ( (x >= m_dimX) || (y >= m_dimY) || !m_memory )
		return;
	
	m_memory[y * m_dimX + x] = value;
}

@end
