//=======================================================================================================================
//  File:		OriSpriteAnimationFrame.m
//  Project:	heli
//	Desc:		sprite animation frame class implementation
//
//  Created by Max Vasin on 3/23/11.
//  Copyright 2011 Origin. All rights reserved.
//=======================================================================================================================

// ******** IMPORTS ********

#import "OriSpriteAnimationFrame.h"
#import "OriSprite.h"
#import "OriTexture.h"

// ******** INTERFACE ********

@implementation OriSpriteAnimationFrame

//#######################################################################################################################
// PROPERTIES
//#######################################################################################################################

//-----------------------------------------------------------------------------------------------------------------------
// SYNTHESIZED
//-----------------------------------------------------------------------------------------------------------------------

@synthesize propMapMin	= m_mapMin;
@synthesize propMapMax	= m_mapMax;
@synthesize propSize	= m_size;

//-----------------------------------------------------------------------------------------------------------------------
// PROPERTY: avalilable frame points count
//-----------------------------------------------------------------------------------------------------------------------

- (oriUint)propPointsCount
{
	return [m_points count];
}


//-----------------------------------------------------------------------------------------------------------------------
// PROPERTY: generic origin point property
//-----------------------------------------------------------------------------------------------------------------------

- (oriIntPoint)propOrigin
{
	OriSpriteAnimationPoint* point = [self getPointByName:m_animation.propOrigin];
	
	if (point)
		return point.propPosition;
	
	oriIntPoint zero;
	
	zero.x = 0;
	zero.y = 0;
	
	return zero;
}

//-----------------------------------------------------------------------------------------------------------------------
// PROPERTY: generic mapping rectangle
//-----------------------------------------------------------------------------------------------------------------------

- (oriRect)propMapRect
{
	oriRect rect;
	
	rect.min.x	= m_mapMin.u;
	rect.min.y	= m_mapMin.v;
	rect.max.x	= m_mapMax.u;
	rect.max.y	= m_mapMax.v;
	
	return rect;
}

//-----------------------------------------------------------------------------------------------------------------------
// PROPERTY: override OriResource
//-----------------------------------------------------------------------------------------------------------------------

- (oriUint)propSysMemUsed
{
	return malloc_size(self);
}

//-----------------------------------------------------------------------------------------------------------------------
// PROPERTY: override OriResource
//-----------------------------------------------------------------------------------------------------------------------

- (oriUint)propVideoMemUsed
{
	return 0;
}

//#######################################################################################################################
// CONSTRUCTION
//#######################################################################################################################

//-----------------------------------------------------------------------------------------------------------------------
// Returns new autoreleased animation frame instance with for given animation
//-----------------------------------------------------------------------------------------------------------------------

+ (id)frameWithAnimation:(OriSpriteAnimation*)animation
{
	return [[[self alloc] initWithAnimation:animation] autorelease];
}

//-----------------------------------------------------------------------------------------------------------------------
// Returns new autoreleased animation frame instance with for given animation and XML data
//-----------------------------------------------------------------------------------------------------------------------

+ (id)frameWithAnimation:(OriSpriteAnimation*)animation withXML:(OriXMLNode*)node
{
	return  [[[self alloc] initWithAnimation:animation withXML:node] autorelease];
}

//#######################################################################################################################
// INITIALIZERS/DESTRUCTORS
//#######################################################################################################################

//-----------------------------------------------------------------------------------------------------------------------
// Defalut initializer
//-----------------------------------------------------------------------------------------------------------------------

- (id)init
{
	if ( (self = [super init]) )
	{
		m_points = [[NSMutableArray alloc] init];
	}
	return self;
}

//-----------------------------------------------------------------------------------------------------------------------
// Destructor
//-----------------------------------------------------------------------------------------------------------------------

- (void)dealloc
{
	SAFE_RELEASE(m_points);
	
	[super dealloc];
}

//-----------------------------------------------------------------------------------------------------------------------
// Initilizes new instance of frame with animation
//-----------------------------------------------------------------------------------------------------------------------

- (id)initWithAnimation:(OriSpriteAnimation*)animation
{
	if ( (self = [self init]) )
	{
		m_animation = animation;
	}
	return self;
}

//-----------------------------------------------------------------------------------------------------------------------
// Initilizes new instance of frame with animation and XML node
//-----------------------------------------------------------------------------------------------------------------------

- (id)initWithAnimation:(OriSpriteAnimation*)animation withXML:(OriXMLNode*)node
{
	if ( (self = [self initWithAnimation:animation]) )
	{
		if (node)
		{
			m_position.x	= [node getFirstChildValueInt:@"PosX"	placeholder:0];
			m_position.y	= [node getFirstChildValueInt:@"PosY"	placeholder:0];
			m_size.width	= [node getFirstChildValueInt:@"Width"	placeholder:0];
			m_size.height	= [node getFirstChildValueInt:@"Height" placeholder:0];
			m_mirrorX		= [node getFirstChildValueInt:@"MirX"	placeholder:NO];
			m_mirrorY		= [node getFirstChildValueInt:@"MirY"	placeholder:NO];
			
			OriXMLNode* pointsRoot = [node getFirstChild:@"Points"];
			
			for (OriXMLNode* pointNode in pointsRoot.propChildren)
				[m_points addObject:[OriSpriteAnimationPoint pointWithXML:pointNode]];
		}
	}
	return self;
}

//-----------------------------------------------------------------------------------------------------------------------
// Returns animaton point at index
//-----------------------------------------------------------------------------------------------------------------------

- (OriSpriteAnimationPoint*)getPointAtIndex:(oriUint)index
{
	if (index >= [m_points count])
		return nil;
	
	return [m_points objectAtIndex:index];
}

//-----------------------------------------------------------------------------------------------------------------------
// Returns animaton point with name
//-----------------------------------------------------------------------------------------------------------------------

- (OriSpriteAnimationPoint*)getPointByName:(NSString*)name
{
	return [m_points getObjectByProp:@"propName" Value:name];
}

//#######################################################################################################################
// ITERNAL METHODS
//#######################################################################################################################

//-----------------------------------------------------------------------------------------------------------------------
// Recalculates texture coordinates of frame with current texture of sprite
//-----------------------------------------------------------------------------------------------------------------------

- (void)updateTextureDepencies:(OriTexture*)texture
{
	oriIntSize texSize = texture.propSize;
	
	oriFloat buf;
	
	m_mapMin.u = (GLfloat)m_position.x / (GLfloat)texSize.width;
	m_mapMin.v = (GLfloat)m_position.y / (GLfloat)texSize.height;
	
	m_mapMax.u = m_mapMin.u + (GLfloat)m_size.width / (GLfloat)texSize.width;
	m_mapMax.v = m_mapMin.v + (GLfloat)m_size.height / (GLfloat)texSize.height;

	if (m_mirrorX)
	{
		buf = m_mapMax.u;
		m_mapMax.u = m_mapMin.u;
		m_mapMin.u = buf;
	}
	
	
	if (m_mirrorY)
	{
		buf = m_mapMax.v;
		m_mapMax.v = m_mapMin.v;
		m_mapMin.v = buf;
	}
}

@end
