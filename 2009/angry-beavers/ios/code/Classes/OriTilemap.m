//=======================================================================================================================
//  File:		OriTilemap.m
//  Project:	heli
//	Desc:		Tilemap resource implementation
//
//  Created by Max Vasin on 5/11/11.
//  Copyright 2011 Origin. All rights reserved.
//=======================================================================================================================

// ***** IMPORTS *****

#import "OriTilemap.h"
#import "OriResourceManager.h"

// ***** IMPLEMENTATION *****

@implementation OriTilemap

// ****** PROPERTIES ******

// Synthesized
//-----------------------------------------------------------------------------------------------------------------------
@synthesize propTileset			= m_tileset;
@synthesize propSize			= m_size;
@synthesize propGridCellSide	= m_gridCellSide;
@synthesize propGridCellAspect	= m_gridCellAspect;
@synthesize propGridOffset		= m_gridOffset;
@synthesize propDrawSize		= m_drawSize;

// Static objects count
//-----------------------------------------------------------------------------------------------------------------------
- (oriUint)propStaticObjectsCount
{
	return [m_static count];
}

//-----------------------------------------------------------------------------------------------------------------------
// PROPERTY: tilemap linked grid size
//-----------------------------------------------------------------------------------------------------------------------

- (oriIntSize)propGridSize
{
	oriIntSize size = {m_cells.propDimX, m_cells.propDimY};
	
	return size;
}

//#######################################################################################################################
// CONSTRUCTION
//#######################################################################################################################

// Returns new tilemap resource with resource manager binding and name
//-----------------------------------------------------------------------------------------------------------------------
+ (id)tilemapWithManager:(OriResourceManager*)resman withName:(NSString*)name
{
	return [[[self alloc] initWithManager:resman withName:name] autorelease];
}

// Returns new tilemap resource with resource manager binding, name and XML data
//-----------------------------------------------------------------------------------------------------------------------
+ (id)tilemapWithManager:(OriResourceManager*)resman withName:(NSString*)name withXML:(OriXMLNode*)node
{
	return [[[self alloc] initWithManager:resman withName:name withXML:node] autorelease];
}

//#######################################################################################################################
// INITIALIZERS/DESTRUCTORS
//#######################################################################################################################

// Initializes new tilemap with resource manager and name
//-----------------------------------------------------------------------------------------------------------------------
- (id)initWithManager:(OriResourceManager*)resman withName:(NSString*)name
{
	if ( (self = [super initWithManager:resman withName:name]) )
	{
		m_nodes		= [[Array2D alloc] initWithDelegate:self];
		m_cells		= [[IntegerArray2D alloc] init];
		m_static	= [[NSMutableArray alloc] init];
		
		DBG_INIT_LOG_NAMED_0(self, m_name);
	}
	return self;
}

