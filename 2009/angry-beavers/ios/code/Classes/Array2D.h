//===============================================================================================
//  File:		Array2D.h
//  Project:	OriginGameToolkit
//	Desc:		2D map array of elements with safe resizing operations
//
//  Created by Max Vasin on 4/28/11.
//  Copyright 2011 Origin Games. All rights reserved.
//===============================================================================================

// ****** IMPORTS ******

#import <UIKit/UIKit.h>
#import "OriBase.h"

// Forward classed declaration
//-----------------------------------------------------------------------------------------------
@class Array2D;

// Array2D delegate protocol
//-----------------------------------------------------------------------------------------------
@protocol Array2DDelegate <NSObject>

- (id)createInstanceOfArray:(Array2D*)array	X:(oriUint)x Y:(oriUint)y	DimX:(oriUint)dimX	DimY:(oriUint)dimY;

@end

// ****** INTERFACE ******

@interface Array2D : NSObject 
{
	id<Array2DDelegate>	m_delegate;
	id*					m_memory;
	oriUint				m_dimX;
	oriUint				m_dimY;
}

// ****** PROPERTIES ******

@property (nonatomic, readonly)		oriUint		propDimX;
@property (nonatomic, readonly)		oriUint		propDimY;
@property (nonatomic, readonly)		oriUint		propMemorySize;	// deprecated - use propCount
@property (nonatomic, readonly)		oriUint		propCount;

// ****** INSTANCE METHODS ******

- (id)			initWithDelegate:	(id<Array2DDelegate>)delegate;
- (void)		resizeToX:			(oriUint)x	Y:(oriUint)y Create:(BOOL)create;
- (void)		replaceAtX:			(oriUint)x	Y:(oriUint)y With:(id)object;
- (id)			getObjectAtX:		(oriUint)x	Y:(oriUint)y;
- (id)			getObjectAtIndex:	(oriUint)index;
- (oriIntPoint)	indexOfObject:		(id)object;
- (int)			flatIndexOfObject:	(id)object;

@end
