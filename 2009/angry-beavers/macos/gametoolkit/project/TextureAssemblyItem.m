//===============================================================================================
//  File:		TextureAssemblyItem.m
//  Project:	OriginGameToolkit
//	Desc:		Texture assembly item class implementation
//
//  Created by Max Vasin on 12/9/10.
//  Copyright 2010 Origin. All rights reserved.
//===============================================================================================

// **** IMPORTS ****

#import "ProjectTextureItem.h"
#import "TextureAssemblyItem.h"
#import "SpriteAnimation.h"
#import "ImageWrapper.h"

// **** PRIVATE METHODS ****

@interface TextureAssemblyItem (Private)

- (NSBitmapImageRep*)	buildFromSourceDir;
- (NSBitmapImageRep*)	buildFromSourceFile:(BOOL)trim;
- (NSMutableArray*)		getImages:(BOOL)trim;
- (NSMutableArray*)		getDirTable;

@end

// **** IMPLEMENTATION ****

@implementation TextureAssemblyItem

// **** SYNTHESIZED PROPERTIES ****

@synthesize propSource					= m_source;
@synthesize propMask					= m_mask;
@synthesize	propType					= m_type;
@synthesize propName					= m_name;
@synthesize propSequenceAlignmentType	= m_alignSequence;
@synthesize propSequenceAlignmentValue	= m_alignValue;
@synthesize propTrimSequence			= m_trimSequence;

// **** IMPLEMENTED PROPERTIES ****

// propDescription
//-----------------------------------------------------------------------------------------------
- (NSString*)propDescription
{
	NSString* dockDesc = @"";
	
	switch (m_dock) 
	{
		case eAssemblyItemDockRight:
			dockDesc = @"right";
			break;
		case eAssemblyItemDockBottom:
			dockDesc = @"bottom";
			break;
		case eAssemblyItemDockNewRow:
			dockDesc = @"new row";
			break;
		case eAssemblyitemDockNewColumn:
			dockDesc = @"new column";
			break;
		case eAssemblyItemDockStatic:
			dockDesc = [NSString stringWithFormat:@"static at (%i:%i)", m_staticDockPos[0], m_staticDockPos[1]];
			break;
	}
	
	return [m_name stringByAppendingFormat:@": docking [%@]", dockDesc];
}

// propIndex
//-----------------------------------------------------------------------------------------------
- (NSInteger)propIndex
{
	return [m_textureItem indexOfAssemblyItem:self];
}

// propDock
//-----------------------------------------------------------------------------------------------
- (enAssemblyItemDock)propDock
{
	return m_dock;
}

- (void)setPropDock:(enAssemblyItemDock)dock
{
	m_dock = dock;
	// trick - to update observers
	self.propIsStaticDock = self.propIsStaticDock;
}

// propTypes
//-----------------------------------------------------------------------------------------------
- (NSArray*)propTypes
{
	return [TextureAssemblyItem getItemTypes];
}

// propDockTypes
//-----------------------------------------------------------------------------------------------
- (NSArray*)propDockTypes
{
	return [TextureAssemblyItem getDockTypes];
}

// propAlignTypes
//-----------------------------------------------------------------------------------------------
- (NSArray*)propAlignTypes
{
	return [TextureAssemblyItem getAlignTypes];
}

// propStaticDockX
//-----------------------------------------------------------------------------------------------
- (NSInteger)propStaticDockX
{
	return m_staticDockPos[0];
}

- (void)setPropStaticDockX:(NSInteger)val
{
	m_staticDockPos[0] = val;
}

// propStaticDockY
//-----------------------------------------------------------------------------------------------
- (NSInteger)propStaticDockY
{
	return m_staticDockPos[1];
}

- (void)setPropStaticDockY:(NSInteger)val
{
	m_staticDockPos[1] = val;
}

// propIsStaticDock
//-----------------------------------------------------------------------------------------------
- (BOOL)propIsStaticDock
{
	return (m_dock == eAssemblyItemDockStatic);
}

- (void)setPropIsStaticDock:(BOOL)val
{
	// just for observers
}


