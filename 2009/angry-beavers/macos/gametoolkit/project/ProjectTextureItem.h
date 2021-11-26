//===============================================================================================
//  File:		ProjectTextureItem.h
//  Project:	OriginGameToolkit
//	Desc:		Project texture item container class interface
//
//  Created by Max Vasin on 11/24/10.
//  Copyright 2010 Origin. All rights reserved.
//===============================================================================================

// **** IMPORTS ****

#import <Cocoa/Cocoa.h>
#import "ProjectItem.h"
#import "TextureAssemblyItem.h"

// Texture source type enumeration

typedef enum
{
	eTextureSourcePersistent = 0,
	eTextureSourceAssembled,
	eTextureSourceGeneric
}enTextureSourceType;

// Texture output format enumeration

typedef enum
{
	eTextureOutputPNG		= 0,
	eTextureOutputPVR		= 1,
	eTextureOutputPVRRaw	= 2
}enTextureOutputFormat;

// Texture output size enumeration

typedef enum
{
	eTextureSizeSquared		= 0,
	eTextureSizePower		= 1,
	eTextureSizeSource		= 2
}enTextureOutputSize;

// PVR bits per pixel enumeration

typedef enum
{
	ePVR4BitsPerPixel		= 0,
	ePVR2BitsPerPixel		= 1
}enTexturePVRBitsPerPixel;

// PVR channel weghting enumeration

typedef enum
{
	ePVRChannelWeightingLinear		= 0,
	ePVRChannelWeightingPerceptual	= 1
}enTexturePVRChannelWeighting;

// Color operation enumeration

typedef enum
{
	eTextureFilterNone				= 0,
	eTextureFilterMono				= 1,
	eTextureFilterInvertMono		= 2,
	eTextureFilterBlackToAlpha		= 3
	
}enTextureOuputFilter;

// Engine texture format

typedef enum
{
	eTextureEngineFormatByteRGBA		= 0,
	eTextureEngineFormatShortRGBA4444	= 1,
	eTextureEngineFormatShortRGBA5551	= 2,
	eTextureEngineFormatShortRGB565		= 3,
	eTextureEngineFormatByteA			= 4,
	eTextureEngineFormatByteL			= 5,
	eTextureEngineFormatByteLA			= 6
	
}enTextureEngineFormat;

// ****** FILE HEADER ******

typedef struct
{
	
	unsigned int			h_magic;
	unsigned int			h_width;
	unsigned int			h_height;
	unsigned int			h_size;
	enTextureEngineFormat	h_format;
	
}oriTextureFileHeader;


// **** INTERFACE ****

@interface ProjectTextureItem : ProjectItem 
{
	enTextureSourceType				m_sourceType;
	enTextureOutputFormat			m_outputFormat;
	enTextureOutputSize				m_outputSize;
	enTexturePVRBitsPerPixel		m_pvrBitsPerPixel;
	enTexturePVRChannelWeighting	m_pvrChannelWeighting;
	enTextureOuputFilter			m_outputFilter;
	BOOL							m_pvrAlphaIndependent;
	NSString*						m_outputFile;
	NSMutableArray*					m_assemblyItems;
	BOOL							m_packed;
	IntSize							m_cachedSize;
	IntSize							m_packedSize;
	enTextureEngineFormat			m_engineFormat;
}

// **** PROPERTIES ****

@property (readonly)			NSArray*						propSourceTypes;
@property (readonly)			NSArray*						propOutputFormats;
@property (readonly)			NSArray*						propOutputSizes;
@property (readonly)			NSArray*						propPVRBits;
@property (readonly)			NSArray*						propPVRWeightings;
@property (nonatomic, readonly)	NSArray*						propFiltersList;
@property (nonatomic, readonly)	NSArray*						propEngineFormatsList;
@property (assign)				enTextureSourceType				propSourceType;
@property (retain)				NSMutableArray*					propAssembly;
@property (copy)				NSString*						propOutputFile;
@property (assign)				enTextureOutputFormat			propOutputFormat;
@property (assign)				enTextureOutputSize				propOutputSize;
@property (assign)				enTexturePVRBitsPerPixel		propPVRBitsPerPixel;
@property (assign)				enTexturePVRChannelWeighting	propPVRChannelWeighting;
@property (nonatomic, assign)	enTextureOuputFilter			propFilter;
@property (nonatomic, assign)	enTextureEngineFormat			propEngineFormat;
@property (assign)				BOOL							propPVRAlphaIdependent;
@property (readonly)			NSString*						propOutputPath;
@property (readonly)			NSString*						propBuildPath;
@property (nonatomic, assign)	BOOL							propPacked;
@property (nonatomic, readonly)	IntSize							propCachedSize;
@property (nonatomic, readonly)	IntSize							propPackedSize;
@property (nonatomic, readonly)	unsigned int					propCachedMemory;
@property (nonatomic, readonly)	unsigned int					propPackedMemory;

// **** CLASS METHODS ****

+ (NSArray*)				getTextureSourceTypes;
+ (NSArray*)				getTextureOutputFormats;
+ (NSArray*)				getTextureOutputSizes;
+ (NSArray*)				getPVRBits;
+ (NSArray*)				getPVRWeightings;

// **** INSTANCE METHODS ****

- (TextureAssemblyItem*)	addAssemblyItem;
- (TextureAssemblyItem*)	deleteAssemblyItem:		(TextureAssemblyItem*)item;
- (void)					shiftAssemblyItem:		(TextureAssemblyItem*)item	offset:(NSInteger)offset;
- (NSInteger)				indexOfAssemblyItem:	(TextureAssemblyItem*)item;
- (TextureAssemblyItem*)	getAssemblyItem:		(NSString*)name;

- (NSArray*)				getLinkedAnimations:	(TextureAssemblyItem*)item;

@end
