//===============================================================================================
//  File:		ProjectItem.h
//  Project:	OriginGameToolkit
//	Desc:		Base project item class interface
//
//  Created by Max Vasin on 11/24/10.
//  Copyright 2010 Origin. All rights reserved.
//===============================================================================================

// ****** IMPORTS ******

#import <Cocoa/Cocoa.h>
#import "BaseObject.h"
#import "Progressor.h"

// ****** INTERFACE ******

@class Project;

@interface ProjectItem : BaseObject 
{
@public
	Project*			m_project;
	ProjectItem*		m_itemParent;
	NSMutableArray*		m_itemChildren;
	NSImage*			m_itemImage;
	NSString*			m_itemName;
	NSString*			m_childName;
	NSString*			m_localBuildDirectory;
	BOOL				m_expandable;
}

// ****** PROPERTIES ******

@property (copy)					NSString*		propName;
@property (copy)					NSString*		propChildName;
@property (assign)					BOOL			propExpandable;
@property (readonly)				NSImage*		propImage;
@property (readonly)				Project*		propProject;
@property (readonly)				NSArray*		propChildren;
@property (readonly)				NSUInteger		propChildrenCount;
@property (copy)					NSString*		propLocalBuildDirectory;
@property (readonly)				NSString*		propItemBuildDirectory;
@property (nonatomic, readonly)		ProjectItem*	propParentItem;

// ****** INSTANCE METHODS ******

// Initializers

- (id)				initWithProject:	(Project*)project;
- (id)				initWithParent:		(ProjectItem*)parent;
- (id)				initWithProject:	(Project*)project		fromXML:(NSXMLElement*)node;
- (id)				initWithParent:		(ProjectItem*)parent	fromXML:(NSXMLElement*)node;
- (void)			setIcon;

// Children control

- (ProjectItem*)	addChild:			(ProjectItem*)item;
- (ProjectItem*)	getChildByName:		(NSString*)name;
- (ProjectItem*)	getChildAtIndex:	(NSInteger)index;
- (ProjectItem*)	removeChild:		(ProjectItem*)item;
- (ProjectItem*)	removeChildAtIndex:	(NSInteger)index;
- (void)			removeSelf;

// I/O

- (NSXMLElement*)	saveAsXML:			(NSXMLElement*)node;

// Building

- (result)			build:(Progressor*)progress;

// Sevice methods

- (void)			updateName;
- (NSString*)		getUniqueName;

@end
