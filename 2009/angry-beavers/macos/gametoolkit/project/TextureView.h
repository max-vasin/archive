//===============================================================================================
//  File:		TextureView.h
//  Project:	OriginGameToolkit
//	Desc:		Texture project item view interface
//
//  Created by Max Vasin on 12/2/10.
//  Copyright 2010 Origin. All rights reserved.
//===============================================================================================

// ***** IMPORTS *****

#import <Cocoa/Cocoa.h>
#import "GuidlineGrid.h"
#import "ApplicationPreferences.h"
#import "ViewsTab.h"
#import "ProjectTextureItem.h"
#import "OriTextureFile.h"

// ***** INTERFACE *****

@interface TextureView : NSOpenGLView 
{
	BOOL					m_contextReady;
	GuidlineGrid*			m_backGrid;
	NSColor*				m_backColor;
	NSColor*				m_borderColor;
	ProjectTextureItem*		m_itemTexture;
	GLuint					m_oglTexture;
	OGLVertex				m_vertices[4];
	OGLColorVertex			m_borderVertices[4];
	OriTextureFile*			m_textureFile;
	
	GLfloat					m_offsetX;
	GLfloat					m_offsetY;
	GLfloat					m_offsetXMin;
	GLfloat					m_offsetXMax;
	GLfloat					m_offsetYMin;
	GLfloat					m_offsetYMax;
	GLfloat					m_zoom;
	
	BOOL					m_viewPacked;
	BOOL					m_noRedraw;
	
	IntSize					m_texSize;
	
	IBOutlet ApplicationPreferences*	m_appPrefs;
	IBOutlet ViewsTab*					m_viewsTab;
}

// ***** PROPERTIES *****

@property (copy)	NSColor*				propBackColor;
@property (assign)	ProjectTextureItem*		propCurrentItem;
@property (assign)	GLfloat					propViewOffsetX;
@property (assign)	GLfloat					propViewOffsetY;
@property (assign)	GLfloat					propViewOffsetXMin;
@property (assign)	GLfloat					propViewOffsetXMax;
@property (assign)	GLfloat					propViewOffsetYMin;
@property (assign)	GLfloat					propViewOffsetYMax;
@property (assign)	GLfloat					propViewZoom;
@property (assign)	BOOL					propViewPacked;
@property (assign)	NSString*				propSizeString;
@property (assign)	NSString*				propMemoryString;

// ***** INSTANCE METHODS *****

- (void)		rebuildResources;
- (void)		cleanResources;
- (void)		renderTexture;

// ***** ACTIONS *****

- (IBAction)	resetOffsets:				(id)sender;

@end
