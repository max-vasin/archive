//===============================================================================================
//  File:		PreferencesPanel.m
//  Project:	OriginGameToolkit
//	Desc:		Preferences panel implementation
//
//  Created by Max Vasin on 12/16/10.
//  Copyright 2010 Origin. All rights reserved.
//===============================================================================================

// **** IMPORTS ****

#import "PreferencesPanel.h"

// **** IMPLEMENTATION ****

@implementation PreferencesPanel

// selectTextureTool
// Action: perform selectin texturetool location from file dialog
//-----------------------------------------------------------------------------------------------

- (IBAction)selectTextureTool:(id)sender
{
	NSOpenPanel* panel = [NSOpenPanel openPanel];
	
	panel.title						= @"Select texture tool binary location";
	panel.canChooseFiles			= YES;
	panel.canChooseDirectories		= NO;
	panel.allowsMultipleSelection	= NO;
	
	if ([panel runModal] != NSFileHandlingPanelOKButton)
		return;
	
	NSArray* filenames = [panel URLs];
	if (![filenames count])
		return;
	
	m_appPrefs.propBuildTextureTool = [[filenames objectAtIndex:0] path];
}

@end
