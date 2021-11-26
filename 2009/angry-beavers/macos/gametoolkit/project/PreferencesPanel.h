//===============================================================================================
//  File:		PreferencesPanel.h
//  Project:	OriginGameToolkit
//	Desc:		Application preferences panel class
//
//  Created by Max Vasin on 12/16/10.
//  Copyright 2010 Origin. All rights reserved.
//===============================================================================================

// **** IMPORTS ****

#import <Cocoa/Cocoa.h>
#import "BaseObject.h"
#import "ApplicationPreferences.h"

// **** INTERFACE ****

@interface PreferencesPanel : NSPanel 
{
	IBOutlet NSObjectController*		m_ctrlAppPrefs;
	IBOutlet ApplicationPreferences*	m_appPrefs;
}

// **** METHODS ****


// **** ACTIONS ****

- (IBAction)selectTextureTool:(id)sender;

@end
