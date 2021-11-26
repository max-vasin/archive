//===============================================================================================
//  File:		ProjectTextureItem.m
//  Project:	OriginGameToolkit
//	Desc:		Project texture item implementation
//
//  Created by Max Vasin on 11/24/10.
//  Copyright 2010 Origin. All rights reserved.
//===============================================================================================

// **** IMPORTS ****

#import "ProjectTextureItem.h"
#import "Project.h"
#import "AppDelegate.h"
#import "SharedResourceManager.h"
#import "ImageWrapper.h"
#import "SubdivideTreeNode.h"

// **** PRIVATE METHODS ****

@interface ProjectTextureItem (Private)

- (NSBitmapImageRep*)	assembly;
- (NSBitmapImageRep*)	loadImage;
- (NSBitmapImageRep*)	generateTexture;
- (NSBitmapImageRep*)	assemblyPacked;
- (result)				compressFile:(NSString*)file Output:(NSString*)output Progress:(Progressor*)progress;
- (GLfloat)				alignToPowerOf2:(GLfloat)val;
- (result)				saveTexture:(NSBitmapImageRep*)image	ToFile:(NSString*)filename;

@end

// **** IMPLEMENTATION ****

@implementation ProjectTextureItem

// **** SYNTHESIZED PROPERTIES ****

@synthesize propSourceType			= m_sourceType;
@synthesize propOutputFile			= m_outputFile;
@synthesize propOutputSize			= m_outputSize;
@synthesize propPVRBitsPerPixel		= m_pvrBitsPerPixel;
@synthesize propPVRChannelWeighting	= m_pvrChannelWeighting;
@synthesize propPVRAlphaIdependent	= m_pvrAlphaIndependent;
@synthesize propPacked				= m_packed;
@synthesize propCachedSize			= m_cachedSize;
@synthesize propPackedSize			= m_packedSize;
@synthesize propFilter				= m_outputFilter;
@synthesize propEngineFormat		= m_engineFormat;

// **** IMPLEMENTED PROPERTIES ****

// Assembly items array
//------------------------------------------------------------------------------------------------
- (NSMutableArray*)propAssembly
{
	return [[m_assemblyItems retain] autorelease];
}

- (void)setPropAssembly:(NSMutableArray*)array
{
	[m_assemblyItems autorelease];
	
	m_assemblyItems = [array copy];
}

// Output format
//------------------------------------------------------------------------------------------------
- (enTextureOutputFormat)propOutputFormat
{
	return m_outputFormat;
}

- (void)setPropOutputFormat:(enTextureOutputFormat)format
{
	m_outputFormat = format;
	if ( (m_outputFormat == eTextureOutputPVR) || (m_outputFormat == eTextureOutputPVRRaw) )
		self.propOutputSize = eTextureSizeSquared;
}

// Cached memory
//------------------------------------------------------------------------------------------------
- (unsigned int)propCachedMemory
{
	return m_cachedSize.width * m_cachedSize.height * 4 * sizeof(GLubyte);
}

// Packed memory
//------------------------------------------------------------------------------------------------
- (unsigned int)propPackedMemory
{
	int pixels = m_packedSize.width * m_packedSize.height;
	
	switch (m_engineFormat) 
	{
		case eTextureEngineFormatByteRGBA:
			return pixels * 4 * sizeof(unsigned char);
		case eTextureEngineFormatShortRGB565:
		case eTextureEngineFormatShortRGBA4444:
		case eTextureEngineFormatShortRGBA5551:
			return pixels * sizeof(unsigned short);
		case eTextureEngineFormatByteA:
		case eTextureEngineFormatByteL:
			return pixels * sizeof(unsigned char);
		case eTextureEngineFormatByteLA:
			return pixels * 2 * sizeof(unsigned char);
	}
	
	return 0;
}

// Source types array
//------------------------------------------------------------------------------------------------
- (NSArray*)propSourceTypes
{
	return [[self class] getTextureSourceTypes];
}

// Output formats array
//------------------------------------------------------------------------------------------------
- (NSArray*)propOutputFormats
{
	return [[self class] getTextureOutputFormats];
}

// Output sizes array
//------------------------------------------------------------------------------------------------
- (NSArray*)propOutputSizes
{
	return [[self class] getTextureOutputSizes];
}

