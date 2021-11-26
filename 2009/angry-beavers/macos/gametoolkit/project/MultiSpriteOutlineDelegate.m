//===============================================================================================
//  File:		MultiSpriteOutlineDelegate.m
//  Project:	OriginGameToolkit
//	Desc:		Multi sprite outline view delegate
//
//  Created by Max Vasin on 3/30/11.
//  Copyright 2011 Origin Games. All rights reserved.
//===============================================================================================

// ****** IMPORTS ******

#import "MultiSpriteOutlineDelegate.h"
#import "ProjectMultiSpriteItem.h"

// ****** IMPLEMENTATION ******

@implementation MultiSpriteOutlineDelegate

// View wants to display cell - we use it to setup cell image
//-----------------------------------------------------------------------------------------------
- (void)outlineView:(NSOutlineView *)outlineView willDisplayCell:(id)cell forTableColumn:(NSTableColumn *)tableColumn item:(id)item
{
	//[cell setImage:[[item representedObject] propIcon]];
}


@end