// Initializes new tilemap with resource manager, name and XML data
//-----------------------------------------------------------------------------------------------------------------------
- (id)initWithManager:(OriResourceManager*)resman withName:(NSString*)name withXML:(OriXMLNode*)node
{
	if ( (self = [self initWithManager:resman withName:name]) )
	{
		NSString* tilesetName = [node getFirstChildValue:@"Tileset" placeholder:nil];
		
		if (tilesetName)
		{
			m_tileset = [m_resMan getTileset:tilesetName];
			
			DBG_ASSERT_LOG2A(!m_tileset, @"OriTilemap: (%@) failed to get tileset (%@)", m_name, tilesetName);
		}
		else
		{
			DBGLOG1A(@"OriTilemap: (%@) no tileset name in XML file", m_name);
		}
		
		m_size.width	= [node getFirstChildValueInt:@"Width"	placeholder:0];
		m_size.height	= [node getFirstChildValueInt:@"Height" placeholder:0];
		
		DBG_ASSERT_LOG1A(!m_size.width || !m_size.height, @"OriTilemap: (%@) invalid map dimensions", m_name);
		
		[m_nodes resizeToX:m_size.width Y:m_size.height Create:NO];
		
		OriXMLNode* nodesRoot = [node getFirstChild:@"Tiles"];
		
		if (nodesRoot)
		{
			for (OriXMLNode* nodeNode in nodesRoot.propChildren)
			{
				OriTilemapNode* node = [[OriTilemapNode alloc] initWithTilemap:self withXML:nodeNode];
				
				oriIntPoint index = node.propNodeIndex;
				
				[m_nodes replaceAtX:index.x Y:index.y With:node];
				
				[node release];
			}
		}
		
		OriXMLNode* staticRoot = [node getFirstChild:@"StaticLayer"];
		
		if (staticRoot)
		{
			for (OriXMLNode* staticNode in staticRoot.propChildren)
			{
				OriTilemapStatic* wrapper = [[OriTilemapStatic alloc] initWithTilemap:self withXML:staticNode];
				
				[m_static addObject:wrapper];
				
				[wrapper release];
			}
		}
		
		OriXMLNode* gridRoot = [node getFirstChild:@"Grid"];
		
		if (gridRoot)
		{
			OriXMLNode* sizeNode = [gridRoot getFirstChild:@"Size"];
			
			if (sizeNode)
			{
				oriInt dimX = [sizeNode getFirstChildValueInt:@"W" placeholder:-1];
				oriInt dimY = [sizeNode getFirstChildValueInt:@"H" placeholder:-1];
				
				if ( (dimX > 0) && (dimY > 0) )
					[m_cells resizeToX:dimX Y:dimY];
			}
			
			OriXMLNode* cellSizeNode = [gridRoot getFirstChild:@"CellSize"];
			
			if (cellSizeNode)
			{
				m_gridCellSide		= [cellSizeNode getFirstChildValueInt:@"S" placeholder:0];
				m_gridCellAspect	= [cellSizeNode getFirstChildValueFloat:@"A" placeholder:0.0f];
			}
			
			OriXMLNode* offsetNode = [gridRoot getFirstChild:@"Offset"];
			
			if (offsetNode)
			{
				m_gridOffset.x		= [offsetNode getFirstChildValueInt:@"X" placeholder:0];
				m_gridOffset.y		= [offsetNode getFirstChildValueInt:@"Y" placeholder:0];
			}
						
			OriXMLNode* cellsNode = [gridRoot getFirstChild:@"Cells"];
			
			if (m_cells.propTotal && cellsNode)
			{
				for (OriXMLNode* cellNode in cellsNode.propChildren)
				{
					oriInt indX = [cellNode getFirstChildValueInt:@"X" placeholder:-1];
					oriInt indY = [cellNode getFirstChildValueInt:@"Y" placeholder:-1];
					oriInt type = [cellNode getFirstChildValueInt:@"Type" placeholder:-1];
					
					if ( (indX >= 0) && (indY >= 0) && (type >= 0) )
						[m_cells setValue:type AtX:indX Y:indY];
				}
			}
		}
		
		oriIntSize tileSize = m_tileset.propTileSize;
		
//		oriFloat w2 = (oriFloat)tileSize.width / 2.0f;
		oriFloat h2 = (oriFloat)tileSize.height / 2.0f;
		
		m_drawSize.width  = 0; // insert correct size;
		m_drawSize.height = (m_size.width + m_size.height) * h2;
	}
	return self;
}

// Destructor
//-----------------------------------------------------------------------------------------------------------------------
- (void)dealloc
{
	SAFE_RELEASE(m_tileset);
	SAFE_RELEASE(m_nodes);
	SAFE_RELEASE(m_static);
	SAFE_RELEASE(m_cells);
	
	DBG_DEALLOC_LOG_NAMED_0(self, m_name);
	
	[super dealloc];
}

//#######################################################################################################################
// DATA ACCESS
//#######################################################################################################################

// Returns node at X:Y index
//-----------------------------------------------------------------------------------------------------------------------
- (OriTilemapNode*)nodeAtX:(oriUint)x Y:(oriUint)y
{
	return [m_nodes getObjectAtX:x Y:y];
}

// Returns static object at index
//-----------------------------------------------------------------------------------------------------------------------
- (OriTilemapStatic*)staticAtIndex:(oriUint)index
{
	return [m_static objectAtIndex:index];
}

// Returns grid cell at index
//-----------------------------------------------------------------------------------------------------------------------
- (oriUint)cellAtX:(oriUint)x Y:(oriUint)y
{
	return [m_cells valueAtX:x Y:y];
}

//#######################################################################################################################
// Array2DDelegate PROTOCOL
//#######################################################################################################################

// Must return new instance of map node
//-----------------------------------------------------------------------------------------------------------------------
- (id)createInstanceOfArray:(Array2D*)array X:(oriUint)x Y:(oriUint)y DimX:(oriUint)dimX DimY:(oriUint)dimY
{
	return [OriTilemapNode nodeWithTilemap:self X:x Y:y];
}



@end
