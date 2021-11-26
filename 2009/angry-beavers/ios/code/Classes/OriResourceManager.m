//=======================================================================================================================
//  File:		ResourceManager.m
//  Project:	heli
//	Desc:		resources manager class implementation
//
//  Created by Max Vasin on 3/21/11.
//  Copyright 2011 Origin. All rights reserved.
//=======================================================================================================================

// ***** IMPORTS *****

#import "OriResourceManager.h"

// ***** PRIVATE METHODS *****
@interface OriResourceManager(Private)

- (OriXMLNode*)		findXML:(NSString*)name			atPath:(NSString*)path;
- (OriResource*)	findResource:(NSString*)name	inList:(NSMutableArray*)list;

@end


// ***** IMPLEMENTATION *****

// Shared instance of resource manager
//-----------------------------------------------------------------------------------------------------------------------
static OriResourceManager* g_resourceManager = NULL;

@implementation OriResourceManager

@synthesize propRenderer = m_renderer;

// Returns current renderer texture bits per pixel
//-----------------------------------------------------------------------------------------------------------------------
- (oriUint)propTextureBPP
{
	return m_renderer.propTextureBPP;
}

// ****** CLASS METHODS ******

// Returns shared resource manager object
//-----------------------------------------------------------------------------------------------------------------------
+ (id)sharedManager
{
	return [[g_resourceManager retain] autorelease];
}


// ****** INSTANCE METHODS ******

//#######################################################################################################################
// CONSTRUCTION/DESTRUCTION
//#######################################################################################################################

// initWithRenderer: designated initializer of resource manager
//-----------------------------------------------------------------------------------------------------------------------
- (id)initWithRenderer:(OriRenderer*)renderer
{
	if ( (self = [super init]) )
	{

#ifdef DEBUG
		if (g_resourceManager)
			DBGLOG0A(@"OriResourceManager: warning - only one instance of resource manager currently allowed by system!");
#endif

		m_renderer			= renderer;
		m_pathTextures		= ORIENGINE_RESOURCE_TEXTURES_PATH;
		m_pathSprites		= ORIENGINE_RESOURCE_SPRITES_PATH;
		m_pathMultiSprites	= ORIENGINE_RESOURCE_MULTISPRITES_PATH;
		m_pathTilesets		= ORIENGINE_RESOURCE_TILESETS_PATH;
		m_pathTilemaps		= ORIENGINE_RESOURCE_TILEMAPS_PATH;
		m_pathFonts			= ORIENGINE_RESOURCE_SPRITES_PATH;
		
		m_listTextures		= [[NSMutableArray alloc] init];
		m_listSprites		= [[NSMutableArray alloc] init];
		m_listMultiSprites	= [[NSMutableArray alloc] init];
		m_listTilesets		= [[NSMutableArray alloc] init];
		m_listTilemaps		= [[NSMutableArray alloc] init];
		m_listFonts			= [[NSMutableArray alloc] init];
		g_resourceManager	= self;
				
		DBGLOG0A(@"OriResourceManager: instance allocated and initialized successfully");
	}
	
	return self;
}

// dealloc: destructor
//-----------------------------------------------------------------------------------------------------------------------
- (void)dealloc
{

#ifdef DEBUG
	for (OriTexture* item in m_listTextures)
		DBGLOG1A(@"<WARNING>OriResourceManager: logic error. Texture (%@) still in list on dealloc", item.propName);
	for (OriSprite* item in m_listSprites)
		DBGLOG1A(@"<WARNING>OriResourceManager: logic error. Sprite (%@) still in list on dealloc", item.propName);
	for (OriMultiSprite* item in m_listMultiSprites)
		DBGLOG1A(@"<WARNING>OriResourceManager: logic error. Multisprite (%@) still in list on dealloc", item.propName);
	for (OriTileset* item in m_listTilesets)
		DBGLOG1A(@"<WARNING>OriResourceManager: logic error. Tileset (%@) still in list on dealloc", item.propName);
	for (OriTilemap* item in m_listTilemaps)
		DBGLOG1A(@"<WARNING>OriResourceManager: logic error. Tilemap (%@) still in list on dealloc", item.propName);
	for (OriFont* item in m_listFonts)
		DBGLOG1A(@"<WARNING>OriResourceManager: logic error. Tilemap (%@) still in list on dealloc", item.propName);
#endif
	
	SAFE_RELEASE(m_listTextures);
	SAFE_RELEASE(m_listSprites);
	SAFE_RELEASE(m_listMultiSprites);
	SAFE_RELEASE(m_listTilesets);
	SAFE_RELEASE(m_listTilemaps);
	SAFE_RELEASE(m_listFonts);
	
	if (g_resourceManager == self)
		g_resourceManager = nil;
	
	DBGLOG0A(@"OriResourceManager: instance deallocated");

	[super dealloc];
}

