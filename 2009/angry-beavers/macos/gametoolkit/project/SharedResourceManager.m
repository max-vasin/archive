//===============================================================================================
//  File:		SharedResourceManager.m
//  Project:	OriginGameToolkit
//	Desc:		Shared images, icons and sounds manager implementation
//
//  Created by Max Vasin on 3/29/11.
//  Copyright 2011 Origin Games. All rights reserved.
//===============================================================================================

// ***** PROPERTIES *****

#import "SharedResourceManager.h"

static SharedResourceManager* g_sharedManager = NULL;

// ***** PRIVATE METHODS *****
@interface SharedResourceManager(Private)

- (void)loadIcons;

@end


// ***** IMPLEMENTATION *****

@implementation SharedResourceManager

// ***** CLASS METHODS *****

// Returns shared instance manager
//-----------------------------------------------------------------------------------------------
+ (id)sharedManager
{
	return g_sharedManager;
}

// Returns shared icon if any loaded
//-----------------------------------------------------------------------------------------------
+ (NSImage*)sharedIcon:(NSString*)name
{
	if (g_sharedManager)
		return (NSImage*)[[g_sharedManager->m_icons objectForKey:name] retain];
	
	return nil;
}

// ***** INSTANCE METHODS *****

//###############################################################################################
// INITIALIZIZERS\DESTRUCTORS
//###############################################################################################

// Initializes resource manager
//-----------------------------------------------------------------------------------------------
- (id)init
{
	if ( (self = [super init]) )
	{
		if (g_sharedManager)
		{
			[self release];
			
			return g_sharedManager;
		}
		else
		{
			g_sharedManager = self;
			
			m_icons = [[NSMutableDictionary alloc] init];
			
			[self loadIcons];
		}
		
	}
	return self;
}

// Destructor
//-----------------------------------------------------------------------------------------------
- (void)dealloc
{
    if (g_sharedManager == self)
		g_sharedManager = NULL;
	
	SAFE_RELEASE(m_icons);
	
	[super dealloc];
}

//###############################################################################################
// PRIVATE METHODS
//###############################################################################################

// Scans icon directory and load all images
//-----------------------------------------------------------------------------------------------
- (void)loadIcons
{
	NSString* iconDir = [NSString stringWithFormat:@"%@/Contents/Resources/icons", [[NSBundle mainBundle] bundlePath]];
	
	NSFileManager* fileMan = [[[NSFileManager alloc] init] autorelease];
	
	NSError* err = nil;
	
	NSArray* fileList = [fileMan contentsOfDirectoryAtPath:iconDir error:&err];
	
	for (NSString* file in fileList)
	{
		if ([[[file fileExtension] uppercaseString] isEqualToString:@"PNG"])
		{
			NSImage* image = [[NSImage alloc] initWithContentsOfFile:[iconDir stringByAppendingFormat:@"/%@", file]];
			
			if (!image)
				continue;
			
			[m_icons setObject:image forKey:[file fileName]];
			
			SAFE_RELEASE(image);
			
			NSLog(@"image %@ loaded", file);
		}
	}
}



@end
