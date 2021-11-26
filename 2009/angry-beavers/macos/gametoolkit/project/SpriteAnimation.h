//=========================================================================================================================
//  File:		SpriteAnimation.h
//  Project:	OriginGameToolkit
//	Desc:		Sprite animation containers interfaces
//
//  Created by Max Vasin on 3/4/11.
//  Copyright 2011 Origin Games. All rights reserved.
//=========================================================================================================================

// ***** IMPORTS *****

#import <Foundation/Foundation.h>
#import "Base.h"
#import "BaseObject.h"

@class ProjectSpriteItem;
@class ProjectTextureItem;
@class TextureAssemblyItem;
@class SpriteAnimation;

//#########################################################################################################################
// AnimationLink INTERFACE
//#########################################################################################################################

@interface AnimationLink : BaseObject 
{
	SpriteAnimation*		m_animation;
	TextureAssemblyItem*	m_textureAssemly;
	int						m_frameOffset;
	BOOL					m_mirrorSequenceX;
	BOOL					m_mirrorSequenceY;
	BOOL					m_mirrorX;
	BOOL					m_mirrorY;
	uint					m_mirrorStart;
	uint					m_mirrorEnd;
}

// ***** PROPERTIES *****

@property (nonatomic, assign)	TextureAssemblyItem*	propAssembly;
@property (nonatomic, assign)	int						propFrameOffset;
@property (nonatomic, assign)	ProjectSpriteItem*		propSprite;
@property (nonatomic, assign)	SpriteAnimation*		propAnimation;
@property (nonatomic, assign)	NSMutableArray*			propAssemblyItems;
@property (nonatomic, assign)	BOOL					propMirrorSequenceX;
@property (nonatomic, assign)	BOOL					propMirrorSequenceY;
@property (nonatomic, assign)	BOOL					propMirrorX;
@property (nonatomic, assign)	BOOL					propMirrorY;
@property (nonatomic, assign)	uint					propMirrorStart;
@property (nonatomic, assign)	uint					propMirrorEnd;

// ***** INSTANCE METHODS *****

- (id)				initWithAnimation:(SpriteAnimation*)animation;
- (id)				initWithAnimation:(SpriteAnimation*)animation	XML:(NSXMLElement*)node;

- (NSXMLElement*)	saveXML:(NSXMLElement*)node;

- (void)			delLink;

@end


//#########################################################################################################################
// FramePoint INTERFACE
//#########################################################################################################################

@interface FramePoint : BaseObject
{
	SpriteAnimation*	m_animation;
	NSString*			m_name;
	IntPoint			m_pos;
	IntPoint			m_packOffset;
	NSColor*			m_color;
}

// ***** PROPERTIES *****

@property (nonatomic, copy)		NSString*	propName;
@property (nonatomic, copy)		NSColor*	propColor;
@property (nonatomic, assign)	int			propPositionX;
@property (nonatomic, assign)	int			propPositionY;
@property (nonatomic, assign)	int			propPackOffsetX;
@property (nonatomic, assign)	int			propPackOffsetY;

// ***** INSTANCE METHODS *****

- (id)				initWithAnimation:(SpriteAnimation*) animation	Name:(NSString*)name;
- (id)				initWithAnimation:(SpriteAnimation*) animation	XML:(NSXMLElement*)node;

- (NSXMLElement*)	saveXML:(NSXMLElement*)node;
- (NSXMLElement*)	buildXML:(NSXMLElement*)node;

@end


//#########################################################################################################################
// AnimationFrame INTERFACE
//#########################################################################################################################

@interface AnimationFrame : BaseObject
{
	SpriteAnimation*	m_animation;		// Parent animation
	IntPoint			m_position;			// Frame absolute position
	IntSize				m_size;				// Frame size
	IntPoint			m_packPosition;		// Frame absolute packed texture position
	IntSize				m_packSize;			// Frame packed size
	IntPoint			m_packOffset;		// Points packed offset
	BOOL				m_mirrorX;			// Frame mirrored by X axis
	BOOL				m_mirrorY;			// Frame mirrored by Y axis
	NSMutableArray*		m_points;			// Frame points
	FramePoint*			m_currentPoint;		// Current point (design)
	NSString*			m_identifier;		// Frame string identifier
}

// ***** PROPERTIES *****

@property (assign)		NSInteger			propPositionX;
@property (assign)		NSInteger			propPositionY;
@property (assign)		NSInteger			propWidth;
@property (assign)		NSInteger			propHeight;

