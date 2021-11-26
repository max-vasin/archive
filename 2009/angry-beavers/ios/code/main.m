//=======================================================================================================================
//  File:		main.m
//  Project:	heli
//	Desc:		application entry point
//
//  Created by Max Vasin on 3/21/11.
//  Copyright 2011 Origin Games. All rights reserved.
//=======================================================================================================================

#import <UIKit/UIKit.h>

int main(int argc, char *argv[]) 
{
    NSAutoreleasePool * pool = [[NSAutoreleasePool alloc] init];
    
	int appResult = UIApplicationMain(argc, argv, nil, nil);
    
	[pool release];
    
	return appResult;
}
