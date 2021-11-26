//===============================================================================================
//  File:		ViewsTab.m
//  Project:	OriginGameToolkit
//	Desc:		NSTabView subclassing
//
//  Created by Max Vasin on 12/16/10.
//  Copyright 2010 Origin. All rights reserved.
//===============================================================================================

// **** IMPORTS ****

#import "ViewsTab.h"

// **** IMPLEMENTATION ****

@implementation ViewsTab

// **** IMPLEMENTED PROPERTIES ****

- (ProjectItem*)propCurrentItem
{
	return m_itemCurrent;
}

- (void)setPropCurrentItem:(ProjectItem*)item
{
	m_itemCurrent = item;
	
	// Select tab by classname
	NSString* classname = NSStringFromClass([m_itemCurrent class]);
	
	NSArray* tabItems = [self tabViewItems];
	if (!tabItems)
		return;
	BOOL founded = NO;
	for (NSTabViewItem* tab in tabItems)
		if (![(NSString*)tab.identifier compare:classname])
		{
			[self selectTabViewItem:tab];
			founded = YES;
			break;
		}
	
	if (!founded)
		[self selectTabViewItem:m_defaultItem];
	
}

// **** INSTANCE METHODS IMPLEMENTATION ****

// awakeFromNib
// Called after NIB loaded
//------------------------------------------------------------------------------------------------

- (void)awakeFromNib
{
	[self selectTabViewItem:m_defaultItem];
}

@end
