//===============================================================================================
//  File:		AssemblyPartPanel.h
//  Project:	OriginGameToolkit
//	Desc:		Assembly item UI panel
//
//  Created by Max Vasin on 12/14/10.
//  Copyright 2010 Origin. All rights reserved.
//===============================================================================================

// **** IMPORTS ****

#import <Cocoa/Cocoa.h>
#import "BaseObject.h"
#import "TextureAssemblyItem.h"


// Forward properties panel class declaration

@class PropPanel;

// **** INTERFACE ****

@interface AssemblyPartPanel : NSPanel 
{
	TextureAssemblyItem*			m_itemCurrent;
	IBOutlet NSObjectController*	m_ctrlAssemblyItem;
	IBOutlet PropPanel*				m_panelProperties;
}

// **** INSTANCE METHODS ****

- (void)doModalForItem:(TextureAssemblyItem*)item;

// **** ACTIONS ****

- (IBAction) confirmDialog:(id)sender;
- (IBAction) selectSource:(id)sender;

@end
