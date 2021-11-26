//===============================================================================================
//  File:		AssemblyPartPanel.m
//  Project:	OriginGameToolkit
//	Desc:		Assembly part UI panel implementation
//
//  Created by Max Vasin on 12/14/10.
//  Copyright 2010 Origin. All rights reserved.
//===============================================================================================

// **** IMPORTS ****

#import "AssemblyPartPanel.h"
#import "PropPanel.h"

// **** IMPLEMENTATION ****

@implementation AssemblyPartPanel

// **** INSTANCE METHODS ****

// doModalForItem
// Shows panel in modal mode with item provided to controller
//-----------------------------------------------------------------------------------------------

- (void)doModalForItem:(TextureAssemblyItem*)item
{
	[m_ctrlAssemblyItem setContent:nil];
	[m_ctrlAssemblyItem setContent:item];
	
	m_itemCurrent = item;
	
	[NSApp runModalForWindow:self];
}

// **** ACTIONS ****

// confirmDialog
// Stops modal mode and closes panel
//-----------------------------------------------------------------------------------------------

- (IBAction)confirmDialog:(id)sender
{
	[NSApp stopModal];
	[self close];
	[m_panelProperties updateAssemblyView];
}

// selectSource
// Open dialog to select file or directory
//-----------------------------------------------------------------------------------------------

- (IBAction)selectSource:(id)sender
{
	NSOpenPanel* panel = [NSOpenPanel openPanel];
	
	panel.title						= (m_itemCurrent.propType == eAssemblyItemSingleFile) ? @"Select file" : @"Select directory";
	panel.canChooseFiles			= (m_itemCurrent.propType == eAssemblyItemSingleFile) ? YES : NO;
	panel.canChooseDirectories		= (m_itemCurrent.propType == eAssemblyItemSingleFile) ? NO : YES;
	panel.allowsMultipleSelection	= NO;
	
	if ([panel runModalForDirectory:m_itemCurrent.propSource file:nil types:nil] != NSFileHandlingPanelOKButton)
		return;
	
	NSArray* filenames = [panel URLs];
	if (![filenames count])
		return;
	
	m_itemCurrent.propSource = [[filenames objectAtIndex:0] path];	
}


@end
