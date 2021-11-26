//===============================================================================================
//  File:		ApplicationPreferences.m
//  Project:	OriginGameToolkit
//  Desc:		Applicaton preferences container data model class implementation
//
//  Created by Max Vasin on 12/3/10.
//  Copyright 2010 Origin. All rights reserved.
//===============================================================================================

// **** IMPORTS ****

#import "ApplicationPreferences.h"

// **** IMPLEMENTATION ****

@implementation ApplicationPreferences

// **** SYNTHESIZED PROPERTIES ****

@synthesize propBuildTextureTool				= m_buildTextureTool;
@synthesize propTextureViewGridDashMultiplier	= m_textureViewGridDashMultiplier;	
@synthesize propTextureViewGridHSpace			= m_textureViewGridHSpace;	
@synthesize propTextureViewGridVSpace			= m_textureViewGridVSpace;			
@synthesize propTextureViewGridBackColor		= m_textureViewGridBackColor;		
@synthesize propTextureViewGridDashColor		= m_textureViewGridDashColor;		
@synthesize propTextureViewGridOriginColor		= m_textureViewGridOriginColor;	
@synthesize propTextureViewTextureAlpha			= m_textureViewTextureAlpha;

@synthesize propShapeViewGridDashMultiplier		= m_shapeViewGridDashMultiplier;	
@synthesize propShapeViewGridHSpace				= m_shapeViewGridHSpace;	
@synthesize propShapeViewGridVSpace				= m_shapeViewGridVSpace;			
@synthesize propShapeViewGridBackColor			= m_shapeViewGridBackColor;		
@synthesize propShapeViewGridDashColor			= m_shapeViewGridDashColor;
@synthesize propShapeViewGridOriginColor		= m_shapeViewGridOriginColor;	
@synthesize propShapeViewShapeColor				= m_shapeViewShapeColor;
@synthesize propShapeviewClosingLineColor		= m_shapeViewClosingLineColor;
@synthesize propShapeViewVertexColor			= m_shapeViewVertexColor;
@synthesize propShapeViewSelectLineColor		= m_shapeViewSelectLineColor;
@synthesize propShapeViewSelectVertexColor		= m_shapeViewSelectVertexColor;
@synthesize propShapeViewShapeWidth				= m_shapeViewShapeWidth;
@synthesize propShapeViewVertexSize				= m_shapeViewVertexSize;

@synthesize propSpriteViewGridDashMultiplier	= m_spriteViewGridDashMultiplier;
@synthesize propSpriteViewGridBackColor			= m_spriteViewGridBackColor;
@synthesize propSpriteViewGridDashColor			= m_spriteViewGridDashColor;
@synthesize propSpriteViewGridOriginColor		= m_spriteViewGridOriginColor;
@synthesize propSpriteViewFrameColor			= m_spriteViewFrameColor;
@synthesize propSpriteViewFrameSelectColor		= m_spriteViewFrameSelectColor;
@synthesize propSpriteViewFrameAlpha			= m_spriteViewFrameAlpha;
@synthesize propSpriteViewFrameSelectAlpha		= m_spriteViewFrameSelectAlpha;
@synthesize propSpriteViewAxisWidth				= m_spriteViewAxisWidth;

@synthesize	propSpriteSeqViewCheckerColor		= m_spriteSeqViewCheckerColor;
@synthesize propSpriteSeqViewActiveAlpha		= m_spriteSeqViewActiveAlpha;
@synthesize propSpriteSeqViewInactiveAlpha		= m_spriteSeqViewInactiveAlpha;

@synthesize propRecentProjects					= m_recentProjects;

// **** IMPLEMENTED PROPERTIES ****

// Last project property
//-----------------------------------------------------------------------------------------------

- (NSString*)propLastProject
{
	if (![m_recentProjects count])
		return nil;
	
	return [m_recentProjects objectAtIndex:0];
}

