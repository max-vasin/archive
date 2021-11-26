//===============================================================================================
//  File:		ProjectShapeItem.m
//  Project:	OriginGameToolkit
//	Desc:		Project shape item container class implementation
//
//  Created by Max Vasin on 11/24/10.
//  Copyright 2010 Origin. All rights reserved.
//===============================================================================================

// **** IMPORTS ****

#import "ProjectShapeItem.h"
#import "SharedResourceManager.h"

// **** SHAPE VERTEX IMPLEMENTATION ****

@implementation ShapeVertex

// makeWithX:Y 
// Makes new vertex with given position
//-----------------------------------------------------------------------------------------------

+ (id)makeWithX:(GLfloat)x	Y:(GLfloat)y
{
	ShapeVertex* newVertex = [[ShapeVertex alloc] init];
	
	newVertex->m_pos->x	= x;
	newVertex->m_pos->y	= y;
	
	return [newVertex autorelease];
}

// init
// Default initializer
//-----------------------------------------------------------------------------------------------

- (id)init
{
	if ((self = [super init]))
	{
		m_pos = [[Vector2 alloc] init];
	}
	return self;
}

// initFromXML 
// XML shape vertex initializer
//-----------------------------------------------------------------------------------------------

- (id)initFromXML:(NSXMLElement*)node
{
	if ((self = [super init]))
	{
		m_pos = [[Vector2 alloc] initWithX:[node getFirstChildFloat:@"X"]
										 Y:[node getFirstChildFloat:@"Y"]];
	}
	
	return self;
}

// saveXML
// XML shape vertex archiver
//-----------------------------------------------------------------------------------------------

- (void)saveXML:(NSXMLElement*)node
{
	[node addNewChild:@"X"	withFloat:m_pos->x];
	[node addNewChild:@"Y"	withFloat:m_pos->y];
}

// dealloc
// Desctuctor
//-----------------------------------------------------------------------------------------------

- (void)dealloc
{
	SAFE_RELEASE(m_pos);
	
	[super dealloc];
}

@end

// **** PROJECT SHAPE ITEM IMPLEMENTATION ****

@implementation ProjectShapeItem

// **** PROJECT SHAPE SYNTHESIZED PROPERTIES ****

@synthesize	propVertices	= m_vertices;

- (int)propVerticesCount
{
	return [m_vertices count];
}

// propBoundRect
// Returns current shape bounding rectangle
//-----------------------------------------------------------------------------------------------

- (NSRect)propBoundRect
{
	NSRect bounds = NSZeroRect;
	
	if (![m_vertices count])
		return bounds;
	
	ShapeVertex* v = [m_vertices objectAtIndex:0];
	
	NSPoint min = {v->m_pos->x, v->m_pos->y}, max = {v->m_pos->x, v->m_pos->y};
	
	for (ShapeVertex* v in m_vertices)
	{
		if (min.x > v->m_pos->x)
			min.x = v->m_pos->x;
		if (max.x < v->m_pos->x)
			max.x = v->m_pos->x;
		if (min.y > v->m_pos->y)
			min.y = v->m_pos->y;
		if (max.y < v->m_pos->y)
			max.y = v->m_pos->y;
	}
	
	bounds.origin.x    = min.x;
	bounds.origin.y    = min.y;
	bounds.size.width  = max.x - min.x;
	bounds.size.height = max.y - min.y;
	
	return bounds;
}


// **** INSTANCE METHODS IMPLEMENTATION ****

// Overriding (ProjectItem) initializer
//-----------------------------------------------------------------------------------------------

- (id)initWithParent:(ProjectItem*)parent
{
	if ((self = [super initWithParent:parent]))
	{
		m_vertices = [[NSMutableArray alloc] init];
	}
	return self;
}

// Overriding (ProjectItem) initializer
//-----------------------------------------------------------------------------------------------
- (id)initWithParent:(ProjectItem*)parent fromXML:(NSXMLElement*)node
{
	if ( (self = [super initWithParent:parent]) )
	{
		m_vertices = [[NSMutableArray alloc] init];
		
		NSXMLElement* vertRoot = [node getFirstChild:@"Vertices"];
		
		if (vertRoot)
			for (NSXMLElement* vertNode in vertRoot.children)
				[m_vertices addObject:[[[ShapeVertex alloc] initFromXML:vertNode] autorelease]];

	}
	return self;
}

// dealloc 
// Destructor
//-----------------------------------------------------------------------------------------------

- (void)dealloc
{
	SAFE_RELEASE(m_vertices);
	
	[super dealloc];
}

