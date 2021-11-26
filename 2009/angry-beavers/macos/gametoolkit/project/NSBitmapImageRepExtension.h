//===============================================================================================
//  File:		NSBitmapImageRepExtension.h
//  Project:	OriginGameToolkit
//	Desc:		NSBitmapImageRep class extension
//
//  Created by Max Vasin on 12/16/10.
//  Copyright 2010 Origin. All rights reserved.
//===============================================================================================

// **** IMPORTS ****

#import <Cocoa/Cocoa.h>
#import "BaseObject.h"

// **** INTERFACE EXTENSION

@interface NSBitmapImageRep (Extension)

- (id)			initWithDataAndConvert:	(NSData*)data;	
- (void)		uploadToTexture:		(GLuint)texid;
- (IntSize)		intSize;
- (id)			trimSaveX:				(uint*)px		Y:(uint*)py;
- (IntSize)		trimSizeToX:			(uint*)px		Y:(uint*)py;
- (id)			trimToSize:				(IntSize)size	X:(uint)x	Y:(uint)y;
- (NSInteger)	areaSort:				(NSBitmapImageRep*)image;

- (void)		applyMonochrome;
- (void)		applyInverse;
- (void)		applyTransparent:(NSColor*)color;

@end
