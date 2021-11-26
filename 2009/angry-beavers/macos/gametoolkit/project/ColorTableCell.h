//
//  ColorCell.h
//  OriginGameToolkit
//
//  Created by Max Vasin on 5/17/11.
//  Copyright 2011 Origin Games. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "Base.h"


@protocol ColorCellObject <NSObject>

- (NSColor*)propColor;
- (void)setPropColor:(NSColor*)color;

@end


@interface ColorTableCell : NSActionCell
{
	id<ColorCellObject>		m_object;
}

@end

@interface ColorTableCellPanel : NSColorPanel 
{
	id<ColorCellObject>		m_object;
}

- (void)runModalForObject:(id<ColorCellObject>)object;
- (void)pickColor:(id)sender;

@end
