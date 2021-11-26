//=======================================================================================================================
//  File:		OriFont.m
//  Project:	heli
//	Desc:		Font resource class implementation
//
//  Created by Max Vasin on 6/8/11.
//  Copyright 2011 Origin. All rights reserved.
//=======================================================================================================================

// ******** IMPORTS ********

#import "OriFont.h"
#import "OriSpriteAnimationFrame.h"
#import "OriResourceManager.h"

// ******** IMPLEMENTATION ********

@implementation OriFont

//#######################################################################################################################
// PROPERTIES
//#######################################################################################################################

//-----------------------------------------------------------------------------------------------------------------------
// SYNTHESIZED PROPERTIES
//-----------------------------------------------------------------------------------------------------------------------

@synthesize propTexture		= m_texture;
@synthesize propSpace		= m_space;

//#######################################################################################################################
// CONSTRUCTORS\DESTRUCTORS
//#######################################################################################################################

//-----------------------------------------------------------------------------------------------------------------------
// Initializes instance of sprite with given resource manager
//-----------------------------------------------------------------------------------------------------------------------

- (id)initWithManager:(OriResourceManager*)resman withName:(NSString*)name
{
	if ( (self = [super initWithManager:resman withName:name]) )
	{
		memset(m_glyphs, 0, sizeof(m_glyphs));
		
		DBG_INIT_LOG_NAMED_0(self, m_name);
	}
	return self;
}


//-----------------------------------------------------------------------------------------------------------------------
// Initializes instance of sprite with given resource manager and XML data
//-----------------------------------------------------------------------------------------------------------------------

- (id)initWithManager:(OriResourceManager*)resman withName:(NSString*)name withXML:(OriXMLNode*)node
{
	if ( (self = [self initWithManager:resman withName:name]) )
	{
		if (node)
		{
			NSString* textureName = [node getFirstChildValue:@"Texture" placeholder:nil];
			
			if (textureName)
			{
				m_texture = [m_resMan getTexture:textureName];
				if (!m_texture)
					DBGLOG2A(@"OriFont: (%@) failed to get texture (%@)", m_name, textureName);
			}
			else
			{
				DBGLOG1A(@"OriFont: (%@) no texture name in XML file", m_name);
			}
			
			OriXMLNode* animRoot = [node getFirstChild:@"Animations"];
			
			if (animRoot)
			{
				for (OriXMLNode* animNode in animRoot.propChildren)
				{
					if ([[animNode getFirstChildValue:@"Name" placeholder:@""] isEqualToString:@"font"])
					{
						OriXMLNode* framesRoot = [animNode getFirstChild:@"Frames"];
						
						if (framesRoot)
						{
							oriInt totalChars = 0;
							oriInt totalWidth = 0;
							oriInt maxHeight  = 0;
							
							for (OriXMLNode* frameNode in framesRoot.propChildren)
							{
								// converts frame to glyph info
								NSString* ID = [frameNode getFirstChildValue:@"Id" placeholder:nil];
								
								if (!ID)
									continue;
								
								oriChar code = [ID characterAtIndex:0];
								
								if (code >= 256)
									continue;
								
								OriSpriteAnimationFrame* frame = [[OriSpriteAnimationFrame alloc] initWithAnimation:nil withXML:frameNode];
								
								if (!frameNode)
									continue;
								
								[frame updateTextureDepencies:m_texture];
																
								oriFontGlyph* glyph = &m_glyphs[code];
								
								glyph->size		= frame.propSize;
								glyph->mapmin	= frame.propMapMin;
								glyph->mapmax	= frame.propMapMax;
								
								[frame release];
								
								totalChars++;
								totalWidth += glyph->size.width;
								
								if (glyph->size.height > maxHeight)
									maxHeight = glyph->size.height;
							}
							
							m_space = (oriInt)((oriFloat)totalWidth / (oriFloat)totalChars);
							
							// generic space glyph
							
							oriFontGlyph* glyph = &m_glyphs[' '];
							
							glyph->size.width	= m_space;
							glyph->size.height	= maxHeight;
							glyph->mapmin.u		= 0;
							glyph->mapmin.v		= 0;
							glyph->mapmax.u		= 0;
							glyph->mapmax.v		= 0;
						}
					}
				}
			}
		}
	}
	return self;
}

//-----------------------------------------------------------------------------------------------------------------------
// Destructor
//-----------------------------------------------------------------------------------------------------------------------

- (void)dealloc
{
	[m_texture release];
	
	[super dealloc];
}


//#######################################################################################################################
// DATA ACCESS
//#######################################################################################################################

//-----------------------------------------------------------------------------------------------------------------------
// Returns font glyph by char code
//-----------------------------------------------------------------------------------------------------------------------

- (oriFontGlyph*)glyph:(oriChar)code
{
	if (code > 255)
		return NULL;
	
	return &m_glyphs[code];
}



@end
