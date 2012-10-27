//
//  SoundPathAppDelegate.mm
//  SoundPath
//
//  Created by Leonard Chioveanu on 10/27/12.
//
//

#import "SoundPathAppDelegate.h"
#import "MainViewController.h"
#import "SPUtils.h"

@implementation SoundPathAppDelegate
@synthesize mainViewController;

#pragma mark -
#pragma mark Application lifecycle

- (void) applicationDidFinishLaunching:(UIApplication *)application {
    
    // customize appearance
	[[UIApplication sharedApplication] setStatusBarHidden:YES withAnimation:UIStatusBarAnimationNone];
    
    // cinder
    [super applicationDidFinishLaunching:application];
    
    UIWindow *window = [[UIApplication sharedApplication] keyWindow];
    
    NSString * nibName = iPhone ?
                        [NSString stringWithFormat:@"%@-iPhone", [[MainViewController class] description]] : [NSString stringWithFormat:@"%@-iPad", [[MainViewController class] description]];
 
    mainViewController = [[MainViewController alloc] initWithNibName:nibName bundle:nil];
    
    [window addSubview:mainViewController.view];
}

- (void)applicationWillEnterForeground:(UIApplication *)application {
    DLog();
    
    // cinder
    [super applicationWillEnterForeground:application];
    
}

- (void) applicationDidBecomeActive:(UIApplication *)application {
    DLog();
    
    // cinder
    [super applicationDidBecomeActive:application];
    
}

- (void) applicationWillResignActive:(UIApplication *)application {
    DLog();
    
    // cinder
    [super applicationWillResignActive:application];
    
}
- (void)applicationDidEnterBackground:(UIApplication *)application {
    DLog();
    
    // cinder
    [super applicationDidEnterBackground:application];
}

- (void)applicationWillTerminate:(UIApplication *)application {
    DLog();
    
    // cinder
    [super applicationWillTerminate:application];
}

@end