// PVR bits per pixel
//------------------------------------------------------------------------------------------------
- (NSArray*)propPVRBits
{
	return [[self class] getPVRBits];
}

// PVR weightings
//------------------------------------------------------------------------------------------------
- (NSArray*)propPVRWeightings
{
	return [[self class] getPVRWeightings];
}

// Filters list
//------------------------------------------------------------------------------------------------
- (NSArray*)propFiltersList
{
	return [NSArray arrayWithObjects:@"no filter", @"monochromic", @"inverse monochromic", @"black to alpha", nil];
}

// Engine format list
//------------------------------------------------------------------------------------------------
- (NSArray*)propEngineFormatsList
{
	return [NSArray arrayWithObjects:@"32 bit - R8G8B8A8",
									@"16 bit - R4G4B4A4",
									@"16 bit - R5G5B5A1",
									@"16 bit - R5G6B5",
									@"8 bit - A8",
									@"8 bit - L8",
									@"16 bit - LA8",
									nil];
}

// Output path
//------------------------------------------------------------------------------------------------
- (NSString*)propOutputPath
{
	if (m_outputFormat != eTextureOutputPNG)
		return [NSString stringWithFormat:@"%@/pack_%@.png", m_project.propCacheDirectory, m_outputFile];
	
	return [NSString stringWithFormat:@"%@/%@.png", self.propItemBuildDirectory, m_outputFile];
}

// Building path
//------------------------------------------------------------------------------------------------
- (NSString*)propBuildPath
{
	return [NSString stringWithFormat:@"%@/%@.png", [m_project propCacheDirectory], m_outputFile];
}

// **** CLASS METHODS ****

// Returns array of available texture types
//-----------------------------------------------------------------------------------------------
+ (NSArray*)getTextureSourceTypes
{
	// In same order as enTextureSourceType
	NSArray* types = [NSArray arrayWithObjects:@"Persistent source file",
											   @"Assembled source file",
											   @"Engine generic",
											   nil];
	return types;
}

// Returns array of available texture output formats
//-----------------------------------------------------------------------------------------------
+ (NSArray*)getTextureOutputFormats
{
	// In same order as enTextureOutputFormat
	NSArray* types = [NSArray arrayWithObjects:	@"PNG",
												@"PVR",
												@"PVR (raw)",
												nil];
	return types;
}

// Returns array of available texture output sizes
//-----------------------------------------------------------------------------------------------
+ (NSArray*)getTextureOutputSizes
{
	// In same order as enTextureOutputFormat
	NSArray* types = [NSArray arrayWithObjects:	@"Squared power of 2",
												@"Aligned power of 2",
												@"Same as source",
												nil];
	return types;	
}

// Returns array of available PVR format bits
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
+ (NSArray*)getPVRBits
{
	// In same order as enTexturePVRBitsPerPixel
	NSArray* types = [NSArray arrayWithObjects:	@"4",
												@"2",
												nil];
	return types;		
}

// Returns array of available PVR format channel weightings
//-----------------------------------------------------------------------------------------------
+ (NSArray*)getPVRWeightings
{
	// In same order as enTexturePVRChannelWeighting
	NSArray* types = [NSArray arrayWithObjects:	@"Linear",
												@"Perceptual",
												nil];
	return types;		
}

// **** INSTANCE METHODS IMPLEMENTATION ****

// Initializes new texture item with parent
//-----------------------------------------------------------------------------------------------
- (id)initWithParent:(ProjectItem*)parent
{
	if ((self = [super initWithParent:parent]))
	{
		m_sourceType	= eTextureSourcePersistent;
		m_assemblyItems = [[NSMutableArray alloc] init];
	}
	return self;
}

