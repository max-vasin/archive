//===============================================================================================
//  File:		NSBitmapImageRepExtension.m
//  Project:	OriginGameToolkit
//	Desc:		NSBitmapImageRep extension implementation
//
//  Created by Max Vasin on 12/16/10.
//  Copyright 2010 Origin. All rights reserved.
//===============================================================================================

// **** IMPORTS ****

#import "NSBitmapImageRepExtension.h"

// **** IMPLEMENTATION ****

@implementation NSBitmapImageRep (Extension)

// initWithDataAndConvert
// Calls initWithData initializer, checks colorspace and convert to RGB if needed
//-----------------------------------------------------------------------------------------------
- (id)initWithDataAndConvert:(NSData*)data
{
	if ( (self = [self initWithData:data]) )
	{
		// Our format is RGBA with 8 bit each channel and alpha value
		
		// Check bitmap parameters
		
		int bits = [self bitsPerPixel];
		
		if ( (bits == 32) && [self hasAlpha] )
			return self;
		
		// We need conversion, check for possibility
		
		if ((bits != 24) && (bits != 48) && (bits != 64))
		{
			[self release];
			return nil;
		}
		
		
		// Make new bitmap surface
		
		int width	= [self pixelsWide];
		int height	= [self pixelsHigh];
		
		NSBitmapImageRep* surface =  [[NSBitmapImageRep alloc] initWithBitmapDataPlanes:nil
																			 pixelsWide:width
																			 pixelsHigh:height
																		  bitsPerSample:8
																		samplesPerPixel:4
																			   hasAlpha:YES
																			   isPlanar:NO
																		 colorSpaceName:[self colorSpaceName]
																		   bitmapFormat:[self bitmapFormat]
																			bytesPerRow:(4 * width)
																			bitsPerPixel:32];
		
		// Obtain pointers
		
		byte* src   = [self bitmapData];
		byte* dst   = [surface bitmapData];
		
		// Perform conversion
		for (int i = 0; i < width * height; i++)
		{
			switch (bits) 
			{
				case 24:
					memcpy(&dst[i * 4], &src[i * 3], sizeof(byte) * 3);
					dst[i * 4 + 3] = 255; 
					break;
					
				case 48:
					// Strip channels to 8 bit
					dst[i * 4]		= ((word*)src)[i * 3]		>> 8;
					dst[i * 4 + 1]	= ((word*)src)[i * 3 + 1]	>> 8;
					dst[i * 4 + 2]	= ((word*)src)[i * 3 + 2]	>> 8;
					dst[i * 4 + 3]	= 255;
					break;
					
				case 64:
					// Strip channels to 8 bit
					dst[i * 4]		= ((word*)src)[i * 4]		>> 8;
					dst[i * 4 + 1]	= ((word*)src)[i * 4 + 1]	>> 8;
					dst[i * 4 + 2]	= ((word*)src)[i * 4 + 2]	>> 8;
					dst[i * 4 + 3]	= ((word*)src)[i * 4 + 3]	>> 8;
					break;
			}
		}
		
		[self release];
		self = surface;
	}
	return self;
}

// uploadToTexture
// Transfers bitmap data to OpenGL texture
//-----------------------------------------------------------------------------------------------

- (void)uploadToTexture:(GLuint)texid
{
	GLsizei			width, height;
    BOOL			hasAlpha;
    GLenum			format;
    unsigned char*	bitmapData;
    
    // Set memory alignment parameters for unpacking the bitmap.
    glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );
    
    // Specify the texture's properties.
    glBindTexture( GL_TEXTURE_2D, texid );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
    
    // Upload the texture bitmap.  (We assume it has power-of-2 dimensions.)
    width		= [self pixelsWide];
    height		= [self pixelsHigh];
    hasAlpha	= [self hasAlpha];
    bitmapData	= [self bitmapData];
    format		= hasAlpha ? GL_RGBA : GL_RGB;
    glTexImage2D( GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, bitmapData );
}

// Returns integer size of bitmap
//-----------------------------------------------------------------------------------------------
- (IntSize)intSize
{
	IntSize size;
	
	size.width	= [self pixelsWide];
	size.height = [self pixelsHigh];
	
	return size;
}

// Trimming unused image space
//-----------------------------------------------------------------------------------------------
- (id)trimSaveX:(uint*)px Y:(uint*)py
{
	uint minx, miny;
	
	IntSize newSize = [self trimSizeToX:&minx Y:&miny];
	
	if (newSize.width != [self pixelsWide] || newSize.height != [self pixelsHigh])
	{
		NSBitmapImageRep* trimmed = [self trimToSize:newSize X:minx Y:miny];
		
		[self release];
		
		self = trimmed;
		
		if (px)
			*px = minx;
		
		if (py)
			*py = miny;
	}
	
	return self;
}

