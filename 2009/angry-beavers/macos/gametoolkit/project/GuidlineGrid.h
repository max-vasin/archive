//===============================================================================================
//  File:		GuidlineGrid.h
//  Project:	OriginGameToolkit
//  Desc:		View guidline grid utility class interface
//
//  Created by Max Vasin on 12/3/10.
//  Copyright 2010 Origin. All rights reserved.
//===============================================================================================

// **** IMPORTS ****

#import <Cocoa/Cocoa.h>
#import "BaseObject.h"
#import "Vector2.h"

// **** INTERFACE ****

@interface GuidlineGrid : BaseObject 
{
	NSOpenGLContext*	m_context;
	GLfloat				m_hSpace;
	GLfloat				m_vSpace;
	GLfloat				m_axisWidth;
	Vector2*			m_vOrigin;
	Vector2*			m_vScaleOrigin;
	NSRect				m_rectFrame;
	OGLColorVertex*		m_pVertices;
	OGLColorVertex*		m_pOriginVertices;
	NSInteger			m_primitiveCount;
	BOOL				m_builded;
	NSInteger			m_dashMultiplier;
	NSColor*			m_dashColor;
	NSColor*			m_axisColor;
	BOOL				m_axisXVisible;
	BOOL				m_axisYVisible;
	BOOL				m_lockScaleOrigin;
	GLfloat				m_zoom;
	
}

// **** PROPERTIES ****

@property	(assign)	GLfloat		propHSpace;
@property	(assign)	GLfloat		propVSpace;
@property	(assign)	GLfloat		propZoom;
@property	(copy)		Vector2*	propOrigin;
@property	(copy)		Vector2*	propZoomOrigin;
@property	(assign)	BOOL		propLockZoomOrigin;
@property	(assign)	NSRect		propFrame;
@property   (assign)	NSInteger	propDashMultiplier;
@property	(copy)		NSColor*	propDashColor;
@property	(copy)		NSColor*	propAxisColor;
@property	(assign)	GLfloat		propAxisWidth;

// **** INSTANCE METHODS ****

- (id)			initWithContext:	(NSOpenGLContext*)context andRect:(NSRect)frame;
- (void)		reBuild;
- (void)		render;
- (Vector2*)	getSnapPointForX:	(GLfloat)x Y:(GLfloat)y onDistance:(GLfloat)dist;
- (Vector2*)	getSnapPointForX:	(GLfloat)x Y:(GLfloat)y GiveMax:(BOOL)max;
- (Vector2*)	getCellOffset:		(GLfloat)x Y:(GLfloat)y;


@end
