//=======================================================================================================================
//  File:		GameLevel.h
//  Project:	heli
//	Desc:		game level container class
//
//  Created by Max Vasin on 5/30/11.
//  Copyright 2011 Origin. All rights reserved.
//=======================================================================================================================

// ******** IMPORTS ********

#import "OriBase.h"
#import "OriScene.h"
#import "OriTilemapContainer.h"
#import "RedneckHelicopter.h"
#import "GameHexGrid.h"
#import "OriHexGridContainer.h"
#import "OriServer.h"
#import "OriStringDrawer.h"
#import "OriMapStaticContainer.h"
#import "GameAIController.h"

// ******** INTERFACE ********

@interface GameLevel : NSObject <OriServerResponder>
{
	NSString*				m_name;
	OriScene*				m_tilemapScene;
	OriScene*				m_hexgridScene;
	OriScene*				m_groundScene;
	OriTilemapContainer*	m_tilemapContainer;
	OriHexGridContainer*	m_hexgridContainer;
	OriMapStaticContainer*	m_staticContainer;
	GameHexGrid*			m_hexgrid;
	GameAIController*		m_ai;

	
	// TEST
	OriStringDrawer*		m_textVisibles;
	OriScene*				m_testScene;
	
}

// ******** PROPERTIES ********

@property (nonatomic, copy)			NSString*			propName;
@property (nonatomic, readonly)		GameHexGrid*		propHexGrid;
@property (nonatomic, readonly)		GameHeliObject*		propHelicopter;

// ******** INSTANCE METHODS ********

- (id)		initWithTilemapName:(NSString*)name;

- (void)	update:(oriFloat)delta;
- (void)	render;

@end
