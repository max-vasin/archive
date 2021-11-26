//===============================================================================================
//  File:		SubdivideTreeNode.m
//  Project:	OriginGameToolkit
//  Desc:		http://www.blackpawn.com/texts/lightmaps/default.html
//  Created by Max Vasin on 4/22/11.
//  Copyright 2011 Origin Games. All rights reserved.
//===============================================================================================  

// ***** IMPORTS *****

#import "SubdivideTreeNode.h"

// ***** IMPLEMENTATION *****

@implementation SubdivideTreeNode

// ***** PROPERTIES *****

// Node width
//-----------------------------------------------------------------------------------------------
- (void)setWidth:(unsigned int)width
{
	m_rect.right = m_rect.left + width - 1;
}

- (unsigned int)width
{
	return m_rect.right - m_rect.left + 1;
}

// Node height
//-----------------------------------------------------------------------------------------------
- (void)setHeight:(unsigned int)height
{
	m_rect.bottom = m_rect.top + height - 1;
}


- (unsigned int)height
{
	return m_rect.bottom - m_rect.top + 1;
}

// ***** INSTANCE METHODS *****

// Inserts image into tree (return node where image placed or nil if no space for image)
//-----------------------------------------------------------------------------------------------
- (SubdivideTreeNode*)insert:(ImageWrapper*)image
{
	if (m_nodeA && m_nodeB)
	{
		// we are branched
		SubdivideTreeNode* tryNode = [m_nodeA insert:image];
		if (tryNode)
			return tryNode;
		
		return [m_nodeB insert:image];
	}
	else
	{
		// we are leaf
		
		// we have image here
		if (m_image)
			return nil;
		
		unsigned int fit = [self canFit:image];
		
		// we too small
		if (!fit)
			return nil;
		
		// we perfectly fit
		if (fit == 2)
		{
			m_image = image;
			return self;
		}
		
		// need to divide self
		
		m_nodeA = [[SubdivideTreeNode alloc] init];
		m_nodeB = [[SubdivideTreeNode alloc] init];
		
		int dw = self.width - image->m_width;
		int dh = self.height - image->m_height;
		
		if (dw > dh)
		{
			m_nodeA->m_rect.left	= m_rect.left;
			m_nodeA->m_rect.top		= m_rect.top;
			m_nodeA->m_rect.right	= m_rect.left + image->m_width - 1;
			m_nodeA->m_rect.bottom	= m_rect.bottom;
			
			m_nodeB->m_rect.left	= m_rect.left + image->m_width;
			m_nodeB->m_rect.top		= m_rect.top;
			m_nodeB->m_rect.right	= m_rect.right;
			m_nodeB->m_rect.bottom	= m_rect.bottom;
		}
		else
		{
			m_nodeA->m_rect.left	= m_rect.left;
			m_nodeA->m_rect.top		= m_rect.top;
			m_nodeA->m_rect.right	= m_rect.right;
			m_nodeA->m_rect.bottom	= m_rect.top + image->m_height - 1;
			
			m_nodeB->m_rect.left	= m_rect.left;
			m_nodeB->m_rect.top		= m_rect.top + image->m_height;
			m_nodeB->m_rect.right	= m_rect.right;
			m_nodeB->m_rect.bottom	= m_rect.bottom;
		}
		
		return [m_nodeA insert:image];
	}
	
	return nil;
}

// Destructor
//-----------------------------------------------------------------------------------------------
- (void)dealloc
{
	SAFE_RELEASE(m_nodeA);
	SAFE_RELEASE(m_nodeB);
	
	[super dealloc];
}

// Returns 2 if image perfectly fit current node, 1 if image can be place in this node, 0 - no space
//-----------------------------------------------------------------------------------------------
- (unsigned int)canFit:(ImageWrapper*)image
{
	if ((image->m_width == self.width) && (image->m_height == self.height))
		return 2;
	
	if ((image->m_width <= self.width) && (image->m_height <= self.height))
		return 1;
	
	return 0;
}

@end
