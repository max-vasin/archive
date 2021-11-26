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

- (BOOL)replaceObjectAtX:(uint)x Y:(uint)y WithObject:(id)object InMemory:(id*)mem MemSize:(IntSize)size;

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
- (uint)propDimX
{
	return m_dimX;
}

// Array Y dimension
//-----------------------------------------------------------------------------------------------
- (uint)propDimY
{
	return m_dimY;
}

// Array total elements
//-----------------------------------------------------------------------------------------------
- (uint)propMemorySize
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
- (void)resizeToX:(uint)x Y:(uint)y Create:(BOOL)create
{
	int totalNew	= y * x;
	int totalExist	= m_dimX * m_dimY;
		
	id* newBlock	= NULL;

	if (totalNew)
	{
		newBlock = calloc(totalNew, sizeof(id));
	
		memset(newBlock, 0, sizeof(id) * totalNew);
	}
		
	IntSize memSize = {x, y};
	
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
- (void)replaceAtX:(uint)x Y:(uint)y With:(id)object
{
	IntSize memSize = {m_dimX, m_dimY};
	
	[self replaceObjectAtX:x Y:y WithObject:object InMemory:m_memory MemSize:memSize];
}

// Returns object at XY
//-----------------------------------------------------------------------------------------------
- (id)getObjectAtX:(uint)x Y:(uint)y
{
	int place = y * m_dimX + x;
	
	if (place >= self.propMemorySize)
		return nil;
	
	return m_memory[place];
}

// Returns object at flat index
//-----------------------------------------------------------------------------------------------
- (id)getObjectAtIndex:(uint)index
{
	if (index >= self.propMemorySize)
		return nil;
	
	return m_memory[index];
}

//###############################################################################################
// PRIVATE METHODS
//###############################################################################################

// Replacing existing object with new object if needed in memory location
//-----------------------------------------------------------------------------------------------
- (BOOL)replaceObjectAtX:(uint)x Y:(uint)y WithObject:(id)object InMemory:(id*)mem MemSize:(IntSize)size
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
