//===============================================================================================
//  File:		ProjectMultiSpriteItem.m
//  Project:	OriginGameToolkit
//	Desc:		Project multisprite class implementation
//
//  Created by Max Vasin on 3/28/11.
//  Copyright 2011 Origin Games. All rights reserved.
//===============================================================================================

// ****** IMPORTS ******

#import "ProjectMultiSpriteItem.h"
#import "ProjectRootItems.h"
#import "Project.h"
#import "SharedResourceManager.h"

//###############################################################################################
// Sprite wrapper implementation
//###############################################################################################

// ****** IMPLEMENTAION ******

@implementation SpriteWrapper

// ****** PROPERTIES ******

// Synthesized
//-----------------------------------------------------------------------------------------------
@synthesize propSprite				= m_sprite;

// Sprite
//-----------------------------------------------------------------------------------------------
- (void)setPropSprite:(ProjectSpriteItem*)sprite
{
	if (m_sprite != sprite)
	{
		m_sprite = sprite;
		[m_multisprite syncRecords];
		m_multisprite.propUpdateTextures = YES;
	}
}

- (ProjectSpriteItem*)propSprite
{
	return m_sprite;
}

// Generic name property
//-----------------------------------------------------------------------------------------------
- (void)setPropName:(NSString*)name
{
	self.propSprite = [m_multisprite getSprite:name];
}

- (NSString*)propName
{
	return m_sprite.propName;
}

// ****** CLASS METHODS ******

// Returns new autoreleased sprite wrapper
//-----------------------------------------------------------------------------------------------
+ (id)spriteWrapperWithMultiSprite:(ProjectMultiSpriteItem*)multisprite andSprite:(ProjectSpriteItem*)sprite
{
	return [[[SpriteWrapper alloc] initWithMultiSprite:multisprite andSprite:sprite] autorelease];
}

// ****** INSTANCE METHODS ******

// Initializes wrapper with multisprite and sprite
//-----------------------------------------------------------------------------------------------
- (id)initWithMultiSprite:(ProjectMultiSpriteItem*)multisprite andSprite:(ProjectSpriteItem*)sprite
{
	if ( (self = [super init]) )
	{
		m_multisprite	= multisprite;
		m_sprite		= sprite;
	}
	return self;
}

@end

//###############################################################################################
// Multi sprite item implementation
//###############################################################################################

@interface ProjectMultiSpriteItem (Private)

//- (void)syncRecords;
- (void)makeRecords;

@end

// ****** IMPLEMENTAION ******

@implementation ProjectMultiSpriteItem

// ****** PROPERTIES ******

// Synthesized
//-----------------------------------------------------------------------------------------------
@synthesize propLinkedSprites	= m_linkedSprites;
//@synthesize propCurrentFrame	= m_currentFrame;
//@synthesize propCurrentRecord	= m_currentRecord;
@synthesize propCurrentSprite	= m_currentSprite;
@synthesize propCurrentKey		= m_currentKey;
@synthesize propLayer			= m_layer;

// Base sprite
//-----------------------------------------------------------------------------------------------
- (void)setPropBaseSprite:(ProjectSpriteItem*)sprite
{
	if (sprite != m_baseSprite)
	{
		m_baseSprite = sprite;
		if ([m_baseSprite.propAnimations count])
			self.propBaseAnimation = [m_baseSprite.propAnimations objectAtIndex:0];
		else
			self.propBaseAnimation = nil;
		
		self.propUpdateTextures = YES;
	}
}

- (ProjectSpriteItem*)propBaseSprite
{
	return m_baseSprite;
}

- (BOOL)validatePropBaseSprite:(id*)ioValue error:(NSError**)outError
{
	if (!m_baseSprite)
		return YES;
	
	if (m_baseSprite != *ioValue)
	{
		NSAlert *alert = [[[NSAlert alloc] init] autorelease];
		[alert addButtonWithTitle:@"Accept"];
		[alert addButtonWithTitle:@"Dismiss"];
		[alert setMessageText:@"Warning"];
		[alert setInformativeText:@"Changing base sprite will erase all existing keys. No undo."];
		[alert setAlertStyle:NSWarningAlertStyle];
		
		NSUInteger result = [alert runModal];
		
		if (result == NSAlertSecondButtonReturn)
		{
			*ioValue = m_baseSprite;
		}
	}
		
	return YES;
}

