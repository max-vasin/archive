//=======================================================================================================================
//  File:		AppDelegate.h
//  Project:	heli
//	Desc:		applicaton delegate interface
//
//  Created by Max Vasin on 3/21/11.
//  Copyright 2011 Origin Games. All rights reserved.
//=======================================================================================================================

// ***** IMPORTS *****

#import <UIKit/UIKit.h>

#import "OriBase.h"
#import "OriTimer.h"
#import "OriServer.h"
#import "RenderView.h"
#import "RenderViewController.h"
#import "OriRenderer.h"
#import "OriResourceManager.h"
#import "OriScene.h"
#import "OriSceneObject.h"
#import "OriSpriteRenderable.h"
#import "OriMultiSpriteContainer.h"
#import "RedneckHelicopter.h"
#import "GameLevel.h"

// ***** INTERFACE *****

@interface AppDelegate : NSObject <UIApplicationDelegate, OriTimerEntry> 
{
@private
    
	UIWindow*				m_mainWindow;
	OriTimer*				m_oriTimer;
	OriRenderer*			m_oriRenderer;
	OriResourceManager*		m_oriResourceManager;
	OriServer*				m_oriServer;
	
	OriScene*	testScene;
	
	
	GameLevel*				m_testLevel;
	
	RedneckHelicopter* testMultiSprite;
	
	
}

@property (nonatomic, retain)	IBOutlet UIWindow*	propWindow;

@end

