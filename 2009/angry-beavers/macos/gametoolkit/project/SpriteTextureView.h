//===============================================================================================
//  File:		SpriteTextureView.h
//  Project:	OriginGameToolkit
//	Desc:		Sprite view/base texture subview interface
//
//  Created by Max Vasin on 3/12/11.
//  Copyright 2011 Origin Games. All rights reserved.
//===============================================================================================

// ************* IMPORTS *************

#import <Cocoa/Cocoa.h>
#import "Base.h"
#import "GuidlineGrid.h"
#import "ApplicationPreferences.h"
#import "ProjectTextureItem.h"
#import "ProjectSpriteItem.h"

// ************* INTERFACE *************

@interface SpriteTextureView : NSOpenGLView 
{
	// ** iternal data **

	BOOL					m_contextReady;
	BOOL					m_bNoFrame;
	GuidlineGrid*			m_backGrid;
	
	// ** OpenGL data **
	
	GLuint					m_oglTexture;
	IntSize					m_texSize;
	OGLVertex				m_vertices[4];
	OGLColorVertex			m_frameVertices[4];
	
	// ** mouse tracking **
	
	BOOL					m_moveFrame;
	BOOL					m_scaleFrame;
	BOOL					m_frameSelected;
	NSPoint					m_selectPoint;
	NSPoint					m_framePoint;
	NSTrackingArea*			m_trackArea;
	
	
	// ** view control **
	
	GLfloat					m_offsetX;
	GLfloat					m_offsetY;
	GLfloat					m_offsetXMin;
	GLfloat					m_offsetXMax;
	GLfloat					m_offsetYMin;
	GLfloat					m_offsetYMax;
	GLfloat					m_zoom;
	GLfloat					m_zoomMin;
	GLfloat					m_zoomMax;
	NSColor*				m_backColor;
	NSColor*				m_frameColor;
	NSColor*				m_frameSelectColor;
	GLfloat					m_frameAlpha;
	GLfloat					m_frameSelectAlpha;
	BOOL					m_lockFrameOffsetX;
	BOOL					m_lockFrameOffsetY;
	BOOL					m_snapFrames;
	IntPoint				m_gridOrigin;
	
	// ** current observing objects **
	
	Project*				m_obsrvProject;
	ProjectSpriteItem*		m_obsrvSprite;
	SpriteAnimation*		m_obsrvAnimation;
	AnimationFrame*			m_obsrvFrame;
	
	
	// ** outlets **
	
	IBOutlet NSObjectController*		m_viewController;
	IBOutlet ApplicationPreferences*	m_appPrefs;
}

// ************* PROPERTIES *************

@property (assign)	GLfloat				propViewOffsetX;
@property (assign)	GLfloat				propViewOffsetY;
@property (assign)	GLfloat				propViewOffsetXMin;
@property (assign)	GLfloat				propViewOffsetXMax;
@property (assign)	GLfloat				propViewOffsetYMin;
@property (assign)	GLfloat				propViewOffsetYMax;
@property (assign)	GLfloat				propViewZoom;
@property (assign)	GLfloat				propViewZoomMin;
@property (assign)	GLfloat				propViewZoomMax;
@property (assign)	BOOL				propLockFrameOffsetX;
@property (assign)	BOOL				propLockFrameOffsetY;
@property (assign)	BOOL				propSnapFrames;

// ************* ACTIONS *************

- (IBAction)	resetOffsets:				(id)sender;

@end
