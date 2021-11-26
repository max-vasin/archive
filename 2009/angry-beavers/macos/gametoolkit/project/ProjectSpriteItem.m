//===============================================================================================
//  File:		ProjectSpriteItem.m
//  Project:	OriginGameToolkit
//	Desc:		Project sprite item container class implementation
//
//  Created by Max Vasin on 11/24/10.
//  Copyright 2010 Origin. All rights reserved.
//===============================================================================================

// IMPORTS

#import "ProjectSpriteItem.h"
#import "AppDelegate.h"
#import "SharedResourceManager.h"

@implementation ProjectSpriteItem

// Synthesized properties
//-----------------------------------------------------------------------------------------------
@synthesize propAnimations	= m_animations;
@synthesize propCurrentAnim = m_currentAnim;
@synthesize propType		= m_type;

// Generic propTexture property
//-----------------------------------------------------------------------------------------------
- (ProjectTextureItem*)propTexture
{
	//ProjectRootItem* root = (ProjectRootItem*)m_project.propRootItem;
	
	//return (ProjectTextureItem*)[root.propRootTextures getChildByName:m_texture];
	
	ProjectRootItem* root = (ProjectRootItem*)m_itemParent->m_itemParent;
	
	return (ProjectTextureItem*)[root.propRootTextures getChildByName:m_texture];
}

- (void)setPropTexture:(ProjectTextureItem*)item
{
	m_texture = [item.propName copy];
}

// Sprite types list property
//-----------------------------------------------------------------------------------------------
- (NSArray*)propTypeList
{
	return [NSArray arrayWithObjects:@"sequence", @"movie", nil];
}

// initWithParent: overriding (ProjectItem) initializer
//-----------------------------------------------------------------------------------------------
- (id)initWithParent:(ProjectItem*)parent
{
	if ( (self = [super initWithParent:parent]) )
	{
		m_animations = [[NSMutableArray alloc] init];
	}
	return self;
}

// dealloc: destructor
//-----------------------------------------------------------------------------------------------
- (void)dealloc
{
	SAFE_RELEASE(m_animations);
	
	[super dealloc];
}

// Sets item icon
//-----------------------------------------------------------------------------------------------
- (void)setIcon
{
	m_itemImage = [SharedResourceManager sharedIcon:@"sprite"];
}

// initXMLItem: overriding (ProjectItem) initializer
//-----------------------------------------------------------------------------------------------
- (id)initWithParent:(ProjectItem*)parent fromXML:(NSXMLElement*)node
{
	if ( (self = [super initWithParent:parent fromXML:node]) )
	{
		m_texture	= [[node getFirstChildValue:@"Texture"] copy];
		m_type		= [node getFirstChildInteger:@"Type"];
		
		NSXMLElement* animNode = [node getFirstChild:@"Animations"];
		
		self.propCurrentAnim = nil;
		
		for (NSXMLElement* itemNode in [animNode children])
		{
			SpriteAnimation* item = [[[SpriteAnimation alloc] initWithSprite:self fromXML:itemNode] autorelease];
			
			[m_animations addObject:item];
		
			if (!self.propCurrentAnim)
				self.propCurrentAnim = item;
		}
	}
	return self;
}

// SaveAsXML: makes XML representation fo current item and attach it to node
//-----------------------------------------------------------------------------------------------
- (NSXMLElement*)saveAsXML:(NSXMLElement*)node
{
	NSXMLElement* thisNode = [node addNewChild:@"Sprite"];
	
	[thisNode addNewChild:@"Name"		withValue:m_itemName];
	[thisNode addNewChild:@"Texture"	withValue:m_texture];
	[thisNode addNewChild:@"Type"		withInteger:m_type];
	
	NSXMLElement* animNode = [thisNode addNewChild:@"Animations"];
	
	for (SpriteAnimation* item in m_animations)
	{
		[item saveXML:animNode];
	}
	
	return thisNode;
}

// Return animation by name
//-----------------------------------------------------------------------------------------------
- (SpriteAnimation*)getAnimation:(NSString*)name
{
	return [m_animations getObjectByProp:@"propName" Value:name];
}

// addAnimation: adds new animation to sprite
//-----------------------------------------------------------------------------------------------
- (void)addAnimation
{
	SpriteAnimation* newAnim = [[[SpriteAnimation alloc] initWithSprite:self] autorelease];
	
	[m_animations addObject:newAnim];

	self.propCurrentAnim = newAnim;
}

// Deletes current animation
//-----------------------------------------------------------------------------------------------
- (void)delAnimation
{
	if (!self.propCurrentAnim)
		return;
	
	unsigned int index = [m_animations indexOfObject:self.propCurrentAnim];
	
	[m_animations removeObject:m_currentAnim];
	
	if ([m_animations count])
	{
		if (index)
			index--;
		
		self.propCurrentAnim = [m_animations objectAtIndex:index];
	}
}

// Returns animations linked to assembly item
//-----------------------------------------------------------------------------------------------
- (NSArray*)getLinkedAnimations:(TextureAssemblyItem*)item
{
	NSMutableArray* temp = [[NSMutableArray alloc] initWithCapacity:10];
	
	for (SpriteAnimation* anim in m_animations)
	{
		AnimationLink* link = anim.propLink;
		
		if (link.propAssembly == item)
			[temp addObject:anim];
	}
	
	NSArray* result = [NSArray arrayWithArray:temp];
	
	SAFE_RELEASE(temp);
	
	return result;
}


// build
// Overriding - building sprite
//-----------------------------------------------------------------------------------------------
- (result)build:(Progressor*)progress
{
	result res = [super build:progress];
	if (FAIL(res))
		return res;
	
	progress.propCurrentState = [NSString stringWithFormat:@"Exporting: %@", m_itemName];
	
	// Make XML scheme of sprite
	
	NSXMLElement* rootNode = [[[NSXMLElement alloc] initWithName:@"Sprite"] autorelease];
	
	[rootNode addNewChild:@"Name"			withValue:m_itemName];
	[rootNode addNewChild:@"Texture"		withValue:m_texture];
	[rootNode addNewChild:@"AnimationCount"	withInteger:[m_animations count]];
	
	NSXMLElement* animNode = [rootNode addNewChild:@"Animations"];
	
	for (SpriteAnimation* animItem in m_animations)
	{
		[animItem buildXML:animNode];
	}
	
	NSString* outFile = [NSString stringWithFormat:@"%@/%@.xml", self.propItemBuildDirectory, m_itemName];
		
	NSXMLDocument* doc = [[[NSXMLDocument alloc] init] autorelease];
	
	doc.version				= @"1.0";
	doc.characterEncoding	= @"UTF-8";
	
	doc.rootElement			= rootNode;
	
	NSData* output = [doc XMLDataWithOptions:NSXMLDocumentIncludeContentTypeDeclaration | NSXMLNodePrettyPrint];
	
	if (![output writeToFile:outFile atomically:YES])
	{
		DBG_START	[self log:"$C[build] - ERROR: cannot write XML schema to [%s]\n", [outFile UTF8String]];	DBG_END
		return RES_ERR_FILESYSTEM;
	}
	
	return res;
}

@end
