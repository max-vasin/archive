//===============================================================================================
//  File:		AppDelegate.h
//  Project:	OriginGameToolkit
//  Desc:		Applcation delegate interface declaration
//
//  Created by Max Vasin on 11/24/10.
//  Copyright 2010 Origin. All rights reserved.
//===============================================================================================

// **** IMPORTS ****

#import <Cocoa/Cocoa.h>
#import "BaseObject.h"
#import "PropPanel.h"
#import "Project.h"
#import "UIStyle.h"
#import "ApplicationPreferences.h"
#import "Progressor.h"
#import "SliderZoomValueTransformer.h"
#import "FloatStepperTransformer.h"

// **** INTERFACE ****

@interface AppDelegate : BaseObject <NSApplicationDelegate, NSWindowDelegate> 
{
@public
	Project*							m_projectCurrent;				// Current project
	UIStyle*							m_uiStyle;						// Style data model
	NSImage*							m_imgFacepalm;					// Facepalm image

	IBOutlet NSWindow*					m_mainWindow;					// Main window
	IBOutlet PropPanel*					m_propPanel;					// Properties panel
	IBOutlet NSOutlineView*				m_treeProject;					// Project tree view
	IBOutlet NSOutlineView*				m_treeMultiSprite;				// Multi sprite tree view
	IBOutlet NSPanel*					m_panelAppPrefs;				// Application preferences
	IBOutlet NSPanel*					m_panelProgress;				// Progress panel
	IBOutlet NSPanel*					m_panelAnimationPoints;
	IBOutlet NSPanel*					m_panelAnimationLink;			// Animation links panel
	IBOutlet NSPanel*					m_panelMapToolbox;				// Tilemap toolbox panel
	IBOutlet NSObjectController*		m_controllerStyle;				// Style controller outlet
	IBOutlet NSObjectController*		m_ctrlProject;					// Project controller outlet
	IBOutlet NSObjectController*		m_controllerAppPrefs;			// Application preferences controller
	IBOutlet NSArrayController*			m_ctrlSpriteAnimations;			// Sprite animations outlet
	IBOutlet ViewsTab*					m_viewTab;						// View tab (openGL)
	IBOutlet NSTabView*					m_tabAppPrefs;					// Application preferences tab
	IBOutlet ApplicationPreferences*	m_appPrefs;						// Application preferences data model
	IBOutlet Progressor*				m_progress;						// Progressor data model
	IBOutlet NSMenuItem*				m_recentSubMenu;				// Recent project submenu

}

// **** PROPERTIES ****

@property (assign)		Project*					propCurrentProject;
@property (readonly)	ApplicationPreferences*		propAppPrefs;

// **** METHODS ****

- (void)makeNewCurrentProject;
- (void)resetControllers;
- (void)rebuildRecentSubMenu;

- (void)windowWillClose:(NSNotification*)notification;

- (void)expandMultiSpriteTree;

- (void)showMapToolbox:(BOOL)show;

// **** ACTIONS ****

- (IBAction)	addTexture:			(id)sender;
- (IBAction)	addShape:			(id)sender;
- (IBAction)	addSprite:			(id)sender;
- (IBAction)	addFont:			(id)sender;
- (IBAction)	addMultiSprite:		(id)sender;
- (IBAction)	addTileset:			(id)sender;
- (IBAction)	addTileMap:			(id)sender;
- (IBAction)	deleteItem:			(id)sender;
- (IBAction)	buildProject:		(id)sender;
- (IBAction)	openAnimationPoints:(id)sender;
- (IBAction)	openAnimationLink:	(id)sender;

- (IBAction)	fileNew:			(id)sender;
- (IBAction)	fileSave:			(id)sender;
- (IBAction)	fileOpen:			(id)sender;
- (IBAction)	fileOpenRecent:		(id)sender;

- (IBAction)	openPreferences:	(id)sender;

@end
