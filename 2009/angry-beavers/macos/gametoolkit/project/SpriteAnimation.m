//=========================================================================================================================
//  File:		SpriteAnimation.m
//  Project:	OriginGameToolkit
//	Desc:		Sprite animation containers implementation
//
//  Created by Max Vasin on 3/4/11.
//  Copyright 2011 Origin Games. All rights reserved.
//=========================================================================================================================

#import "SpriteAnimation.h"
#import "SharedResourceManager.h"
#import "ProjectSpriteItem.h"
#import "ProjectTextureItem.h"

//#########################################################################################################################
// AnimatoinLink IMPLEMENTATION
//#########################################################################################################################

@implementation AnimationLink

//=========================================================================================================================
// PROPERTIES
//=========================================================================================================================

// Synthesized
//-------------------------------------------------------------------------------------------------------------------------
@synthesize propAssembly		= m_textureAssemly;
@synthesize propFrameOffset		= m_frameOffset;
@synthesize propMirrorX			= m_mirrorX;
@synthesize propMirrorY			= m_mirrorY;
@synthesize propMirrorStart		= m_mirrorStart;
@synthesize propMirrorEnd		= m_mirrorEnd;
@synthesize propMirrorSequenceX	= m_mirrorSequenceX;
@synthesize propMirrorSequenceY	= m_mirrorSequenceY;

// Generic sprite
//-------------------------------------------------------------------------------------------------------------------------
- (void)setPropSprite:(ProjectSpriteItem*)sprite
{
	// dummy
}

- (ProjectSpriteItem*)propSprite
{
	return m_animation.propSprite;
}

// Generic animation
//-------------------------------------------------------------------------------------------------------------------------
- (void)setPropAnimation:(SpriteAnimation*)anim
{
	// dummy
}

- (SpriteAnimation*)propAnimation
{
	return m_animation;
}

// Generic assembly items
//-------------------------------------------------------------------------------------------------------------------------
- (void)setPropAssemblyItems:(NSMutableArray*)array
{
	// dummy
}

- (NSMutableArray*)propAssemblyItems
{
	NSMutableArray* list = self.propSprite.propTexture.propAssembly;
	
	return list;
}


//=========================================================================================================================
// INSTANCE METHODS
//=========================================================================================================================

// New link initializer
//-------------------------------------------------------------------------------------------------------------------------
- (id)initWithAnimation:(SpriteAnimation*)animation
{
	if ( (self = [super init]) )
	{
		m_animation = animation;
	}
	return self;
}

// Initializer from XML data
//-------------------------------------------------------------------------------------------------------------------------
- (id)initWithAnimation:(SpriteAnimation*)animation XML:(NSXMLElement*)node
{
	if ( (self = [self initWithAnimation:animation]) )
	{
		m_textureAssemly	= [self.propSprite.propTexture getAssemblyItem:[node getFirstChildValue:@"Assembly"]];
		m_frameOffset		= [node getFirstChildInteger:@"Offset"];
		m_mirrorX			= [node getFirstChildInteger:@"MirrorX"];
		m_mirrorY			= [node getFirstChildInteger:@"MirrorY"];
		m_mirrorSequenceX	= [node getFirstChildInteger:@"MirrorSequenceX"];
		m_mirrorSequenceY	= [node getFirstChildInteger:@"MirrorSequenceY"];
		m_mirrorStart		= [node getFirstChildInteger:@"MirrorStart"];
		m_mirrorEnd			= [node getFirstChildInteger:@"MirrorEnd"];
	}
	return self;
}

// Saves XML data
//-------------------------------------------------------------------------------------------------------------------------
- (NSXMLElement*)saveXML:(NSXMLElement*)node
{
	NSXMLElement* thisNode = [node addNewChild:@"Link"];
	
	[thisNode addNewChild:@"Assembly"			withValue:m_textureAssemly.propName];
	[thisNode addNewChild:@"Offset"				withInteger:m_frameOffset];
	[thisNode addNewChild:@"MirrorX"			withInteger:m_mirrorX];
	[thisNode addNewChild:@"MirrorY"			withInteger:m_mirrorY];
	[thisNode addNewChild:@"MirrorSequenceX"	withInteger:m_mirrorSequenceX];
	[thisNode addNewChild:@"MirrorSequenceY"	withInteger:m_mirrorSequenceY];
	[thisNode addNewChild:@"MirrorStart"		withInteger:m_mirrorStart];
	[thisNode addNewChild:@"MirrorEnd"			withInteger:m_mirrorEnd];
	
	return thisNode;
}

// Clears current link
//-------------------------------------------------------------------------------------------------------------------------
- (void)delLink
{
	self.propAssembly = nil;
}

@end


//#########################################################################################################################
// FramePoint IMPLEMENTATION
//#########################################################################################################################

