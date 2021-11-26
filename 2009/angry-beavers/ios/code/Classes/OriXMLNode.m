//=======================================================================================================================
//  File:		OriXMLNode.m
//  Project:	heli
//  Desc:		Simple XML node class implementation
//
//  Created by Max Vasin on 3/23/11.
//  Copyright 2011 Origin. All rights reserved.
//=======================================================================================================================

// ***** IMPORTS *****

#import "OriXMLNode.h"

//#######################################################################################################################
// ITERNAL NSXMLParser DELEGATE CLASS INTERFACE
//#######################################################################################################################

@interface __ParserDelegate : NSObject <NSXMLParserDelegate>
{
	NSMutableString*	m_currentCharaters;
	OriXMLNode*			m_currentNode;
	OriXMLNode*			m_rootNode;
}

@property (readonly) OriXMLNode* propRootNode;
@property (readonly) OriXMLNode* propCurrentNode;

- (void)parser:(NSXMLParser*)parser didStartElement:(NSString *)elementName namespaceURI:(NSString*)namespaceURI qualifiedName:(NSString*)qName	  attributes:(NSDictionary*)attributeDict;
- (void)parser:(NSXMLParser*)parser didEndElement:(NSString*)elementName namespaceURI:(NSString*)namespaceURI qualifiedName:(NSString*)qName;
- (void)parser:(NSXMLParser*)parser foundCharacters:(NSString *)string;

@end

// ***** IMPLEMENTATION *****

@implementation OriXMLNode

// ***** PROPERTIES *****

@synthesize propName		= m_name;
@synthesize propValue		= m_value;
@synthesize propParent		= m_parent;
@synthesize propChildren	= m_children;

// propValueInt
//-----------------------------------------------------------------------------------------------------------------------
- (void)setPropValueInt:(NSInteger)value
{
	self.propValue = [NSString stringWithFormat:@"%i", value];
}

- (NSInteger)propValueInt
{
	return [m_value intValue];
}

// propValueFloat
//-----------------------------------------------------------------------------------------------------------------------
- (void)setPropValueFloat:(float)value
{
	self.propValue = [NSString stringWithFormat:@"%f", value];
}

- (float)propValueFloat
{
	return [m_value floatValue];
}

// propAttributeCount
//-----------------------------------------------------------------------------------------------------------------------
- (NSInteger)propAttributeCount
{
	return [m_attributes count];
}

// ***** CLASS METHODS *****

//#######################################################################################################################
// CONSTRUCTORS
//#######################################################################################################################

// Returns new autoreleased instance of node with specified name
//-----------------------------------------------------------------------------------------------------------------------
+ (id)nodeWithName:(NSString*)name
{
	return [[[self alloc] initWithName:name] autorelease];
}

// Returns new autoreleased instance of node with specified name and string value
//-----------------------------------------------------------------------------------------------------------------------
+ (id)nodeWithName:(NSString*)name withValue:(NSString*)value
{
	return [[[self alloc] initWithName:name] autorelease];
}

// Returns new autoreleased instance of node with specified name and int value
//-----------------------------------------------------------------------------------------------------------------------
+ (id)nodeWithName:(NSString *)name withValueInt:(NSInteger)value
{
	return [[[self alloc] initWithName:name withValueInt:value] autorelease];
}

// Returns new autoreleased instance of node with specified name and float value
//-----------------------------------------------------------------------------------------------------------------------
+ (id)nodeWithName:(NSString*)name withValueFloat:(float)value
{
	return [[[self alloc] initWithName:name withValueFloat:value] autorelease];
}

// ***** INSTANCE METHODS *****

//#######################################################################################################################
// INITIALIZERS/DEALLOCATORS
//#######################################################################################################################

// Default initializer
//-----------------------------------------------------------------------------------------------------------------------
- (id)init
{
	if ( (self = [super init]) )
	{
		m_children		= [[NSMutableArray alloc]		init];
		m_attributes	= [[NSMutableDictionary alloc]	init];
	}
	return self;
}

// Initializes instance of node with specified name
//-----------------------------------------------------------------------------------------------------------------------
- (id)initWithName:(NSString*)name
{
	if ( (self = [self init]) )
	{
		self.propName = name;
	}
	return self;
}

// Initializes instance of node with specified name and string value
//-----------------------------------------------------------------------------------------------------------------------
- (id)initWithName:(NSString*)name withValue:(NSString*)value
{
	if ( (self = [self initWithName:name]) )
	{
		self.propValue	= value;
	}
	return self;
}

