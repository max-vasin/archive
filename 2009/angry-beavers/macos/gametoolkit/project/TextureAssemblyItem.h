//===============================================================================================
//  File:		TextureAssemblyItem.h
//  Project:	OriginGameToolkit
//  Desc:		Assembled texture item
//
//  Created by Max Vasin on 12/9/10.
//  Copyright 2010 Origin. All rights reserved.
//===============================================================================================

// **** IMPORTS ****

#import <Cocoa/Cocoa.h>
#import "BaseObject.h"

// Assembly item type enumeration

typedef enum _enAssemblyItemType
{
	eAssemblyItemSingleFile		= 0,
	eAssemblyItemDirectory		= 1	
}enAssemblyItemType;

// Assembly item dock enumeration

typedef enum _enAssemblyItemDock
{
	eAssemblyItemDockRight				= 0,
	eAssemblyItemDockBottom				= 1,
	eAssemblyitemDockNewColumn			= 2,
	eAssemblyItemDockNewRow				= 3,
	eAssemblyItemDockStatic				= 4
}enAssemblyItemDock;

// Sequence alignment enumeration

typedef enum _enSequenceAlignmentType
{
	eSequenceAlignAuto			= 0,
	eSequenceAlignRow			= 1,
	eSequenceAlignColumn		= 2
}enSequenceAlignmentType;

// Forward texture item class declaration

@class ProjectTextureItem;

// **** INTERFACE ****

@interface TextureAssemblyItem : BaseObject 
{
	ProjectTextureItem*		m_textureItem;			// Parent texture item
	
	// Data model
	NSString*				m_source;				// Source string
	NSString*				m_name;					// Item name
	NSString*				m_mask;					// Source mask
	enAssemblyItemType		m_type;					// Item source type
	enAssemblyItemDock		m_dock;					// Item docking type
	enSequenceAlignmentType	m_alignSequence;		// Image sequence alignment type
	NSInteger				m_alignValue;			// Image sequence alignment value
	NSInteger				m_staticDockPos[2];		// Static docking position
	BOOL					m_trimSequence;
	IntPoint				m_trimSequencePoint;	// Last build sequence trim point
}

// **** PROPERTIES ****

@property (readonly)	NSString*				propDescription;
@property (readonly)	NSInteger				propIndex;
@property (copy)		NSString*				propName;
@property (copy)		NSString*				propSource;
@property (copy)		NSString*				propMask;
@property (assign)		enAssemblyItemDock		propDock;
@property (assign)		enAssemblyItemType		propType;
@property (assign)		enSequenceAlignmentType	propSequenceAlignmentType;
@property (assign)		NSInteger				propSequenceAlignmentValue;
@property (readonly)	NSArray*				propTypes;
@property (readonly)	NSArray*				propDockTypes;
@property (readonly)	NSArray*				propAlignTypes;
@property (assign)		NSInteger				propStaticDockX;
@property (assign)		NSInteger				propStaticDockY;
@property (assign)		BOOL					propIsStaticDock;
@property (nonatomic, assign)	BOOL			propTrimSequence;

// **** CLASS METHODS ****

+ (NSArray*)getItemTypes;
+ (NSArray*)getDockTypes;
+ (NSArray*)getAlignTypes;

// **** METHODS ****

- (id)					initWithTextureItem:	(ProjectTextureItem*)item;
- (id)					initWithTextureItem:	(ProjectTextureItem*)item	fromXML:(NSXMLElement*)node;
- (void)				generateName;
- (NSXMLElement*)		saveXML:				(NSXMLElement*)node;

- (NSBitmapImageRep*)	build;
- (NSMutableArray*)		trimmedParts;

@end
