//===============================================================================================
//  File:		ViewsTab.h
//  Project:	OriginGameToolkit
//	Desc:		NSTabView subclassing
//
//  Created by Max Vasin on 12/16/10.
//  Copyright 2010 Origin. All rights reserved.
//===============================================================================================

// ****	IMPORTS ****

#import <Cocoa/Cocoa.h>
#import "BaseObject.h"
#import "ProjectItem.h"

// **** INTERFACE ****

@interface ViewsTab : NSTabView 
{
	ProjectItem*			m_itemCurrent;
	
	IBOutlet NSTabViewItem* m_defaultItem;
}

// **** PROPERTIES ****

@property (assign) ProjectItem* propCurrentItem;

@end
