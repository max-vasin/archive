//=======================================================================================================================
//  File:		OriTexture.m
//  Project:	heli
//	Desc:		texture resource implementation
//
//  Created by Max Vasin on 3/21/11.
//  Copyright 2011 Origin. All rights reserved.
//=======================================================================================================================

// ***** IMPORTS *****

#import "OriTexture.h"
#import "OriResourceManager.h"

// ******** ORITEX TEXTURE FILE DATA FORMAT ********

typedef enum
{
	eOriTexFormatByteRGBA		= 0,
	eOriTexFormatShortRGBA4444	= 1,
	eOriTexFormatShortRGBA5551	= 2,
	eOriTexFormatShortRGB565	= 3,
	eOriTexFormatByteA			= 4,
	eOriTexFormatByteL			= 5,
	eOriTexFormatByteLA			= 6
	
}enOriTexFormat;

// ******** ORITEX TEXTURE FILE HEADER ********

typedef struct
{
	
	unsigned int	h_magic;
	unsigned int	h_width;
	unsigned int	h_height;
	unsigned int	h_size;
	enOriTexFormat	h_format;
	
}oriTexFileHeader;


// ***** PRIVATE METHODS *****

@interface OriTexture (Private)

- (BOOL)loadTexture;
- (BOOL)loadTexturePNG;
- (BOOL)loadTextureOriTex;

@end

// ***** IMPLEMENTATION *****

@implementation OriTexture

@synthesize propSource	= m_source;
@synthesize propHandle	= m_texture;

// ****** PROPERTIES ******

// Texture size
//-----------------------------------------------------------------------------------------------------------------------
- (oriIntSize)propSize
{
	return m_size;
}

// (OriResource overriding) system memory used by this resource
//-----------------------------------------------------------------------------------------------------------------------
- (oriUint)propSysMemUsed
{
	return 0;
}

// (OriResource overriding) video memory used by thid resource
//-----------------------------------------------------------------------------------------------------------------------
- (oriUint)propVideoMemUsed
{
	return m_size.width * m_size.height * 4 * sizeof(GLubyte);
}


// ****** CLASS METHODS ******

//#######################################################################################################################
// CONSTRUCTION
//#######################################################################################################################

// Returns new autoreleased instance of texture with resource manager and name provided
//-----------------------------------------------------------------------------------------------------------------------
+ (id)textureWithManager:(OriResourceManager*)resman withName:(NSString*)name
{
	return [[[self alloc] initWithManager:resman withName:name] autorelease];
}

// Returns new autoreleased instance of texture with resource manager and name provided, when try to load texture
//-----------------------------------------------------------------------------------------------------------------------
+ (id)textureWithManager:(OriResourceManager*)resman withName:(NSString*)name fromFile:(NSString*)file
{
	return [[[self alloc] initWithManager:resman withName:name fromFile:file] autorelease];
}

// ****** INSTANCE METHODS ******

//#######################################################################################################################
// INITIALIZERS/DESTRUCTORS
//#######################################################################################################################

// Initializes new instance with resource manager and name
//-----------------------------------------------------------------------------------------------------------------------
- (id)initWithManager:(OriResourceManager*)resman withName:(NSString*)name
{
	if ( (self = [super initWithManager:resman withName:name]) )
	{
		DBG_INIT_LOG_NAMED_0(self, m_name);
	}
	return self;
}

// Initializes new instance with resource manager and name, when loads texture from file
//-----------------------------------------------------------------------------------------------------------------------
- (id)initWithManager:(OriResourceManager*)resman withName:(NSString*)name fromFile:(NSString*)file
{
	if ( (self = [self initWithManager:resman withName:name]) )
	{
		self.propSource = file;
		
		if (![self loadTexture])
		{
			[self release];
			return nil;
		}
	}
	return self;
}

// Destructor
//-----------------------------------------------------------------------------------------------------------------------
- (void)dealloc
{
	[m_resMan.propRenderer destroyTexture:m_texture];
	
	SAFE_RELEASE(m_source);
	
	DBG_DEALLOC_LOG_NAMED_0(self, m_name);
	
	[super dealloc];
}


//#######################################################################################################################
// PRIVATE METHODS
//#######################################################################################################################

// Loads texture from file
//-----------------------------------------------------------------------------------------------------------------------
- (BOOL)loadTexture
{
	// TODO: another formats of textures here
	
	NSRange range = [m_source rangeOfString:@".png" options:NSCaseInsensitiveSearch];
	
	if (range.location != NSNotFound)
		return [self loadTexturePNG];
	
	range = [m_source rangeOfString:@".oritex" options:NSCaseInsensitiveSearch];
	
	if (range.location != NSNotFound)
		return [self loadTextureOriTex];
	
	return [self loadTexturePNG];
}

