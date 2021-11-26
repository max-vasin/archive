//===============================================================================================
//  File:		SpriteFramesView.h
//  Project:	OriginGameToolkit
//	Desc:		Sprite editor frames view
//
//  Created by Max Vasin on 3/12/11.
//  Copyright 2011 Origin Games. All rights reserved.
//===============================================================================================

// ************* IMPORTS *************

#import <Cocoa/Cocoa.h>
#import "Base.h"
#import "Project.h"
#import "ProjectSpriteItem.h"
#import "AppDelegate.h"
#import "ApplicationPreferences.h"
#import "GLMovie.h"

// ************ VIEW MODE ************
typedef enum
{
	eSpriteFramesViewSequence	= 0,
	eSpriteFramesViewAnimation	= 1
}enSpriteFramesViewMode;

// ************* INTERFACE *************

@interface SpriteFramesView : NSOpenGLView 
{
	// ** internal data **
	
	BOOL					m_contextReady;			// OpenGL context and related resources creation flag
	enSpriteFramesViewMode	m_viewMode;				// Current view mode
	NSTimer*				m_animTimer;			// Animation timer
	BOOL					m_animPlaying;

	// ** graphics data **
	
	OGLVertex				m_vertsAnim;			// Animation mode: sprite vertices
	OGLVertex				m_vertsBack[4];			// Background shape vertices
	OGLVertex				m_vertsFrame[4];		// Frame rectanger shape vertices
	OGLColorVertex			m_vertsPoint[4];		// Frame point vertices
	OGLVertex*				m_vertsFrames;			// Sequence mode: frames vertices
	BOOL*					m_framesValid;			// Buffer contains frame validity flag
	NSInteger				m_numFrames;			// Number of frames
	NSInteger				m_numFramesVerts;		// Number of frames vertices
	GLfloat					m_framesWidth;			// Builded frames total width
	GLfloat					m_framesHeight;			// Builded frames total height
	GLfloat					m_currentFramePos;		// Builded current frame position on x (to center)
	GLuint					m_oglTexture;			// OpenGL texture
	IntSize					m_texSize;				// OpenGL texture size
	GLuint					m_oglNoFrameTexture;	// OpenGL no frame texture
	IntSize					m_texNoFrameSize;		// OpenGL no frame texture size
	GLuint					m_oglCheckerTexture;	// OpenGL checkers texture
	IntSize					m_texCheckerSize;		// OpenGL checkers texture size
	GLMovie*				m_movie;
	
	// ** preferences **
	
	NSColor*				m_colorChecker;
	GLfloat					m_alphaActive;
	GLfloat					m_alphaInactive;
	
	// ** current observing objects **
	
	Project*				m_obsrvProject;
	ProjectSpriteItem*		m_obsrvSprite;
	SpriteAnimation*		m_obsrvAnimation;
	AnimationFrame*			m_obsrvFrame;
	FramePoint*				m_obsrvPoint;
	
	// ** view control **
	
	GLfloat					m_zoom;
	BOOL					m_lockControls;
	BOOL					m_useOriginPoint;
	
	// ** mouse tracking **
	
	NSPoint					m_selectPoint;
	NSPoint					m_currentOffset;
	NSTrackingArea*			m_trackArea;
	BOOL					m_movePoint;

	// ** outlets **
	
	IBOutlet ApplicationPreferences*	m_appPrefs;
	IBOutlet NSObjectController*		m_viewController;
}

// ************* PROPERTIES *************

@property (assign)	GLfloat		propZoom;
@property (assign)	BOOL		propLockControls;
@property (assign)	BOOL		propUseOriginPoint;

// ************* ACTIONS *************

- (IBAction)	togglePlayAnimation:(id)sender;


@end
