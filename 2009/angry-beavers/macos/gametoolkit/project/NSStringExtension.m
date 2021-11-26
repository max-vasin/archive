//===============================================================================================
//  File:		NSStringExtension.m
//  Project:	OriginGameToolkit
//	Desc:		NSString additional functionality implementation
//
//  Created by Max Vasin on 11/29/10.
//  Copyright 2010 Origin. All rights reserved.
//===============================================================================================

// **** IMPORTS ****

#import "NSStringExtension.h"

// **** IMPLEMENTATION ****

@implementation NSString (Extension)

// **** INSTANCE METHODS IMPLEMENTATION ****

// fileExtension
// Returns extension of file or nil if no extension
//-----------------------------------------------------------------------------------------------

- (NSString*)fileExtension
{
	if (![self length])
		return nil;
	
	NSArray* parts = [self componentsSeparatedByString:@"."];
	
	if ([parts count] == 1)
		return nil;
	
	return [parts objectAtIndex:[parts count] - 1];
}

// fileName
// Returns name of file or nil if no filename
//-----------------------------------------------------------------------------------------------

- (NSString*)fileName
{
	if (![self length])
		return nil;
	
	// Try to remove any extension
	NSArray*			parts = [self componentsSeparatedByString:@"."];
	NSMutableString*	temp  = [[[NSMutableString alloc] initWithCapacity:[self length]] autorelease];
	
	if ([parts count] == 1)
	{
		[temp setString:[parts objectAtIndex:0]];
	}
	else
	{
		// Just cut last part - this is extension
		for (NSInteger i = 0; i < [parts count] - 1; i++)
		{
			[temp appendString:[parts objectAtIndex:i]];
		}
	}
	
	// Try to remove any directory entries
	parts = [temp componentsSeparatedByString:@"/"];
	
	// Return last part - anyway we will got filename
	return [parts objectAtIndex:[parts count] - 1];
}

// fileDirectory
// Returns directory of file or nil if no directory
//-----------------------------------------------------------------------------------------------

- (NSString*)fileDirectory
{
	if (![self length])
		return nil;
	
	// Split on directory entries and filename
	NSArray*			parts = [self componentsSeparatedByString:@"/"];
	NSMutableString*	temp  = [[[NSMutableString alloc] initWithCapacity:[self length]] autorelease];
	
	if ([parts count] == 1)
	{
		// No-no - at least two parts must be if string contains something like directory
		return nil;
	}
	else
	{
		// Just cut last part - this is name + extension
		for (NSInteger i = 0; i < [parts count] - 1; i++)
		{
			[temp appendString:[parts objectAtIndex:i]];
		}
	}
	
	return [NSString stringWithString:temp];
}


@end
