//===============================================================================================
//  File:		ProjectFontItem.m
//  Project:	OriginGameToolkit
//	Desc:		Project font item container class implementation
//
//  Created by Max Vasin on 11/24/10.
//  Copyright 2010 Origin. All rights reserved.
//===============================================================================================

// ****** IMPORTS ******

#import "ProjectFontItem.h"
#import "SharedResourceManager.h"

// ****** IMPLEMENTATION ******

@implementation ProjectFontItem

// ****** INSTANCE METHODS ******

//###############################################################################################
// INITIALIZIZERS\DESTRUCTORS
//###############################################################################################

// Initializes new item with project
//-----------------------------------------------------------------------------------------------
- (id)initWithProject:(Project*)project
{
	if ( (self = [super initWithProject:project]) )
	{
	
	}
	return self;
}

// Initializes new item with parent
//-----------------------------------------------------------------------------------------------
- (id)initWithParent:(ProjectItem*)parent
{
	if ( (self = [super initWithParent:parent]) )
	{
	
	}
	return self;
}

// Sets item icon
//-----------------------------------------------------------------------------------------------
- (void)setIcon
{
	m_itemImage = [SharedResourceManager sharedIcon:@"font"];
}

//###############################################################################################
// I\O
//###############################################################################################

// Saves item to XML schema
//-----------------------------------------------------------------------------------------------
- (NSXMLElement*)saveAsXML:(NSXMLElement*)node
{
	NSXMLElement* thisNode = [super saveAsXML:[node addNewChild:@"Font"]];
	
	return thisNode;
}

@end