// Initializes new texture item with parent and XML data
//-----------------------------------------------------------------------------------------------
- (id)initWithParent:(ProjectItem*)parent fromXML:(NSXMLElement*)node
{
	if ((self = [super initWithParent:parent fromXML:node]))
	{
		self.propSourceType		= [node getFirstChildInteger:@"SourceType"];
		self.propOutputFile		= [node getFirstChildValue:@"Output"];
		self.propOutputSize		= [node getFirstChildInteger:@"OutputSize"];
		self.propOutputFormat	= [node getFirstChildInteger:@"OutputFormat"];
		self.propPacked			= [node getFirstChildInteger:@"Packed"];
		self.propFilter			= [node getFirstChildInteger:@"OutputFilter"];
		self.propEngineFormat	= [node getFirstChildInteger:@"EngineFormat"];
		
		m_cachedSize.width		= [node getFirstChildInteger:@"CachedWidth"];
		m_cachedSize.height		= [node getFirstChildInteger:@"CachedHeight"];
		m_packedSize.width		= [node getFirstChildInteger:@"PackedWidth"];
		m_packedSize.height		= [node getFirstChildInteger:@"PackedHeight"];
		
		if ( (m_outputFormat == eTextureOutputPVR) || (m_outputFormat == eTextureOutputPVRRaw) )
		{
			NSXMLElement* pvrNode = [node getFirstChild:@"PVR"];
			
			if (pvrNode)
			{
				self.propPVRBitsPerPixel		= [pvrNode getFirstChildInteger:@"Bits"];
				self.propPVRChannelWeighting	= [pvrNode getFirstChildInteger:@"Weighting"];
				self.propPVRAlphaIdependent		= [pvrNode getFirstChildInteger:@"AlphaIndependent"];
			}
		}
		
		m_assemblyItems = [[NSMutableArray alloc] init];
		
		if (m_sourceType == eTextureSourceAssembled)
		{
			NSXMLElement* asmNode = [node getFirstChild:@"Assembly"];
			
			for (NSXMLElement* itemNode in [asmNode children])
			{
				TextureAssemblyItem* item = [[[TextureAssemblyItem alloc] initWithTextureItem:self fromXML:itemNode] autorelease];
				[m_assemblyItems addObject:item];
			}
		}
	}
	return self;
}

// Destructor
//-----------------------------------------------------------------------------------------------
- (void)dealloc
{
	SAFE_RELEASE(m_assemblyItems);
	SAFE_RELEASE(m_outputFile);
	
	[super dealloc];
}

// Sets item icon
//-----------------------------------------------------------------------------------------------
- (void)setIcon
{
	m_itemImage = [SharedResourceManager sharedIcon:@"texture"];
}

// Makes XML representation of current item and attach it to node
//-----------------------------------------------------------------------------------------------
- (NSXMLElement*)saveAsXML:(NSXMLElement*)node
{
	NSXMLElement* thisNode = [node addNewChild:@"Texture"];
	
	[thisNode addNewChild:@"Name"			withValue:m_itemName];
	[thisNode addNewChild:@"SourceType"		withInteger:m_sourceType];
	[thisNode addNewChild:@"Output"			withValue:m_outputFile];
	[thisNode addNewChild:@"OutputSize"		withInteger:m_outputSize];
	[thisNode addNewChild:@"OutputFormat"	withInteger:m_outputFormat];
	[thisNode addNewChild:@"OutputFilter"	withInteger:m_outputFilter];
	[thisNode addNewChild:@"EngineFormat"	withInteger:m_engineFormat];
	[thisNode addNewChild:@"Packed"			withInteger:m_packed];
	[thisNode addNewChild:@"CachedWidth"	withInteger:m_cachedSize.width];
	[thisNode addNewChild:@"CachedHeight"	withInteger:m_cachedSize.height];
	[thisNode addNewChild:@"PackedWidth"	withInteger:m_packedSize.width];
	[thisNode addNewChild:@"PackedHeight"	withInteger:m_packedSize.height];
		
	if ( (m_outputFormat == eTextureOutputPVR) || (m_outputFormat == eTextureOutputPVRRaw) )
	{
		NSXMLElement* pvrNode = [thisNode addNewChild:@"PVR"];
		
		[pvrNode addNewChild:@"Bits"				withInteger:m_pvrBitsPerPixel];
		[pvrNode addNewChild:@"Weighting"			withInteger:m_pvrChannelWeighting];
		[pvrNode addNewChild:@"AlphaIdependent"		withInteger:m_pvrAlphaIndependent];
	}
	
	// Assembly
	if (m_sourceType == eTextureSourceAssembled)
	{
		NSXMLElement* asmNode = [thisNode addNewChild:@"Assembly"];
		
		for (TextureAssemblyItem* item in m_assemblyItems)
		{
			[item saveXML:asmNode];
		}
	}
	
	return thisNode;
}

