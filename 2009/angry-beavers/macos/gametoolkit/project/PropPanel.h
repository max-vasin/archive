//===============================================================================================
//  File:		PropPanel.h
//  Project:	OriginGameToolkit
//	Desc:		Properties panel class interface
//
//  Created by Max Vasin on 11/29/10.
//  Copyright 2010 Origin. All rights reserved.
//===============================================================================================

// IMPORTS
//-----------------------------------------------------------------------------------------------
#import <Cocoa/Cocoa.h>
#import "BaseObject.h"
#import "ProjectItem.h"
#import "AssemblyPartPanel.h"

// [PropPanel]	INTERFACE
//-----------------------------------------------------------------------------------------------
@interface PropPanel : NSPanel 
{
	ProjectItem*	m_itemCurrent;
	NSTabView*		m_propTab;
	
	NSString*		m_dir;
	
	NSDictionary*	m_dictControllers;
	
	IBOutlet NSTabView*				m_tabTextureType;
	IBOutlet AssemblyPartPanel*		m_panelAssemblyItem;
	IBOutlet NSTableView*			m_tableAssembly;
	
	IBOutlet NSObjectController*	m_ctrlTexture;
	IBOutlet NSArrayController*		m_ctrlTextureAssembly;
}

// [PropPanel]	PROPERTIES
//-----------------------------------------------------------------------------------------------
@property (assign)				ProjectItem*		propCurrentItem;
@property (assign)  IBOutlet	NSTabView*			propTabView;
@property (assign)				NSString*			propDir;


- (NSString*)selectDir:(NSString*)initial;
- (void)updateAssemblyView;

- (void)deleteAssemblyItemConfirm:(NSWindow*)sheet returnCode:(NSInteger)returnCode contextInfo:(void*)contextInfo;

- (IBAction) selectDirProject:(id)sender;
- (IBAction) selectDirBuild:(id)sender;
- (IBAction) selectDirCache:(id)sender;

- (IBAction) addAssemblyItem:(id)sender;
- (IBAction) deleteAssemblyItem:(id)sender;
- (IBAction) shiftAssemblyItemUp:(id)sender;
- (IBAction) shiftAssemblyItemDown:(id)sender;


@end
