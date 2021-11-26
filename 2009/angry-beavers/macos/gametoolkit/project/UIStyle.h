//===============================================================================================
//  File:		UIStyle.h
//  Project:	OriginGameToolkit
//	Desc:		User interface style data model interface
//
//  Created by Max Vasin on 12/1/10.
//  Copyright 2010 Origin. All rights reserved.
//===============================================================================================

// **** IMPORTS ****

#import <Cocoa/Cocoa.h>
#import "BaseObject.h"

// **** INTERFACE ****

@interface UIStyle : BaseObject 
{
	NSFont*		m_fontLabelEditField;
	NSFont*		m_fontLabelEmptySheet;
	NSFont*		m_fontProjectTree;
	NSFont*		m_fontTableTextCell;
}

// **** PROPERTEIES ****

@property (assign) NSFont*	propFontLabelEditField;
@property (assign) NSFont*	propFontLabelEmptySheet;
@property (assign) NSFont*	propFontProjectTree;
@property (assign) NSFont*	propFontTableTextCell;

@end