// Adds new assembly item
//-----------------------------------------------------------------------------------------------
- (TextureAssemblyItem*)addAssemblyItem
{
	TextureAssemblyItem* item = [[[TextureAssemblyItem alloc] initWithTextureItem:self] autorelease];
	
	[self.propAssembly addObject:item];
	[item generateName];
	
	return item;
}

// Removes assembly item from list
//-----------------------------------------------------------------------------------------------
- (TextureAssemblyItem*)deleteAssemblyItem:(TextureAssemblyItem*)item
{
	// retain item to avoid permanent destruction
	[item retain];
	
	[self.propAssembly removeObject:item];
	
	return [item autorelease];
}

// Shifts assembly item to new location
//-----------------------------------------------------------------------------------------------
- (void)shiftAssemblyItem:(TextureAssemblyItem*)item offset:(NSInteger)offset
{
	[m_assemblyItems shiftObject:item offset:offset];
}

// Returns index of specifies texture item object
//-----------------------------------------------------------------------------------------------
- (NSInteger)indexOfAssemblyItem:(TextureAssemblyItem*)item
{
	return [m_assemblyItems indexOfObject:item];
}

// Returns texture assembly item by name
//-----------------------------------------------------------------------------------------------
- (TextureAssemblyItem*)getAssemblyItem:(NSString*)name
{
	return [m_assemblyItems getObjectByProp:@"propName" Value:name];
}

// Returns array of animatins linked to assembly item
//-----------------------------------------------------------------------------------------------
- (NSArray*)getLinkedAnimations:(TextureAssemblyItem*)item
{
	ProjectRootItem* root = (ProjectRootItem*)m_project.propRootItem;
	
	return [(ProjectSpriteRootItem*)root.propRootSprites getLinkedAnimations:item];
}

// Final texture building
//-----------------------------------------------------------------------------------------------
- (result)build:(Progressor*)progress
{
	result res = [super build:progress];
	if (FAIL(res))
		return res;
	
	if (progress)
		progress.propCurrentState = [NSString stringWithFormat:@"Assembling: %@", m_itemName];
	
	NSBitmapImageRep* cache = nil, *packed = nil;
	
	switch (m_sourceType)
	{
		case eTextureSourceAssembled:
			cache = [self assembly];
			if (m_packed)
				packed = [self assemblyPacked];
			else
				packed = cache;
			break;
		
		case eTextureSourcePersistent:
			cache  = [self loadImage];
			packed = cache;
			break;
		
		case eTextureSourceGeneric:
			cache  = [self generateTexture];
			packed = cache;
			break;
	}
	
	NSData*		outData;
	NSString*	outFile;
	
	NSColor* black = [NSColor colorWithCalibratedRed:0.0f green:0.0f blue:0.0f alpha:1.0f];
	
	switch (m_outputFilter)
	{
		case eTextureFilterMono:
			[cache applyMonochrome];
			if (cache != packed)
				[packed applyMonochrome];
			break;
		
		case eTextureFilterInvertMono:
			
			[cache applyMonochrome];
			if (cache != packed)
				[packed applyMonochrome];
			
			[cache applyInverse];
			if (cache != packed)
				[packed applyInverse];
			
			break;
	
		case eTextureFilterNone:
			break;
			
		case eTextureFilterBlackToAlpha:
			[cache applyTransparent:black];
			if (cache != packed)
				[packed applyTransparent:black];
			break;
	}
	
	
	if (cache)
	{
		m_cachedSize.width	= [cache pixelsWide];
		m_cachedSize.height = [cache pixelsHigh];
		
		outData = [cache representationUsingType:NSPNGFileType properties:nil];
		outFile = [NSString stringWithFormat:@"%@/%@.png", m_project.propCacheDirectory, m_outputFile];
		
		[outData writeToFile:outFile atomically:YES];
		
		[self compressFile:outFile Output:[NSString stringWithFormat:@"%@/%@.pvr", m_project.propCacheDirectory, m_outputFile] Progress:progress];
	}
	
	if (packed)
	{
		m_packedSize.width	= [packed pixelsWide];
		m_packedSize.height	= [packed pixelsHigh];
		
		// internal format
		
		outFile = [NSString stringWithFormat:@"%@/%@.oritex", self.propItemBuildDirectory, m_outputFile];

		[self saveTexture:packed ToFile:outFile];
		
		[self compressFile:outFile Output:[NSString stringWithFormat:@"%@/%@.pvr", self.propItemBuildDirectory, m_outputFile] Progress:progress];
	}
	
	[m_itemParent updateName];
	
	return res;
}