- (void)setPropLastProject:(NSString*)item
{
	int index		= 0;
	BOOL founded	= NO;
	
	for (NSString* path in m_recentProjects)
	{
		if ([path isEqualToString:item])
		{
			founded =YES;
			break;
		}
		index++;
	}
	
	if (founded)
		[m_recentProjects removeObjectAtIndex:index];
	
	[m_recentProjects insertObject:item atIndex:0];
}


// **** INSTANCE METHODS ****

// init 
// Default initializer, called by NIB loader
//-----------------------------------------------------------------------------------------------

- (id)init
{
	if ((self = [super init]))
	{
		m_recentProjects = [[NSMutableArray alloc] init];
		
		// Try to load preferences
		result res;
		@try
		{
			res = [self load];
		}
		@catch (NSException* e) 
		{
		}
		
		if (ISOK(res))
			return self;
		
		// Defaults
		
		// *** Build options ***
		
		m_buildTextureTool					= @"/Developer/Platforms/iPhoneOS.platform/Developer/usr/bin/texturetool";
		
		// *** Texture view ***
		
		m_textureViewGridDashMultiplier		= 2;
		m_textureViewGridHSpace				= 25;
		m_textureViewGridVSpace				= 25;
		m_textureViewGridBackColor			= [[NSColor colorWithCalibratedRed:0.98f green:0.98f blue:0.98f alpha:1.0f] retain];
		m_textureViewGridDashColor			= [[NSColor colorWithCalibratedRed:0.5f green:0.5f blue:0.5f alpha:1.0f] retain];
		m_textureViewGridOriginColor		= [[NSColor colorWithCalibratedRed:0.5f green:0.5f blue:0.5f alpha:1.0f] retain];
		m_textureViewTextureAlpha			= 1.0f;
		
		// *** Shape view ***
		
		m_shapeViewGridDashMultiplier		= 2;
		m_shapeViewGridHSpace				= 25;
		m_shapeViewGridVSpace				= 25;
		m_shapeViewGridBackColor			= [[NSColor colorWithCalibratedRed:0.98f green:0.98f blue:0.98f alpha:1.0f] retain];
		m_shapeViewGridDashColor			= [[NSColor colorWithCalibratedRed:0.5f green:0.5f blue:0.5f alpha:1.0f] retain];
		m_shapeViewGridOriginColor			= [[NSColor colorWithCalibratedRed:0.5f green:0.5f blue:0.5f alpha:1.0f] retain];

		// *** Sprite view ***
		
		m_spriteViewGridDashMultiplier		= 2;
		m_spriteViewGridBackColor			= [[NSColor colorWithCalibratedRed:0.98f green:0.98f blue:0.98f alpha:1.0f] retain];
		m_spriteViewGridDashColor			= [[NSColor colorWithCalibratedRed:0.5f green:0.5f blue:0.5f alpha:1.0f] retain];
		m_spriteViewGridOriginColor			= [[NSColor colorWithCalibratedRed:0.5f green:0.5f blue:0.5f alpha:1.0f] retain];
		m_spriteViewFrameColor				= [[NSColor colorWithCalibratedRed:1.0f green:1.0f blue:1.0f alpha:1.0f] retain];
		m_spriteViewFrameSelectColor		= [[NSColor colorWithCalibratedRed:0.0f green:1.0f blue:0.0f alpha:1.0f] retain];
		m_spriteViewFrameAlpha				= 0.5f;
		m_spriteViewFrameSelectAlpha		= 0.5f;
		m_spriteViewAxisWidth				= 3.0f;
		
		// *** Sprite sequence view ***
		m_spriteSeqViewCheckerColor			= [[NSColor colorWithCalibratedRed:1.0f green:1.0f blue:1.0f alpha:1.0f] retain];
		m_spriteSeqViewActiveAlpha			= 1.0f;
		m_spriteSeqViewInactiveAlpha		= 0.5f;
	}
	return self;
}

