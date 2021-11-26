//=======================================================================================================================
//  File:		EAGLView.m
//  Project:	heli
//	Desc:		EAGL surface wrapper implementation
//
//  Created by Max Vasin on 3/21/11.
//  Copyright 2011 Origin Games. All rights reserved.
//=======================================================================================================================

// ***** IMPORTS *****

#import <QuartzCore/QuartzCore.h>
#import "RenderView.h"
#import "OriServer.h"
#import "Vector2.h"
#import "OriResourceManager.h"
#import "OriRenderer.h"

// ***** IMPLEMENTATION *****

@implementation RenderView

// ***** PROPERTIES *****

// ***** METHODS *****

// layerClass: returns class of view layer
//-----------------------------------------------------------------------------------------------------------------------
+ (Class)layerClass
{
    return [CAEAGLLayer class];
}

// initWithFrame: init new view of given size
//-----------------------------------------------------------------------------------------------------------------------
- (id)initWithFrame:(CGRect)frame andRenderer:(OriRenderer*)renderer
{
	if ( (self = [super initWithFrame:frame]) )
    {
		m_renderer = renderer;
		
		CAEAGLLayer* eaglLayer = (CAEAGLLayer*)self.layer;
        
        eaglLayer.opaque = TRUE;
        eaglLayer.drawableProperties = [NSDictionary dictionaryWithObjectsAndKeys:
                                        [NSNumber numberWithBool:FALSE],		kEAGLDrawablePropertyRetainedBacking,
                                        ORIENGINE_RENDERER_BACKBUFFER_FORMAT,	kEAGLDrawablePropertyColorFormat,
                                        nil];
		
		DBGLOG0A(@"RendererView: instance allocated and initialized");
	}
    
    return self;
}

// dealloc: destructor
//-----------------------------------------------------------------------------------------------------------------------
- (void)dealloc
{
	DBGLOG0A(@"RendererView: instance deallocated");
	
	[super dealloc];
}

- (void)touchesBegan:(NSSet*)touches withEvent:(UIEvent*)event
{
	NSEnumerator *enumerator = [touches objectEnumerator];
	
	UITouch* touch = [[enumerator allObjects] objectAtIndex:0]; 
	
	CGPoint pos = [m_renderer projectTouch:[touch locationInView:self]];
	
	[[OriServer sharedServer] pushCommand:[OriServerCommand commandId:eServerCmdPlayerLook Args:[NSArray arrayWithObject:[Vector2 vectorWithX:pos.x Y:pos.y]]]];
}

- (void)touchesMoved:(NSSet*)touches withEvent:(UIEvent*)event
{
	NSEnumerator *enumerator = [touches objectEnumerator];
	
	UITouch* touch = [[enumerator allObjects] objectAtIndex:0]; 
	
	CGPoint pos = [m_renderer projectTouch:[touch locationInView:self]];
	
	[[OriServer sharedServer] pushCommand:[OriServerCommand commandId:eServerCmdPlayerLook Args:[NSArray arrayWithObject:[Vector2 vectorWithX:pos.x Y:pos.y]]]];

}

- (void)touchesEnded:(NSSet*)touches withEvent:(UIEvent*)event
{
/*
	NSEnumerator *enumerator = [touches objectEnumerator];
	
	UITouch* touch = [[enumerator allObjects] objectAtIndex:0]; 
	
	CGPoint pos = [touch locationInView:self];
	
	[[OriServer sharedServer] pushCommand:[OriServerCommand commandId:eServerCmdPlayerLook Args:[NSArray arrayWithObject:[Vector2 vectorWithX:pos.x * 2 Y:(480 - pos.y) * 2]]]];
*/
}

@end