// Initializes instance of node with specified name and int value
//-----------------------------------------------------------------------------------------------------------------------
- (id)initWithName:(NSString*)name withValueInt:(NSInteger)value
{
	if ( (self = [self initWithName:name]) )
	{
		self.propValueInt = value;
	}
	return self;
}

// Initializes instance of node with specified name and float value
//-----------------------------------------------------------------------------------------------------------------------
- (id)initWithName:(NSString *)name withValueFloat:(float)value
{
	if ( (self = [self initWithName:name]) )
	{
		self.propValueFloat = value;
	}
	return self;
}

// Destructor
//-----------------------------------------------------------------------------------------------------------------------
- (void)dealloc
{
	SAFE_RELEASE(m_name);
	SAFE_RELEASE(m_value);
	SAFE_RELEASE(m_children);
	SAFE_RELEASE(m_attributes);
	
	[super dealloc];
}

//#######################################################################################################################
// HIERARCHY CONTROL
//#######################################################################################################################

// Adds existing node to children
//-----------------------------------------------------------------------------------------------------------------------
- (void)addChildNode:(OriXMLNode*)node
{
	if (!node)
		return;
	
	node->m_parent = self;
	
	[m_children addObject:node];
}

// Allocates new node with name and adds it to children
//-----------------------------------------------------------------------------------------------------------------------
- (id)newChildNode:(NSString*)name
{
	OriXMLNode* node = [[OriXMLNode alloc] initWithName:name];
	
	[self addChildNode:node];
	
	[node release];
	
	return node;
}

// Allocates new node with name and string value and adds it to children
//-----------------------------------------------------------------------------------------------------------------------
- (id)newChildNode:(NSString*)name withValue:(NSString*)value
{
	OriXMLNode* node = [[OriXMLNode alloc] initWithName:name withValue:value];
	
	[self addChildNode:node];
	
	[node release];
	
	return node;
}

// Allocates new node with name and integer value and adds it to children
//-----------------------------------------------------------------------------------------------------------------------
- (id)newChildNode:(NSString*)name withValueInt:(NSInteger)value
{
	OriXMLNode* node = [[OriXMLNode alloc] initWithName:name withValueInt:value];
	
	[self addChildNode:node];
	
	[node release];
	
	return node;
}

// Allocates new node with name and float value and adds it to children
//-----------------------------------------------------------------------------------------------------------------------
- (id)newChildNode:(NSString*)name withValueFloat:(float)value
{
	//OriXMLNode* node = [[OriXMLNode nodeWithName:name withValueFloat:value] retain];
	OriXMLNode* node = [[OriXMLNode alloc] initWithName:name withValueFloat:value];
	
	[self addChildNode:node];
	
	[node release];
	
	return node;
}

// Returns first child node founded by name
//-----------------------------------------------------------------------------------------------------------------------
- (id)getFirstChild:(NSString*)name
{
	for (OriXMLNode* node in m_children)
	{
		if ([node.propName isEqualToString:name])
			return node;
	}
	return nil;
}

// Returns first child node value founded by name (placeholder if no such node exist)
//-----------------------------------------------------------------------------------------------------------------------
- (NSString*)getFirstChildValue:(NSString*)name placeholder:(NSString*)value
{
	OriXMLNode* node = [self getFirstChild:name];
	if (node)
		return node.propValue;
	
	return value;
}

// Returns first child node integer founded by name (placeholder if no such node exist)
//-----------------------------------------------------------------------------------------------------------------------
- (NSInteger)getFirstChildValueInt:(NSString*)name placeholder:(NSInteger)value
{
	OriXMLNode* node = [self getFirstChild:name];
	if (node)
		return node.propValueInt;
	
	return value;
}

// Returns first child node float value founded by name (placeholder if no such node exist)
//-----------------------------------------------------------------------------------------------------------------------
- (float)getFirstChildValueFloat:(NSString*)name placeholder:(float)value
{
	OriXMLNode* node = [self getFirstChild:name];
	if (node)
		return node.propValueFloat;
	
	return value;
}

//#######################################################################################################################
// ATTRIBUTES CONTROL
//#######################################################################################################################

// Set node attribute with specified name and string value
//-----------------------------------------------------------------------------------------------------------------------
- (void)setAttribute:(NSString*)name withValue:(NSString*)value
{
	if (!name || !value)
		return;
	
	[m_attributes setValue:value forKey:name];
}

// Set node attribute with specified name and integer value
//-----------------------------------------------------------------------------------------------------------------------
- (void)setAttribute:(NSString*)name withValueInt:(NSInteger *)value
{
	[self setAttribute:name withValue:[NSString stringWithFormat:@"%i",value]];
}