// dealloc 
// Destructor
//-----------------------------------------------------------------------------------------------

- (void)dealloc
{
	// Try to save preferences
	@try 
	{
		[self save];
	}
	@catch (NSException * e) 
	{
	}
	
	// Mem cleanup
	SAFE_RELEASE(m_textureViewGridBackColor);
	SAFE_RELEASE(m_textureViewGridDashColor);
	SAFE_RELEASE(m_textureViewGridOriginColor);
	
	SAFE_RELEASE(m_shapeViewGridBackColor);
	SAFE_RELEASE(m_shapeViewGridDashColor);
	SAFE_RELEASE(m_shapeViewGridOriginColor);
	
	SAFE_RELEASE(m_spriteViewGridBackColor);
	SAFE_RELEASE(m_spriteViewGridDashColor);
	SAFE_RELEASE(m_spriteViewGridOriginColor);
	SAFE_RELEASE(m_spriteViewFrameColor);
	SAFE_RELEASE(m_spriteViewFrameSelectColor);
	
	SAFE_RELEASE(m_recentProjects);
	
	[super dealloc];
}

// save 
// Stores preferences in XML scheme
//-----------------------------------------------------------------------------------------------

- (result)save
{
	NSString* file = [[[NSBundle mainBundle] bundlePath] stringByAppendingString:APP_PREFERENCES_FILE];
	
	NSXMLDocument* doc = [[[NSXMLDocument alloc] init] autorelease];
	
	doc.version				= @"1.0";
	doc.characterEncoding	= @"UTF-8";
	
	NSXMLElement* root	= [[[NSXMLElement alloc] initWithName:@"OriginGameToolkitPreferences"] autorelease];
	
	// Common
	NSXMLElement* commonRoot  = [root addNewChild:@"Common"];
	
	NSXMLElement* recentRoot  = [commonRoot addNewChild:@"RecentProjects"];
	
	for (NSString* path in m_recentProjects)
		[recentRoot addNewChild:@"Path" withValue:path];
	
	// Building
	NSXMLElement* buildRoot   = [root addNewChild:@"Building"];
	
	[buildRoot addNewChild:@"Texturetool"	withValue:m_buildTextureTool];
	
	// Texture view
	NSXMLElement* texViewRoot = [root addNewChild:@"TextureView"];
	
	[texViewRoot addNewChild:@"GridDashMultiplier"	withInteger:m_textureViewGridDashMultiplier];
	[texViewRoot addNewChild:@"GridHSpace"			withInteger:m_textureViewGridHSpace];
	[texViewRoot addNewChild:@"GridVSpace"			withInteger:m_textureViewGridVSpace];
	[texViewRoot addNewChild:@"GridBackColor"		withObject:m_textureViewGridBackColor];
	[texViewRoot addNewChild:@"GridDashColor"		withObject:m_textureViewGridDashColor];
	[texViewRoot addNewChild:@"GridOriginColor"		withObject:m_textureViewGridOriginColor];
	[texViewRoot addNewChild:@"TextureAlpha"		withFloat:m_textureViewTextureAlpha];
	
	// Shape view
	NSXMLElement* shapeViewRoot = [root addNewChild:@"ShapeView"];
	
	[shapeViewRoot addNewChild:@"GridDashMultiplier"	withInteger:m_shapeViewGridDashMultiplier];
	[shapeViewRoot addNewChild:@"GridHSpace"			withInteger:m_shapeViewGridHSpace];
	[shapeViewRoot addNewChild:@"GridVSpace"			withInteger:m_shapeViewGridVSpace];
	[shapeViewRoot addNewChild:@"GridBackColor"			withObject:m_shapeViewGridBackColor];
	[shapeViewRoot addNewChild:@"GridDashColor"			withObject:m_shapeViewGridDashColor];
	[shapeViewRoot addNewChild:@"GridOriginColor"		withObject:m_shapeViewGridOriginColor];
	[shapeViewRoot addNewChild:@"ShapeColor"			withObject:m_shapeViewShapeColor];
	[shapeViewRoot addNewChild:@"ClosingLineColor"		withObject:m_shapeViewClosingLineColor];
	[shapeViewRoot addNewChild:@"VertexColor"			withObject:m_shapeViewVertexColor];
	[shapeViewRoot addNewChild:@"SelectLineColor"		withObject:m_shapeViewSelectLineColor];
	[shapeViewRoot addNewChild:@"SelectVertexColor"		withObject:m_shapeViewSelectVertexColor];
	[shapeViewRoot addNewChild:@"ShapeWidth"			withFloat:m_shapeViewShapeWidth];
	[shapeViewRoot addNewChild:@"VertexSize"			withFloat:m_shapeViewVertexSize];
	
	// Sprite view
	
	NSXMLElement* spriteViewRoot = [root addNewChild:@"SpriteView"];
	
	[spriteViewRoot addNewChild:@"GridDashMultiplier"	withInteger:m_spriteViewGridDashMultiplier];
	[spriteViewRoot addNewChild:@"GridBackColor"		withObject:m_spriteViewGridBackColor];
	[spriteViewRoot addNewChild:@"GridDashColor"		withObject:m_spriteViewGridDashColor];
	[spriteViewRoot addNewChild:@"GridOriginColor"		withObject:m_spriteViewGridOriginColor];
	[spriteViewRoot addNewChild:@"FrameColor"			withObject:m_spriteViewFrameColor];
	[spriteViewRoot addNewChild:@"FrameSelectColor"		withObject:m_spriteViewFrameSelectColor];
	[spriteViewRoot addNewChild:@"FrameAlpha"			withFloat:m_spriteViewFrameAlpha];
	[spriteViewRoot addNewChild:@"FrameSelectAlpha"		withFloat:m_spriteViewFrameSelectAlpha];
	[spriteViewRoot addNewChild:@"AxisWidth"			withFloat:m_spriteViewAxisWidth];
	
	// Sprite sequence view
	NSXMLElement* spriteSeqViewRoot = [root addNewChild:@"SpriteSequenceView"];
	
	[spriteSeqViewRoot addNewChild:@"CheckerColor"		withObject:m_spriteSeqViewCheckerColor];
	[spriteSeqViewRoot addNewChild:@"ActiveAlpha"		withFloat:m_spriteSeqViewActiveAlpha];
	[spriteSeqViewRoot addNewChild:@"InactiveAlpha"		withFloat:m_spriteSeqViewInactiveAlpha];
		
	doc.rootElement = root;
		
	NSData* output = [doc XMLDataWithOptions:NSXMLDocumentIncludeContentTypeDeclaration | NSXMLNodePrettyPrint];
	
	if (![output writeToFile:file atomically:YES])
	{
		DBG_START	[self log:"$C[save] - ERROR: cannot write XML scheme to [%s]\n", [file UTF8String]];	DBG_END
		return RES_ERR_FILESYSTEM;
	}
	
	return RES_OK;
}

