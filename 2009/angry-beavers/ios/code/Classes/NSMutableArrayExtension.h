//===============================================================================================
//  File:		NSMutableArrayExtension.h
//  Project:	Origin Mobile
//	Desc:		NSMutableArray class extension
//
//  Created by Max Vasin on 12/16/10.
//  Copyright 2010 Origin. All rights reserved.
//===============================================================================================

// **** IMPORTS ****

#import <UIKit/UIKit.h>
#import "OriBase.h"

// **** INTERFACE ****
	
@interface NSMutableArray (Extension)

- (void)	shiftObject:			(id)object		offset:(NSInteger)to;
- (void)	insertObjectSafe:		(id)object		atIndex:(int)index;
- (id)		getObjectByProp:		(NSString*)name Value:(id)value;

@end
