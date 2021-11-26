//===============================================================================================
//  File:		NSMutableArrayExtension.m
//  Project:	Origin Mobile
//	Desc:		Extenstion of NSMutableArray
//
//  Created by Max Vasin on 12/16/10.
//  Copyright 2010 Origin. All rights reserved.
//===============================================================================================

// **** IMPORTS ****

#import "NSMutableArrayExtension.h"

// **** IMPLEMENTATION ****

@implementation NSMutableArray (Extension)

// shiftObject
// Shifts object to given offset
//-----------------------------------------------------------------------------------------------
- (void)shiftObject:(id)object offset:(NSInteger)to
{
	int count = [self count];
	int start = [self indexOfObject:object];
	int end   = start + to;
	
	if (!count || (end < 0) || (end >= count))
		return;
	
	for (int i = 1; i <= ABS(to); i++)
	{
		int indA = (to < 0) ? start - (i - 1) : start + i - 1;
		int indB = (to < 0) ? start - i : start + i;
				
		id objA = [[self objectAtIndex:indA] retain];
		id objB = [[self objectAtIndex:indB] retain];
		
		[self replaceObjectAtIndex:indA withObject:objB];
		[self replaceObjectAtIndex:indB withObject:objA];
		
		[objA release];
		[objB release];
	}
}

// insertObjectSafe:atIndex
// This method safely adds new items to array to store object exactly at index or replace object
// if index already in use. Useful to create ordered collection when you not sure about adding
// objects ordering
//-----------------------------------------------------------------------------------------------

- (void)insertObjectSafe:(id)object atIndex:(int)index
{
	int toAdd = index - [self count];
	
	if (toAdd == 0)
	{
		[self addObject:object];
		return;
	}
	
	if (toAdd < 0)
	{
		[self replaceObjectAtIndex:index withObject:object];
		return;
	}
	
	for (int i = 0; i < toAdd; i++)
		[self addObject:[[[NSObject alloc] init] autorelease]];
	
	[self addObject:object];
}

// Returns object by property value
//-----------------------------------------------------------------------------------------------
- (id)getObjectByProp:(NSString*)name Value:(id)value
{
	SEL getter = NSSelectorFromString(name);
	
	for (id object in self)
	{
		id result = [object performSelector:getter];
		if (result && [result isEqual:value])
			return object;
	}
	return nil;
}

@end
