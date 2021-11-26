//===============================================================================================
//  File:		AppDelegate.m
//  Project:	Origin Game Toolkit
//	Desc:		Applcation delegate from root object
//
//  Created by Max Vasin on 11/24/10.
//  Copyright 2010 Origin. All rights reserved.
//===============================================================================================

// **** IMPORTS ****

#import "AppDelegate.h"
#import "SharedResourceManager.h"

// **** IMPLEMENTATION ****

@implementation AppDelegate

// **** PROPERTIES ****

@synthesize propCurrentProject	= m_projectCurrent;
@synthesize propAppPrefs		= m_appPrefs;

// **** METHODS ****

// DidFinishLaunching
// Event signalizes that application finishing loading NIB
//------------------------------------------------------------------------------------------------

- (void)applicationDidFinishLaunching:(NSNotification*)aNotification 
{
	[SliderZoomValueTransformer registerSelf];
	[FloatStepperTransformer registerSelf];
	
	[m_mainWindow setDelegate:self];
	
	[[SharedResourceManager alloc] init];
	
	[m_viewTab setTabViewType:NSNoTabsBezelBorder];
	
	m_uiStyle = [[UIStyle alloc] init];
	[m_controllerStyle setContent:m_uiStyle];
	
	m_imgFacepalm = [[NSImage alloc] initWithContentsOfFile:[[NSBundle mainBundle] pathForResource:@"facepalm" ofType:@"png" inDirectory:@""]];
	
	[self makeNewCurrentProject];
	
	if (m_appPrefs.propLastProject)
		[m_projectCurrent load:m_appPrefs.propLastProject];
	
	[self rebuildRecentSubMenu];
	
	[m_treeProject reloadData];
	
	[self resetControllers];
}

// WillTerminate
// Event signalizes that application will be totally terminated
//------------------------------------------------------------------------------------------------

- (void)applicationWillTerminate:(NSNotification *)aNotification
{
	SAFE_RELEASE(m_propPanel);
	
	[m_projectCurrent unbindAll];
	
	SAFE_RELEASE(m_projectCurrent);
	SAFE_RELEASE(m_uiStyle);
	SAFE_RELEASE(m_imgFacepalm);
	[[SharedResourceManager sharedManager] release];
	
	[m_appPrefs save];
	
	[self log:"$Cobject destroyed\n"];
}

// makeNewCurrentProject
// This method deallocates current project, create new and reestablish its bindings
//------------------------------------------------------------------------------------------------

- (void)makeNewCurrentProject
{
	Project* oldProject = m_projectCurrent;
	
	[oldProject unbindAll];
	
	SAFE_RELEASE(oldProject);
	
	//[m_ctrlProject setContent:nil];

	self.propCurrentProject = [[Project alloc] initWithTree:m_treeProject andController:m_ctrlProject andPropPanel:m_propPanel andViewsTab:m_viewTab];
	
	//[m_ctrlProject setContent:m_projectCurrent];

	
}

// resetControllers
// Called when model mutated somehow but not thru properties - this updates all controllers
//-----------------------------------------------------------------------------------------------

- (void)resetControllers
{
	//[m_ctrlProject setContent:nil];
	//[m_ctrlProject setContent:m_projectCurrent];
}

// rebuildRecentSubMenu
// Called when recent project changed
//-----------------------------------------------------------------------------------------------

- (void)rebuildRecentSubMenu
{
	NSMenu* recentMenu = [[[NSMenu alloc] init] autorelease];
	
	for (NSString* path in m_appPrefs.propRecentProjects)
	{
		NSMenuItem* recentItem = [[[NSMenuItem alloc] initWithTitle:path action:@selector(fileOpenRecent:) keyEquivalent:@""] autorelease];
		
		[recentItem setTarget:self];
		
		[recentMenu addItem:recentItem];
	}	 
	
	[m_recentSubMenu setSubmenu:recentMenu];
}

- (void)expandMultiSpriteTree
{
	[m_treeMultiSprite expandItem:nil expandChildren:YES];
}

// addTexture
// Action: main menu - 'Add texture'
//-----------------------------------------------------------------------------------------------

- (IBAction)addTexture:(id)sender
{
	[m_projectCurrent addTexture];
}

// addShape
// Action: main menu - 'Add shape'
//-----------------------------------------------------------------------------------------------

- (IBAction)addShape:(id)sender
{
	[m_projectCurrent addShape];
}

// addSprite
// Action: main menu - 'Add sprite'
//-----------------------------------------------------------------------------------------------

- (IBAction)addSprite:(id)sender
{
	[m_projectCurrent addSprite];
}

// addFont
// Action: main menu - 'Add font'
//-----------------------------------------------------------------------------------------------

- (IBAction)addFont:(id)sender
{
	[m_projectCurrent addFont];
}

// addFont
// Action: main menu - 'Add multisprite'
//-----------------------------------------------------------------------------------------------

- (IBAction)addMultiSprite:(id)sender
{
	[m_projectCurrent addMultiSprite];
}

// Action: main menu - 'Add tileset'
//-----------------------------------------------------------------------------------------------
- (IBAction)addTileset:(id)sender
{
	[m_projectCurrent addTileset];
}

// Action: main menu - 'Add tilemap'
//-----------------------------------------------------------------------------------------------
- (IBAction)addTileMap:(id)sender
{
	[m_projectCurrent addTileMap];
}

// deleteItem
// Action: main menu - 'Delete item'
//-----------------------------------------------------------------------------------------------

