//===============================================================================================
//  File:		PropPanel.m
//  Project:	OriginGameToolkit
//	Desc:		Properties panel class implementation
//
//  Created by Max Vasin on 11/29/10.
//  Copyright 2010 Origin. All rights reserved.
//===============================================================================================

// IMPORTS
//-----------------------------------------------------------------------------------------------
#import "PropPanel.h"
#import "ProjectRootItems.h"

// [PropPanel]	PRIVATE
//-----------------------------------------------------------------------------------------------
@interface PropPanel (Private)

- (void)switchPanel;
- (void)assemblyDoubleClick;

@end

// [PropPanel]	IMPLEMENTATION
//-----------------------------------------------------------------------------------------------
@implementation PropPanel

// [PropPanel]	PROPERTIES
//-----------------------------------------------------------------------------------------------
@synthesize propTabView = m_propTab;
@synthesize propDir		= m_dir;

// [propCurrentItem] property accessors implementation
//-----------------------------------------------------------------------------------------------
@dynamic propCurrentItem;

- (ProjectItem*)propCurrentItem
{
	return m_itemCurrent;
}

- (void)setPropCurrentItem:(ProjectItem*)item
{
	m_itemCurrent = item;
	[self switchPanel];
}

- (void)awakeFromNib
{	
	// Remove tabs buttons anyway
	[m_propTab setTabViewType:NSNoTabsBezelBorder];
	[m_tabTextureType setTabViewType:NSNoTabsBezelBorder];
	
	[m_tableAssembly setTarget:self];
	[m_tableAssembly setDoubleAction:@selector(assemblyDoubleClick)];
	
	// Build controllers dictonary for easy access
	//m_dictControllers = [[NSDictionary dictionaryWithObjectsAndKeys: m_ctrlTexture, [ProjectTextureItem class], 
	//															    nil] retain];
	m_dictControllers = [[NSDictionary alloc] init];
	
}

// [switchPanel]
// Switches panel to reflect current project item properties
//-----------------------------------------------------------------------------------------------
- (void)switchPanel
{
	// Select tab by classname
	NSString* classname = NSStringFromClass([m_itemCurrent class]);
	
	NSArray* tabItems = [m_propTab tabViewItems];
	if (!tabItems)
		return;
	
	for (NSTabViewItem* tab in tabItems)
		if (![(NSString*)tab.identifier compare:classname])
		{
			[m_propTab selectTabViewItem:tab];
			self.title = tab.label;
			// Try to update controller
			NSObjectController* ctrl = [m_dictControllers objectForKey:[m_itemCurrent class]];
			if (ctrl)
			{
				[ctrl setContent:nil];
				[ctrl setContent:m_itemCurrent];
			}
		}
}
	 
- (void)assemblyDoubleClick
{
	NSArray* selection = [m_ctrlTextureAssembly selectedObjects];
	if (![selection count])
		return;
	
	[m_panelAssemblyItem doModalForItem:[selection objectAtIndex:0]];
}		 


- (NSString*)selectDir:(NSString*)initial
{
	NSOpenPanel* panel = [NSOpenPanel openPanel];
	
	panel.title						= @"Select directory";
	panel.canChooseFiles			= NO;
	panel.canChooseDirectories		= YES;
	panel.allowsMultipleSelection	= NO;
	
	if ([panel runModalForDirectory:initial file:nil types:nil] != NSFileHandlingPanelOKButton)
		return initial;
	
	NSArray* filenames = [panel URLs];
	if (![filenames count])
		return initial;
	
	return [[filenames objectAtIndex:0] path];
}

- (void)updateAssemblyView
{
	[m_ctrlTextureAssembly rearrangeObjects];
}

- (IBAction)selectDirProject:(id)sender
{
	if ([m_itemCurrent class] != [ProjectRootItem class])
		return;
	
	[(ProjectRootItem*)m_itemCurrent setPropRootDirectory:[self selectDir:[(ProjectRootItem*)m_itemCurrent propRootDirectory]]];
}

- (IBAction)selectDirBuild:(id)sender
{
	if ([m_itemCurrent class] != [ProjectRootItem class])
		return;
	
	[(ProjectRootItem*)m_itemCurrent setPropBuildDirectory:[self selectDir:[(ProjectRootItem*)m_itemCurrent propBuildDirectory]]];
}

- (IBAction)selectDirCache:(id)sender
{
	if ([m_itemCurrent class] != [ProjectRootItem class])
		return;
	
	[(ProjectRootItem*)m_itemCurrent setPropCacheDirectory:[self selectDir:[(ProjectRootItem*)m_itemCurrent propCacheDirectory]]];
}

- (IBAction)addAssemblyItem:(id)sender
{
	[m_panelAssemblyItem doModalForItem:[(ProjectTextureItem*)m_itemCurrent addAssemblyItem]];
}

- (IBAction)deleteAssemblyItem:(id)sender
{
	NSArray* selection = [m_ctrlTextureAssembly selectedObjects];
	if (![selection count])
		return;
	
	NSBeginAlertSheet(@"Please confirm delete assembly item", @"Delete", nil, @"Dismiss", self, self, @selector(deleteAssemblyItemConfirm:returnCode:contextInfo:), nil, nil,  @"There is no undo for this operation");
}

- (IBAction)shiftAssemblyItemUp:(id)sender
{
	NSArray* selection = [m_ctrlTextureAssembly selectedObjects];
	if (![selection count])
		return;
	
	[(ProjectTextureItem*)m_itemCurrent shiftAssemblyItem:[selection objectAtIndex:0] offset:-1];
	
	[m_ctrlTextureAssembly rearrangeObjects];
}

- (IBAction)shiftAssemblyItemDown:(id)sender
{
	NSArray* selection = [m_ctrlTextureAssembly selectedObjects];
	if (![selection count])
		return;
	
	[(ProjectTextureItem*)m_itemCurrent shiftAssemblyItem:[selection objectAtIndex:0] offset:1];
	
	[m_ctrlTextureAssembly rearrangeObjects];
}


- (void)deleteAssemblyItemConfirm:(NSWindow*)sheet returnCode:(NSInteger)returnCode contextInfo:(void*)contextInfo
{
	if (returnCode == NSAlertDefaultReturn)
	{
		NSArray* selection = [m_ctrlTextureAssembly selectedObjects];
		[(ProjectTextureItem*)m_itemCurrent deleteAssemblyItem:[selection objectAtIndex:0]];
		[m_ctrlTextureAssembly rearrangeObjects];
	}
}



@end
