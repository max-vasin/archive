//===============================================================================================
//  File:		ImageWrapper.h
//  Project:	OriginGameToolkit
//	Desc:		Image wrapper class
//
//  Created by Max Vasin on 4/22/11.
//  Copyright 2011 Origin Games. All rights reserved.
//===============================================================================================

// ***** IMPORTS *****

#import <Foundation/Foundation.h>
#import "BaseObject.h"

// ***** INTERFACE *****

@class TextureAssemblyItem;

@interface ImageWrapper : BaseObject 
{
@public
	NSBitmapImageRep*		m_image;
	TextureAssemblyItem*	m_assembly;
	unsigned int			m_area;
	unsigned int			m_width;
	unsigned int			m_height;
	unsigned int			m_posX;
	unsigned int			m_posY;
	uint					m_index;
	uint					m_trimX;
	uint					m_trimY;
}

// ***** PROPERTIES *****

@property (nonatomic, assign)	unsigned int left;
@property (nonatomic, assign)	unsigned int right;
@property (nonatomic, assign)	unsigned int top;
@property (nonatomic, assign)	unsigned int bottom;

// ***** INSTANCE METHODS *****

- (id)			initWithImage:(NSBitmapImageRep*)image andAssemblyItem:(TextureAssemblyItem*)item;
- (NSInteger)	areaSort:(ImageWrapper*)image;

@end
