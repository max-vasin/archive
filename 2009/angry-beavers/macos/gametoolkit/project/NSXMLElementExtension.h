//===============================================================================================
//  File:		NSXMLElementExtension.h
//  Project:	OriginGameToolkit
//	Desc:		NSXMLElement class extension
//
//  Created by Max Vasin on 11/30/10.
//  Copyright 2010 Origin. All rights reserved.
//===============================================================================================

// **** IMPORTS ****

#import <Cocoa/Cocoa.h>

// **** INTERFACE ****

@interface NSXMLElement (Extension)

// **** INSTANCE METHODS ****

// Obtaining child nodes

- (NSXMLElement*)	getFirstChild:			(NSString*)name;

// Obtaining child nodes values

- (id)				getFirstChildValue:		(NSString*)name;
- (GLfloat)			getFirstChildFloat:		(NSString*)name;
- (NSInteger)		getFirstChildInteger:	(NSString*)name;
- (id)				getFirstChildObject:	(NSString*)name;

// Obtaining child nodes values with error placeholders

- (id)				getFirstChildValue:		(NSString*)name	 placeholder:(id)placeholder;
- (GLfloat)			getFirstChildFloat:		(NSString*)name	 placeholder:(GLfloat)placeholder;
- (NSInteger)		getFirstChildInteger:	(NSString*)name	 placeholder:(NSInteger)placeholder;
- (id)				getFirstChildObject:	(NSString*)name	 placeholder:(id)placeholder;

// Adding new childs (autoreleased)

- (NSXMLElement*)	addNewChild:			(NSString*)name;
- (NSXMLElement*)	addNewChild:			(NSString*)name	 withValue:(NSString*)value;
- (NSXMLElement*)	addNewChild:			(NSString*)name  withObject:(id)object;
- (NSXMLElement*)	addNewChild:			(NSString*)name  withInteger:(NSInteger)value;
- (NSXMLElement*)	addNewChild:			(NSString*)name  withFloat:(GLfloat)value;

@end
