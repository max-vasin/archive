//=======================================================================================================================
//  File:		IntegerArray2D.h
//  Project:	heli
//	Desc:		2 dimensional array on integers
//
//  Created by Max Vasin on 5/30/11.
//  Copyright 2011 Origin. All rights reserved.
//=======================================================================================================================

// ******** IMPORTS ********

#import "OriBase.h"

// ******** INTERFACE ********

@interface IntegerArray2D : NSObject 
{
	oriUint*			m_memory;
	oriUint				m_dimX;
	oriUint				m_dimY;
}

// ******** PROPERTIES ********

@property (nonatomic, readonly)		oriUint		propDimX;
@property (nonatomic, readonly)		oriUint		propDimY;
@property (nonatomic, readonly)		oriUint		propTotal;
@property (nonatomic, readonly)		oriUint		propMemory;

// ******** INSTANCE METHODS ********

- (void)		resizeToX:(oriUint)x		Y:(oriUint)y;
- (oriUint)		valueAtX:(oriUint)x			Y:(oriUint)y;
- (void)		setValue:(oriUint)value		AtX:(oriUint)x	Y:(oriUint)y;


@end