@implementation FramePoint

//=========================================================================================================================
// PROPERTIES
//=========================================================================================================================

// Framepoint name
//-------------------------------------------------------------------------------------------------------------------------
- (void)setPropName:(NSString*)name
{
	if (m_name != name)
	{
		if (m_animation)
			for (AnimationFrame* frame in m_animation.propFrames)
				[frame renamePoint:m_name to:name];
		
		SAFE_RELEASE(m_name);
		m_name = [name copy];
	}
}

- (NSString*)propName
{
	return m_name;
}

- (BOOL)validatePropName:(id*)ioValue error:(NSError**)outError
{
	if (!m_animation)
		return YES;
	
	FramePoint* founded = [m_animation.propFramePoints getObjectByProp:@"propName" Value:*ioValue];
	
	if (!founded || (founded == self))
		return YES;
	
	if (outError)
	{
		NSString *errorString = NSLocalizedStringFromTable(@"Duplicate point name", @"Name",
														   @"validation: duplicate name error");
		NSDictionary *userInfoDict = [NSDictionary dictionaryWithObject:errorString forKey:NSLocalizedDescriptionKey];
		
		*outError = [[[NSError alloc] initWithDomain:@"Point domain" code:1 userInfo:userInfoDict] autorelease];
	}
	return NO;
}

// Frame point color
//-------------------------------------------------------------------------------------------------------------------------
- (void)setPropColor:(NSColor*)color
{
	if (m_color != color)
	{
		if (m_animation)
			for (AnimationFrame* frame in m_animation.propFrames)
				[frame changeColor:m_name Color:color];
		
		SAFE_RELEASE(m_color);
		m_color = [color copy];
	}
}

- (NSColor*)propColor
{
	return m_color;
}


// X frame point coordinate
//-------------------------------------------------------------------------------------------------------------------------
- (void)setPropPositionX:(int)x
{
	m_pos.x = x;
}

- (int)propPositionX
{
	return m_pos.x;
}

// Y frame point coordinate
//-------------------------------------------------------------------------------------------------------------------------
- (void)setPropPositionY:(int)y
{
	m_pos.y = y;
}

- (int)propPositionY
{
	return m_pos.y;
}

// X packed frame point coordinate
//-------------------------------------------------------------------------------------------------------------------------
- (void)setPropPackOffsetX:(int)x
{
	m_packOffset.x = x;
}

- (int)propPackOffsetX
{
	return m_packOffset.x;
}

// Y packed frame point coordinate
//-------------------------------------------------------------------------------------------------------------------------
- (void)setPropPackOffsetY:(int)y
{
	m_packOffset.y = y;
}

- (int)propPackOffsetY
{
	return m_packOffset.y;
}


//=========================================================================================================================
// INSTANCE METHODS
//=========================================================================================================================

// Initializes new instance of frame point class with specified name
//-------------------------------------------------------------------------------------------------------------------------
- (id)initWithAnimation:(SpriteAnimation*)animation Name:(NSString*)name
{
	if ( (self = [self init]) )
	{
		m_animation		= animation;
		self.propColor	= [NSColor colorWithCalibratedRed:1.0f green:0.0f blue:0.0f alpha:1.0f];
		self.propName	= name;
	}
	
	return self;
}

// Destructor
//-------------------------------------------------------------------------------------------------------------------------
- (void)dealloc
{
	SAFE_RELEASE(m_color);
	SAFE_RELEASE(m_name);
	
	[super dealloc];
}

// Initializes new instance of frame point with XML data
//-------------------------------------------------------------------------------------------------------------------------
- (id)initWithAnimation:(SpriteAnimation*)animation XML:(NSXMLElement*)node
{
	if ( (self = [self init]) )
	{
		m_animation		= animation;
		self.propName	= [node getFirstChildValue:@"Name"];
		m_pos.x			= [node getFirstChildInteger:@"X"];
		m_pos.y			= [node getFirstChildInteger:@"Y"];
		self.propColor	= [node getFirstChildObject:@"Color"];
	}
	return self;
}

// Saves frame to XML schema
//-------------------------------------------------------------------------------------------------------------------------
- (NSXMLElement*)saveXML:(NSXMLElement*)node
{
	NSXMLElement* thisNode = [node addNewChild:@"Point"];
	
	[thisNode addNewChild:@"Name"   withValue:m_name];
	[thisNode addNewChild:@"X"		withInteger:m_pos.x];
	[thisNode addNewChild:@"Y"		withInteger:m_pos.y];
	[thisNode addNewChild:@"Color"	withObject:m_color];
	
	return thisNode;
}

