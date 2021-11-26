//
//  GLFont.h
//  OriginGameToolkit
//
//  Created by Max Vasin on 4/26/11.
//  Copyright 2011 Origin Games. All rights reserved.
//

#import <Cocoa/Cocoa.h>

@interface GLGlyph : NSObject 
{

}

@end


@interface GLFont : NSObject 
{
	GLuint				m_glTexture;
}

- (id)	initWithName:(NSString*)name	Size:(CGFloat)size;


@end