// Base animation
//-----------------------------------------------------------------------------------------------
- (void)setPropBaseAnimation:(SpriteAnimation*)animation
{
	if (m_baseAnimation != animation)
	{
		m_baseAnimation = animation;
		[self makeRecords];
		
		self.propCurrentKey = nil;
		
		if (m_currentRecord && m_currentSprite)
			self.propCurrentKey = [m_currentRecord getKey:m_currentSprite];
	}
}

- (SpriteAnimation*)propBaseAnimation
{
	return m_baseAnimation;
}

- (BOOL)validatePropBaseAnimation:(id*)ioValue error:(NSError**)outError
{
	if (!m_baseAnimation)
		return YES;
	
	if (m_baseAnimation != *ioValue)
	{
		NSAlert *alert = [[[NSAlert alloc] init] autorelease];
		[alert addButtonWithTitle:@"Accept"];
		[alert addButtonWithTitle:@"Dismiss"];
		[alert setMessageText:@"Warning"];
		[alert setInformativeText:@"Changing base animation will erase all existing keys. No undo."];
		[alert setAlertStyle:NSWarningAlertStyle];
		
		NSUInteger result = [alert runModal];
		
		if (result == NSAlertSecondButtonReturn)
		{
			*ioValue = m_baseAnimation;
		}
	}
	return YES;
}

// This property used to refresh view via KVO notification
//-----------------------------------------------------------------------------------------------
- (void)setPropRefreshView:(BOOL)dummy
{
	// dummy
}

- (BOOL)propRefreshView
{
	// dummy
	return YES;
}

// This property used to reload textures via KVO notification
//-----------------------------------------------------------------------------------------------
- (void)setPropUpdateTextures:(BOOL)dummy
{
	// dummy
}

- (BOOL)propUpdateTextures
{
	// dummy
	return YES;
}

// Current frame
//-----------------------------------------------------------------------------------------------
- (void)setPropCurrentFrame:(unsigned int)index
{
	m_currentFrame = index;
	if (m_currentFrame < [m_records count])
		self.propCurrentRecord = [m_records objectAtIndex:m_currentFrame];
}

- (unsigned int)propCurrentFrame
{
	return m_currentFrame;
}

// Current record
//-----------------------------------------------------------------------------------------------
- (void)setPropCurrentRecord:(MultiSpriteRecord*)record
{
	if (m_currentRecord != record)
	{
		m_currentRecord = record;
		
		self.propCurrentKey = [m_currentRecord getKey:self.propCurrentSprite];
	}
}

- (MultiSpriteRecord*)propCurrentRecord
{
	return m_currentRecord;
}

// Selection indexes
//-----------------------------------------------------------------------------------------------
- (void)setPropSelectionIndexes:(NSIndexSet*)indexes
{
	if (!indexes || ![indexes count])
	{
		self.propCurrentSprite	= nil;
		self.propCurrentKey		= nil;
	}
	else
	{
		self.propCurrentSprite	= [m_linkedSprites objectAtIndex:[indexes firstIndex]];
		self.propCurrentKey		= [m_currentRecord getKey:self.propCurrentSprite];
	}
}

- (NSIndexSet*)propSelectionIndexes
{
	if (!m_currentSprite)
		return nil;
	
	return [NSIndexSet indexSetWithIndex:[m_linkedSprites indexOfObject:self.propCurrentSprite]];
}


// ****** INSTANCE METHODS ******

//###############################################################################################
// INITIALIZERS\DESTURCTORS
//###############################################################################################

// Initializes item with parent item
//-----------------------------------------------------------------------------------------------
- (id)initWithParent:(ProjectItem*)parent
{
	if ( (self = [super initWithParent:parent]) )
	{
		m_linkedSprites = [[NSMutableArray alloc] init];
		m_records		= [[NSMutableArray alloc] init];
	}
	return self;
}