@property (assign)		NSInteger			propPackPositionX;
@property (assign)		NSInteger			propPackPositionY;
@property (assign)		NSInteger			propPackWidth;
@property (assign)		NSInteger			propPackHeight;
@property (assign)		NSInteger			propPackOffsetX;
@property (assign)		NSInteger			propPackOffsetY;

@property (copy)		NSString*			propIdentifier;


@property (assign)		NSMutableArray*		propPoints;
@property (assign)		FramePoint*			propCurrentPoint;
@property (readonly)	FramePoint*			propOriginPoint;
@property (assign)		BOOL				propMirrorX;
@property (assign)		BOOL				propMirrorY;


// ***** INSTANCE METHODS *****

- (id)				initWithAnimation:(SpriteAnimation*)anim;
- (id)				initWithAnimation:(SpriteAnimation*)anim fromXML:(NSXMLElement*)node;

- (void)			addPoint:(NSString*)name	Color:(NSColor*)color;
- (void)			delPoint:(NSString*)name;
- (FramePoint*)		getPoint:(NSString*)name;
- (void)			renamePoint:(NSString*)nameOld	to:(NSString*)name;
- (void)			changeColor:(NSString*)name     Color:(NSColor*)color;

- (NSXMLElement*)	saveXML:	(NSXMLElement*)node;
- (NSXMLElement*)	buildXML:	(NSXMLElement*)node;

@end

//#########################################################################################################################
// SpriteAnimation INTERFACE
//#########################################################################################################################

@interface SpriteAnimation : BaseObject
{
	ProjectSpriteItem*	m_sprite;				// Parent sprite item
	
	NSString*			m_name;					// Animation name
	NSInteger			m_speed;				// Animation speed (fps)
	NSMutableArray*		m_frames;				// Animation frames collection
	NSMutableArray*		m_framePoints;			// Animation common frame points
	
	AnimationFrame*		m_currentFrame;			// Current animation frame (design)
	FramePoint*			m_currentFramePoint;	// Current frame point (design)
	FramePoint*			m_originFramePoint;		// Origin frame point used by this animation
	NSInteger			m_gridSpaceH;			// Grid spacing (design)
	NSInteger			m_gridSpaceV;			// Grid spacing (design)
	IntPoint			m_gridOrigin;			// Grid origin (design)
	AnimationLink*		m_link;					// Link animation to texture assembly item
	uint				m_linkFramesCount;		// True images count
}

// ***** PROPERTIES *****

@property (copy)		NSString*			propName;
@property (readonly)	NSMutableArray*		propFrames;
@property (assign)		NSInteger			propFramesCount;
@property (assign)		NSInteger			propCurrentFrameIndex;
@property (assign)		NSInteger			propSpeed;
@property (assign)		AnimationFrame*		propCurrentFrame;
@property (assign)		FramePoint*			propCurrentPoint;
@property (assign)		FramePoint*			propOriginPoint;
@property (readonly)	NSMutableArray*		propFramePoints;
@property (assign)		NSString*			propFrameStatistic;
@property (assign)		NSInteger			propGridSpaceH;
@property (assign)		NSInteger			propGridSpaceV;
@property (assign)		NSInteger			propGridOriginX;
@property (assign)		NSInteger			propGridOriginY;
@property (readonly)	ProjectSpriteItem*	propSprite;
@property (readonly)	AnimationLink*		propLink;

// ***** INSTANCE METHODS *****

- (id)					initWithSprite:		(ProjectSpriteItem*)sprite;
- (id)					initWithSprite:		(ProjectSpriteItem*)sprite fromXML:(NSXMLElement*)node;
- (NSXMLElement*)		saveXML:			(NSXMLElement*)node;
- (NSXMLElement*)		buildXML:			(NSXMLElement*)node;

- (IBAction)			addFrame:			(id)sender;
- (IBAction)			delFrame:			(id)sender;
- (IBAction)			selectNextFrame:	(id)sender;
- (IBAction)			selectPrevFrame:	(id)sender;

- (IBAction)			addFramePoint:		(id)sender;
- (IBAction)			delFramePoint:		(id)sender;
- (FramePoint*)			getFramePoint:		(NSString*)name;

- (ProjectTextureItem*)	getSpriteTexture;

// This methods used by texture assembly item to update linked animations
- (void)				linkSetFramesCount:			(uint)count;
- (void)				linkSetUnpackedFrame:		(uint)index X:(uint)x Y:(uint)y W:(uint)w H:(uint)h;
- (void)				linkSetUnpackedOffsetX:		(uint)x		Y:(uint)y;
- (void)				linkSetPackedFrame:			(uint)index X:(uint)x Y:(uint)y W:(uint)w H:(uint)h OffX:(uint)offx OffY:(uint)offy;

@end
