//
//  SoundPathAppDelegate.h
//  SoundPath
//
//  Created by Leonard Chioveanu on 10/27/12.
//
//

#import <UIKit/UIKit.h>
#import "Facebook.h"

#include "cinder/app/AppCocoaTouch.h"
#include "cinder/app/CinderAppDelegateIPhone.h"

@class LoginViewController;

@interface SoundPathAppDelegate : CinderAppDelegateIPhone<FBRequestDelegate,FBSessionDelegate> {
    Facebook *facebook;
    NSArray *userPermissions;
}

@property (nonatomic, strong) UINavigationController * navigationController;
@property (nonatomic, strong) UIViewController * mainViewController;
@property (strong, nonatomic) Facebook * facebook;
@property (strong, nonatomic) LoginViewController * login;
@property (nonatomic, strong) NSArray *userPermissions;

// test AFHTTP methods
- (void) testBands;
@end
