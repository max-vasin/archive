//=======================================================================================================================
//  File:		heliViewController.m
//  Project:	heli
//	Desc:		renderer view controller class implementation
//
//  Created by Max Vasin on 3/21/11.
//  Copyright 2011 Origin Games. All rights reserved.
//=======================================================================================================================

// ***** IMPORTS *****

#import <QuartzCore/QuartzCore.h>
#import "RenderViewController.h"
#import "RenderView.h"
#import "OriRenderer.h"

// ***** IMPLEMENTATION *****

@implementation RenderViewController

// initWithRenderer: designated initializer of resource manager
//-----------------------------------------------------------------------------------------------------------------------
- (id)initWithRenderer:(OriRenderer *)renderer
{
	if ( (self = [super init]) )
	{
		m_oriRenderer = renderer;
		self.view = m_oriRenderer.propRenderView;
		
		DBGLOG0A(@"RenderViewController: initialized successfully");
	}
	return self;
}

// dealloc: destructor
//-----------------------------------------------------------------------------------------------------------------------
- (void)dealloc
{
    [super dealloc];
}

// viewWillAppear: view will appear handler
//-----------------------------------------------------------------------------------------------------------------------
- (void)viewWillAppear:(BOOL)animated
{
    [super viewWillAppear:animated];
	
	DBGLOG0A(@"RenderViewController: view will appear");
}

// viewWillDisappear: view will disappear handler
//-----------------------------------------------------------------------------------------------------------------------
- (void)viewWillDisappear:(BOOL)animated
{
    [super viewWillDisappear:animated];
}

// viewDidUnload: view unloading handler
//-----------------------------------------------------------------------------------------------------------------------
- (void)viewDidUnload
{
	[super viewDidUnload];
}

// didReceiveMemoryWarning: memory warning handler
//-----------------------------------------------------------------------------------------------------------------------
- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
}

- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)orientation
{
	if (orientation ==  UIInterfaceOrientationLandscapeRight)
		return YES;
	
	return NO;
}

- (void)didRotateFromInterfaceOrientation:(UIInterfaceOrientation)oldOrientation
{
	[m_oriRenderer rotateTo:self.interfaceOrientation];
}

@end