// Saves frame to XML schema for engine (without editor stuff)
//-------------------------------------------------------------------------------------------------------------------------
- (NSXMLElement*)buildXML:(NSXMLElement*)node
{
	NSXMLElement* thisNode = [node addNewChild:@"Point"];
	
	[thisNode addNewChild:@"Name"   withValue:m_name];
	[thisNode addNewChild:@"X"		withInteger:m_pos.x - m_packOffset.x];
	[thisNode addNewChild:@"Y"		withInteger:m_pos.y - m_packOffset.y];
	
	return thisNode;
}

@end

//#########################################################################################################################
// AnimationFrame IMPLEMENTATION
//#########################################################################################################################

@implementation AnimationFrame

//=========================================================================================================================
// PROPERTIES
//=========================================================================================================================

// Synthesized
//-------------------------------------------------------------------------------------------------------------------------
@synthesize propCurrentPoint	= m_currentPoint;
@synthesize propMirrorX			= m_mirrorX;
@synthesize propMirrorY			= m_mirrorY;
@synthesize propIdentifier		= m_identifier;

// Origin point
//-------------------------------------------------------------------------------------------------------------------------
- (FramePoint*)propOriginPoint
{
	return [m_points getObjectByProp:@"propName" Value:m_animation.propOriginPoint.propName];
}

// Frame points
//-------------------------------------------------------------------------------------------------------------------------
- (void)setPropPoints:(NSMutableArray*)propPoints
{
	// dummy
}

- (NSMutableArray*)propPoints
{
	return m_points;
}

// X frame position coordinate
//-------------------------------------------------------------------------------------------------------------------------
- (void)setPropPositionX : (NSInteger)x
{
	m_position.x = x;
}

- (NSInteger)propPositionX
{
	return m_position.x;
}

// Y frame position coordinate
//-------------------------------------------------------------------------------------------------------------------------
- (void)setPropPositionY : (NSInteger)y
{
	m_position.y = y;
}

- (NSInteger)propPositionY
{
	return m_position.y;
}

// Frame width
//-------------------------------------------------------------------------------------------------------------------------
- (void)setPropWidth:(NSInteger)width
{
	m_size.width = width;
}

- (NSInteger)propWidth
{
	return m_size.width;
}

// Frame height
//-------------------------------------------------------------------------------------------------------------------------
- (void)setPropHeight:(NSInteger)height
{
	m_size.height = height;
}

- (NSInteger)propHeight
{
	return m_size.height;
}

// Packed texture position X
//-------------------------------------------------------------------------------------------------------------------------
- (void)setPropPackPositionX:(NSInteger)x
{
	m_packPosition.x = x;
}

- (NSInteger)propPackPositionX
{
	return m_packPosition.x;
}

// Packed texture position Y
//-------------------------------------------------------------------------------------------------------------------------
- (void)setPropPackPositionY:(NSInteger)y
{
	m_packPosition.y = y;
}

- (NSInteger)propPackPositionY
{
	return m_packPosition.y;
}

// Packed width
//-------------------------------------------------------------------------------------------------------------------------
- (void)setPropPackWidth:(NSInteger)w
{
	m_packSize.width = w;
}

- (NSInteger)propPackWidth
{
	return m_packSize.width;
}

// Packed height
//-------------------------------------------------------------------------------------------------------------------------
- (void)setPropPackHeight:(NSInteger)h
{
	m_packSize.height = h;
}

- (NSInteger)propPackHeight
{
	return m_packSize.height;
}

// Packed offset X
//-------------------------------------------------------------------------------------------------------------------------
- (void)setPropPackOffsetX:(NSInteger)offs
{
	m_packOffset.x = offs;
}

- (NSInteger)propPackOffsetX
{
	return m_packOffset.x;
}

// Packed offset Y
//-------------------------------------------------------------------------------------------------------------------------
- (void)setPropPackOffsetY:(NSInteger)offs
{
	m_packOffset.y = offs;
}

- (NSInteger)propPackOffsetY
{
	return m_packOffset.y;
}

//=========================================================================================================================
// INSTANCE METHODS
//=========================================================================================================================

// Initializes new frame instance with parent animation object
//-------------------------------------------------------------------------------------------------------------------------
- (id)initWithAnimation:(SpriteAnimation*)anim
{
	if ( (self = [super init]) )
	{
		m_animation	= anim;
		m_points	= [[NSMutableArray alloc] init];
	
		if (m_animation.propFramesCount)
		{
			AnimationFrame* firstFrame = [m_animation.propFrames objectAtIndex:0];
			m_size.width  = firstFrame.propWidth;
			m_size.height = firstFrame.propHeight;
		}
		else 
		{
			m_size.width  = 32;
			m_size.height = 32;
		}

	}
	return self;
}

// Destructor
//-------------------------------------------------------------------------------------------------------------------------
- (void)dealloc
{
	SAFE_RELEASE(m_points);
	
	[super dealloc];
}

