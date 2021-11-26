//===============================================================================================
//  File:		BaseObject.h
//  Project:	Fenix
//	Desc:		base class for all project classes
//
//  Created by Max Vasin on 11/3/10.
//  Copyright 2010 Origin. All rights reserved.
//===============================================================================================

// IMPORTS
//-----------------------------------------------------------------------------------------------
#import <Foundation/Foundation.h>
#import "Base.h"


// [BaseObject] INTERFACE
//-----------------------------------------------------------------------------------------------
@interface BaseObject: NSObject 
{

}

// [BaseObject] METHODS
//-----------------------------------------------------------------------------------------------
- (void)log:(const char*)msg, ...;

@end