// Initializes item with parent item and XML data
//-----------------------------------------------------------------------------------------------
- (id)initWithParent:(ProjectItem*)parent fromXML:(NSXMLElement*)node
{
	if ( (self = [super initWithParent:parent fromXML:node]) )
	{
		m_linkedSprites = [[NSMutableArray alloc] init];
		m_records		= [[NSMutableArray alloc] init];
		
		m_baseSprite	= [self getSprite:[node getFirstChildValue:@"BaseSprite"]];
		m_baseAnimation	= [m_baseSprite getAnimation:[node getFirstChildValue:@"BaseAnimation"]];
		m_layer			= [node getFirstChildFloat:@"Layer"];
		
		NSXMLElement* linkNode = [node getFirstChild:@"LinkedSprites"];
		
		for (NSXMLElement* link in [linkNode children])
			[m_linkedSprites addObject:[SpriteWrapper spriteWrapperWithMultiSprite:self andSprite:[self getSprite:[link getFirstChildValue:@"Sprite"]]]];
		
		NSXMLElement* recsNode = [node getFirstChild:@"Records"];
		
		for (NSXMLElement* recNode in [recsNode children])
			[m_records addObject:[MultiSpriteRecord recordWithMultiSprite:self fromXML:recNode]];
		
		self.propCurrentFrame = 0;
	}
	return self;
}

// Destructor
//-----------------------------------------------------------------------------------------------
- (void)dealloc
{
	SAFE_RELEASE(m_linkedSprites);
	SAFE_RELEASE(m_records);
	
	[super dealloc];
}

// Sets item icon
//-----------------------------------------------------------------------------------------------
- (void)setIcon
{
	m_itemImage = [SharedResourceManager sharedIcon:@"multisprite"];
}

//###############################################################################################
// I\O
//###############################################################################################

// Saves multipsrite to XML schema
//-----------------------------------------------------------------------------------------------
- (NSXMLElement*)saveAsXML:(NSXMLElement*)node
{
	NSXMLElement* thisNode = [super saveAsXML:[node addNewChild:@"MultiSprite"]];
	
	[thisNode addNewChild:@"BaseSprite"		withValue:m_baseSprite.propName];
	[thisNode addNewChild:@"BaseAnimation"	withValue:m_baseAnimation.propName];
	[thisNode addNewChild:@"Layer"			withFloat:m_layer];
	
	NSXMLElement* linkNode = [thisNode addNewChild:@"LinkedSprites"];
	
	for (SpriteWrapper* item in m_linkedSprites)
	{
		NSXMLElement* linkedNode = [linkNode addNewChild:@"Link"];
		
		[linkedNode addNewChild:@"Sprite"	withValue:item.propName];
	}
	
	NSXMLElement* recNode = [thisNode addNewChild:@"Records"];
	
	for (MultiSpriteRecord* record in m_records)
	{
		[record saveXML:recNode];
	}
	
	return thisNode;
}

//###############################################################################################
// BUILDING
//###############################################################################################

