//===============================================================================================
//  File:		ProjectItem.m
//  Project:	OriginGameToolkit
//	Desc:		Project item base class implementation
//
//  Created by Max Vasin on 11/24/10.
//  Copyright 2010 Origin. All rights reserved.
//===============================================================================================

// ***** IMPORTS *****

#import "ProjectItem.h"
#import "Project.h"

// ***** IMPLEMENTATION *****

@implementation ProjectItem

// ***** PROPERTIES *****

// Synthesized
//-----------------------------------------------------------------------------------------------
@synthesize propProject				= m_project;
@synthesize propChildren			= m_itemChildren;
@synthesize propLocalBuildDirectory = m_localBuildDirectory;
@synthesize propChildName			= m_childName;
@synthesize propExpandable			= m_expandable;
@synthesize propParentItem			= m_itemParent;

// Item name
//-----------------------------------------------------------------------------------------------
- (NSString*)propName
{
	return m_itemName;
}

- (void)setPropName:(NSString*)value
{
	if (m_itemName != value)
	{
		SAFE_RELEASE(m_itemName);
		m_itemName = [value copy];
		
		[m_project updateTree:self];
	}
}

- (BOOL)validatePropName:(id*)ioValue error:(NSError**)outError
{
	return YES;
}

// Item image
//-----------------------------------------------------------------------------------------------
- (NSImage*)propImage
{
	return m_itemImage;
}

- (void)setPropImage:(NSImage*)value
{
	if (m_itemImage != value)
	{
		[m_itemImage release];
		m_itemImage = [value retain];
	}
	
	[m_project updateTree:self];
}

// Item full build directory
//-----------------------------------------------------------------------------------------------
- (NSString*)propItemBuildDirectory
{
	if (!m_itemParent)
		return m_project.propBuildDirectory;
	
	return [NSString stringWithFormat:@"%@/%@", m_itemParent.propItemBuildDirectory, m_localBuildDirectory];
}

// Children count
//-----------------------------------------------------------------------------------------------
- (NSUInteger)propChildrenCount
{
	return [m_itemChildren count];
}

// ***** INSTANCE METHODS *****

//###############################################################################################
// INITIALIZIZERS\DESTRUCTORS
//###############################################################################################

// Initializes new item with project container
//-----------------------------------------------------------------------------------------------
- (id)initWithProject:(Project*)project
{
	if ( (self = [super init]) )
	{
		m_project				= project;
		m_itemChildren			= [[NSMutableArray alloc] init];
		m_itemName				= [[NSMutableString alloc] init];
		m_localBuildDirectory	= @"";
		
		self.propName			= @"undefined item";
		self.propChildName		= @"unknown";
		
		[self setIcon];
	}
	return self;
}

// Initializes new item with parent item
//-----------------------------------------------------------------------------------------------
- (id)initWithParent:(ProjectItem*)parent
{
	if ( (self = [self initWithProject:parent->m_project]) )
	{
		[parent addChild:self];
		
		self.propName = [parent getUniqueName];
		
		[parent updateName];
	}
	return self;
}

// Initializes new item with project and XML data
//-----------------------------------------------------------------------------------------------
- (id)initWithProject:(Project*)project fromXML:(NSXMLElement*)node
{
	if ( (self = [self initWithProject:project]) )
	{
		self.propName = [node getFirstChildValue:@"Name"];
	}
	return self;
}

// Initializes new item with parent and XML data
//-----------------------------------------------------------------------------------------------
- (id)initWithParent:(ProjectItem*)parent fromXML:(NSXMLElement*)node
{
	if ( (self = [((ProjectItem*)self) initWithParent:parent]) )
	{
		self.propName = [node getFirstChildValue:@"Name"];		
	}
	return self;
}

// Destructor
//-----------------------------------------------------------------------------------------------
- (void)dealloc
{
	SAFE_RELEASE(m_itemChildren);
	SAFE_RELEASE(m_itemName);
	SAFE_RELEASE(m_itemImage);
	m_itemParent = nil;
	m_project    = nil;
	
	[super dealloc];
}

// Called during initialize to set up item image
//-----------------------------------------------------------------------------------------------
- (void)setIcon
{
	
}


//###############################################################################################
// CHILDREN ITEMS CONTROL
//###############################################################################################

// Adds new child
//-----------------------------------------------------------------------------------------------
- (ProjectItem*)addChild:(ProjectItem*)item
{
	if (!item)
		return item;
	
	if (![m_itemChildren containsObject:item])
	{
		item->m_itemParent = self;
	
		[m_itemChildren addObject:item];
	}
	return item;
}


// Return item at specified index
//-----------------------------------------------------------------------------------------------
- (ProjectItem*)getChildAtIndex:(NSInteger)index
{
	if (index >= [m_itemChildren count])
		return nil;
	
	return [m_itemChildren objectAtIndex:index];
}

// Return item with specified name
//-----------------------------------------------------------------------------------------------
- (ProjectItem*)getChildByName:(NSString*)name
{
	for (ProjectItem* item in m_itemChildren)
	{
		if ([item.propName isEqualToString:name])
			return item;
	}
	return nil;
}

// Removes child, child will be retained before removal and autoreleased
//-----------------------------------------------------------------------------------------------
- (ProjectItem*)removeChild:(ProjectItem *)item
{
	if (!item)
		return item;
	
	[item retain];
	
	[m_itemChildren removeObject:item];
	
	[self updateName];
	
	return [item autorelease];
}

// Removes child, child will be retained before removal and autoreleased
//-----------------------------------------------------------------------------------------------
- (ProjectItem*)removeChildAtIndex:(NSInteger)index
{
	if (index <= [m_itemChildren count])
		return nil;
	
	ProjectItem* item = [m_itemChildren objectAtIndex:index];
	
	[item retain];
	
	[m_itemChildren removeObjectAtIndex:index];
	
	[self updateName];
	
	return [item autorelease];
}

// Removes self from parent
//-----------------------------------------------------------------------------------------------
- (void)removeSelf
{
	if (m_itemParent)
	{
		[m_itemParent removeChild:self];
	}
}

//###############################################################################################
// I/O
//###############################################################################################

// Makes XML representation fo current item and attach it to node
//-----------------------------------------------------------------------------------------------
- (NSXMLElement*)saveAsXML:(NSXMLElement*)node
{
	[node addNewChild:@"Name" withValue:m_itemName];
	
	return node;
}

//###############################################################################################
// BUILDING
//###############################################################################################

// Builds project item: every subclass must implement
//-----------------------------------------------------------------------------------------------
- (result)build:(Progressor*)progress
{
	if (![m_itemChildren count])
		return RES_OK;
	
	result res;
	
	progress.propCurrentMinVal = 0;
	progress.propCurrentMaxVal = [m_itemChildren count];
	progress.propCurrentVal	   = 1;
	
	for (ProjectItem* item in m_itemChildren)
	{
		res = [item build:progress];
		if (FAIL(res))
			return res;
		
		progress.propCurrentVal += 1;
		progress.propOverallVal += 1;
	}
	
	return res;
}

//###############################################################################################
// SERVICE METHODS
//###############################################################################################

// Updates generic names: this method used for root items
//-----------------------------------------------------------------------------------------------
- (void)updateName
{
	[m_itemParent updateName];
}

// Returns unique child name
//-----------------------------------------------------------------------------------------------
- (NSString*)getUniqueName
{
	int i = 0;
	
	while (++i)
	{
		NSString* newName = [NSString stringWithFormat:@"%@ %i", m_childName, i];
		
		if (![self getChildByName:newName])
			return newName;
	}
	
	return nil;
}



@end