// Sets item icon
//-----------------------------------------------------------------------------------------------
- (void)setIcon
{
	m_itemImage = [SharedResourceManager sharedIcon:@"shape"];
}

// SaveAsXML
// Makes XML representation fo current item and attach it to node
//-----------------------------------------------------------------------------------------------

- (NSXMLElement*)saveAsXML:(NSXMLElement*)node
{
	NSXMLElement* thisNode = [node addNewChild:@"Shape"];
	
	[thisNode addNewChild:@"Name"	withValue:m_itemName];
	
	NSXMLElement* vertRoot = [thisNode addNewChild:@"Vertices"];
	
	for (ShapeVertex* vertex in m_vertices)
	{
		NSXMLElement* vertNode = [vertRoot addNewChild:@"Vertex"];
		
		[vertex saveXML:vertNode];
	}
	
	return thisNode;
}

// addVertexAtX:Y
// Add new vertex to shape with given coordinates
//-----------------------------------------------------------------------------------------------

- (ShapeVertex*)addVertexAtX:(GLfloat)x Y:(GLfloat)y afterVertex:(ShapeVertex*)vertex
{
	ShapeVertex* newVertex = [ShapeVertex makeWithX:x Y:y];
	
	if (!vertex)
	{
		[m_vertices addObject:newVertex];
	}
	else
	{
		int index = [m_vertices indexOfObject:vertex];
		[m_vertices insertObject:newVertex atIndex:index + 1];
	}
	
	return newVertex;
}

// deleteVertex
// Deletes vertex
//-----------------------------------------------------------------------------------------------

- (void)deleteVertex:(ShapeVertex*)vertex
{
	[m_vertices removeObject:vertex];
}

// cleanVertices
// Clean vertices
//-----------------------------------------------------------------------------------------------

- (void)cleanVertices
{
	[m_vertices removeAllObjects];
}

// getNextVertex
// Returns next to given vertex of shape
//-----------------------------------------------------------------------------------------------

- (ShapeVertex*)getNextVertex:(ShapeVertex*)vertex
{
	int index = [m_vertices indexOfObject:vertex];
	
	if (index == ([m_vertices count] - 1))
		return [m_vertices objectAtIndex:0];
	
	return [m_vertices objectAtIndex:index + 1];
}

// getVertexAroundX:Y:onSquare
// Detect vertex around some point within square size
//-----------------------------------------------------------------------------------------------
	 
- (ShapeVertex*)getVertexAroundX:(GLfloat)x Y:(GLfloat)y onSquare:(GLfloat)side
{
	if (!side)
		return nil;
	
	for (ShapeVertex* vertex in m_vertices)
	{
		if ( (vertex->m_pos->x >= (x - side / 2.0f)) &&
			 (vertex->m_pos->x <= (x + side / 2.0f)) &&
			 (vertex->m_pos->y >= (y - side / 2.0f)) &&
			 (vertex->m_pos->y <= (y + side / 2.0f)) )
			return vertex;
	}
	
	return nil;
}

// getLineAroundX:Y:OnDistance
// Detect two vertices wich represents line nearest to XY on distance
//-----------------------------------------------------------------------------------------------

- (NSArray*)getLineAroundX:(GLfloat)x Y:(GLfloat)y onDistance:(GLfloat)dist
{
	if ([m_vertices count] < 2)
		return nil;
	
	NSArray* ret = nil;
	
	for (int i = 1; i <= [m_vertices count]; i++)
	{
		ShapeVertex* vA = [m_vertices objectAtIndex:i-1];
		ShapeVertex* vB = [m_vertices objectAtIndex:(i == [m_vertices count]) ? 0 : i];
		
		Vector2* a	= [[Vector2 alloc] initWithPointsStart:vA->m_pos End:vB->m_pos];
		Vector2* b  = [[Vector2 alloc] initWithX:x-vA->m_pos->x Y:y-vA->m_pos->y];
		
		GLfloat proj = [a dotWith:b];
		
		if ( (proj > 0.0f) && (proj <= [a propLengthSq]) )
		{
			proj /= [b propLengthSq];
			
			Vector2* c  = [b copy];
			
			[c makeScaleTo:proj];
			
			if ([c propLength] <= [a propLength])
			{
				
				Vector2* n  = [[Vector2 alloc] initWithPointsStart:a End:c];
				
				if ([n propLength] <= dist)
					ret = [[NSArray alloc] initWithObjects:vA, vB, nil];
			
				SAFE_RELEASE(n);
			}
		}
		SAFE_RELEASE(a);
		SAFE_RELEASE(b);
		
		if (ret)
			return ret;
	}
	
	return nil;
}



@end