// **** CLASS METHODS IMPLEMENTATION ****

// Returns item types collection array wich represents 'enAssemblyItemType'
//-----------------------------------------------------------------------------------------------
+ (NSArray*)getItemTypes
{
	NSArray* types = [NSArray arrayWithObjects: @"Single source file",
											    @"Multiple files with source directory and mask",
											    nil];
	return types;
}

// Returns docking types collection array wich represents 'enAssemblyItemDock'
//-----------------------------------------------------------------------------------------------
+ (NSArray*)getDockTypes
{
	NSArray* types = [NSArray arrayWithObjects:	@"Right to previous item rectangle",
												@"Bottom to previous item rectangle",
												@"Start new column",
												@"Start new row",
												@"Static position",
												nil];
	return types;
}

// Returns sequence alignment types collection array wich represents 'enSequenceAlignmentType'
//-----------------------------------------------------------------------------------------------
+ (NSArray*)getAlignTypes
{
	NSArray* types = [NSArray arrayWithObjects:	@"Automatic",
												@"Fixed rows",
												@"Fixed columns",
												nil];
	return types;	
}

// Default initializer not allowed for this class
//-----------------------------------------------------------------------------------------------
- (id)init
{
	DBG_START [self log:"$CERROR - method (init) not allowed for instances of this class\n"]; DBG_END
	[super dealloc];
	return nil;
}

// Designated initializer - uses existing texture item
//-----------------------------------------------------------------------------------------------
- (id)initWithTextureItem:(ProjectTextureItem*)item
{
	if ( (self = [super init]) )
	{
		m_textureItem = item;
	}
	return self;
}

// XML decoder
//-----------------------------------------------------------------------------------------------
- (id)initWithTextureItem:(ProjectTextureItem*)item fromXML:(NSXMLElement*)node
{
	if ( (self = [self initWithTextureItem:item]) )
	{
		m_name		= [[node getFirstChildValue:@"Name"] copy];
		m_type		= [node getFirstChildInteger:@"Type"];
		m_dock		= [node getFirstChildInteger:@"Dock"];
		m_source	= [[node getFirstChildValue:@"Source"] copy];
		m_mask		= [[node getFirstChildValue:@"Mask"] copy];
		m_trimSequence = [node getFirstChildInteger:@"TrimSequence"];
		
		NSXMLElement* posNode = [node getFirstChild:@"StaticDockPos"];
		
		m_staticDockPos[0] = [posNode getFirstChildInteger:@"X"];
		m_staticDockPos[1] = [posNode getFirstChildInteger:@"Y"];
		
		NSXMLElement* alignNode = [node getFirstChild:@"SequenceAlignment"];
		
		m_alignSequence	= [alignNode getFirstChildInteger:@"Type"];
		m_alignValue	= [alignNode getFirstChildInteger:@"Value"];
	}
	return self;
}

// generateName
// Generates new name of item based on its index (used for new items)
//-----------------------------------------------------------------------------------------------
- (void)generateName
{
	m_name = [[NSString stringWithFormat:@"Assembly item %i", [m_textureItem indexOfAssemblyItem:self]] retain];
}

// saveXML
// Creates XML scheme of assembly item
//-----------------------------------------------------------------------------------------------
- (NSXMLElement*)saveXML:(NSXMLElement*)node
{
	NSXMLElement* thisNode = [node addNewChild:@"AssemblyItem"];
	
	[thisNode addNewChild:@"Name"	withValue:m_name];
	[thisNode addNewChild:@"Type"	withInteger:m_type];
	[thisNode addNewChild:@"Dock"	withInteger:m_dock];
	[thisNode addNewChild:@"Source"	withValue:m_source];
	[thisNode addNewChild:@"Mask"	withValue:m_mask];
	[thisNode addNewChild:@"TrimSequence" withInteger:m_trimSequence];
	
	NSXMLElement* posNode = [thisNode addNewChild:@"StaticDockPos"];
	
	[posNode addNewChild:@"X"		withInteger:m_staticDockPos[0]];
	[posNode addNewChild:@"Y"		withInteger:m_staticDockPos[1]];
	
	NSXMLElement* alignNode = [thisNode addNewChild:@"SequenceAlignment"];
	
	[alignNode addNewChild:@"Type"	withInteger:m_alignSequence];
	[alignNode addNewChild:@"Value"	withInteger:m_alignValue];
	
	return thisNode;
}