// Initializes new frame instance with parent animation object and XML data
//-------------------------------------------------------------------------------------------------------------------------
- (id)initWithAnimation:(SpriteAnimation*)anim fromXML:(NSXMLElement*)node
{
	if ( (self = [self initWithAnimation:anim]) )
	{
		m_position.x		= [node getFirstChildInteger:@"PosX"];
		m_position.y		= [node getFirstChildInteger:@"PosY"];
		m_size.width		= [node getFirstChildInteger:@"Width"];
		m_size.height		= [node getFirstChildInteger:@"Height"];
		m_mirrorX			= [node getFirstChildInteger:@"MirrorX"];
		m_mirrorY			= [node getFirstChildInteger:@"MirrorY"];
		m_packSize.width	= [node getFirstChildInteger:@"PackWidth"];
		m_packSize.height	= [node getFirstChildInteger:@"PackHeight"];
		m_packPosition.x	= [node getFirstChildInteger:@"PackPosX"];
		m_packPosition.y	= [node getFirstChildInteger:@"PackPosY"];
		m_packOffset.x		= [node getFirstChildInteger:@"PackOffsetX"];
		m_packOffset.y		= [node getFirstChildInteger:@"PackOffsetY"];
		self.propIdentifier = [node getFirstChildValue:@"Identifier"];
		
		// Cleaning on load
		// Load points created by animation only
		
		NSXMLElement* pointsNode = [node getFirstChild:@"Points"];
				
		for (NSXMLElement* itemNode in [pointsNode children])
		{
			NSString* name = [itemNode getFirstChildValue:@"Name"];
			
			if ([m_animation.propFramePoints getObjectByProp:@"propName" Value:name])
			{
				FramePoint* point = [[[FramePoint alloc] initWithAnimation:nil XML:itemNode] autorelease];
				
				[m_points addObject:point];
				
				if (!m_currentPoint)
					self.propCurrentPoint = point;
			}
		}
		
		// Create points
		for (FramePoint* point in m_animation.propFramePoints)
		{
			if (![m_points getObjectByProp:@"propName" Value:point.propName])
				[self addPoint:point.propName Color:point.propColor];
		}
		
	}
	return self;
}

// Adds new frame point
//-------------------------------------------------------------------------------------------------------------------------
- (void)addPoint:(NSString*)name Color:(NSColor*)color
{
	FramePoint* point = [[[FramePoint alloc] initWithAnimation:nil Name:name] autorelease];
	
	point.propColor = color;
	
	[m_points addObject:point];
	
	if (!self.propCurrentPoint)
		self.propCurrentPoint = point;
	
	self.propPoints = nil; // setter is dummy, just for KVO
}

// Deletes frame point
//-------------------------------------------------------------------------------------------------------------------------
- (void)delPoint:(NSString*)name
{
	FramePoint* point = (FramePoint*)[m_points getObjectByProp:@"propName" Value:name];
	
	if (point)
	{
		unsigned int ind = [m_points indexOfObject:m_currentPoint];

		self.propCurrentPoint = nil;
		
		
		if (point)
			[m_points removeObject:point];
		
		if (ind >= [m_points count])
			ind = [m_points count] - 1;
		
		if ([m_points count])
			self.propCurrentPoint = [m_points objectAtIndex:ind];
		
		self.propPoints		  = nil;  // to refresh lists, its dummy setter
	}
}

// Returns frame point by name
//-------------------------------------------------------------------------------------------------------------------------
- (FramePoint*)getPoint:(NSString*)name
{
	return [m_points getObjectByProp:@"propName" Value:name];
}

// Renames point
//-------------------------------------------------------------------------------------------------------------------------
- (void)renamePoint:(NSString*)nameOld to:(NSString*)name
{
	FramePoint* point = [m_points getObjectByProp:@"propName" Value:nameOld];
	
	if (point)
		point.propName = name;
}

// Changes point color
//-------------------------------------------------------------------------------------------------------------------------
- (void)changeColor:(NSString*)name Color:(NSColor*)color
{
	FramePoint* point = [m_points getObjectByProp:@"propName" Value:name];
	
	if (point)
		point.propColor = color;
}


