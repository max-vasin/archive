//===============================================================================================
//  File:		Array2D.m
//  Project:	OriginGameToolkit
//	Desc:		2D array class implementation
//
//  Created by Max Vasin on 4/28/11.
//  Copyright 2011 Origin Games. All rights reserved.
//===============================================================================================

// ****** IMPORTS ******

#import "Array2D.h"

// ****** PRIVATE METHODS ******

@interface Array2D (Private)

- (BOOL)replaceObjectAtX:(oriUint)x Y:(oriUint)y WithObject:(id)object InMemory:(id*)mem MemSize:(oriIntSize)size;

@end


//###############################################################################################
// Array2D IMPLEMENTATION
//###############################################################################################




@implementation Array2D

//###############################################################################################
// PROPERTIES
//###############################################################################################

// Array X dimension
//-----------------------------------------------------------------------------------------------
- (oriUint)propDimX
{
	return m_dimX;
}

// Array Y dimension
//-----------------------------------------------------------------------------------------------
- (oriUint)propDimY
{
	return m_dimY;
}

// Array total elements
//-----------------------------------------------------------------------------------------------
- (oriUint)propMemorySize
{
	return m_dimY * m_dimX;
}

// Array total elements
//-----------------------------------------------------------------------------------------------
- (oriUint)propCount
{
	return m_dimY * m_dimX;
}


//###############################################################################################
// INITIALIZERS\DESTRUCTORS
//###############################################################################################

// Initializer with delegate object
//-----------------------------------------------------------------------------------------------
- (id)initWithDelegate:(id<Array2DDelegate>)delegate
{
	if ( (self = [self init]) )
	{
		m_delegate = delegate;
	}
	return self;
}

// Destructor
//-----------------------------------------------------------------------------------------------
- (void)dealloc
{
	if (m_memory)
	{
		for (int i = 0; i < self.propMemorySize; i++)
			[m_memory[i] release];
		
		SAFE_FREE(m_memory);
	}
	
    [super dealloc];
}

//###############################################################################################
// ARRAY CONTROL
//###############################################################################################

// Resizes array to given dimensions
//-----------------------------------------------------------------------------------------------
- (void)resizeToX:(oriUint)x Y:(oriUint)y Create:(BOOL)create
{
	int totalNew	= y * x;
	int totalExist	= m_dimX * m_dimY;
	
	id* newBlock = (totalNew) ? calloc(totalNew, sizeof(id)) : NULL;
	
	if (newBlock)
		memset(newBlock, 0, sizeof(id) * totalNew);
	
	oriIntSize memSize = {x, y};
	
	if (m_memory && newBlock)
	{
		for (int iy = 0; iy < m_dimY; iy++)
			for (int ix = 0; ix < m_dimX; ix++)
				[self replaceObjectAtX:ix Y:iy WithObject:m_memory[iy * m_dimX + ix] InMemory:newBlock MemSize:memSize];
	}
	
	if (create && m_delegate && newBlock)
	{
		for (int iy = 0; iy < y; iy++)
			for (int ix = 0; ix < x; ix++)
			{
				if (!newBlock[iy * x + ix])
				{
					id newObject = [m_delegate createInstanceOfArray:self X:ix Y:iy DimX:x DimY:y];
					newBlock[iy * x + ix]	= newObject;
				}
			}
	}
	
	if (m_memory)
	{
		for (int i = 0; i < totalExist; i++)
			[m_memory[i] release];
		
		SAFE_FREE(m_memory);
	}
	
	m_memory = newBlock;
	m_dimX	 = x;
	m_dimY	 = y;
}

// Replaces object at XY with new object
//-----------------------------------------------------------------------------------------------
- (void)replaceAtX:(oriUint)x Y:(oriUint)y With:(id)object
{
	oriIntSize memSize = {m_dimX, m_dimY};
	
	[self replaceObjectAtX:x Y:y WithObject:object InMemory:m_memory MemSize:memSize];
}

// Returns object at XY
//-----------------------------------------------------------------------------------------------
- (id)getObjectAtX:(oriUint)x Y:(oriUint)y
{
	int place = y * m_dimX + x;
	
	if (place >= self.propMemorySize)
		return nil;
	
	return m_memory[place];
}

// Returns object at flat index
//-----------------------------------------------------------------------------------------------
- (id)getObjectAtIndex:(oriUint)index
{
	if (index >= self.propMemorySize)
		return nil;
	
	return m_memory[index];
}

// Returns X:Y index of object
//-----------------------------------------------------------------------------------------------
- (oriIntPoint)indexOfObject:(id)object
{
	int index = [self flatIndexOfObject:object];
	
	if (index < 0)
	{
		oriIntPoint invalidIndex = {-1, -1};
		
		return invalidIndex;
	}
	
	oriIntPoint result;
	
	result.y = floor(index / m_dimX);
	result.x = index - (result.y * m_dimX);
		
	return result;
}

// Returns flat index of object
//-----------------------------------------------------------------------------------------------
- (int)flatIndexOfObject:(id)object
{
	for (int i = 0; i < (m_dimX * m_dimY); i++)
	{
		if ([m_memory[i] isEqual:object])
			return i;
	}
	
	return -1;
}

//###############################################################################################
// PRIVATE METHODS
//###############################################################################################

// Replacing existing object with new object if needed in memory location
//-----------------------------------------------------------------------------------------------
- (BOOL)replaceObjectAtX:(oriUint)x Y:(oriUint)y WithObject:(id)object InMemory:(id*)mem MemSize:(oriIntSize)size
{
	if ((x >= size.width) || (y >= size.height))
		return NO;
	
	int place = y * size.width + x;
	
	if (mem[place] == object)
		return YES;
	
	if (mem[place])
		[mem[place] release];
	
	mem[place] = [object retain];
	
	return YES;
}



@end