// Builds assembly image and return as result
//-----------------------------------------------------------------------------------------------
- (NSBitmapImageRep*)build
{
	switch (m_type)
	{
		case eAssemblyItemDirectory:
			return [self buildFromSourceDir];
		case eAssemblyItemSingleFile:
			return [self buildFromSourceFile:NO];
	}
	
	return nil;
}

// Returns array of trimmed assembly parts
//-----------------------------------------------------------------------------------------------
- (NSMutableArray*)trimmedParts
{
	switch (m_type)
	{
		case eAssemblyItemDirectory:
			return [self getImages:YES];
		case eAssemblyItemSingleFile:
			return [NSMutableArray arrayWithObject:[self buildFromSourceFile:YES]];
	}
	return nil;

}

// Builds image from source dir with mask
//-----------------------------------------------------------------------------------------------
- (NSBitmapImageRep*)buildFromSourceDir
{
	// No files loaded
	
	NSMutableArray* parts = [self getImages:NO];
	
	if (![parts count])
		return nil;
	
	// Check images size
	
	int partWidth	= [(NSBitmapImageRep*)[parts objectAtIndex:0] pixelsWide];
	int partHeight	= [(NSBitmapImageRep*)[parts objectAtIndex:0] pixelsHigh];
	
	for (NSBitmapImageRep* part in parts)
	{
		int checkWidth	= [part pixelsWide];
		int checkHeight = [part pixelsHigh];
		
		if ((partWidth != checkWidth) || (partHeight != checkHeight))
			[NSException raise:@"Build failed" format:@"Assembly item (%@) of texture (%@): size of one of images does not match", m_name, [m_textureItem propName]];
	}
	
	// All ok, combine images
	
	int wdim = 0, hdim = 0;
	
	switch (m_alignSequence)
	{
		case eSequenceAlignAuto:
			wdim = floor(sqrt([parts count]));
			hdim = ceil((double)[parts count] / (double)wdim);
			break;
			
		case eSequenceAlignRow:
			if (!m_alignValue)
				[NSException raise:@"Build failed" format:@"Assembly item (%@) of texture (%@): wrong alignment value", m_name, [m_textureItem propName]];
			
			hdim = m_alignValue;
			wdim = ceil((double)[parts count] / (double)hdim);
			break;
			
		case eSequenceAlignColumn:
			if (!m_alignValue)
				[NSException raise:@"Build failed" format:@"Assembly item (%@) of texture (%@): wrong alignment value", m_name, [m_textureItem propName]];
			
			wdim = m_alignValue;
			hdim = ceil((double)[parts count] / (double)wdim);
			break;
	}
	
	if (!wdim || !hdim)
		[NSException raise:@"Build failed" format:@"Assembly item (%@) of texture (%@): cant determine size of combined image", m_name, [m_textureItem propName]];
		
	
	// Create surface
	
	NSBitmapImageRep* surface = [[[NSBitmapImageRep alloc] initWithBitmapDataPlanes:nil
																	    pixelsWide:partWidth * wdim
																	    pixelsHigh:partHeight * hdim
																	 bitsPerSample:8
																   samplesPerPixel:4
																		  hasAlpha:YES
																		  isPlanar:NO
																    colorSpaceName:NSCalibratedRGBColorSpace
																	  bitmapFormat:0
																	   bytesPerRow:(4 * partWidth * wdim)
																	  bitsPerPixel:32] autorelease];
	// Get linked animations
	
	NSArray* linkedAnimations = [m_textureItem getLinkedAnimations:self];
	
	// Align frames
	
	for (SpriteAnimation* anim in linkedAnimations)
		[anim linkSetFramesCount:[parts count]];
	
	// Draw parts to surface
	
	[NSGraphicsContext saveGraphicsState];
	[NSGraphicsContext setCurrentContext:[NSGraphicsContext graphicsContextWithBitmapImageRep:surface]];
	
	int x = 0, y = 0, horigin = partHeight * (hdim - 1);
	NSPoint pos; 
	
	uint index = 0;
	
	for (NSBitmapImageRep* part in parts)
	{
		pos.x = x * partWidth;
		pos.y = horigin - y * partHeight;
		
		[part drawAtPoint:pos];
		
		for (SpriteAnimation* anim in linkedAnimations)
			[anim linkSetUnpackedFrame:index X:x * partWidth Y:y * partHeight W:partWidth H:partHeight];
		
		x++;
		if (x == wdim)
		{
			x = 0;
			y++;
		}
		
		index++;
	}
	
	[NSGraphicsContext restoreGraphicsState];
	
	return surface;
}

