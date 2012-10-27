//
//  SoundPathAppDelegate.h
//  SoundPath
//
//  Created by Leonard Chioveanu on 10/27/12.
//
//

#import <UIKit/UIKit.h>

#include "cinder/app/AppCocoaTouch.h"
#include "cinder/app/CinderAppDelegateIPhone.h"

@interface SoundPathAppDelegate : CinderAppDelegateIPhone

@property (nonatomic, strong) UINavigationController * navigationController;
@property (nonatomic, strong) UIViewController * mainViewController;

@end
