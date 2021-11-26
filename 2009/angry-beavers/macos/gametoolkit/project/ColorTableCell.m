//
//  ColorCell.m
//  OriginGameToolkit
//
//  Created by Max Vasin on 5/17/11.
//  Copyright 2011 Origin Games. All rights reserved.
//

#import "ColorTableCell.h"

@implementation ColorTableCellPanel

- (id)init
{
	if ( (self = [super init]) )
	{
		[self setReleasedWhenClosed:YES];
		[self setContinuous:YES];
	}
	return self;
}

- (void)runModalForObject:(id<ColorCellObject>)object
{
	m_object = object;
	
	[self setColor:(NSColor*)[object propColor]];
	[self setTarget:self];
	[self setAction:@selector(pickColor:)];
	
	[NSApp runModalForWindow:self];
}

- (void)close
{
	[NSApp stopModal];
	
	[super close];
}

- (void)pickColor:(id)sender
{
	[m_object setPropColor:[self color]];
}

@end



@implementation ColorTableCell

- (void)setObjectValue:(id<NSCopying>)obj
{
	NSColor* color = [(id<ColorCellObject>)obj propColor];
	
	[super setObjectValue:color];
	
	m_object	= (id<ColorCellObject>)obj;
}

- (void)drawInteriorWithFrame:(NSRect)frame inView:(NSView *)controlView 
{
	[super drawInteriorWithFrame:frame inView:controlView];
	
	NSColor* color = nil;
	
	if ([self hasValidObjectValue] && [[[self objectValue] class] isSubclassOfClass:[NSColor class]])
		color = (NSColor*)[self objectValue];
	
	if (color) 
	{
        [color set];
        NSRectFill(frame);
        [[NSColor lightGrayColor] set];
        NSFrameRectWithWidth(frame, 1.0);
    }
}

- (NSUInteger)hitTestForEvent:(NSEvent *)event inRect:(NSRect)cellFrame ofView:(NSView *)controlView
{
	return NSCellHitTrackableArea;
}

- (BOOL)trackMouse:(NSEvent*)theEvent inRect:(NSRect)frame ofView:(NSView *)controlView untilMouseUp:(BOOL)flag 
{
	ColorTableCellPanel* picker = [[ColorTableCellPanel alloc] init];
	
	[picker runModalForObject:m_object];
	
	return YES;
}


@end
