//
//  LoginViewController.h
//  SoundPath
//
//  Created by Bogdan Florescu on 10/27/12.
//
//

#import <UIKit/UIKit.h>
#import "Facebook.h"

@interface LoginViewController : UIViewController<FBSessionDelegate, FBRequestDelegate>

@property (nonatomic,assign) IBOutlet UIButton * loginBtn;

- (IBAction) fbLogin : (id) sender;
- (void)apiFQLIMe;

@end
