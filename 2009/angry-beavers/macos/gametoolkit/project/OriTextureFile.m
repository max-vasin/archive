//
//  OriTextureFile.m
//  OriginGameToolkit
//
//  Created by Max Vasin on 6/10/11.
//  Copyright 2011 Origin Games. All rights reserved.
//

#import "OriTextureFile.h"


@implementation OriTextureFile

- (unsigned int)width
{
	return m_header.h_width;
}

- (unsigned int)height
{
	return m_header.h_height;
}

- (IntSize)size
{
	IntSize size;
	size.width	= (int)m_header.h_width;
	size.height = (int)m_header.h_height;
	
	return size;
}

- (GLuint)texture
{
	return m_texture;
}



- (id)initWithPath:(NSString*)path
{
	if ( (self = [self init]) )
	{
		FILE* file = fopen([path cStringUsingEncoding:NSASCIIStringEncoding], "rb");
		
		if (!file)
		{
			[self release];
			return nil;
		}
		
		if (fread(&m_header, sizeof(oriTextureFileHeader), 1, file) < 1)
		{
			fclose(file);
			[self release];
			return nil;
		}
		
		void* data = calloc(m_header.h_size, 1);
		
		if (fread(data, m_header.h_size, 1, file) < 1)
		{
			free(data);
			fclose(file);
			[self release];
			return nil;
		}
		
		glGenTextures(1, &m_texture);
		glBindTexture(GL_TEXTURE_2D, m_texture);
		glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
		
		GLenum			format, internalformat;
		
		switch (m_header.h_format)
		{
			case eTextureEngineFormatByteRGBA:
				format			= GL_RGBA;
				internalformat	= GL_UNSIGNED_BYTE;
				break;
			case eTextureEngineFormatShortRGBA4444:
				format			= GL_RGBA;
				internalformat	= GL_UNSIGNED_SHORT_4_4_4_4;
				break;
			case eTextureEngineFormatShortRGBA5551:
				format			= GL_RGBA;
				internalformat	= GL_UNSIGNED_SHORT_5_5_5_1;
				break;
			case eTextureEngineFormatShortRGB565:
				format			= GL_RGB;
				internalformat	= GL_UNSIGNED_SHORT_5_6_5;
				break;
			case eTextureEngineFormatByteA:
				format			= GL_ALPHA;
				internalformat	= GL_UNSIGNED_BYTE;
				break;
			case eTextureEngineFormatByteL:
				format			= GL_LUMINANCE;
				internalformat	= GL_UNSIGNED_BYTE;
				break;
			case eTextureEngineFormatByteLA:
				format			= GL_LUMINANCE_ALPHA;
				internalformat	= GL_UNSIGNED_BYTE;
				break;
		}
		
		glTexImage2D( GL_TEXTURE_2D, 0, format, m_header.h_width, m_header.h_height, 0, format, internalformat, data );
		
		free(data);
		fclose(file);
	}
	return self;
}

- (void)dealloc
{
	glDeleteTextures(1, &m_texture);
	
	[super dealloc];
}


@end