// Builds image from source file
//-----------------------------------------------------------------------------------------------
- (NSBitmapImageRep*)buildFromSourceFile:(BOOL)trim
{
	if (![m_source length])
		[NSException raise:@"Build failed" format:@"Assembly item (%@) of texture (%@): empty source string", m_name, [m_textureItem propName]];
	
	NSData* data = [NSData dataWithContentsOfFile:m_source];
	
	if (!data)
		[NSException raise:@"Build failed" format:@"Assembly item (%@) of texture (%@): cant obtain data from file (%@)", m_name, [m_textureItem propName], m_source];
	
	
	NSBitmapImageRep* source = [[NSBitmapImageRep alloc] initWithDataAndConvert:data];
	
	if (!source)
		[NSException raise:@"Build failed" format:@"Assembly item (%@) of texture (%@): cant create image from (%@)", m_name, [m_textureItem propName], m_source];
	
	// Get linked animations
	
	NSArray* linkedAnimations = [m_textureItem getLinkedAnimations:self];
	
	// Align frames
	
	if (!trim && m_trimSequence)
	{
		source = [source trimSaveX:NULL Y:NULL];
	}
	
	if (!trim)
	{
		for (SpriteAnimation* anim in linkedAnimations)
		{
			IntSize size = [source intSize];
			
			[anim linkSetFramesCount:1];
			
			[anim linkSetUnpackedFrame:0 X:0 Y:0 W:size.width H:size.height];
		}
	}
	
	if (trim)
	{
		uint trimx, trimy;
		
		source = [source trimSaveX:&trimx Y:&trimy];
		
		ImageWrapper* wrapper = [[ImageWrapper alloc] initWithImage:source andAssemblyItem:self];
		
		wrapper->m_index = 0;
		
		if (!m_trimSequence)
		{
			wrapper->m_trimX = trimx;
			wrapper->m_trimY = trimy;
		}
		
		[source release];
		
		return [wrapper autorelease];
	}
	
	
	return [source autorelease];
}