// Saves frame to XML schema
//-------------------------------------------------------------------------------------------------------------------------
- (NSXMLElement*)saveXML:(NSXMLElement*)node
{
	NSXMLElement* thisNode = [node addNewChild:@"Frame"];
	
	[thisNode addNewChild:@"PosX"			withInteger:m_position.x];
	[thisNode addNewChild:@"PosY"			withInteger:m_position.y];
	[thisNode addNewChild:@"Width"			withInteger:m_size.width];
	[thisNode addNewChild:@"Height"			withInteger:m_size.height];
	[thisNode addNewChild:@"MirrorX"		withInteger:m_mirrorX];
	[thisNode addNewChild:@"MirrorY"		withInteger:m_mirrorY];
	[thisNode addNewChild:@"PackWidth"		withInteger:m_packSize.width];
	[thisNode addNewChild:@"PackHeight"		withInteger:m_packSize.height];
	[thisNode addNewChild:@"PackPosX"		withInteger:m_packPosition.x];
	[thisNode addNewChild:@"PackPosY"		withInteger:m_packPosition.y];
	[thisNode addNewChild:@"PackOffsetX"	withInteger:m_packOffset.x];
	[thisNode addNewChild:@"PackOffsetY"	withInteger:m_packOffset.y];
	[thisNode addNewChild:@"Identifier"		withValue:m_identifier];
	
	NSXMLElement* pointsNode = [thisNode addNewChild:@"Points"];
	
	for (FramePoint* point in m_points)
		[point saveXML:pointsNode];
	
	return thisNode;
}

// Build frame object for engine (without editor stuff)
//-------------------------------------------------------------------------------------------------------------------------
- (NSXMLElement*)buildXML:(NSXMLElement *)node
{
	NSXMLElement* thisNode = [node addNewChild:@"Frame"];
	
	if (m_animation.propSprite.propTexture.propPacked)
	{	
		[thisNode addNewChild:@"PosX"   withInteger:m_packPosition.x];
		[thisNode addNewChild:@"PosY"   withInteger:m_packPosition.y];
		[thisNode addNewChild:@"Width"  withInteger:m_packSize.width];
		[thisNode addNewChild:@"Height" withInteger:m_packSize.height];
	}
	else
	{
		[thisNode addNewChild:@"PosX"   withInteger:m_position.x];
		[thisNode addNewChild:@"PosY"   withInteger:m_position.y];
		[thisNode addNewChild:@"Width"  withInteger:m_size.width];
		[thisNode addNewChild:@"Height" withInteger:m_size.height];
	}
	[thisNode addNewChild:@"MirX"	withInteger:m_mirrorX];
	[thisNode addNewChild:@"MirY"	withInteger:m_mirrorY];
	[thisNode addNewChild:@"Id"		withValue:m_identifier];
	
	NSXMLElement* pointsNode = [thisNode addNewChild:@"Points"];
	
	for (FramePoint* point in m_points)
	{
		point.propPackOffsetX = m_mirrorX ? (m_size.width - m_packSize.width - m_packOffset.x) : m_packOffset.x;
		point.propPackOffsetY = m_packOffset.y;
		
		[point buildXML:pointsNode];
	}

	return thisNode;
}

@end

//#########################################################################################################################
// SpriteAnimation IMPLEMENTATION
//#########################################################################################################################

@implementation SpriteAnimation

//=========================================================================================================================
// PROPERTIES
//=========================================================================================================================

// Synthesized
//-------------------------------------------------------------------------------------------------------------------------
@synthesize propName			= m_name;
@synthesize propFrames			= m_frames;
@synthesize propSpeed			= m_speed;
@synthesize propGridSpaceH		= m_gridSpaceH;
@synthesize propGridSpaceV		= m_gridSpaceV;
@synthesize propFramePoints		= m_framePoints;
@synthesize propCurrentPoint	= m_currentFramePoint;
@synthesize propOriginPoint		= m_originFramePoint;
@synthesize propSprite			= m_sprite;
@synthesize propLink			= m_link;

// Animation guid grid origin X coordinate (editor only)
//-------------------------------------------------------------------------------------------------------------------------
- (void)setPropGridOriginX:(NSInteger)x
{
	m_gridOrigin.x = x;
}

- (NSInteger)propGridOriginX
{
	return m_gridOrigin.x;
}

// Animation guid grid origin Y coordinate (editor only)
//-------------------------------------------------------------------------------------------------------------------------
- (void)setPropGridOriginY:(NSInteger)y
{
	m_gridOrigin.y = y;
}

- (NSInteger)propGridOriginY
{
	return m_gridOrigin.y;
}


// Current animaton frame (editor only)
//-------------------------------------------------------------------------------------------------------------------------
- (void)setPropCurrentFrame:(AnimationFrame*)frame
{
	m_currentFrame = frame;
	self.propFrameStatistic = @"";
}

- (AnimationFrame*)propCurrentFrame
{
	return m_currentFrame;
}

// Current animaton frame index (editor only)
//-------------------------------------------------------------------------------------------------------------------------
- (void)setPropCurrentFrameIndex:(NSInteger)index
{
	self.propCurrentFrame = [m_frames objectAtIndex:index];
}

- (NSInteger)propCurrentFrameIndex
{
	return [m_frames indexOfObject:m_currentFrame];
}

