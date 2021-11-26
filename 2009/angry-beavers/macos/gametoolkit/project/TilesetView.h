//===============================================================================================
//  File:		TilesetView.h
//  Project:	OriginGameToolkit
//	Desc:		Tileset OpenGL view class
//
//  Created by Max Vasin on 4/26/11.
//  Copyright 2011 Origin Games. All rights reserved.
//===============================================================================================

// ***** IMPORTS *****

#import <Cocoa/Cocoa.h>
#import "Base.h"
#import "BaseObject.h"
#import "ProjectTilesetItem.h"
#import "ProjectTileMapItem.h"
#import "GLFrame.h"

// ***** INTERFACE *****

@interface TilesetView : NSOpenGLView
{
	// ** internal data **
   	
	BOOL					m_contextReady;			// Context initialization flag
	BOOL					m_tilemapMode;			// Tilemap mode
	
	// ** graphics data **
	
	OGLVertex				m_vertsBack[4];			// Background shape vertices
	GLuint					m_oglCheckerTexture;	// OpenGL checkers texture
	IntSize					m_texCheckerSize;		// OpenGL checkers texture size
	OGLVertex*				m_vertsFrames;			// Sequence mode: frames vertices
	NSInteger				m_numFrames;			// Number of frames
	NSInteger				m_numFramesVerts;		// Number of frames vertices
	GLuint					m_oglTexture;			// OpenGL texture
	GLuint					m_oglFrameTexture;		// OpenGL frame texture
	IntSize					m_texSize;				// OpenGL texture size
	GLFrame*				m_glFrameSelect;
	GLFrame*				m_glFrameMove;
	float					m_fullOffset;
	
	// ** preferences **
	
	NSColor*				m_colorChecker;			// Background checker texture modulation color

	// ** current observing objects **
	
	Project*				m_obsrvProject;			// Current project tracking
	ProjectTilesetItem*		m_obsrvTileset;			// Current tileset tracking
	ProjectTileMapItem*		m_obsrvTilemap;			// Current tilemap tracking
	
	// ** view control **
	
	GLfloat					m_offsetX;				// View X offset (slider)
	GLfloat					m_offsetY;				// View Y offset (slider)
	GLfloat					m_offsetXMin;			// View X offset min bound (slider)
	GLfloat					m_offsetXMax;			// View X offset max bound (slider)
	GLfloat					m_offsetYMin;			// View Y offset min bound (slider)
	GLfloat					m_offsetYMax;			// View Y offset max bound (slider)
	GLfloat					m_zoom;					// View zooming
	
	// ** mouse tracking **

	NSTrackingArea*			m_trackArea;			// Mouse tracking area
}

// ***** PROPERTIES *****

@property (nonatomic, assign)	GLfloat		propViewOffsetXMin;
@property (nonatomic, assign)	GLfloat		propViewOffsetXMax;
@property (nonatomic, assign)	GLfloat		propViewOffsetYMin;
@property (nonatomic, assign)	GLfloat		propViewOffsetYMax;
@property (nonatomic, assign)	GLfloat		propViewOffsetX;
@property (nonatomic, assign)	GLfloat		propViewOffsetY;
@property (nonatomic, assign)	GLfloat		propViewZoom;
@property (nonatomic, assign)	BOOL		propTileMapMode;
@property (nonatomic, copy)		NSColor*	propCheckerColor;

@end