// Returns images by table
//-----------------------------------------------------------------------------------------------
- (NSMutableArray*)getImages:(BOOL)trim
{
	NSMutableArray* files = [self getDirTable];
	
	if (![files count])
		return nil;
	
	// Load all images
	
	NSMutableArray* parts = [NSMutableArray arrayWithCapacity:[files count]];
	
	uint index = 0;
	
	for (NSString* file in files)
	{
		// Skip missing files
		if ([file class] == [BaseObject class])
			continue;
		
		NSData* data = [NSData dataWithContentsOfFile:file];
		if (!data)
			[NSException raise:@"Build failed" format:@"Assembly item (%@) of texture (%@): cant read data (%@)", m_name, [m_textureItem propName], file];
		
		NSBitmapImageRep* part = [[NSBitmapImageRep alloc] initWithDataAndConvert:data];
		
		if (!part)
			[NSException raise:@"Build failed" format:@"Assembly item (%@) of texture (%@): cant create image from (%@)", m_name, [m_textureItem propName], file];
		
		if (trim)
		{
			uint trimx, trimy;
			
			part = [part trimSaveX:&trimx Y:&trimy];
			
			ImageWrapper* wrapper = [[ImageWrapper alloc] initWithImage:part andAssemblyItem:self];
			
			wrapper->m_index = index;
			
			if (!m_trimSequence)
			{
				wrapper->m_trimX = trimx;
				wrapper->m_trimY = trimy;
			}
			else
			{
				wrapper->m_trimX = trimx - m_trimSequencePoint.x;
				wrapper->m_trimY = trimy - m_trimSequencePoint.y;
			}
			
			[parts addObject:wrapper];
			
			[wrapper release];
		}
		else
			[parts addObject:part];
		
		index++;
	}
	
	if (!trim && m_trimSequence)
	{
		IntSize		curSize;
		uint		minx, miny;
		IntPoint	trimMin, trimMax;
		BOOL		start = YES;
		
		
		for (NSBitmapImageRep* part in parts)
		{
			curSize = [part trimSizeToX:&minx Y:(uint*)&miny];
			
			if (start)
			{
				trimMin.x = minx;
				trimMin.y = miny;
				trimMax.x = curSize.width + minx;
				trimMax.y = curSize.height + miny;
				
				start = NO;
			}
			else
			{
				if (minx < trimMin.x)
					trimMin.x = minx;
				
				if (miny < trimMin.y)
					trimMin.y = miny;
				
				if ((minx + curSize.width) > trimMax.x)
					trimMax.x = minx + curSize.width;
				
				if ((miny + curSize.height) > trimMax.y)
					trimMax.y = miny + curSize.height;
			}
		}
		
		NSMutableArray* trimmedParts = [NSMutableArray arrayWithCapacity:[parts count]];
		
		IntSize trimSize	= {trimMax.x - trimMin.x, trimMax.y - trimMin.y};
		m_trimSequencePoint = trimMin;
		
		for (NSBitmapImageRep* part in parts)
		{
			[trimmedParts addObject:[part trimToSize:trimSize X:trimMin.x Y:trimMin.y]];
		}
		
		parts = trimmedParts;
	}

	return parts;
}

// Returns table of directory contents with mask sorted by numbered mask part
//-----------------------------------------------------------------------------------------------
- (NSMutableArray*)getDirTable
{
	if (![m_source length])
		[NSException raise:@"Build failed" format:@"Assembly item (%@) of texture (%@): empty source string", m_name, [m_textureItem propName]];
	
	if (![m_mask length])
		[NSException raise:@"Build failed" format:@"Assembly item (%@) of texture (%@): empty file mask for source directory", m_name, [m_textureItem propName]];
	
	NSArray* maskParts = [m_mask componentsSeparatedByString:@"$N"];
	
	if ([maskParts count] != 2)
		[NSException raise:@"Build failed" format:@"Assembly item (%@) of texture (%@): invalid file mask for source directory", m_name, [m_textureItem propName]];
		
	NSFileManager* fileMan = [[[NSFileManager alloc] init] autorelease];
	
	NSError* err = nil;
	
	NSArray* fileList = [fileMan contentsOfDirectoryAtPath:m_source error:&err];
	
	if (!fileList)
		[NSException raise:@"Build failed" format:@"Assembly item (%@) of texture (%@): cant obtain contents of source directory", m_name, [m_textureItem propName]];
	
	NSMutableArray*		files = [[[NSMutableArray alloc] init] autorelease];
	NSAutoreleasePool*	localPool = [[NSAutoreleasePool alloc] init];
	
	for (NSString* file in fileList)
	{
		int numerator;
		
		NSScanner* scan = [NSScanner scannerWithString:file];
		
		if (![scan scanString:[maskParts objectAtIndex:0] intoString:NULL])
			continue;
		
		if (![scan scanInt:&numerator])
			continue;
		
		if (![scan scanString:[maskParts objectAtIndex:1] intoString:NULL])
			continue;	
		
		[files insertObjectSafe:[m_source stringByAppendingFormat:@"/%@", file] atIndex:numerator];
	}
	
	[localPool release];
	
	return files;
}


@end