// Set node attribute with specified name and float value
//-----------------------------------------------------------------------------------------------------------------------
- (void)setAttribute:(NSString*)name withValueFloat:(float)value
{
	[self setAttribute:name withValue:[NSString stringWithFormat:@"%f",value]];
}

// Returns node attribute string value by attribute name (placeholder value otherwise)
//-----------------------------------------------------------------------------------------------------------------------
- (NSString*)getAttributeValue:(NSString*)name placeholder:(NSString*)value
{
	NSString* attrValue = [m_attributes valueForKey:name];
	if (attrValue)
		return attrValue;
	
	return value;
}

// Returns node attribute integer value by attribute name (placeholder value otherwise)
//-----------------------------------------------------------------------------------------------------------------------
- (NSInteger)getAttributeValueInt:(NSString*)name placeholder:(NSInteger)value
{
	NSString* attrValue = [self getAttributeValue:name placeholder:nil];
	if (attrValue)
		return [attrValue intValue];
	
	return value;
}

// Returns node attribute float value by attribute name (placeholder value otherwise)
//-----------------------------------------------------------------------------------------------------------------------
- (float)getAttributeValueFloat:(NSString*)name placeholder:(float)value
{
	NSString* attrValue = [self getAttributeValue:name placeholder:nil];
	if (attrValue)
		return [attrValue intValue];
	
	return value;
}

//#######################################################################################################################
// XML PARSER
//#######################################################################################################################

// Returns root node of parsed XML document
//-----------------------------------------------------------------------------------------------------------------------
+ (id)parseFile:(NSString*)file
{
	NSData* fileData = [[NSData alloc] initWithContentsOfFile:file];
	
	NSXMLParser* parser	= [[NSXMLParser alloc] initWithData:fileData];
	__ParserDelegate* delegate = [[__ParserDelegate alloc] init];
	
	parser.delegate = delegate;
	
	[parser parse];
	
	OriXMLNode* node		= [delegate.propRootNode retain];
	OriXMLNode* nodeLast	= delegate.propCurrentNode;
	NSError*	error		= [parser parserError];
	
	SAFE_RELEASE(delegate);
	SAFE_RELEASE(parser);
	SAFE_RELEASE(fileData);
	
	if (nodeLast)
	{
		DBGLOG1A(@"OriXMLNode: failed to create nodes tree - last element not nil (broken XML scheme?), file (%@)", file);
		DBGLOG1A(@"OriXMLNode: parser error (%@)", [error description]);
		[node release];
		return nil;
	}
		
	// return [node autorelease];
	return node;
}

@end

//#######################################################################################################################
// ITERNAL NSXMLParser DELEGATE IMPLEMENTATION
//#######################################################################################################################

@implementation __ParserDelegate

@synthesize propRootNode	= m_rootNode;
@synthesize propCurrentNode = m_currentNode;


// Delegate initializer
//-----------------------------------------------------------------------------------------------------------------------
- (id)init
{
	if ( (self = [super init]) )
	{
		m_currentCharaters = [[NSMutableString alloc] initWithCapacity:50];
	}
	return self;
}

// Delegate destructor
//-----------------------------------------------------------------------------------------------------------------------
- (void)dealloc
{
	SAFE_RELEASE(m_currentCharaters);
	SAFE_RELEASE(m_rootNode);
	
	[super dealloc];
}

// Entering new element. <ELEMENT>
//-----------------------------------------------------------------------------------------------------------------------
- (void)parser:(NSXMLParser*)parser didStartElement:(NSString*)elementName namespaceURI:(NSString*)namespaceURI qualifiedName:(NSString*)qName attributes:(NSDictionary*)attributeDict
{
	if (!m_rootNode)
	{
		m_rootNode		= [[OriXMLNode alloc] initWithName:elementName];
		m_currentNode	= m_rootNode;
	}
	else
	{
		m_currentNode	= [m_currentNode newChildNode:elementName];
	}
}

// Quiting element. </ELEMENT>
//-----------------------------------------------------------------------------------------------------------------------
- (void)parser:(NSXMLParser*)parser didEndElement:(NSString*)elementName namespaceURI:(NSString *)namespaceURI qualifiedName:(NSString *)qName
{
	if (m_currentCharaters.length)
		m_currentNode.propValue = m_currentCharaters;
	
	[m_currentCharaters setString:@""];
	m_currentNode = m_currentNode.propParent;
}

// Element value. <ELEMENT>####</ELEMENT>
//-----------------------------------------------------------------------------------------------------------------------
- (void)parser:(NSXMLParser*)parser foundCharacters:(NSString*)string
{
	[m_currentCharaters appendString:[string stringByTrimmingCharactersInSet:[NSCharacterSet whitespaceAndNewlineCharacterSet]]];
}

@end






