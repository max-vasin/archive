//=======================================================================================================================
//  File:		OriResource.h
//  Project:	heli
//	Desc:		base resouce class interface
//
//  Created by Max Vasin on 3/23/11.
//  Copyright 2011 Origin. All rights reserved.
//=======================================================================================================================

// ***** IMPORTS *****

#import <UIKit/UIKit.h>
#import "OriBase.h"

// ***** INTERFACE *****

@class OriResourceManager;

@interface OriResource : NSObject 
{
	NSUInteger				m_refs;
    OriResourceManager*		m_resMan;
	NSString*				m_name;
	BOOL					m_removing;
}

// ***** PROPERTIES *****

@property (nonatomic, copy)		NSString*	propName;
@property (nonatomic, readonly)	oriUint		propSysMemUsed;
@property (nonatomic, readonly)	oriUint		propVideoMemUsed;

// ***** INSTANCE METHODS *****

- (id)initWithManager:(OriResourceManager*)resman withName:(NSString*)name;

@end
