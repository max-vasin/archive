//===============================================================================================
//  File:		ImageWrapper.m
//  Project:	OriginGameToolkit
//	Desc:		Image wrapper class implementation
//
//  Created by Max Vasin on 4/22/11.
//  Copyright 2011 Origin Games. All rights reserved.
//===============================================================================================

// ***** IMPORTS *****

#import "ImageWrapper.h"

// ***** IMPLEMENTATION *****

@implementation ImageWrapper

// Synthesized
//-----------------------------------------------------------------------------------------------

// Left image rectangle
//-----------------------------------------------------------------------------------------------
- (void)setLeft:(unsigned int)left
{
	m_posX = left;
}

- (unsigned int)left
{
	return m_posX;
}

// Top image rectangle
//-----------------------------------------------------------------------------------------------
- (void)setTop:(unsigned int)top
{
	m_posY = top;
}

- (unsigned int)top
{
	return m_posY;
}

// Right image rectangle
//-----------------------------------------------------------------------------------------------
- (void)setRight:(unsigned int)right
{
	m_posX = right - m_width;
}

- (unsigned int)right
{
	return m_posX + m_width;
}

// Bottom image rectangle
//-----------------------------------------------------------------------------------------------
- (void)setBottom:(unsigned int)bottom
{
	m_posY = bottom - m_height;
}

- (unsigned int)bottom
{
	return m_posY + m_height;
}

// ***** INSTANCE METHODS *****

// Initializes new wrapper with image
//-----------------------------------------------------------------------------------------------
- (id)initWithImage:(NSBitmapImageRep*)image andAssemblyItem:(TextureAssemblyItem*)item
{
	if ( (self = [super init]) )
	{
		m_image		= [image retain];
		m_assembly	= item;
		m_width		= [image pixelsWide];
		m_height	= [image pixelsHigh];
		m_area		= m_width * m_height;
	}
	return self;
}

// Destructor
//-----------------------------------------------------------------------------------------------
- (void)dealloc
{
	SAFE_RELEASE(m_image);
	
	[super dealloc];
}

// Area sort selector
//-----------------------------------------------------------------------------------------------
- (NSInteger)areaSort:(ImageWrapper*)image
{
	if (m_area < image->m_area)
		return NSOrderedDescending;
	
	if (m_area == image->m_area)
		return NSOrderedSame;
	
	return NSOrderedAscending;
}
@end
