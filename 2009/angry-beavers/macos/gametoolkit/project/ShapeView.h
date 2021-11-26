//===============================================================================================
//  File:		ShapeView.h
//  Project:	OriginGameToolkit
//	Desc:		Shape OpenGL view subclass
//
//  Created by Max Vasin on 12/21/10.
//  Copyright 2010 Origin. All rights reserved.
//===============================================================================================

// ************* IMPORTS *************

#import <Cocoa/Cocoa.h>
#import "BaseObject.h"
#import "GuidlineGrid.h"
#import "ApplicationPreferences.h"
#import "ProjectShapeItem.h"

// ************* SHAPE VIEW STATE ENUMERATION *************

typedef enum
{
	eShapeViewStateNop	= 0,
	eShapeViewStateEdit	= 1,
	eShapeViewStateMove	= 2
}enShapeViewState;

// ************* SHAPE VIEW INTERFACE *************

@interface ShapeView : NSOpenGLView 
{
	BOOL				m_contextReady;
	GuidlineGrid*		m_backGrid;
	
	// Properties containers
	
	NSColor*			m_backColor;
	NSColor*			m_shapeColor;
	NSColor*			m_shapeClosingColor;
	NSColor*			m_shapeVertexColor;
	NSColor*			m_shapeSelectLineColor;
	NSColor*			m_shapeSelectVertexColor;
	GLfloat				m_shapeWidth;
	GLfloat				m_shapeVertexSize;
	GLfloat				m_offsetX;
	GLfloat				m_offsetY;
	GLfloat				m_offsetXMin;
	GLfloat				m_offsetXMax;
	GLfloat				m_offsetYMin;
	GLfloat				m_offsetYMax;
	
	// Current view state
	
	ProjectShapeItem*	m_itemCurrent;
	OGLColorVertex*		m_shapeVertices;
	OGLColorVertex*		m_shapePoints;
	OGLColorVertex		m_closeVertices[3];
	OGLColorVertex		m_selLineVertices[2];
	OGLColorVertex		m_snapPoint;
	BOOL				m_snapped;
	BOOL				m_snapEnabled;
	BOOL				m_selLineExist;
	BOOL				m_selLineMode;
	NSArray*			m_selLineShapeVertices;
	int					m_shapeVertexCount;
	NSTrackingArea*		m_trackArea;
	enShapeViewState	m_state;
	ShapeVertex*		m_lastVertex;
	NSRect				m_lastFrameRect;
	
	// IB outlets
	
	IBOutlet ApplicationPreferences*	m_appPrefs;
	IBOutlet NSTabView*					m_viewsTab;
}

// ************* SHAPE VIEW PROPERTIES *************

@property (assign)	ProjectShapeItem*	propCurrentItem;		// Current project shape item
@property (copy)	NSColor*			propBackColor;			// PREFS: view background color
@property (copy)	NSColor*			propShapeColor;			// PREFS: shape lines color
@property (copy)	NSColor*			propClosingLineColor;	// PREFS: closing lines color
@property (copy)	NSColor*			propVertexColor;		// PREFS: shape vertex color
@property (copy)	NSColor*			propSelectLineColor;	// PREFS: shape selected line color 
@property (copy)	NSColor*			propSelectVertexColor;	// PREFS: shape selected vertex color
@property (assign)	GLfloat				propShapeWidth;			// PREFS: shape line width
@property (assign)	GLfloat				propVertexSize;			// PREFS: shape vertex point size
@property (assign)	GLfloat				propViewOffsetX;		// CTRL: view X-offset
@property (assign)	GLfloat				propViewOffsetY;		// CTRL: view Y-offset
@property (assign)	GLfloat				propViewOffsetXMin;		// CTRL: view min X-offset border
@property (assign)	GLfloat				propViewOffsetXMax;		// CTRL: view max X-offset border
@property (assign)	GLfloat				propViewOffsetYMin;		// CTRL: view min Y-offset border
@property (assign)	GLfloat				propViewOffsetYMax;		// CTRL: view max Y-offset border
@property (assign)	BOOL				propIsEditState;		// CTRL: get/set edit state of view
@property (assign)	BOOL				propIsMoveState;		// CTRL: get/set move state of view
@property (assign)	BOOL				propSnapEnabled;		// CTRL: snapping to guid grid


// ************* SHAPE VIEW ACTIONS *************

- (IBAction)	resetOffsets:			(id)sender;

@end