// Trims image to new size with start point
//-----------------------------------------------------------------------------------------------
- (id)trimToSize:(IntSize)size X:(uint)minx Y:(uint)miny
{

	NSBitmapImageRep* surface =  [[NSBitmapImageRep alloc] initWithBitmapDataPlanes:nil
																		 pixelsWide:size.width
																		 pixelsHigh:size.height
																	  bitsPerSample:8
																	samplesPerPixel:4
																		   hasAlpha:YES
																		   isPlanar:NO
																	 colorSpaceName:[self colorSpaceName]
																	   bitmapFormat:[self bitmapFormat]
																		bytesPerRow:(4 * size.width)
																	   bitsPerPixel:32];
	
	
	

	byte*	dst   = [surface bitmapData];
	byte*	src   = [self bitmapData];
	uint	width = [self pixelsWide];
	
	for (uint y = 0; y < size.height; y++)
		for (uint x = 0; x < size.width; x++)
			memcpy(&dst[(y * size.width + x) * 4], &src[((y + miny) * width + (x + minx)) * 4], sizeof(byte) * 4);
	
	return surface;
}

// Returns size of trimmed image
//-----------------------------------------------------------------------------------------------
- (IntSize)trimSizeToX:(uint*)px Y:(uint*)py;
{
	IntSize currentSize = {[self pixelsWide], [self pixelsHigh]};
	
	int bits = [self bitsPerPixel];
	
	if (bits != 32 || ![self hasAlpha])
		return currentSize;
	
	
	int width	= [self pixelsWide];
	int height	= [self pixelsHigh];
	
	byte* src   = [self bitmapData];
	
	int x, y;
	
	int minx = width;
	int miny = height;
	int maxx = 0;
	int maxy = 0;
	
	for (y = 0; y < height; y++)
		for (x = 0; x < width; x++)
			if (src[(y * width + x) * 4 + 3])
			{
				if (x < minx)
					minx = x;
				if (y < miny)
					miny = y;
			}
	
	for (y = (height - 1); y >= 0; y--)
		for (x = (width - 1); x >= 0; x--)
			if (src[(y * width + x) * 4 + 3])
			{
				if (x > maxx)
					maxx = x;
				if (y > maxy)
					maxy = y;
			}
	
	currentSize.width	= maxx - minx;
	currentSize.height	= maxy - miny;
	
	if (px)
		*px = minx;
	
	if (py)
		*py = miny;
	
	return currentSize;
}

// Area sort selector
//-----------------------------------------------------------------------------------------------
- (NSInteger)areaSort:(NSBitmapImageRep*)image
{
	int areaA = [self pixelsHigh] * [self pixelsWide];
	int areaB = [image pixelsHigh] * [image pixelsWide];
	
	if (areaA < areaB)
		return NSOrderedAscending;
	
	if (areaA == areaB)
		return NSOrderedSame;
	
	return NSOrderedDescending;
}

// Monochromic filter
//-----------------------------------------------------------------------------------------------

- (void)applyMonochrome
{
	// Obtain data
	
	int bits	= [self bitsPerPixel];
	int	width	= [self pixelsWide];
	int	height	= [self pixelsHigh];

	byte* data   = [self bitmapData];
	byte  r, g, b, mono;
	
	// Only 32 || 24 bit applies now
	
	for (int i = 0; i < width * height; i++)
	{
		switch (bits) 
		{
			case 24:
			case 32:
				r = data[i * 4 + 0];
				g = data[i * 4 + 1];
				b = data[i * 4 + 2];
				
				mono = (byte)(((float)r + (float)g + (float)b) / 768 * 256);
				
				data[i * 4 + 0] = mono;
				data[i * 4 + 1] = mono;
				data[i * 4 + 2] = mono;
				break;
		
		}
	}
}

// Inverse color filter
//-----------------------------------------------------------------------------------------------

- (void)applyInverse
{
	// Obtain data
	
	int bits	= [self bitsPerPixel];
	int	width	= [self pixelsWide];
	int	height	= [self pixelsHigh];
	
	byte* data   = [self bitmapData];
	
	// Only 32 || 24 bit applies now
	
	for (int i = 0; i < width * height; i++)
	{
		switch (bits) 
		{
			case 24:
			case 32:
				data[i * 4 + 0] = 255 - data[i * 4 + 0];
				data[i * 4 + 1] = 255 - data[i * 4 + 1];
				data[i * 4 + 2] = 255 - data[i * 4 + 2];
				break;
				
		}
	}
}

//-----------------------------------------------------------------------------------------------
// Set transparent color
//-----------------------------------------------------------------------------------------------

- (void)applyTransparent:(NSColor*)color
{
	// Obtain data
	
	int bits	= [self bitsPerPixel];
	int	width	= [self pixelsWide];
	int	height	= [self pixelsHigh];
	
	byte* data   = [self bitmapData];
	
	byte r = (byte)[color redComponent] * 255;
	byte g = (byte)[color greenComponent] * 255;
	byte b = (byte)[color blueComponent] * 255;
	
	
	// Only 32 bit applies now
	
	for (int i = 0; i < width * height; i++)
	{
		switch (bits) 
		{
			case 32:
				if ( (data[i * 4 + 0] == r) && (data[i * 4 + 1] == g) && (data[i * 4 + 2] == b) )
				{
					data[i * 4 + 3] = 0;
				}
				break;
		}
	}
}


@end
