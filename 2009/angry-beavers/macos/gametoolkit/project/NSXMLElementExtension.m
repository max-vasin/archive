//===============================================================================================
//  File:		NSXMLElementExtension.m
//  Project:	OriginGameToolkit
//	Desc:		NSXMLElement class extension implementation
//
//  Created by Max Vasin on 11/30/10.
//  Copyright 2010 Origin. All rights reserved.
//===============================================================================================

// **** IMPORTS ****

#import "NSXMLElementExtension.h"

// **** IMPLEMENTATION ****

@implementation NSXMLElement(Extension)

// getFirstChild
// This method returns first child of node with given name
//-----------------------------------------------------------------------------------------------

- (NSXMLElement*)getFirstChild:(NSString*)name
{
	NSArray* childNodes = [self elementsForName:name];
	
	if ([childNodes count] == 0)
		return nil;
	
	return [childNodes objectAtIndex:0];	
}

// getFirstChildValue
// This method returns value of first child of node with given name
//-----------------------------------------------------------------------------------------------

- (id)getFirstChildValue:(NSString*)name
{
	return [self getFirstChildValue:name placeholder:@""];
}

// getFirstChildFloat
// This method returns float value of first child of node with given name
//-----------------------------------------------------------------------------------------------

- (GLfloat)getFirstChildFloat:(NSString*)name
{
	return [self getFirstChildFloat:name placeholder:0];
}

// getFirstChildInteger
// This method returns integer value of first child of node with given name
//-----------------------------------------------------------------------------------------------

- (NSInteger)getFirstChildInteger:(NSString*)name
{
	return [self getFirstChildInteger:name placeholder:0];	
}

// getFirstChildObject
// This method returns object of first child of node with given name
//-----------------------------------------------------------------------------------------------

- (id)getFirstChildObject:(NSString*)name
{
	return [self getFirstChildObject:name placeholder:nil];
}

// getFirstChildValue:placeholder
// This method returns value of first child of node with given name or placeholder if no node
//-----------------------------------------------------------------------------------------------

- (id)getFirstChildValue:(NSString*)name placeholder:(id)placeholder
{
	NSXMLElement* node = [self getFirstChild:name];
	
	if (!node)
		return placeholder;
	
	id objval = nil;
	
	@try
	{
		objval = [node objectValue];
	}
	@catch (NSException * e)
	{
		return placeholder;
	}
	return objval;	
}

// getFirstChildInteger:placeholder
// This method returns integer value of first child of node with given name or placeholder
//-----------------------------------------------------------------------------------------------

- (NSInteger)getFirstChildInteger:(NSString*)name placeholder:(NSInteger)placeholder
{
	NSXMLElement* node = [self getFirstChild:name];
	
	if (!node)
		return placeholder;
	
	NSInteger objval = 0;
	
	@try
	{
		objval = [[node objectValue] integerValue];
	}
	@catch (NSException * e)
	{
		return placeholder;
	}
	return objval;	
}

// getFirstChildFloat:placeholder
// This method returns float value of first child of node with given name or placeholder
//-----------------------------------------------------------------------------------------------

- (GLfloat)getFirstChildFloat:(NSString*)name placeholder:(GLfloat)placeholder
{
	NSXMLElement* node = [self getFirstChild:name];
	
	if (!node)
		return placeholder;
	
	GLfloat objval = 0.0f;
	
	@try
	{
		objval = [[node objectValue] floatValue];
	}
	@catch (NSException * e)
	{
		return placeholder;
	}
	return objval;	
}

// getFirstChildObject
// This method returns object of first child of node with given name or placeholder if error
//-----------------------------------------------------------------------------------------------

- (id)getFirstChildObject:(NSString*)name placeholder:(id)placeholder
{
	NSXMLElement* objectNode = [self getFirstChild:name];
	
	if (!objectNode)
		return placeholder;
	
	// Its child element must declare object
	NSXMLElement* objectTypeNode = (NSXMLElement*)[objectNode childAtIndex:0];
	if (!objectTypeNode)
		return nil;
	
	id objval = nil;
	
	@try 
	{
		
		if ([[objectTypeNode name] isEqualToString:@"Color"])
		{
			objval =  [NSColor colorWithCalibratedRed:[objectTypeNode getFirstChildFloat:@"R"]
												green:[objectTypeNode getFirstChildFloat:@"G"]
												 blue:[objectTypeNode getFirstChildFloat:@"B"]
												alpha:[objectTypeNode getFirstChildFloat:@"A"] ];
		}
		
	}
	@catch (NSException * e) 
	{
		return placeholder;
	}
	
	return objval;
}


// addNewChild
// Fast allocating and adding new child
//-----------------------------------------------------------------------------------------------

- (NSXMLElement*)addNewChild:(NSString*)name
{
	NSXMLElement* newChild = [[[NSXMLElement alloc] initWithName:name] autorelease];
	
	[self addChild:newChild];
	
	return newChild;
}

// addNewChild:withValue
// Fast allocating and adding new child (withValue)
//-----------------------------------------------------------------------------------------------

- (NSXMLElement*)addNewChild:(NSString*)name withValue:(NSString*)value
{
	NSXMLElement* newChild = [[[NSXMLElement alloc] initWithName:name stringValue:value] autorelease];
	
	[self addChild:newChild];
	
	return newChild;
}

// addNewChild:withInteger
// Fast allocating and adding new child (withInteger)
//-----------------------------------------------------------------------------------------------

- (NSXMLElement*)addNewChild:(NSString*)name withInteger:(NSInteger)value
{
	NSXMLElement* newChild = [[[NSXMLElement alloc] initWithName:name stringValue:[NSString stringWithFormat:@"%i", value]] autorelease];
	
	[self addChild:newChild];
	
	return newChild;
}

// addNewChild:withFloat
// Fast allocating and adding new child (withFloat)
//-----------------------------------------------------------------------------------------------

- (NSXMLElement*)addNewChild:(NSString*)name withFloat:(GLfloat)value
{
	NSXMLElement* newChild = [[[NSXMLElement alloc] initWithName:name stringValue:[NSString stringWithFormat:@"%f", value]] autorelease];
	
	[self addChild:newChild];
	
	return newChild;
}


// addNewChild:withObject
// Fast allocating and adding new child (withObject)
//-----------------------------------------------------------------------------------------------

- (NSXMLElement*)addNewChild:(NSString*)name withObject:(id)object
{
	Class class = [object class];
	
	NSXMLElement* newChild = [[[NSXMLElement alloc] initWithName:name] autorelease];
	
	// NSColor
	if (class = [NSColor class])
	{
		NSXMLElement* objRoot = [newChild addNewChild:@"Color"];
		[objRoot addNewChild:@"R" withValue:[NSString stringWithFormat:@"%f", [(NSColor*)object redComponent]]];
		[objRoot addNewChild:@"G" withValue:[NSString stringWithFormat:@"%f", [(NSColor*)object greenComponent]]];
		[objRoot addNewChild:@"B" withValue:[NSString stringWithFormat:@"%f", [(NSColor*)object blueComponent]]];
		[objRoot addNewChild:@"A" withValue:[NSString stringWithFormat:@"%f", [(NSColor*)object alphaComponent]]];
	}
	
	[self addChild:newChild];
	
	return newChild;
}



@end
