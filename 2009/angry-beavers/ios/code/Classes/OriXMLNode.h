//=======================================================================================================================
//  File:		OriXMLNode.h
//  Project:	heli
//	Desc:		XML simple node utility class interface
//
//  Created by Max Vasin on 3/23/11.
//  Copyright 2011 Origin. All rights reserved.
//=======================================================================================================================

// ***** IMPORTS *****

#import <UIKit/UIKit.h>
#import "OriBase.h"

// ***** INTERFACE *****

@interface OriXMLNode : NSObject 
{
	OriXMLNode*				m_parent;
	NSString*				m_name;
	NSString*				m_value;
	NSMutableArray*			m_children;
	NSMutableDictionary*	m_attributes;
}

// ***** PROPERTIES *****

@property (copy)		NSString*	propName;
@property (copy)		NSString*	propValue;
@property (assign)		NSInteger	propValueInt;
@property (assign)		float		propValueFloat;
@property (readonly)	NSInteger	propAttributeCount;
@property (readonly)	OriXMLNode*	propParent;
@property (readonly)	NSArray*	propChildren;

// ***** CLASS METHODS *****

+ (id)			parseFile:(NSString*)file;

// Returns autoreleased

+ (id)			nodeWithName:(NSString*)name;
+ (id)			nodeWithName:(NSString*)name		withValue:(NSString*)value;
+ (id)			nodeWithName:(NSString*)name		withValueInt:(NSInteger)value;
+ (id)			nodeWithName:(NSString*)name		withValueFloat:(float)value;

// ***** INSTANCE METHODS *****

// Instance initializers

- (id)			initWithName:(NSString*)name;
- (id)			initWithName:(NSString*)name		withValue:(NSString*)value;
- (id)			initWithName:(NSString*)name		withValueInt:(NSInteger)value;
- (id)			initWithName:(NSString*)name		withValueFloat:(float)value;

// Hierarchy control

- (void)		addChildNode:(OriXMLNode*)node;

- (id)			newChildNode:(NSString*)name;
- (id)			newChildNode:(NSString*)name		withValue:(NSString*)value;
- (id)			newChildNode:(NSString*)name		withValueInt:(NSInteger)value;
- (id)			newChildNode:(NSString*)name		withValueFloat:(float)value;

- (id)			getFirstChild:(NSString*)name;
- (NSString*)	getFirstChildValue:(NSString*)name			placeholder:(NSString*)value;
- (NSInteger)	getFirstChildValueInt:(NSString*)name		placeholder:(NSInteger)value;
- (float)		getFirstChildValueFloat:(NSString*)name		placeholder:(float)value;

// Attributes access

- (NSString*)	getAttributeValue:(NSString*)name			placeholder:(NSString*)value;
- (NSInteger)	getAttributeValueInt:(NSString*)name		placeholder:(NSInteger)value;
- (float)		getAttributeValueFloat:(NSString*)name		placeholder:(float)value;

- (void)		setAttribute:(NSString*)name		withValue:(NSString*)value;
- (void)		setAttribute:(NSString*)name		withValueInt:(NSInteger*)value;
- (void)		setAttribute:(NSString*)name		withValueFloat:(float)value;

@end
