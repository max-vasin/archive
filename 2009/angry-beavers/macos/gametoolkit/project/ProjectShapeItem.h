//===============================================================================================
//  File:		ProjectShapeItem.h
//  Project:	OriginGameToolkit
//	Desc:		Project shape item container class interface
//
//  Created by Max Vasin on 11/24/10.
//  Copyright 2010 Origin. All rights reserved.
//===============================================================================================

// **** IMPORTS ****

#import <Cocoa/Cocoa.h>
#import "BaseObject.h"
#import "Vector2.h"
#import "ProjectItem.h"

// **** SHAPE VERTEX INTERFACE ****

@interface ShapeVertex : BaseObject
{
@public
	Vector2*	m_pos;
}

+ (id)		makeWithX:		(GLfloat)x Y:(GLfloat)y;

- (id)		initFromXML:	(NSXMLElement*)node;
- (void)	saveXML:		(NSXMLElement*)node;

@end

// **** SHAPE ITEM INTERFACE ****

@interface ProjectShapeItem : ProjectItem 
{
	NSMutableArray*		m_vertices;
}

@property (copy,readonly)	NSArray*	propVertices;
@property (readonly)		int			propVerticesCount;
@property (readonly)		NSRect		propBoundRect;


- (ShapeVertex*)	addVertexAtX:		(GLfloat)x Y:(GLfloat)y afterVertex:(ShapeVertex*)vertex;
- (void)			deleteVertex:		(ShapeVertex*)vertex;
- (void)			cleanVertices;
- (ShapeVertex*)	getNextVertex:		(ShapeVertex*)vertex;
- (ShapeVertex*)	getVertexAroundX:	(GLfloat)x Y:(GLfloat)y onSquare:(GLfloat)side; 
- (NSArray*)		getLineAroundX:		(GLfloat)x Y:(GLfloat)y onDistance:(GLfloat)dist;


@end
