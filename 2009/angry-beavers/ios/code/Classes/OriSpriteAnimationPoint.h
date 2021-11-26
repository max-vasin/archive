//=======================================================================================================================
//  File:		OriSpriteAnimationPoint.h
//  Project:	heli
//	Desc:		sprite animation point container class
//
//  Created by Max Vasin on 4/15/11.
//  Copyright 2011 Origin. All rights reserved.
//=======================================================================================================================

// ******** IMPORTS ********

#import <UIKit/UIKit.h>
#import "OriBase.h"
#import "OriXMLNode.h"

// ******** INTERFACE ********

@interface OriSpriteAnimationPoint : NSObject 
{
@private
	NSString*		m_name;
	oriIntPoint		m_position;
}

// ******** PROPERTIES ********

@property (nonatomic, readonly)		NSString*		propName;
@property (nonatomic, readonly)		oriIntPoint		propPosition;

// ******** CLASS METHODS ********

+ (id)	pointWithXML:(OriXMLNode*)node;

// ******** INSTANCE METHODS ********

- (id)	initWithXML:(OriXMLNode*)node;

@end
