//
//  OMCCommandMenu.h
//  Abracode
//
//  Created by Tomasz Kukielka on 4/6/08.
//  Copyright 2008 Abracode. All rights reserved.
//

#import <Cocoa/Cocoa.h>

IB_DESIGNABLE
@interface OMCCommandMenu : NSMenu
{
	NSString *	commandFilePath;
}

@property(retain) IBInspectable NSString * commandFilePath;

- (void)executeCommand:(id)sender;

@end