// Loads a PNG texture from file
//-----------------------------------------------------------------------------------------------------------------------
- (BOOL)loadTexturePNG
{
	UIImage* image = [[UIImage alloc] initWithContentsOfFile:m_source];
	
	if (!image)
	{
		DBGLOG1A(@"OriTexture: failed to create image with file (%@)", m_source);
		return NO;
	}
	
	CGImageRef	source			= image.CGImage;
	
	size_t		width			= CGImageGetWidth(source);
	size_t		height			= CGImageGetHeight(source);
	
	GLubyte* buffer		= (GLubyte*)calloc(width * height * 4, sizeof(GLubyte));
	CGContextRef bmp	= CGBitmapContextCreate(buffer, width, height, 8, width * 4, CGImageGetColorSpace(source), kCGImageAlphaPremultipliedLast);

	CGContextDrawImage(bmp, CGRectMake(0.0, 0.0, (CGFloat)width, (CGFloat)height), source);
	CGContextRelease(bmp);

	m_texture = [m_resMan.propRenderer createTextureW:width H:height Data:buffer Format:GL_RGBA Packing:GL_UNSIGNED_BYTE];
	
	SAFE_FREE(buffer);
	SAFE_RELEASE(image);
	
	if (!m_texture)
	{
		DBGLOG1A(@"OriTexture: failed to load texture from file (%@)", m_source);
		return NO;
	}
	
	m_size.width	= width;
	m_size.height	= height;
	
	return YES;
}

//-----------------------------------------------------------------------------------------------------------------------
// Loads a ORITEX texture from file
//-----------------------------------------------------------------------------------------------------------------------

- (BOOL)loadTextureOriTex
{
	FILE* file = fopen([m_source cStringUsingEncoding:NSASCIIStringEncoding], "rb");
	
	if (!file)
		return NO;
	
	oriTexFileHeader header;
	
	if (fread(&header, sizeof(oriTexFileHeader), 1, file) < 1)
	{
		DBGLOG1A(@"OriTexture: (%@) cant read file header - corrupter file?", m_name);
		fclose(file);
		return NO;
	}
	
	oriUint magic = 'O' << 4 | 'P';
	
	if (header.h_magic != magic)
	{
		DBGLOG1A(@"OriTexture: (%@) file magic number mismatch!", m_name);
		fclose(file);
		return NO;
	}
	
	if (!header.h_size)
	{
		DBGLOG1A(@"OriTexture: (%@) invalid file data size!", m_name);
		fclose(file);
		return NO;
	}
	
	void* data = calloc(header.h_size, 1);
	
	if (!data)
	{
		DBGLOG1A(@"OriTexture: (%@) cannot alloc memory for file data!", m_name);
		fclose(file);
		return NO;
	}
	
	if (fread(data, header.h_size, 1, file) < 1)
	{
		DBGLOG1A(@"OriTexture: (%@) cant read texture data - corrupted file?", m_name);
		fclose(file);
		free(data);
		return NO;
	}
	
	fclose(file);
	
	GLenum	format, packing;
	
	switch (header.h_format)
	{
		case eOriTexFormatByteRGBA:
			format	= GL_RGBA;
			packing	= GL_UNSIGNED_BYTE;
			break;
		case eOriTexFormatShortRGBA4444:
			format	= GL_RGBA;
			packing	= GL_UNSIGNED_SHORT_4_4_4_4;
			break;
		case eOriTexFormatShortRGBA5551:
			format	= GL_RGBA;
			packing	= GL_UNSIGNED_SHORT_5_5_5_1;
			break;
		case eOriTexFormatShortRGB565:
			format	= GL_RGB;
			packing	= GL_UNSIGNED_SHORT_5_6_5;
			break;
		case eOriTexFormatByteA:
			format	= GL_ALPHA;
			packing	= GL_UNSIGNED_BYTE;
			break;
		case eOriTexFormatByteL:
			format	= GL_LUMINANCE;
			packing	= GL_UNSIGNED_BYTE;
			break;
		case eOriTexFormatByteLA:
			format	= GL_LUMINANCE_ALPHA;
			packing	= GL_UNSIGNED_BYTE;
			break;
	}
	
	m_texture = [m_resMan.propRenderer createTextureW:header.h_width H:header.h_height Data:data Format:format Packing:packing];
	
	free(data);
	
	if (!m_texture)
	{
		DBGLOG1A(@"OriTexture: failed to load texture from file (%@)", m_source);
		return NO;
	}
	
	m_size.width	= header.h_width;
	m_size.height	= header.h_height;
	
	return YES;
}

@end