// Compressing file with PVR compressor
//-----------------------------------------------------------------------------------------------
- (result)compressFile:(NSString*)file Output:(NSString*)output Progress:(Progressor*)progress
{
	// Perform compressing
	
	if ( (m_outputFormat == eTextureOutputPVR) || (m_outputFormat == eTextureOutputPVRRaw) )
	{
		if (progress)
			progress.propCurrentState = [NSString stringWithFormat:@"Compressing: %@", m_itemName];
		
		AppDelegate* app		= [NSApp delegate];
		NSString*    textool	= app.propAppPrefs.propBuildTextureTool;
		NSString*	 tempFile	= [file stringByAppendingString:@".temp"];
		
		NSArray*	 args	 = [NSArray arrayWithObjects:
								
								@"-e", @"PVRTC",
								(m_pvrChannelWeighting == ePVRChannelWeightingLinear) ? @"--channel-weighting-linear"  : @"--channel-weighting-perceptual",
								(m_pvrBitsPerPixel     == ePVR2BitsPerPixel)		  ? @"--bits-per-pixel-2" : @"--bits-per-pixel-4",
								(m_pvrAlphaIndependent)								  ? @"--alpha-is-independent" : @"--alpha-is-opacity",
								@"-f", (m_outputFormat == eTextureOutputPVR) ? @"PVR" : @"Raw",
								@"-o", output,
								@"-p", tempFile,
								file,
								nil];
		
		NSTask* texturetool = [NSTask launchedTaskWithLaunchPath:textool arguments:args];
		
		if (!texturetool)
			[NSException raise:@"Build failed" format:@"Texture (%@): cant create (texturetool) task process", m_itemName];
		
		[texturetool waitUntilExit];
		
		int status = [texturetool terminationStatus];
		
		if (status != 0)
			[NSException raise:@"Build failed" format:@"Texture (%@): compressing texture failed with code (%i)", m_itemName, status];
		
		// Replace input file with preview
		NSFileManager* fileMan = [NSFileManager defaultManager];
		
		NSError* err;
		
		[fileMan removeItemAtPath:file error:&err];
		[fileMan moveItemAtPath:tempFile toPath:file error:&err];
	}
	
	return RES_OK;
}


