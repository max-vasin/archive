//===============================================================================================
//  File:		UIStyle.m
//  Project:	OriginGameToolkit
//	Desc:		User interface style data model implementation
//
//  Created by Max Vasin on 12/1/10.
//  Copyright 2010 Origin. All rights reserved.
//===============================================================================================

// **** IMPORTS ****

#import "UIStyle.h"

// **** IMPLEMENTATION ****

@implementation UIStyle

// **** SYNTHESIZED PROPERTIES ****

@synthesize propFontLabelEditField		= m_fontLabelEditField;
@synthesize propFontLabelEmptySheet		= m_fontLabelEmptySheet;
@synthesize propFontProjectTree			= m_fontProjectTree;
@synthesize propFontTableTextCell		= m_fontTableTextCell;

// **** INSTANCE METHODS IMPLEMENTATION ****

// init
// Initializes new style instance
//-----------------------------------------------------------------------------------------------

- (id)init
{
	if (self = [super init])
	{
		m_fontLabelEditField	= [NSFont fontWithName:@"Helvetica" size:11.0];
		m_fontLabelEmptySheet	= [NSFont fontWithName:@"Helvetica" size:20.0];
		m_fontProjectTree		= [NSFont fontWithName:@"Helvetica" size:11.0];
		m_fontTableTextCell		= [NSFont fontWithName:@"Helvetica" size:11.0];
	}
	return self;
}

// dealloc
// Style destructor
//-----------------------------------------------------------------------------------------------

- (void)dealloc
{
	SAFE_RELEASE(m_fontLabelEditField);
	SAFE_RELEASE(m_fontLabelEmptySheet);
	SAFE_RELEASE(m_fontProjectTree);
	SAFE_RELEASE(m_fontTableTextCell);
	
	[super dealloc];
}

@end
