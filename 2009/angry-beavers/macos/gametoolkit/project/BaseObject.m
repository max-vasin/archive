//===============================================================================================
//  File:		BaseObject.m
//  Project:	Fenix
//	Desc:		base class of project classes implementation
//
//  Created by Max Vasin on 11/3/10.
//  Copyright 2010 Origin. All rights reserved.
//===============================================================================================

// IMPORTS
//-----------------------------------------------------------------------------------------------
#import "BaseObject.h"

// [BaseObject] IMPLEMENTATION
//-----------------------------------------------------------------------------------------------
@implementation BaseObject

// Alternative output to console
//-----------------------------------------------------------------------------------------------
- (void)log:(const char*)msg,...
{
	// Protect as i can
	if (!msg)
		return;
	
	int len = strlen(msg);
	if (!len)
		return;
	
	char* input = (char*)msg;
		
	// Determine class name output flag
	bool dumpname = false; 
	// If input message length less than 2 then no classname flag can be stored in
	if (len > 2)
	{
		char check[3];
		memcpy(check, input, sizeof(char) << 1);
		check[2] = 0;
		dumpname = (strcmp(check, "$C") == 0);
	}
	
	// We must remove classname output flag if needs
	if (dumpname)
	{
		input = malloc(sizeof(char) * (len - 1));
		memcpy(input, &msg[2], sizeof(char) * (len - 1));
	}
	
	NSString* classname = nil;
	
	if (dumpname)
		classname = NSStringFromClass([self class]);
	
	// Allocate buffers
	char* buffer = malloc(sizeof(char) * (len << 4));
	
	// Format message to memory
	va_list	args;
	va_start(args, msg);
	
	vsprintf(buffer, input, args);
	
	va_end(args);
	
	// Concatenate final message
	if (dumpname)
	{
		printf("[%s]: %s", [classname UTF8String], buffer);
	}
	else 
	{
		printf("%s", buffer);
	}

	free(buffer);
	if (dumpname)
		free(input);
}

@end