// Building of assembled texture
//-----------------------------------------------------------------------------------------------
- (NSBitmapImageRep*)assembly
{
	// No assembly - no result
	if (![m_assemblyItems count])
		return nil;
	
	// Obtain every assembly item surface
	
	NSMutableArray* parts = [[[NSMutableArray alloc] initWithCapacity:[m_assemblyItems count]] autorelease];
	
	for (TextureAssemblyItem* item in m_assemblyItems)
	{
		NSBitmapImageRep* part = [item build];
		if (!part)
			[NSException raise:@"Build failed" format:@"Assembly item (%@) of texture (%@): building surface failed", [item propName], m_itemName];
		
		ImageWrapper* wrapper = [[ImageWrapper alloc] initWithImage:part andAssemblyItem:item];
		
		[parts addObject:wrapper];
		
		[wrapper release];
	}
	
	// Scan surfaces to determine size of output
	
	IntSize  surfaceSize	= {0, 0};
	IntSize  prevSize		= {0, 0};
	IntPoint origin			= {0, 0};
	
	for (int i = 0; i < [m_assemblyItems count]; i++)
	{
		TextureAssemblyItem* item = [m_assemblyItems objectAtIndex:i];
		ImageWrapper*		 part = [parts objectAtIndex:i];
		
		IntSize partSize = [part->m_image intSize];
		
		switch ([item propDock])
		{
			case eAssemblyItemDockRight:
				origin.x += prevSize.width;
				break;
			
			case eAssemblyItemDockBottom:
				origin.y += prevSize.height;
				break;
			
			case eAssemblyitemDockNewColumn:
				origin.y = 0;
				origin.x = surfaceSize.width;
				break;
			
			case eAssemblyItemDockNewRow:
				origin.x = 0;
				origin.y = surfaceSize.height;
				break;
			
			case eAssemblyItemDockStatic:
				origin.x = item.propStaticDockX;
				origin.y = item.propStaticDockY;
				break;
		}
		
		surfaceSize.width	= MAX(surfaceSize.width,  origin.x + partSize.width);
		surfaceSize.height	= MAX(surfaceSize.height, origin.y + partSize.height);
		
		prevSize = partSize;
	}
	
	// Modify size
	
	switch (m_outputSize)
	{
		case eTextureSizeSquared:
			surfaceSize.width  = [self alignToPowerOf2:surfaceSize.width];
			surfaceSize.height = [self alignToPowerOf2:surfaceSize.height ];
			surfaceSize.width  = MAX(surfaceSize.width, surfaceSize.height);
			surfaceSize.height = surfaceSize.width;
			break;
		case eTextureSizePower:
			surfaceSize.width  = [self alignToPowerOf2:surfaceSize.width ];
			surfaceSize.height = [self alignToPowerOf2:surfaceSize.height ];
			break;
		case eTextureSizeSource:
			// nothing to do
			break;
	}
	
	// Create surface
	
	NSBitmapImageRep* surface = [[[NSBitmapImageRep alloc] initWithBitmapDataPlanes:nil
																		 pixelsWide:surfaceSize.width
																		 pixelsHigh:surfaceSize.height
																	  bitsPerSample:8
																	samplesPerPixel:4
																		   hasAlpha:YES
																		   isPlanar:NO
																	 colorSpaceName:NSCalibratedRGBColorSpace
																	   bitmapFormat:0
																		bytesPerRow:(4 * surfaceSize.width)
																	   bitsPerPixel:32] autorelease];
	
	// Draw parts to surface
	
	[NSGraphicsContext saveGraphicsState];
	[NSGraphicsContext setCurrentContext:[NSGraphicsContext graphicsContextWithBitmapImageRep:surface]];
	
	origin.x = 0;
	origin.y = 0;
	
	prevSize.width	= 0;
	prevSize.height = 0;
	
	IntSize  curSize = {0, 0};
	NSPoint  outPos  = {0.0, 0.0};
	
	for (int i = 0; i < [m_assemblyItems count]; i++)
	{
		TextureAssemblyItem* item = [m_assemblyItems objectAtIndex:i];
		ImageWrapper*		 part = [parts objectAtIndex:i];
		
		IntSize	partSize = [part->m_image intSize];
		
		switch ([item propDock])
		{
			case eAssemblyItemDockRight:
				origin.x += prevSize.width;
				break;
			
			case eAssemblyItemDockBottom:
				origin.y += prevSize.height;
				break;
			
			case eAssemblyitemDockNewColumn:
				origin.y = 0;
				origin.x = curSize.width;
				break;
			
			case eAssemblyItemDockNewRow:
				origin.x = 0;
				origin.y = curSize.height;
				break;
			
			case eAssemblyItemDockStatic:
				origin.x = item.propStaticDockX;
				origin.y = item.propStaticDockY;
				break;
		}
		
		outPos.x = origin.x;
		outPos.y = surfaceSize.height - origin.y - partSize.height;
		
		[part->m_image drawAtPoint:outPos];
		
		// Place linked animations frames
		NSArray* animations = [self getLinkedAnimations:part->m_assembly];
		for (SpriteAnimation* anim in animations)
			[anim linkSetUnpackedOffsetX:origin.x Y:origin.y];
		
		curSize.width	= MAX(curSize.width,  origin.x + partSize.width);
		curSize.height	= MAX(curSize.height, origin.y + partSize.height);

		prevSize = partSize;
	}
	
	[NSGraphicsContext restoreGraphicsState];
	
	return surface;
}