// Animation frames count (readonly)
//-------------------------------------------------------------------------------------------------------------------------
- (void)setPropFramesCount:(NSInteger)count
{
	// dummy
}

- (NSInteger)propFramesCount
{
	return [m_frames count];
}


// String representation of frame statistic (editor only)
//-------------------------------------------------------------------------------------------------------------------------
- (void)setPropFrameStatistic:(NSString*)stat
{
	// don't care
}

- (NSString*)propFrameStatistic
{
	if (![m_frames count])
		return @"empty";
	
	return [NSString stringWithFormat:@"frame %i of %i", [m_frames indexOfObject:self.propCurrentFrame] + 1, [m_frames count]];
}


//=========================================================================================================================
// INSTANCE METHODS
//=========================================================================================================================

// Initializes new instance of class with parent sprite
//-------------------------------------------------------------------------------------------------------------------------
- (id)initWithSprite:(ProjectSpriteItem*)sprite
{
	if ( (self = [self init]) )
	{
		m_sprite		= sprite;
		self.propName	= @"unnamed animation";
		m_frames		= [[NSMutableArray alloc] init];
		m_framePoints	= [[NSMutableArray alloc] init];
		m_link			= [[AnimationLink alloc] initWithAnimation:self];
		m_gridSpaceH	= 25;
		m_gridSpaceV	= 25;
	}
	return self;
}

// Initializes new instance of class with parent sprite and XML data
//-------------------------------------------------------------------------------------------------------------------------
- (id)initWithSprite:(ProjectSpriteItem*)sprite fromXML:(NSXMLElement*)node
{
	if ( (self = [self initWithSprite:sprite]) )
	{
		self.propName	= [node getFirstChildValue:@"Name"];
		m_gridSpaceH	= [node getFirstChildInteger:@"GridSpaceH"		placeholder:25];
		m_gridSpaceV	= [node getFirstChildInteger:@"GridSpaceV"		placeholder:25];
		m_gridOrigin.x	= [node getFirstChildInteger:@"GridOriginX"		placeholder:0];
		m_gridOrigin.y	= [node getFirstChildInteger:@"GridOriginY"		placeholder:0];
		m_speed			= [node getFirstChildInteger:@"AnimationSpeed"	placeholder:25];
		
		NSXMLElement* linksNode   = [node getFirstChild:@"Links"];
		
		NSXMLElement* linkNode    = [linksNode getFirstChild:@"Link"];
		
		m_link	= [[AnimationLink alloc] initWithAnimation:self XML:linkNode];
				
		NSXMLElement* pointsNode = [node getFirstChild:@"Points"];
		
		self.propCurrentPoint = nil;
		
		for (NSXMLElement* itemNode in [pointsNode children])
		{
			FramePoint* point = [[[FramePoint alloc] initWithAnimation:self XML:itemNode] autorelease];
			
			[m_framePoints addObject:point];
			
			if (!self.propCurrentPoint)
				self.propCurrentPoint = point;
		}
		
		self.propOriginPoint = [m_framePoints getObjectByProp:@"propName" Value:[node getFirstChildValue:@"OriginPoint"]];
		
		NSXMLElement* framesNode = [node getFirstChild:@"Frames"];
		
		self.propCurrentFrame = nil;
		
		for (NSXMLElement* itemNode in [framesNode children])
		{
			AnimationFrame* item = [[[AnimationFrame alloc] initWithAnimation:self fromXML:itemNode] autorelease];
			
			[m_frames addObject:item];
			
			if (!self.propCurrentFrame)
				self.propCurrentFrame = item;
		}
	}
	return self;
}

// Destructor
//-------------------------------------------------------------------------------------------------------------------------
- (void)dealloc
{
	SAFE_RELEASE(m_frames);
	SAFE_RELEASE(m_framePoints);
	SAFE_RELEASE(m_name);
	SAFE_RELEASE(m_link);
	
	[super dealloc];
}

// Saves object to XML schema
//-------------------------------------------------------------------------------------------------------------------------
- (NSXMLElement*)saveXML:(NSXMLElement*)node
{
	NSXMLElement* thisNode = [node addNewChild:@"Animation"];
	
	[thisNode addNewChild:@"Name"			withValue:m_name];
	[thisNode addNewChild:@"GridSpaceH"		withInteger:m_gridSpaceH];
	[thisNode addNewChild:@"GridSpaceV"		withInteger:m_gridSpaceV];
	[thisNode addNewChild:@"GridOriginX"	withInteger:m_gridOrigin.x];
	[thisNode addNewChild:@"GridOriginY"	withInteger:m_gridOrigin.y];
	[thisNode addNewChild:@"AnimationSpeed"	withInteger:m_speed];
	[thisNode addNewChild:@"OriginPoint"	withValue:m_originFramePoint.propName];
	
	NSXMLElement* linksNode  = [thisNode addNewChild:@"Links"];
	
	[m_link saveXML:linksNode];
	
	NSXMLElement* pointsNode = [thisNode addNewChild:@"Points"];
	
	for (FramePoint* point in m_framePoints)
	{
		[point saveXML:pointsNode];
	}
		
	NSXMLElement* framesNode = [thisNode addNewChild:@"Frames"];
	
	for (AnimationFrame* frame in m_frames)
	{
		[frame saveXML:framesNode];
	}
	
	return thisNode;
}