// load
// Loads preferences from XML file
//-----------------------------------------------------------------------------------------------

- (result)load
{
	NSString* file = [[[NSBundle mainBundle] bundlePath] stringByAppendingString:APP_PREFERENCES_FILE];	
	
	NSData*	data = [NSData dataWithContentsOfFile:file];
	
	if (!data)
	{
		DBG_START	[self log:"$C[load] - ERROR: cant read data from [%s]\n", [file UTF8String]];	DBG_END
		return RES_ERR_FILESYSTEM;
	}
	
	NSError* parseError = nil;
	
	NSXMLDocument* doc = [[[NSXMLDocument alloc] initWithData:data options:0 error:&parseError] autorelease];
	
	if (!doc)
	{
		DBG_START	
		[self log:"$C[load] - ERROR: cant parse XML in [%s]\n", [file UTF8String]];	
		[self log:"$CXML parser: %s\n", [[parseError description] UTF8String]];
		DBG_END
		return RES_ERR_FILESYSTEM;
	}
	
	// XML schema loaded successfully
	NSXMLElement* root = doc.rootElement;
	
	if (!root)
	{
		DBG_START [self log:"$C[load] - ERROR: no root node in XML scheme\n"];
		return RES_ERR_NO_OBJECT;
	}
	
	// Common preferences
	NSXMLElement* commonRoot  = [root getFirstChild:@"Common"];
	if (commonRoot)
	{
		NSXMLElement* recentRoot = [commonRoot getFirstChild:@"RecentProjects"];
		if (recentRoot)
		{
			for (NSXMLElement* nodePath in recentRoot.children)
				[m_recentProjects addObject:[nodePath objectValue]];
		}
	}
	
	// Build preferences
	NSXMLElement* buildRoot	  = [root getFirstChild:@"Building"];
	
	if (!buildRoot)
	{
		DBG_START [self log:"$C[load] - ERROR: <Build> node not found\n"];
		return RES_ERR_NO_OBJECT;
	}
	
	m_buildTextureTool	= [buildRoot getFirstChildValue:@"Texturetool" placeholder: @"/Developer/Platforms/iPhoneOS.platform/Developer/usr/bin/texturetool"];
	
	// Texture view preferences
	NSXMLElement* texViewRoot = [root getFirstChild:@"TextureView"];
	
	if (!texViewRoot)
	{
		DBG_START [self log:"$C[load] - ERROR: <TextureView> node not found\n"];
		return RES_ERR_NO_OBJECT;
	}
	
	m_textureViewGridDashMultiplier		= [texViewRoot  getFirstChildInteger:@"GridDashMultiplier"	placeholder:2];
	m_textureViewGridHSpace				= [texViewRoot  getFirstChildInteger:@"GridHSpace"			placeholder:25];
	m_textureViewGridVSpace				= [texViewRoot  getFirstChildInteger:@"GridVSpace"			placeholder:25];
	m_textureViewGridBackColor			= [[texViewRoot getFirstChildObject:@"GridBackColor"		placeholder:[NSColor redColor]]	retain];
	m_textureViewGridDashColor			= [[texViewRoot getFirstChildObject:@"GridDashColor"		placeholder:[NSColor redColor]]	retain];
	m_textureViewGridOriginColor		= [[texViewRoot getFirstChildObject:@"GridOriginColor"		placeholder:[NSColor redColor]] retain];
	m_textureViewTextureAlpha			= [texViewRoot  getFirstChildFloat:@"TextureAlpha"			placeholder:1.0f];
	
	// Shape view preferences
	NSXMLElement* shapeViewRoot = [root getFirstChild:@"ShapeView"];
	
	if (!shapeViewRoot)
	{
		DBG_START [self log:"$C[load] - ERROR: <ShapeView> node not found\n"];
		return RES_ERR_NO_OBJECT;
	}
	
	m_shapeViewGridDashMultiplier	= [shapeViewRoot  getFirstChildInteger:@"GridDashMultiplier" placeholder:2];
	m_shapeViewGridHSpace			= [shapeViewRoot  getFirstChildInteger:@"GridHSpace"		 placeholder:25];
	m_shapeViewGridVSpace			= [shapeViewRoot  getFirstChildInteger:@"GridVSpace"		 placeholder:25];
	m_shapeViewGridBackColor		= [[shapeViewRoot getFirstChildObject:@"GridBackColor"		 placeholder:[NSColor redColor]] retain];
	m_shapeViewGridDashColor		= [[shapeViewRoot getFirstChildObject:@"GridDashColor"		 placeholder:[NSColor redColor]] retain];
	m_shapeViewGridOriginColor		= [[shapeViewRoot getFirstChildObject:@"GridOriginColor"	 placeholder:[NSColor redColor]] retain];
	m_shapeViewShapeColor			= [[shapeViewRoot getFirstChildObject:@"ShapeColor"			 placeholder:[NSColor redColor]] retain];
	m_shapeViewClosingLineColor		= [[shapeViewRoot getFirstChildObject:@"ClosingLineColor"	 placeholder:[NSColor redColor]] retain];
	m_shapeViewSelectLineColor		= [[shapeViewRoot getFirstChildObject:@"SelectLineColor"	 placeholder:[NSColor greenColor]] retain];
	m_shapeViewVertexColor			= [[shapeViewRoot getFirstChildObject:@"VertexColor"		 placeholder:[NSColor yellowColor]] retain];
	m_shapeViewSelectVertexColor	= [[shapeViewRoot getFirstChildObject:@"SelectVertexColor"   placeholder:[NSColor magentaColor]] retain];
	m_shapeViewShapeWidth			= [shapeViewRoot  getFirstChildFloat:@"ShapeWidth"			 placeholder:3.0f];
	m_shapeViewVertexSize			= [shapeViewRoot  getFirstChildFloat:@"VertexSize"			 placeholder:8.0f];
	
	// Shape view preferences
	NSXMLElement* spriteViewRoot = [root getFirstChild:@"SpriteView"];
	
	if (!spriteViewRoot)
	{
		DBG_START [self log:"$C[load] - ERROR: <SpriteView> node not found\n"];
		return RES_ERR_NO_OBJECT;
	}
	
	m_spriteViewGridDashMultiplier	= [spriteViewRoot  getFirstChildInteger:@"GridDashMultiplier"	placeholder:2];
	m_spriteViewGridBackColor		= [[spriteViewRoot getFirstChildObject:@"GridBackColor"			placeholder:[NSColor redColor]] retain];
	m_spriteViewGridDashColor		= [[spriteViewRoot getFirstChildObject:@"GridDashColor"			placeholder:[NSColor redColor]] retain];
	m_spriteViewGridOriginColor		= [[spriteViewRoot getFirstChildObject:@"GridOriginColor"		placeholder:[NSColor redColor]] retain];
	m_spriteViewFrameColor			= [[spriteViewRoot getFirstChildObject:@"FrameColor"			placeholder:[NSColor redColor]] retain];
	m_spriteViewFrameSelectColor	= [[spriteViewRoot getFirstChildObject:@"FrameSelectColor"		placeholder:[NSColor redColor]] retain];
	m_spriteViewFrameAlpha			= [spriteViewRoot  getFirstChildFloat:@"FrameAlpha"				placeholder:0.5f];
	m_spriteViewFrameSelectAlpha	= [spriteViewRoot  getFirstChildFloat:@"FrameSelectAlpha"		placeholder:0.5f];
	m_spriteViewAxisWidth			= [spriteViewRoot  getFirstChildFloat:@"AxisWidth"				placeholder:3.0f];
	
	// Sprite sequence view preferences
	NSXMLElement* spriteSeqViewRoot = [root getFirstChild:@"SpriteSequenceView"];
	
	if (!spriteSeqViewRoot)
	{
		DBG_START [self log:"$C[load] - ERROR: <SpriteSequenceView> node not found\n"];
		return RES_ERR_NO_OBJECT;
	}
	
	m_spriteSeqViewCheckerColor		= [[spriteSeqViewRoot getFirstChildObject:@"CheckerColor"		placeholder:[NSColor whiteColor]] retain];
	m_spriteSeqViewActiveAlpha		= [spriteSeqViewRoot  getFirstChildFloat:@"ActiveAlpha"			placeholder:1.0f];
	m_spriteSeqViewInactiveAlpha	= [spriteSeqViewRoot  getFirstChildFloat:@"InactiveAlpha"		placeholder:0.5f];
		
	return RES_OK;
}



@end
