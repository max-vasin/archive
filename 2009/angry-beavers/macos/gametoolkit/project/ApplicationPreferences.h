//===============================================================================================
//  File:		ApplicationPreferences.h
//  Project:	OriginGameToolkit
//	Desc:		Applicaton preferences container data model class interface
//
//  Created by Max Vasin on 12/3/10.
//  Copyright 2010 Origin. All rights reserved.
//===============================================================================================

// **** IMPORTS ****

#import <Cocoa/Cocoa.h>
#import "BaseObject.h"

// **** INTERFACE ****

@interface ApplicationPreferences : BaseObject
{
	// ***** Common preferences *****
	NSMutableArray*	m_recentProjects;
	
	// ***** Building preferences ****
	NSString*		m_buildTextureTool;
	
	// ***** Texture view preferences *****
	NSInteger		m_textureViewGridDashMultiplier;
	NSInteger		m_textureViewGridHSpace;
	NSInteger		m_textureViewGridVSpace;
	NSColor*		m_textureViewGridBackColor;
	NSColor*		m_textureViewGridDashColor;
	NSColor*		m_textureViewGridOriginColor;
	GLfloat			m_textureViewTextureAlpha;
	
	// ***** Shape view preferences *****
	NSInteger		m_shapeViewGridDashMultiplier;
	NSInteger		m_shapeViewGridHSpace;
	NSInteger		m_shapeViewGridVSpace;
	NSColor*		m_shapeViewGridBackColor;
	NSColor*		m_shapeViewGridDashColor;
	NSColor*		m_shapeViewGridOriginColor;
	
	NSColor*		m_shapeViewShapeColor;
	NSColor*		m_shapeViewClosingLineColor;
	NSColor*		m_shapeViewVertexColor;
	NSColor*		m_shapeViewSelectLineColor;
	NSColor*		m_shapeViewSelectVertexColor;
	GLfloat			m_shapeViewShapeWidth;
	GLfloat			m_shapeViewVertexSize;
	
	// ***** Sprite view preferences *****
	NSInteger		m_spriteViewGridDashMultiplier;
	NSColor*		m_spriteViewGridBackColor;
	NSColor*		m_spriteViewGridDashColor;
	NSColor*		m_spriteViewGridOriginColor;
	NSColor*		m_spriteViewFrameColor;
	NSColor*		m_spriteViewFrameSelectColor;
	GLfloat			m_spriteViewFrameAlpha;
	GLfloat			m_spriteViewFrameSelectAlpha;
	GLfloat			m_spriteViewAxisWidth;
	
	// ***** Sprite sequence view preferences *****
	NSColor*		m_spriteSeqViewCheckerColor;
	GLfloat			m_spriteSeqViewActiveAlpha;
	GLfloat			m_spriteSeqViewInactiveAlpha;
	
	
}

// **** PROPERTIES ****

@property	(copy)		NSString*		propBuildTextureTool;

@property	(assign)	NSInteger		propTextureViewGridDashMultiplier;
@property	(assign)	NSInteger		propTextureViewGridHSpace;
@property	(assign)	NSInteger		propTextureViewGridVSpace;
@property	(copy)		NSColor*		propTextureViewGridBackColor;
@property	(copy)		NSColor*		propTextureViewGridDashColor;
@property	(copy)		NSColor*		propTextureViewGridOriginColor;
@property	(assign)	GLfloat			propTextureViewTextureAlpha;

@property	(assign)	NSInteger		propShapeViewGridDashMultiplier;
@property	(assign)	NSInteger		propShapeViewGridHSpace;
@property	(assign)	NSInteger		propShapeViewGridVSpace;
@property	(copy)		NSColor*		propShapeViewGridBackColor;
@property	(copy)		NSColor*		propShapeViewGridDashColor;
@property	(copy)		NSColor*		propShapeViewGridOriginColor;
@property	(copy)		NSColor*		propShapeViewShapeColor;
@property	(copy)		NSColor*		propShapeviewClosingLineColor;
@property	(copy)		NSColor*		propShapeViewVertexColor;
@property	(copy)		NSColor*		propShapeViewSelectLineColor;
@property	(copy)		NSColor*		propShapeViewSelectVertexColor;
@property	(assign)	GLfloat			propShapeViewShapeWidth;
@property	(assign)	GLfloat			propShapeViewVertexSize;

@property	(assign)	NSInteger		propSpriteViewGridDashMultiplier;
@property	(copy)		NSColor*		propSpriteViewGridBackColor;
@property	(copy)		NSColor*		propSpriteViewGridDashColor;
@property	(copy)		NSColor*		propSpriteViewGridOriginColor;
@property	(copy)		NSColor*		propSpriteViewFrameColor;
@property	(copy)		NSColor*		propSpriteViewFrameSelectColor;
@property	(assign)	GLfloat			propSpriteViewFrameAlpha;
@property	(assign)	GLfloat			propSpriteViewFrameSelectAlpha;
@property	(assign)	GLfloat			propSpriteViewAxisWidth;

@property	(copy)		NSColor*		propSpriteSeqViewCheckerColor;
@property	(assign)	GLfloat			propSpriteSeqViewActiveAlpha;
@property	(assign)	GLfloat			propSpriteSeqViewInactiveAlpha;

@property	(assign)	NSString*		propLastProject;
@property	(readonly)	NSArray*		propRecentProjects;

// **** INSTANCE METHODS ****

- (result)load;
- (result)save;

@end
