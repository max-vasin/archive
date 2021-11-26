//===============================================================================================
//  File:		NSStringExtension.h
//  Project:	OriginGameToolkit
//	Desc:		NSString class additional functionality
//
//  Created by Max Vasin on 11/29/10.
//  Copyright 2010 Origin. All rights reserved.
//===============================================================================================

// **** IMPORTS ****

#import <Cocoa/Cocoa.h>

// **** INTERFACE ****

@interface NSString (Extension)

// **** INSTANCE METHODS ****

- (NSString*)	fileExtension;
- (NSString*)	fileName;
- (NSString*)	fileDirectory;

@end