// Building packed assembly texture
//-----------------------------------------------------------------------------------------------
- (NSBitmapImageRep*)assemblyPacked
{
	// 1. Obtain full set of trimmed parts
	
	NSMutableArray* fullSet = [[NSMutableArray alloc] initWithCapacity:100];
		
	for (TextureAssemblyItem* item in m_assemblyItems)
	{
		NSMutableArray* itemImages = [item trimmedParts];
		
		for (ImageWrapper* image in itemImages)
		{
			[fullSet addObject:image];
		}
	}
	
	// 2. Sort by area
	
	[fullSet sortUsingSelector:@selector(areaSort:)];
	
	// 3. Start recursive building
	
	int curWidth = 8;
	int curHeight = 8;
	
	BOOL dirFlag = NO;
	
	while (TRUE)
	{
		// Make new node with texture size rectangle
		SubdivideTreeNode* node = [[SubdivideTreeNode alloc] init];
		
		node->m_rect.right	= curWidth - 1;
		node->m_rect.bottom = curHeight - 1;
		
		BOOL noSpace = NO;
		
		for (ImageWrapper* item in fullSet)
		{
			SubdivideTreeNode* insert = [node insert:item];
			if (!insert)
			{
				noSpace = YES;
				break;
			}
			
			item->m_posX = insert->m_rect.left;
			item->m_posY = insert->m_rect.top;
		}
		
		SAFE_RELEASE(node);
		
		if (!noSpace)
			break;
		
		switch (m_outputSize) 
		{
			case eTextureSizeSource:
				if (dirFlag)
					curHeight++;
				else
					curWidth++;
				break;
			
			case eTextureSizePower:
				if (dirFlag)
					curHeight *= 2;
				else
					curWidth *= 2;
				break;
			
			case eTextureSizeSquared:
				curHeight *= 2;
				curWidth  *= 2;
				break;
		}
		
		dirFlag = !dirFlag;
	}
	
	// All done
	
	NSBitmapImageRep* surface = [[[NSBitmapImageRep alloc] initWithBitmapDataPlanes:nil
																		 pixelsWide:curWidth
																		 pixelsHigh:curHeight
																	  bitsPerSample:8
																	samplesPerPixel:4
																		   hasAlpha:YES
																		   isPlanar:NO
																	 colorSpaceName:NSCalibratedRGBColorSpace
																	   bitmapFormat:0
																		bytesPerRow:(4 * curWidth)
																	   bitsPerPixel:32] autorelease];
	
	// Draw parts to surface
	
	[NSGraphicsContext saveGraphicsState];
	[NSGraphicsContext setCurrentContext:[NSGraphicsContext graphicsContextWithBitmapImageRep:surface]];
	
	
	NSAutoreleasePool* localPool = [[NSAutoreleasePool alloc] init];
	
	for (ImageWrapper* item in fullSet)
	{
		NSPoint outPos;
		
		outPos.x = item->m_posX;
		outPos.y = curHeight - item->m_height - item->m_posY;
		
		NSArray* links = [self getLinkedAnimations:item->m_assembly];
		
		for (SpriteAnimation* anim in links)
		{
			uint offX = 0, offY = 0;
			
			offX = item->m_trimX;
			offY = item->m_trimY;
			
			[anim linkSetPackedFrame:item->m_index X:item->m_posX Y:item->m_posY W:item->m_width H:item->m_height OffX:offX OffY:offY];
		}
		
		[item->m_image drawAtPoint:outPos];
	}
	
	SAFE_RELEASE(localPool);
	
	[NSGraphicsContext restoreGraphicsState];
	
	SAFE_RELEASE(fullSet);
	
	return surface;
}

// Building of persistent texture
//-----------------------------------------------------------------------------------------------
- (NSBitmapImageRep*)loadImage
{
	return nil;
}

// Building of generic texture
//-----------------------------------------------------------------------------------------------
- (NSBitmapImageRep*)generateTexture
{
	return nil;
}

// Returns aligned value by power of 2 border
//-----------------------------------------------------------------------------------------------
- (GLfloat)alignToPowerOf2:(GLfloat)val
{
	GLfloat pow = 2;
	while (YES)
	{
		if (val <= pow)
			return pow;
		
		pow *= 2;
	}
	return val;
}

//-----------------------------------------------------------------------------------------------
// Returns aligned value by power of 2 border
//-----------------------------------------------------------------------------------------------

