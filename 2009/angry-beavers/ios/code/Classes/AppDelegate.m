//=======================================================================================================================
//  File:		AppDelegate.m
//  Project:	heli
//	Desc:		applicaton delegate implementation
//
//  Created by Max Vasin on 3/21/11.
//  Copyright 2011 Origin Games. All rights reserved.
//=======================================================================================================================

// ***** IMPORTS *****

#import "AppDelegate.h"

// ***** DELEGATE PRIVATE METHODS *****

@interface AppDelegate (Private)

- (BOOL)buildEngineObjects;
- (void)destroyEngineObjects;

@end

// ***** IMPLEMENTATION *****

@implementation AppDelegate

// ***** PROPERTIES *****

@synthesize propWindow = m_mainWindow;

//#######################################################################################################################
// DELEGATE SYSTEM INTERFACE
//#######################################################################################################################

// didFinishLaunchingWithOptions: application construction
//-----------------------------------------------------------------------------------------------------------------------
- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions
{
	return YES;
}

// applicationWillResignActive: application become inactive
//-----------------------------------------------------------------------------------------------------------------------
- (void)applicationWillResignActive:(UIApplication *)application
{
	DBGLOG0A(@"###### GO TO BACKGROUND #######");
	[self destroyEngineObjects];
}

// applicationDidBecomeActive: application become active
//-----------------------------------------------------------------------------------------------------------------------
- (void)applicationDidBecomeActive:(UIApplication *)application
{
	DBGLOG0A(@"###### ACTIVATION #######");
	[self buildEngineObjects];
}

// applicationWillTerminate: application will be terminated after ending of this method
//-----------------------------------------------------------------------------------------------------------------------
- (void)applicationWillTerminate:(UIApplication *)application
{

}

// applicationDidEnterBackground: application go to background
//-----------------------------------------------------------------------------------------------------------------------
- (void)applicationDidEnterBackground:(UIApplication *)application
{
    // Handle any background procedures not related to animation here.
}

// applicationWillEnterForeground: application go to foreground
//-----------------------------------------------------------------------------------------------------------------------
- (void)applicationWillEnterForeground:(UIApplication *)application
{
    // Handle any foreground procedures not related to animation here.
}

//#######################################################################################################################
// DELEGATE ITERNAL METHODS
//#######################################################################################################################

// dealloc: destructor
//-----------------------------------------------------------------------------------------------------------------------
- (void)dealloc
{
	[self destroyEngineObjects];
	
	SAFE_RELEASE(m_mainWindow);
    
    [super dealloc];
}

// buildEngineObjects: initial build of topmost engine system objects
//-----------------------------------------------------------------------------------------------------------------------
- (BOOL)buildEngineObjects
{
	m_oriRenderer			= [[OriRenderer alloc] initWithWindow:m_mainWindow];
	m_oriResourceManager	= [[OriResourceManager alloc] initWithRenderer:m_oriRenderer];
	m_oriTimer				= [[OriTimer alloc] init];
	m_oriServer				= [[OriServer alloc] init];
	m_testLevel				= [[GameLevel alloc] initWithTilemapName:@"level zero"];
	[m_oriTimer attach:self];
	[m_oriTimer start];
	return YES;
}

// destroyEngineObjects: final full destruction of all engine system objects
//-----------------------------------------------------------------------------------------------------------------------
- (void)destroyEngineObjects
{
	// delete this
	//[m_oriTimer detach:testScene];
//	SAFE_RELEASE(testScene);
	[m_testLevel release];
//	SAFE_RELEASE(testTilemap);
//	SAFE_RELEASE(testMultiSprite);
	
	SAFE_RELEASE(m_oriServer);
	SAFE_RELEASE(m_oriTimer);
	SAFE_RELEASE(m_oriResourceManager);
	SAFE_RELEASE(m_oriRenderer);
}

- (BOOL)isTimerWeakLink
{
	return NO;
}

- (void)update:(oriFloat)delta
{
	[m_testLevel update:delta];
	
	[m_oriRenderer clear];
	
	[m_testLevel render];
	
	[m_oriRenderer present];
}


@end