// Building multisprite (final engine build)
//-----------------------------------------------------------------------------------------------
- (result)build:(Progressor*)progress
{
	result res = [super build:progress];
	if (FAIL(res))
		return res;
	
	progress.propCurrentState = [NSString stringWithFormat:@"Exporting: %@", m_itemName];
	
	// Make XML scheme of sprite
	
	NSXMLElement* rootNode = [[[NSXMLElement alloc] initWithName:@"MultiSprite"] autorelease];
	
	[rootNode addNewChild:@"Name"			withValue:m_itemName];
	[rootNode addNewChild:@"BaseSprite"		withValue:m_baseSprite.propName];
	[rootNode addNewChild:@"BaseAnimation"	withValue:m_baseAnimation.propName];
	[rootNode addNewChild:@"Layer"			withFloat:m_layer];
	
	// Linked sprites
	
	NSXMLElement* linksNode = [rootNode addNewChild:@"Links"];
	
	for (SpriteWrapper* item in m_linkedSprites)
	{
		NSXMLElement* linkNode = [linksNode addNewChild:@"Link"];
		
		[linkNode addNewChild:@"Sprite" withValue:item.propSprite.propName];
	}
	
	// Records
		
	NSXMLElement* recordsNode = [rootNode addNewChild:@"Records"];
	
	for (MultiSpriteRecord* record in m_records)
	{
		[record buildXML:recordsNode];
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

//###############################################################################################
// ACTIONS
//###############################################################################################

// Adds new record
//-----------------------------------------------------------------------------------------------
- (void)addSprite
{
	// pick first sprite
	if ([m_project.propSpriteList count])
	{
		[m_linkedSprites addObject:[SpriteWrapper spriteWrapperWithMultiSprite:self andSprite:[m_project.propSpriteList objectAtIndex:0]]];
		[self syncRecords];
	}
}

// Deletes record
//-----------------------------------------------------------------------------------------------
- (void)delSprite
{
	if (m_currentSprite)
	{
		unsigned int index = [m_linkedSprites indexOfObject:m_currentSprite];
		
		self.propCurrentKey = nil;
		
		[m_linkedSprites removeObject:m_currentSprite];
		
		m_currentSprite = nil;
		
		if (index)
			index--;
		
		if ([m_linkedSprites count])
		{
			self.propCurrentSprite	= [m_linkedSprites objectAtIndex:index];
		}
		
		[self syncRecords];
	}
}

//###############################################################################################
// ITERNAL
//###############################################################################################

// Returns project sprite item by name
//-----------------------------------------------------------------------------------------------
- (ProjectSpriteItem*)getSprite:(NSString*)name
{
	// Warning: this is not best solution
	// 1. Sprites root can be invalid during loading XML
	// 2. Groups added later will break loading
	
	ProjectSpriteRootItem* spriteNode = (ProjectSpriteRootItem*)((ProjectRootItem*)m_itemParent->m_itemParent).propRootSprites;
	
	return (ProjectSpriteItem*)[spriteNode getChildByName:name];
}

// Returns base animation point by name
//-----------------------------------------------------------------------------------------------
- (FramePoint*)pointWithName:(NSString*)name
{
	return [m_baseAnimation getFramePoint:name];
}

// Returns origin point of base animation
//-----------------------------------------------------------------------------------------------
- (FramePoint*)originPoint
{
	return m_baseAnimation.propOriginPoint;
}

// Returns index of linked sprite
//-----------------------------------------------------------------------------------------------
- (unsigned int)indexOfSprite:(SpriteWrapper*)sprite
{
	return [m_linkedSprites indexOfObject:sprite];
}

// Returns linked sprite by index
//-----------------------------------------------------------------------------------------------
- (SpriteWrapper*)spriteAtIndex:(unsigned int)index
{
	if (index < [m_linkedSprites count])
		return [m_linkedSprites objectAtIndex:index];
	
	return nil;
}

// Synchronize all records keys
//-----------------------------------------------------------------------------------------------
- (void)syncRecords
{
	for (MultiSpriteRecord* record in m_records)
		[record syncKeys];
}

// Returns record at index
//-----------------------------------------------------------------------------------------------
- (MultiSpriteRecord*)recordAtIndex:(uint)index
{
	if (index < [m_records count])
		return [m_records objectAtIndex:index];
	
	return nil;
}


// Makes new record set
//-----------------------------------------------------------------------------------------------
- (void)makeRecords
{
	self.propCurrentRecord	= nil;
	self.propCurrentKey		= nil;

	SAFE_RELEASE(m_records);
		
	m_records = [[NSMutableArray alloc] init];
	
	if (m_baseAnimation)
	{
		for (int i = 0; i < m_baseAnimation.propFramesCount; i++)
		{
			[m_records addObject:[MultiSpriteRecord recordWithMultiSprite:self]];
		}
	}
	
	[self syncRecords];
	
	if ([m_records count])
		self.propCurrentRecord = [m_records objectAtIndex:0];
}


@end