- (result)saveTexture:(NSBitmapImageRep*)image ToFile:(NSString*)filename
{
	unsigned int width		= [image pixelsWide];
	unsigned int height		= [image pixelsHigh];
	unsigned int pixelcount	= width * height;
	unsigned int datasize	= 0;
	
	unsigned char*	source   = [image bitmapData];
	void*			convert  = NULL;
	unsigned short*	buffer16 = NULL;
	unsigned char*	buffer8	 = NULL;
	
	byte  r, g, b, mono;
	
	BOOL			needfree = NO;
	
	// all incoming images must be in RGBA8
	// perform conversion
	
	switch (m_engineFormat) 
	{
		case eTextureEngineFormatByteRGBA:
			convert = source;
			datasize = pixelcount * sizeof(unsigned char) * 4;
			break;
			
		case eTextureEngineFormatShortRGBA4444:
			needfree = YES;
			datasize = pixelcount * sizeof(unsigned short);
			buffer16 = (unsigned short*)calloc(pixelcount, sizeof(unsigned short));
			
			for (int i = 0; i < pixelcount; i++)
			{
				buffer16[i] = source[i * 4 + 0] >> 4 << 12 |
						  	  source[i * 4 + 1] >> 4 << 8  |
							  source[i * 4 + 2] >> 4 << 4  |
						  	  source[i * 4 + 3] >> 4;
			}
			convert = buffer16;
			break;
		
		case eTextureEngineFormatShortRGBA5551:
			needfree = YES;
			datasize = pixelcount * sizeof(unsigned short);
			buffer16 = (unsigned short*)calloc(pixelcount, sizeof(unsigned short));
			
			for (int i = 0; i < pixelcount; i++)
			{
				buffer16[i] = source[i * 4 + 0] >> 3 << 11 |
							  source[i * 4 + 1] >> 3 << 6  |
							  source[i * 4 + 2] >> 3 << 1  |
							  source[i * 4 + 3] >> 7;
			}
			convert = buffer16;
			break;
		
		case eTextureEngineFormatShortRGB565:
			needfree = YES;
			datasize = pixelcount * sizeof(unsigned short);
			buffer16 = (unsigned short*)calloc(pixelcount, sizeof(unsigned short));
			
			for (int i = 0; i < pixelcount; i++)
			{
				buffer16[i] = source[i * 4 + 0] >> 3 << 11 |
							  source[i * 4 + 1] >> 3 << 6  |
							  source[i * 4 + 2] >> 3;
			}
			
			convert = buffer16;
			break;
		
		case eTextureEngineFormatByteA:
			needfree = YES;
			datasize = pixelcount * sizeof(unsigned char);
			buffer8  = (unsigned char*)calloc(pixelcount, sizeof(unsigned char));
			
			for (int i = 0; i < pixelcount; i++)
			{
				buffer8[i] = source[i * 4 + 3];
			}
			
			convert = buffer8;
			break;
			
		case eTextureEngineFormatByteL:
			needfree = YES;
			datasize = pixelcount * sizeof(unsigned char) * 2;
			buffer8  = (unsigned char*)calloc(pixelcount, sizeof(unsigned char) * 2);
			
			for (int i = 0; i < pixelcount; i++)
			{
				r = source[i * 4 + 0];
				g = source[i * 4 + 1];
				b = source[i * 4 + 2];
				
				mono = (byte)(((float)r + (float)g + (float)b) / 768 * 256);
				
				buffer8[i] = mono;
			}
			convert = buffer8;
			break;
				
		case eTextureEngineFormatByteLA:
			needfree = YES;
			datasize = pixelcount * sizeof(unsigned char) * 2;
			buffer8  = (unsigned char*)calloc(pixelcount, sizeof(unsigned char) * 2);
			
			for (int i = 0; i < pixelcount; i++)
			{
				r = source[i * 4 + 0];
				g = source[i * 4 + 1];
				b = source[i * 4 + 2];
				
				mono = (byte)(((float)r + (float)g + (float)b) / 768 * 256);
				
				buffer8[i * 2 + 0] = mono;
				buffer8[i * 2 + 1] = source[i * 4 + 3];
			}
			
			convert = buffer8;
			break;
	}
	
	oriTextureFileHeader fileHeader;
	
	fileHeader.h_magic	= 'O' << 4 | 'P';
	fileHeader.h_width	= width;
	fileHeader.h_height	= height;
	fileHeader.h_size	= datasize;
	fileHeader.h_format	= m_engineFormat;
	
	FILE* file = fopen([filename cStringUsingEncoding:NSASCIIStringEncoding], "wb");
	
	if (!file)
	{
		if (needfree)
			free(convert);
		
		return RES_ERR_FILESYSTEM;
	}
	
	fwrite(&fileHeader, sizeof(oriTextureFileHeader), 1, file);
	fwrite(convert, datasize, 1, file);
	
	fclose(file);
	
	if (needfree)
		free(convert);
	
	return RES_OK;
}


@end
