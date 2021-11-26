//===============================================================================================
//  File:		SubdivideTreeNode.h
//  Project:	OriginGameToolkit
//	Desc:		2D subdivision binary tree node
//
//  Created by Max Vasin on 4/22/11.
//  Copyright 2011 Origin Games. All rights reserved.
//===============================================================================================

// ***** IMPORTS *****

#import <Foundation/Foundation.h>
#import "BaseObject.h"
#import "ImageWrapper.h"

// Tree node rectange structure

typedef struct
{
	unsigned int top;
	unsigned int left;
	unsigned int right;
	unsigned int bottom;
}SudivideRect;

// ***** INTERFACE *****

@interface SubdivideTreeNode : BaseObject 
{
@public
	SubdivideTreeNode*		m_nodeA;
	SubdivideTreeNode*		m_nodeB;
	SudivideRect			m_rect;
	ImageWrapper*			m_image;
}

// ***** PROPERTIES *****

@property (nonatomic, assign)	unsigned int width;
@property (nonatomic, assign)	unsigned int height;

// ***** INSTANCE METHODS *****

- (SubdivideTreeNode*)	insert:(ImageWrapper*)image;
- (unsigned int)		canFit:(ImageWrapper*)image;

@end