//#######################################################################################################################
// RESOURCES ACCESS
//#######################################################################################################################

// Returns existing or loads new texture by name
//-----------------------------------------------------------------------------------------------------------------------
- (OriTexture*)getTexture:(NSString*)name
{
	OriResource* existing = [self findResource:name inList:m_listTextures];
	
	if (existing)
		return (OriTexture*)[existing retain];
		
	NSAutoreleasePool* localPool = [[NSAutoreleasePool alloc] init];
	
	NSBundle* mainBundle = [NSBundle mainBundle];
	
	NSString* resPath = [mainBundle pathForResource:name ofType:@"png" inDirectory:m_pathTextures];
	
	if (!resPath)
		resPath = [mainBundle pathForResource:name ofType:@"oritex" inDirectory:m_pathTextures];
	
	
	if (!resPath)
	{
		DBGLOG1A(@"<FAIL>OriResourceManager: failed to locate texture resource with name (%@)", name);
		SAFE_RELEASE(localPool);
		return nil;
	}
	
	OriTexture* texture = [[OriTexture alloc] initWithManager:self withName:name fromFile:resPath];
	
	if (!texture)
	{
		DBGLOG1A(@"<FAIL>OriResourceManager: failed to create texture (%@) from file", name);
		SAFE_RELEASE(localPool);
		return nil;
	}
	
	[m_listTextures addObject:texture];
	
	SAFE_RELEASE(localPool);
	
	return texture;
}

// Returns existing or loads new sprite by name
//-----------------------------------------------------------------------------------------------------------------------
- (OriSprite*)getSprite:(NSString*)name
{
	OriResource* existing = [self findResource:name inList:m_listSprites];
	
	if (existing)
		return (OriSprite*)[existing retain];
	
	NSAutoreleasePool* localPool = [[NSAutoreleasePool alloc] init];
	
	OriXMLNode* rootNode = [self findXML:name atPath:m_pathSprites];

	if (!rootNode)
		return nil;

	OriSprite* sprite = [[OriSprite alloc] initWithManager:self withName:name withXML:rootNode];
	
	[m_listSprites addObject:sprite];
		
	SAFE_RELEASE(localPool);
	
	[rootNode release];
			
	return sprite;
}

// Returns existing or loads new multisprite by name
//-----------------------------------------------------------------------------------------------------------------------
- (OriMultiSprite*)getMultiSprite:(NSString*)name
{
	OriResource* existing = [self findResource:name inList:m_listMultiSprites];
	
	if (existing)
		return (OriMultiSprite*)[existing retain];
	
	NSAutoreleasePool* localPool = [[NSAutoreleasePool alloc] init];
	
	OriXMLNode* rootNode = [self findXML:name atPath:m_pathMultiSprites];
	
	if (!rootNode)
		return nil;
	
	OriMultiSprite* multisprite = [[OriMultiSprite alloc] initWithManager:self withName:name withXML:rootNode];
	
	[m_listMultiSprites addObject:multisprite];
	
	SAFE_RELEASE(localPool);
	
	[rootNode release];
	
	return multisprite;
}

