//===============================================================================================
//  File:		NSMutableArrayExtension.h
//  Project:	OriginGameToolkit
//	Desc:		NSMutableArray class extension
//
//  Created by Max Vasin on 12/16/10.
//  Copyright 2010 Origin. All rights reserved.
//===============================================================================================

// **** IMPORTS ****

#import <Cocoa/Cocoa.h>
#import "BaseObject.h"

// **** INTERFACE ****
	
@interface NSMutableArray (Extension)

- (void)	shiftObject:			(id)object		offset:(NSInteger)to;
- (void)	insertObjectSafe:		(id)object		atIndex:(int)index;
- (id)		removeAndGet:			(id)object;
- (id)		getObjectByProp:		(NSString*)name Value:(id)value;

@end
