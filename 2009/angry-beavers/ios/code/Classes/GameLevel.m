//=======================================================================================================================
//  File:		GameLevel.m
//  Project:	heli
//	Desc:		Game level container class implementation
//
//  Created by Max Vasin on 5/30/11.
//  Copyright 2011 Origin. All rights reserved.
//=======================================================================================================================

// ******** IMPORTS ********

#import "GameLevel.h"

// ******** IMPLEMENTATION ********

@implementation GameLevel

//#######################################################################################################################
// PROPERTIES
//#######################################################################################################################

//-----------------------------------------------------------------------------------------------------------------------
// SYNTHESIZED PROPERTIES
//-----------------------------------------------------------------------------------------------------------------------

@synthesize propName			= m_name;
@synthesize propHexGrid			= m_hexgrid;
@synthesize propHelicopter		= m_helicopter;

//#######################################################################################################################
// CONSTRUCTION
//#######################################################################################################################

//-----------------------------------------------------------------------------------------------------------------------
// Default initializer
//-----------------------------------------------------------------------------------------------------------------------

- (id)init
{
	if ( (self = [super init]) )
	{
	}
	return self;
}

//-----------------------------------------------------------------------------------------------------------------------
// Designated initializer
//-----------------------------------------------------------------------------------------------------------------------

- (id)initWithTilemapName:(NSString*)name
{
	if ( (self = [self init]) )
	{
		self.propName			= name;
		
		m_hexgrid			= [[GameHexGrid alloc] initWithTilemapName:name];
		m_ai				= [[GameAIController alloc] initWithHexGrid:m_hexgrid];
				
		m_tilemapScene		= [[OriScene alloc] initWithName:name];
		m_hexgridScene		= [[OriScene alloc] initWithName:name];
		m_groundScene		= [[OriScene alloc] initWithName:name];
				
		m_tilemapContainer	= [[OriTilemapContainer alloc] initWithTilemapName:name];
		m_hexgridContainer	= [[OriHexGridContainer alloc] initWithTilemapName:name andSpriteName:@"hex"];
		m_staticContainer	= [[OriMapStaticContainer alloc] initWithTilemapName:name Parent:nil];
		
		
				
		[m_tilemapScene attach:m_tilemapContainer];
		[m_hexgridScene attach:m_hexgridContainer];
		[m_groundScene	attach:m_staticContainer];
		
		
		m_testScene			= [[OriScene alloc] initWithName:@"test"];
		
		OriSpriteRenderable* test = [[OriSpriteRenderable alloc] initWithSpriteName:@"turrel 01" Parent:nil];
		
		[m_testScene attach:test];
		
		test.propFPS = 1;
		
		[test play];
		
		[test release];
		
		
		m_textVisibles		= [[OriStringDrawer alloc] initWithFontName:@"main font" withCapacity:40];
		
		oglColor redColor = {255, 0, 0, 255};
		
		m_textVisibles.propColor	= redColor;
		m_textVisibles.propSpacing	= 0;
				
		
		[[OriServer sharedServer] addResponder:self];
	}
	return self;
}

//-----------------------------------------------------------------------------------------------------------------------
// Destructor
//-----------------------------------------------------------------------------------------------------------------------

- (void)dealloc
{
	[[OriServer sharedServer]  delResponder:self];
	
	[m_tilemapScene detach:m_tilemapContainer];
	[m_hexgridScene detach:m_hexgridContainer];
	[m_groundScene  detach:m_staticContainer];
	
	[m_tilemapContainer release];
	[m_hexgridContainer release];
	[m_staticContainer release];
	
	[m_tilemapScene release];
	[m_hexgridScene release];
	[m_groundScene release];
	
	[m_hexgrid release];
	
	[m_textVisibles release];
	
	[m_testScene release];
	
	[m_ai release];
	[m_hexgrid release];
	
	
	[m_name	release];
	
	[super dealloc];
}

//#######################################################################################################################
// LOOP
//#######################################################################################################################

//-----------------------------------------------------------------------------------------------------------------------
// Updates game level with timer
//-----------------------------------------------------------------------------------------------------------------------

- (void)update:(oriFloat)delta
{
	[m_tilemapScene update:delta];
	//[m_hexgridScene update:delta];
	[m_groundScene update:delta];
	
	//[m_testScene update:delta];
}

//-----------------------------------------------------------------------------------------------------------------------
// Renders whole game level
//-----------------------------------------------------------------------------------------------------------------------

- (void)render
{
	[m_tilemapScene render];
	//[m_hexgridScene render];
	[m_groundScene render];
	
	//[m_testScene render];
	
	m_textVisibles.propString = [NSString stringWithFormat:@"VISIBLE TILES %i", m_tilemapScene.propVisibleObjectsCount];
	
	[m_textVisibles render];
	
	glPushMatrix();
	
	glTranslatef(0.0f, 20.0f, 1.0f);
	
	m_textVisibles.propString = [NSString stringWithFormat:@"VISIBLE HEXES %i", m_hexgridScene.propVisibleObjectsCount];
	
	[m_textVisibles render];
	
	glTranslatef(0.0f, 20.0f, 1.0f);
	
	m_textVisibles.propString = [NSString stringWithFormat:@"VISIBLE STATIC OBJECTS %i", m_groundScene.propVisibleObjectsCount];
	
	[m_textVisibles render];
	
	
	glTranslatef(0.0f, 20.0f, 1.0f);
	
	m_textVisibles.propString = [NSString stringWithFormat:@"VISIBLE TEST OBJECTS %i", m_testScene.propVisibleObjectsCount];
	
	[m_textVisibles render];
	
	
	glTranslatef(0.0f, 20.0f, 1.0f);
	
	m_textVisibles.propString = [NSString stringWithFormat:@"FPS %i", [[OriTimer sharedTimer] propFPS]];
	
	[m_textVisibles render];
	
	glPopMatrix();
}

//#######################################################################################################################
// SERVER PROTOCOL SUPPORT
//#######################################################################################################################

//-----------------------------------------------------------------------------------------------------------------------
// Recieving server command
//-----------------------------------------------------------------------------------------------------------------------

- (void)handleCommand:(OriServerCommand*)cmd
{
	Vector2* point, *position;
	
	switch (cmd.propId)
	{
		case eServerCmdPlayerMove:
			break;
		case eServerCmdPlayerLook:
			
			point    = [cmd.propArgs objectAtIndex:0];
			
			position = m_tilemapContainer.propPosition;
			
			oriIntSize size = [[[OriResourceManager sharedManager] propRenderer] propBufferSize];
			
			m_testScene.propTranslate = point;
			
			point->x -= size.width / 2.0f;
			point->y -= size.height / 2.0f;
			
			point->x = -point->x;
			point->y = -point->y;
			
			
			Vector2* newDir = [[Vector2 alloc] initWithX:point->x Y:point->y];
			
			[newDir makeScaleTo:0.5f];
			
			m_tilemapContainer.propVelocity = newDir;
			m_hexgridContainer.propVelocity = newDir;
			m_staticContainer.propVelocity = newDir;
			
			[newDir release];
			
			
			
			/*oriVector2 pt = {point->x, point->y};
			
			OriTileRenderable* tile = [m_tilemapContainer pickTileAtPoint:pt];
			
			oglColor redColor = {1.0f, 0.0f, 0.0f, 1.0f};
			
			if (tile)
			{
				tile.propColor = redColor;
			}*/
						
			
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

- (BOOL)isWeakLinkToServer
{
	return YES;
}





@end