// Returns existing or loads new tileset
//-----------------------------------------------------------------------------------------------------------------------
- (OriTileset*)getTileset:(NSString*)name
{
	OriResource* existing = [self findResource:name inList:m_listTilesets];
	
	if (existing)
		return (OriTileset*)[existing retain];
	
	NSAutoreleasePool* localPool = [[NSAutoreleasePool alloc] init];
	
	OriXMLNode* rootNode = [self findXML:name atPath:m_pathTilesets];
	
	if (!rootNode)
		return nil;
	
	OriTileset* tileset = [[OriTileset alloc] initWithManager:self withName:name withXML:rootNode];
	
	[m_listTilesets addObject:tileset];
	
	SAFE_RELEASE(localPool);
	
	[rootNode release];
	
	return tileset;
}

// Returns existing or loads new tilemap
//-----------------------------------------------------------------------------------------------------------------------
- (OriTilemap*)getTilemap:(NSString*)name
{
	OriResource* existing = [self findResource:name inList:m_listTilemaps];
	
	if (existing)
		return (OriTilemap*)[existing retain];
	
	NSAutoreleasePool* localPool = [[NSAutoreleasePool alloc] init];
	
	OriXMLNode* rootNode = [self findXML:name atPath:m_pathTilemaps];
	
	if (!rootNode)
		return nil;
	
	OriTilemap* tilemap = [[OriTilemap alloc] initWithManager:self withName:name withXML:rootNode];
	
	[m_listTilemaps addObject:tilemap];
	
	SAFE_RELEASE(localPool);
	
	[rootNode release];
	
	return tilemap;
}

// Returns existing or loads new font
//-----------------------------------------------------------------------------------------------------------------------
- (OriFont*)getFont:(NSString*)name
{
	OriResource* existing = [self findResource:name inList:m_listFonts];
	
	if (existing)
		return (OriFont*)[existing retain];
	
	NSAutoreleasePool* localPool = [[NSAutoreleasePool alloc] init];
	
	OriXMLNode* rootNode = [self findXML:name atPath:m_pathFonts];
	
	if (!rootNode)
		return nil;
	
	OriFont* font = [[OriFont alloc] initWithManager:self withName:name withXML:rootNode];
	
	[m_listFonts addObject:font];
	
	SAFE_RELEASE(localPool);
	
	[rootNode release];
	
	return font;
}


// Removes resource from manager list (only for iternal use)
//-----------------------------------------------------------------------------------------------------------------------
- (void)removeResource:(OriResource*)resource
{
	if (!resource)
		return;
	
	if		([resource class] == [OriTexture class])
		[m_listTextures removeObject:resource];
	else if ([resource class] == [OriSprite class])
		[m_listSprites removeObject:resource];
	else if ([resource class] == [OriMultiSprite class])
		[m_listMultiSprites removeObject:resource];
	else if ([resource class] == [OriTileset class])
		[m_listTilesets removeObject:resource];
	else if ([resource class] == [OriTilemap class])
		[m_listTilemaps removeObject:resource];
	else if ([resource class] == [OriFont class])
		[m_listFonts removeObject:resource];
}

//#######################################################################################################################
// PRITVATE METHODS
//#######################################################################################################################

// Returns resource by name from list
//-----------------------------------------------------------------------------------------------------------------------
- (OriResource*)findResource:(NSString*)name	inList:(NSMutableArray*)list
{
	if (name)
	{
		for (OriResource* resource in list)
		{
			if ([resource.propName isEqualToString:name])
				return resource;
		}
	}
	
	return nil;
}

// Finds, parse and returns root node of XML file
//-----------------------------------------------------------------------------------------------------------------------
- (OriXMLNode*)findXML:(NSString*)name atPath:(NSString*)path
{
	NSBundle* mainBundle = [NSBundle mainBundle];
	
	NSString* resPath = [mainBundle pathForResource:name ofType:@"xml" inDirectory:path];
	
	if (!resPath)
	{
		DBGLOG1A(@"<FAIL>OriResourceManager: failed to locate XML resource with name (%@)", name);
		return nil;
	}
	
	// no autorelease !!!!
	
	OriXMLNode* rootNode = [OriXMLNode parseFile:resPath];
	
	if (!rootNode)
	{
		DBGLOG1A(@"<FAIL>OriResourceManager: failed to parse XML resource (%@)", resPath);
		return nil;
	}
	
	return rootNode;
}


@end