// Saves object to XML for engine (without any editor stuff)
//-------------------------------------------------------------------------------------------------------------------------
- (NSXMLElement*)buildXML:(NSXMLElement*)node
{
	NSXMLElement* thisNode = [node addNewChild:@"Animation"];
	
	[thisNode addNewChild:@"Name"			withValue:m_name];
	[thisNode addNewChild:@"AnimationSpeed"	withInteger:m_speed];
	[thisNode addNewChild:@"Origin"			withValue:m_originFramePoint.propName];
	[thisNode addNewChild:@"FramesCount"	withInteger:[m_frames count]];
	
	NSXMLElement* framesNode = [thisNode addNewChild:@"Frames"];
	
	for (AnimationFrame* frame in m_frames)
	{
		[frame buildXML:framesNode];
	}
	
	return thisNode;
}

// Adds new frame to animation (button handler)
//-------------------------------------------------------------------------------------------------------------------------
- (IBAction)addFrame:(id)sender
{
	AnimationFrame* newFrame = [[[AnimationFrame alloc] initWithAnimation:self] autorelease];
	
	// add points
	for (FramePoint* point in m_framePoints)
		[newFrame addPoint:point.propName Color:point.propColor];
	
	
	[m_frames addObject:newFrame];
	
	self.propCurrentFrame	= newFrame;
	self.propFramesCount	= [m_frames count];
}

// Deletes current frame (button handler)
//-------------------------------------------------------------------------------------------------------------------------
- (IBAction)delFrame:(id)sender
{
	if (self.propCurrentFrame)
	{
		unsigned int index = [m_frames indexOfObject:m_currentFrame];
		
		[m_frames removeObject:m_currentFrame];
		
		if ([m_frames count])
		{
			if (index >= [m_frames count])
				index--;
			
			self.propCurrentFrame = [m_frames objectAtIndex:index];
		}
		else
		{
			self.propCurrentFrame = nil;
		}
		self.propFramesCount	= [m_frames count];
	}
}


// Selects next animation frame as current frame
//-------------------------------------------------------------------------------------------------------------------------
- (IBAction)selectNextFrame:(id)sender
{
	NSInteger curCount = [m_frames count];
	if (!curCount)
		return;
	
	NSInteger curIndex = [m_frames indexOfObject:self.propCurrentFrame];
	
	if (curIndex >= (curCount - 1))
		return;
	
	self.propCurrentFrameIndex = curIndex + 1;
}

// Selects previous animation frame as current frame
//-------------------------------------------------------------------------------------------------------------------------
- (IBAction)selectPrevFrame:(id)sender
{
	NSInteger curCount = [m_frames count];
	if (!curCount)
		return;
	
	NSInteger curIndex = [m_frames indexOfObject:self.propCurrentFrame];
	
	if (curIndex <= 0)
		return;
	
	self.propCurrentFrameIndex = curIndex - 1;
}

// Adds new animation frame point
//-------------------------------------------------------------------------------------------------------------------------
- (IBAction)addFramePoint:(id)sender
{
	int i = 0;
	NSString* newName;
	
	while (++i)
	{
		newName = [NSString stringWithFormat:@"point %i", i];
		if (![m_framePoints getObjectByProp:@"propName" Value:newName])
			break;
	}
	
	FramePoint* newPoint = [[[FramePoint alloc] initWithAnimation:self Name:newName] autorelease];
	
	[m_framePoints addObject:newPoint];
	
	self.propCurrentPoint = newPoint;
	
	if (!self.propOriginPoint)
		self.propOriginPoint = newPoint;
	
	for (AnimationFrame* frame in m_frames)
		[frame addPoint:newName Color:newPoint.propColor];
}

// Deletes current animation frame point
//-------------------------------------------------------------------------------------------------------------------------
- (IBAction)delFramePoint:(id)sender
{
	if (m_currentFramePoint)
	{
		int index = [m_framePoints indexOfObject:m_currentFramePoint];
		
		if (self.propOriginPoint == m_currentFramePoint)
			self.propOriginPoint = nil;
		
		for (AnimationFrame* frame in m_frames)
			[frame delPoint:m_currentFramePoint.propName];
				
		[m_framePoints removeObject:m_currentFramePoint];
		
		if ([m_framePoints count])
		{
			if (index >= [m_framePoints count])
				index = [m_framePoints count] - 1;
			
			self.propCurrentPoint = [m_framePoints objectAtIndex:index];
			
			if (!self.propOriginPoint)
				self.propOriginPoint = self.propCurrentPoint;
		}
		else
			self.propCurrentPoint = nil;
	}
}

