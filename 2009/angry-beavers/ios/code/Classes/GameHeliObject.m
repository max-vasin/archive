//
//  GameHeliObject.m
//  heli
//
//  Created by Max Vasin on 4/19/11.
//  Copyright 2011 Origin. All rights reserved.
//

#import "GameHeliObject.h"
#import "OriServer.h"

/*
@interface GameHeliObject (Private)

- (void)	initHeli;

@end
*/

@implementation GameHeliObject

/*
@synthesize propDirection = m_curDirection;
@synthesize propSpeed		= m_curSpeed;

- (void)setPropMultiSprite:(NSString*)name;
{
	[super setPropMultiSprite:name];
	
	[self initHeli];
}

- (id)initWithName:(NSString*)name fromResource:(NSString*)resname
{
	if ( (self = [super initWithName:name fromResource:resname]) )
	{
		[self initHeli];
		
		self.propDirection	= [Vector2 vectorWithX:0 Y:-1];
		
		[[OriServer sharedServer] addResponder:self];
	}
	return  self;
}

- (void)dealloc
{
	[[OriServer sharedServer] delResponder:self];
	SAFE_RELEASE(m_curDirection);
	
	[super dealloc];
}

- (BOOL)isWeakLinkToServer
{
	return YES;
}

- (void)setDirectionX:(oriFloat)x Y:(oriFloat)y
{
	m_curDirection->x = x;
	m_curDirection->y = y;
	
	[m_curDirection makeNormalize];
	
	oriFloat angle = [m_curDirection angleFullWith:[Vector2 vectorWithX:0 Y:-1]];
	
	angle = 2 * PI - angle;
	
	oriFloat frame   = floorf(angle / m_frameAngle);
	oriFloat segment = frame * m_frameAngle;
	oriFloat offset  = angle - segment;
	
	if (offset > (m_frameAngle / 2.0f))
		frame++;
	
	if (frame >= m_curMultiSprite.propBaseAnimation.propFrameCount)
		frame = 0;
	
	m_baseSpriteObject.propFrame = (oriUint)frame;
}

- (void)getDirectionX:(oriFloat*)x Y:(oriFloat*)y
{
	*x = m_curDirection->x;
	*y = m_curDirection->y;
}

- (void)update:(oriFloat)delta
{
//	m_position.x += (m_curDirection->x * m_curSpeed * delta);
//	m_position.y += (m_curDirection->y * m_curSpeed * delta);
	
	if (m_curRotationTime != m_needRotationTime)
	{
		m_curRotationTime += delta;
		if (m_curRotationTime > m_needRotationTime)
			m_curRotationTime = m_needRotationTime;
		
		Vector2* lerp = [m_startDir copy];
		
		[lerp makeRotateTo:m_rotAngle * (m_curRotationTime / m_needRotationTime)];
		
		[lerp makeNormalize];
		
		[self setDirectionX:lerp->x Y:lerp->y];
		
		[lerp release];
	}
	
	[super update:delta];
}

- (void)handleCommand:(OriServerCommand*)cmd
{
	Vector2* point;
	
	switch (cmd.propId)
	{
		case eServerCmdPlayerMove:
			point = [cmd.propArgs objectAtIndex:0];
		
			m_position.x = point->x;
			m_position.y = point->y;
			break;
		case eServerCmdPlayerLook:
			point = [cmd.propArgs objectAtIndex:0];
			
			Vector2* newDir = [Vector2 vectorWithX:point->x - m_position.x Y:point->y - m_position.y];
			
			[self setDirectionX:newDir->x Y:newDir->y];
			
			m_curSpeed = newDir.propLength;
			
			
			//Vector2* newDir	= [Vector2 vectorWithX:point->x - m_position.x Y:point->y - m_position.y];
			//[newDir makeNormalize];
			
			//[m_startDir release];
			
			//m_startDir = [m_curDirection copy];
			
			//m_curRotationTime	= 0.0f;
		//	m_rotAngle			= [m_curDirection angleWith:newDir];
		//	m_needRotationTime	= m_rotAngle / m_curRotationSpeed;
			
			//oriFloat angle = [m_curDirection
			
			break;
		case eServerCmdNone:
			break;
	}
}

- (void)initHeli
{
	DBG_ASSERT_LOG0A(!m_baseSpriteObject, @"GameHeliObject: base body sprite unassigned");
	
	if (m_rotorSpriteName)
	{
		m_spriteRotor	= [m_linkSpriteObjects getObjectByProp:@"propName" Value:m_rotorSpriteName];

		DBG_ASSERT_LOG1A(!m_spriteRotor, @"GameHeliObject: cant find rotor sprite (%@)", m_rotorSpriteName);
	
		if (m_spriteRotor)
		{
			m_spriteRotor.propAnimationPlaying = YES;
		}
	}
	
	m_frameAngle = 2 * PI / (float)m_curMultiSprite.propBaseAnimation.propFrameCount;
}
*/
@end
