//=======================================================================================================================
//  File:		RendererViewController.h
//  Project:	heli
//	Desc:		renderer view controller
//
//  Created by Max Vasin on 3/21/11.
//  Copyright 2011 Origin Games. All rights reserved.
//=======================================================================================================================

// ***** IMPORTS *****

#import <UIKit/UIKit.h>
#import "RenderView.h"
#import "OriBase.h"

// ***** INTERFACE *****

@class OriRenderer;

@interface RenderViewController : UIViewController
{
    OriRenderer*	m_oriRenderer;
}

// ***** INSTANCE METHODS *****

- (id)		initWithRenderer:		(OriRenderer*)renderer;

@end
