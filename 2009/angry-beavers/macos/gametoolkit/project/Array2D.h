//===============================================================================================
//  File:		Array2D.h
//  Project:	OriginGameToolkit
//	Desc:		2D map array of elements with safe resizing operations
//
//  Created by Max Vasin on 4/28/11.
//  Copyright 2011 Origin Games. All rights reserved.
//===============================================================================================

// ****** IMPORTS ******

#import <Cocoa/Cocoa.h>
#import "Base.h"

// Forward classed declaration
//-----------------------------------------------------------------------------------------------
@class Array2D;

// Array2D delegate protocol
//-----------------------------------------------------------------------------------------------
@protocol Array2DDelegate <NSObject>

- (id)createInstanceOfArray:(Array2D*)array	X:(uint)x Y:(uint)y	DimX:(uint)dimX	DimY:(uint)dimY;

@end

// ****** INTERFACE ******

@interface Array2D : NSObject 
{
	id<Array2DDelegate>	m_delegate;
	id*					m_memory;
	uint				m_dimX;
	uint				m_dimY;
}

// ****** PROPERTIES ******

@property (nonatomic, readonly)		uint	propDimX;
@property (nonatomic, readonly)		uint	propDimY;
@property (nonatomic, readonly)		uint	propMemorySize;

// ****** INSTANCE METHODS ******

- (id)		initWithDelegate:	(id<Array2DDelegate>)delegate;
- (void)	resizeToX:			(uint)x	Y:(uint)y Create:(BOOL)create;
- (void)	replaceAtX:			(uint)x	Y:(uint)y With:(id)object;
- (id)		getObjectAtX:		(uint)x	Y:(uint)y;
- (id)		getObjectAtIndex:	(uint)index;

@end
