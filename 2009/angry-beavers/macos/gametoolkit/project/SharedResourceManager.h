//===============================================================================================
//  File:		SharedResourceManager.h
//  Project:	OriginGameToolkit
//	Desc:		Shared images, icons and sounds manager
//
//  Created by Max Vasin on 3/29/11.
//  Copyright 2011 Origin Games. All rights reserved.
//===============================================================================================

// ***** PROPERTIES *****

#import <Cocoa/Cocoa.h>
#import "Base.h"
#import "BaseObject.h"

// ***** INTERFACE *****

@interface SharedResourceManager : BaseObject
{
    NSMutableDictionary*	m_icons;
}

// ***** CLASS METHODS *****

+ (id)				sharedManager;
+ (NSImage*)		sharedIcon:(NSString*)name;




@end