// Returns point by name
//-------------------------------------------------------------------------------------------------------------------------
- (FramePoint*)getFramePoint:(NSString*)name
{
	return [m_framePoints getObjectByProp:@"propName" Value:name];
}

// Returns sprite base texture
//-------------------------------------------------------------------------------------------------------------------------
- (ProjectTextureItem*)getSpriteTexture
{
	return m_sprite.propTexture;
}

// Sets from texture assembly item
//-------------------------------------------------------------------------------------------------------------------------
- (void)linkSetFramesCount:(uint)count
{
	m_linkFramesCount	= count;

	uint realCount		= count;
	
	if (m_link.propMirrorX || m_link.propMirrorY)
		realCount = count + (count - m_link.propMirrorStart - m_link.propMirrorEnd);

	if ([m_frames count] != realCount)
	{
		NSMutableArray* newContainer = [[NSMutableArray alloc] initWithCapacity:realCount];
		
		uint curCount = 0;
		
		for (AnimationFrame* frame in m_frames)
		{
			if (curCount == realCount)
				break;
			
			[newContainer addObject:frame];
			
			curCount++;
		}
		
		int diff = realCount - [m_frames count];
		
		if (diff > 0)
			for (uint i = 0; i < diff; i++)
			{
				AnimationFrame* newFrame = [[AnimationFrame alloc] initWithAnimation:self];
				
				for (FramePoint* point in m_framePoints)
					[newFrame addPoint:point.propName Color:point.propColor];
				
				[newContainer addObject:newFrame];
				
				[newFrame release];
			}
		
		SAFE_RELEASE(m_frames);
		
		m_frames = newContainer;
	}
}

// Unpacked frame set by texture
//-------------------------------------------------------------------------------------------------------------------------
- (void)linkSetUnpackedFrame:(uint)index X:(uint)x Y:(uint)y W:(uint)w H:(uint)h
{
	AnimationFrame* frame = [m_frames objectAtIndex:index];
	
	frame.propPositionX = x;
	frame.propPositionY = y;
	frame.propWidth		= w;
	frame.propHeight	= h;
	frame.propMirrorX	= m_link.propMirrorSequenceX;
	frame.propMirrorY	= m_link.propMirrorSequenceY;
	
	if ( (index >= m_link.propMirrorStart) && (index < (m_linkFramesCount - m_link.propMirrorEnd)) && (m_link.propMirrorX || m_link.propMirrorY) )
	{
		uint offset = [m_frames count] - (index - m_link.propMirrorStart) - 1;
		
		frame = [m_frames objectAtIndex:offset];
		
		frame.propPositionX = x;
		frame.propPositionY = y;
		frame.propWidth		= w;
		frame.propHeight	= h;
		
		frame.propMirrorX = m_link.propMirrorX;
		frame.propMirrorY = m_link.propMirrorY;
	}
	
}

// Move all frames to offset (final assembly placing in atlas)
//-------------------------------------------------------------------------------------------------------------------------
- (void)linkSetUnpackedOffsetX:(uint)x Y:(uint)y
{
	for (AnimationFrame* frame in m_frames)
	{
		frame.propPositionX += x;
		frame.propPositionY += y;
	}
}


// Packed frame set by texture
//-------------------------------------------------------------------------------------------------------------------------
- (void)linkSetPackedFrame:(uint)index X:(uint)x Y:(uint)y W:(uint)w H:(uint)h OffX:(uint)offx OffY:(uint)offy
{
	AnimationFrame* frame = [m_frames objectAtIndex:index];
	
	frame.propPackPositionX = x;
	frame.propPackPositionY = y;
	frame.propPackWidth		= w;
	frame.propPackHeight	= h;
	frame.propPackOffsetX	= offx;
	frame.propPackOffsetY	= offy;
	
	if ( (index >= m_link.propMirrorStart) && (index < (m_linkFramesCount - m_link.propMirrorEnd)) && (m_link.propMirrorX || m_link.propMirrorY) )
	{
		uint offset = [m_frames count] - (index - m_link.propMirrorStart) - 1;
		
		frame = [m_frames objectAtIndex:offset];
		
		frame.propPackPositionX = x;
		frame.propPackPositionY = y;
		frame.propPackWidth		= w;
		frame.propPackHeight	= h;
		frame.propPackOffsetX	= offx;
		frame.propPackOffsetY	= offy;
	}
}

@end
