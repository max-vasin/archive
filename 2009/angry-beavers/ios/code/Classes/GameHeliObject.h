//=======================================================================================================================
//  File:		GameHeliObject.h
//  Project:	heli
//	Desc:		Base helicopter object
//
//  Created by Max Vasin on 4/19/11.
//  Copyright 2011 Origin. All rights reserved.
//=======================================================================================================================

#import "OriBase.h"
#import "OriMultiSpriteContainer.h"
#import "Vector2.h"
#import "OriServerResponder.h"

@interface GameHeliObject : OriMultiSpriteContainer// <OriServerResponder>
{
	/*
	NSString*			m_rotorSpriteName;
	NSString*			m_rotorLowAnimationName;
	NSString*			m_rotorMidAnimationName;
	NSString*			m_rotorHighAnimationName;
	
	OriSpriteObject*	m_spriteRotor;
	
	oriFloat			m_frameAngle;
	Vector2*			m_curDirection;
	
	oriFloat			m_curSpeed;
	
	oriFloat			m_curRotationSpeed;
	oriFloat			m_curRotationTime;
	oriFloat			m_needRotationTime;
	oriFloat			m_rotAngle;
	Vector2*			m_startDir;
	 */
}

/*
@property (nonatomic, copy)		Vector2*	propDirection;
@property (nonatomic, assign)	oriFloat	propSpeed;

- (id)		initWithName:(NSString*)name	fromResource:(NSString*)resname;

- (void)	setDirectionX:(oriFloat)x		Y:(oriFloat)y;
- (void)	getDirectionX:(oriFloat*)x		Y:(oriFloat*)y;
*/
 
@end