- (IBAction)deleteItem:(id)sender
{
	NSBeginAlertSheet(@"Please confirm delete item", @"Delete", nil, @"Dismiss", m_mainWindow, self, @selector(deleteItemConfirm:returnCode:contextInfo:), nil, nil,  @"There is no undo for this operation");
}

// deleteItemConfirm
// User assepted item deletion
//-----------------------------------------------------------------------------------------------

- (void)deleteItemConfirm:(NSWindow*)sheet returnCode:(NSInteger)returnCode contextInfo:(void*)contextInfo
{
	if (returnCode == NSAlertDefaultReturn)
		[m_projectCurrent deleteCurrentItem];
}

// buildProject
// Action: main menu - 'Project->Build'
//-----------------------------------------------------------------------------------------------

- (IBAction)buildProject:(id)sender
{
	[m_panelProgress makeKeyAndOrderFront:self];
	
	m_progress.propCurrentAnimate	= YES;
	m_progress.propOverallAnimate	= YES;
	
	@try 
	{
		[m_projectCurrent build:m_progress];
	}
	@catch (NSException* e) 
	{
		NSAlert *alert = [[[NSAlert alloc] init] autorelease];
		[alert addButtonWithTitle:@"OK"];
		[alert setMessageText:[e name]];
		[alert setInformativeText:[e reason]];
		[alert setAlertStyle:NSWarningAlertStyle];
		[alert setIcon:m_imgFacepalm];
		
		[alert beginSheetModalForWindow:m_mainWindow modalDelegate:nil didEndSelector:nil contextInfo:nil];
	}
	
	[m_panelProgress close];
}


// fileNew
// Action: main menu - 'File new'
//-----------------------------------------------------------------------------------------------
- (IBAction)fileNew:(id)sender
{
	[self makeNewCurrentProject];
	
//	[m_treeProject reloadData];
	
//	[self resetControllers];
}

// fileSave
// Action: main menu - 'File save'
//-----------------------------------------------------------------------------------------------

- (IBAction)fileSave:(id)sender
{
	if (!m_projectCurrent)
		return;
	
	NSString* fileName = nil;
	
	if (m_projectCurrent.propIsNewProject)
	{
		NSSavePanel* panel = [NSSavePanel savePanel];
		
		panel.title					= @"Select file to save project";
		panel.canCreateDirectories	= NO;
		panel.allowedFileTypes		= [NSArray arrayWithObject:@"oriproj"];
		panel.allowsOtherFileTypes	= NO;
		panel.nameFieldStringValue	= m_projectCurrent.propName;
		
		if ([panel runModal] != NSFileHandlingPanelOKButton)
			return;
		
		fileName = [[panel.nameFieldStringValue fileName] stringByAppendingString:@".oriproj"];
		fileName = [[panel.directoryURL.path stringByAppendingString:@"/"] stringByAppendingString:fileName];
	}
	else 
	{
		fileName	= m_projectCurrent.propFileName;
	}

	[m_projectCurrent save:fileName];
	
	m_appPrefs.propLastProject = fileName;
	
	[self rebuildRecentSubMenu];
}

// fileOpen
// Action: main menu - 'File open'
//-----------------------------------------------------------------------------------------------

- (IBAction)fileOpen:(id)sender
{
	NSOpenPanel* panel = [NSOpenPanel openPanel];
	
	panel.title						= @"Select file to load";
	panel.canChooseFiles			= YES;
	panel.canChooseDirectories		= NO;
	panel.allowsMultipleSelection	= NO;
	
	if ([panel runModal] != NSFileHandlingPanelOKButton)
		return;
	
	NSArray* filenames = [panel URLs];
	if (![filenames count])
		return;
	
	NSString* filename = [[filenames objectAtIndex:0] path];
	
	[self makeNewCurrentProject];
	
	[m_projectCurrent load:filename];
	
	[m_treeProject reloadData];
	
	//[self resetControllers];
	
	m_appPrefs.propLastProject = filename;
	
	[self rebuildRecentSubMenu];
}

// fileOpenRecent
// Action: main menu - 'Recent'
//-----------------------------------------------------------------------------------------------

- (IBAction)fileOpenRecent:(id)sender
{
	NSString* filename = [(NSMenuItem*)sender title];
	
	[self makeNewCurrentProject];
	
	[m_projectCurrent load:filename];
	
	[m_treeProject reloadData];
	
	[self resetControllers];
	
	m_appPrefs.propLastProject = filename;
	
	[self rebuildRecentSubMenu];
}

// openPreferences
// Action: main menu - 'Preferences'
//-----------------------------------------------------------------------------------------------

- (IBAction)openPreferences:(id)sender
{
	[m_panelAppPrefs makeKeyAndOrderFront:self];
}

// Opens animation frame points panel
//-----------------------------------------------------------------------------------------------
- (IBAction)openAnimationPoints:(id)sender
{
	[m_panelAnimationPoints makeKeyAndOrderFront:self];
}

// Opens animation link
//-----------------------------------------------------------------------------------------------
- (IBAction)openAnimationLink:(id)sender
{
	[m_panelAnimationLink makeKeyAndOrderFront:self];
}

// Show/hides map toolbox
//-----------------------------------------------------------------------------------------------
- (void)showMapToolbox:(BOOL)show
{
	if (show)
		[m_panelMapToolbox makeKeyAndOrderFront:self];
	else
		[m_panelMapToolbox close];
}


// windowWillClose
// This window delegate method used to terminate application on main window closing
//-----------------------------------------------------------------------------------------------

- (IBAction)windowWillClose:(id)sender
{
	[NSApp terminate:self];
}


@end
