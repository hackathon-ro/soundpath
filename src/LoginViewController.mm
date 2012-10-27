//
//  LoginViewController.m
//  SoundPath
//
//  Created by Bogdan Florescu on 10/27/12.
//
//

#import "LoginViewController.h"
#import "SoundPathAppDelegate.h"


@interface LoginViewController ()
{
    SoundPathAppDelegate * appDelegate;
    UIAlertView * fbFailAlertView;
}
@end

@implementation LoginViewController

@synthesize loginBtn;

- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil
{
    self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
    if (self) {
        // Custom initialization
    }
    return self;
}

- (void)viewDidLoad
{
    [super viewDidLoad];
    // Do any additional setup after loading the view from its nib.
    appDelegate = (SoundPathAppDelegate*)[[UIApplication sharedApplication] delegate];

  }

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

- (void)viewWillAppear:(BOOL)animated
{
    [super viewWillAppear:animated];
    [appDelegate facebook].sessionDelegate = self;
//    [appDelegate setFacebook:facebook];
}

- (void)viewDidAppear:(BOOL)animated
{
    [super viewDidAppear:animated];
}

- (void)viewWillDisappear:(BOOL)animated
{
    [super viewWillDisappear:animated];
    [appDelegate facebook].sessionDelegate = appDelegate;
}

- (void) showFbLoginFail
{
    if (fbFailAlertView == nil) {
        fbFailAlertView = [[UIAlertView alloc] initWithTitle:NSLocalizedString(@"Something went wrong", @"Something went wrong")
                                                     message:NSLocalizedString(@"Failed to login on Facebook. Try again later", @"Facebook login fail")
                                                    delegate:self cancelButtonTitle:NSLocalizedString(@"OK", @"OK")
                                           otherButtonTitles: nil];
    }
    
    [fbFailAlertView show];
}

- (void) showFbFail
{
    if (fbFailAlertView == nil) {
        fbFailAlertView = [[UIAlertView alloc] initWithTitle:NSLocalizedString(@"Something went wrong", @"Something went wrong")
                                                     message:NSLocalizedString(@"Failed to post video on Facebook. Try again later", @"Facebook fail")
                                                    delegate:self cancelButtonTitle:NSLocalizedString(@"OK", @"OK")
                                           otherButtonTitles: nil];
    }
    
    [fbFailAlertView show];
}


#pragma mark - Facebook handlers

- (IBAction) fbLogin : (id) sender
{
    
    if (![appDelegate.facebook isSessionValid]) {
        
        [appDelegate.facebook authorize:[appDelegate userPermissions]];
        
    }else {
        NSUserDefaults *defaults = [NSUserDefaults standardUserDefaults];
        NSLog(@"token:%@",[defaults objectForKey:@"FBAccessTokenKey"]);
        [self apiFQLIMe];
    }
}

- (void)storeAuthData:(NSString *)accessToken expiresAt:(NSDate *)expiresAt {
    NSUserDefaults *defaults = [NSUserDefaults standardUserDefaults];
    [defaults setObject:accessToken forKey:@"FBAccessTokenKey"];
    [defaults setObject:expiresAt forKey:@"FBExpirationDateKey"];
    [defaults synchronize];
}

- (void)fbDidLogin {
    
    DLog();
    
    [self apiFQLIMe];
    [self storeAuthData:[appDelegate.facebook accessToken] expiresAt:[appDelegate.facebook expirationDate]];
    
    NSUserDefaults *defaults = [NSUserDefaults standardUserDefaults];
    NSLog(@"token:%@",[defaults objectForKey:@"FBAccessTokenKey"]);
    
    [self dismissModalViewControllerAnimated:YES];
}

-(void)fbDidNotLogin:(BOOL)cancelled {
    NSLog(@"did not login");
    
    [self showFbLoginFail];
}

-(void)fbDidLogout
{
    NSUserDefaults *defaults = [NSUserDefaults standardUserDefaults];
    [defaults removeObjectForKey:@"FBAccessTokenKey"];
    [defaults removeObjectForKey:@"FBExpirationDateKey"];
    [defaults synchronize];
}

-(void)fbDidExtendToken:(NSString *)accessToken expiresAt:(NSDate *)expiresAt
{   NSLog(@"token extended");
    [self storeAuthData:accessToken expiresAt:expiresAt];
    
}

- (void)fbSessionInvalidated
{
    [self fbDidLogout];
}

#pragma mark - FBRequestDelegate Methods
/**
 * Called when the Facebook API request has returned a response.
 *
 * This callback gives you access to the raw response. It's called before
 * (void)request:(FBRequest *)request didLoad:(id)result,
 * which is passed the parsed response object.
 */
- (void)request:(FBRequest *)request didReceiveResponse:(NSURLResponse *)response {
    NSLog(@"received response");
}

/**
 * Called when a request returns and its response has been parsed into
 * an object.
 *
 * The resulting object may be a dictionary, an array or a string, depending
 * on the format of the API response. If you need access to the raw response,
 * use:
 *
 * (void)request:(FBRequest *)request
 *      didReceiveResponse:(NSURLResponse *)response
 */
- (void)request:(FBRequest *)request didLoad:(id)result {
    if ([result isKindOfClass:[NSArray class]]) {
        result = [result objectAtIndex:0];
    }
    // This callback can be a result of getting the user's basic
    // information or getting the user's permissions.
    if ([result objectForKey:@"name"]) {
        // If basic information callback, set the UI objects to
        // display this.
        NSLog(@"name:%@\nuid:%@",[result objectForKey:@"name"],[result objectForKey:@"uid"]);
        // Get the profile image
//        UIImage *image = [UIImage imageWithData:[NSData dataWithContentsOfURL:[NSURL URLWithString:[result objectForKey:@"pic"]]]];
        NSLog(@"iamge:%@",[result objectForKey:@"pic"]);
        // Resize, crop the image to make sure it is square and renders
        // well on Retina display
//        float ratio;
//        float delta;
//        float px = 100; // Double the pixels of the UIImageView (to render on Retina)
//        CGPoint offset;
//        CGSize size = image.size;
//        if (size.width > size.height) {
//            ratio = px / size.width;
//            delta = (ratio*size.width - ratio*size.height);
//            offset = CGPointMake(delta/2, 0);
//        } else {
//            ratio = px / size.height;
//            delta = (ratio*size.height - ratio*size.width);
//            offset = CGPointMake(0, delta/2);
//        }
//        CGRect clipRect = CGRectMake(-offset.x, -offset.y,
//                                     (ratio * size.width) + delta,
//                                     (ratio * size.height) + delta);
//        UIGraphicsBeginImageContext(CGSizeMake(px, px));
//        UIRectClip(clipRect);
//        [image drawInRect:clipRect];
//        UIImage *imgThumb = UIGraphicsGetImageFromCurrentImageContext();
//        UIGraphicsEndImageContext();
//        [profilePhotoImageView setImage:imgThumb];
        
        [self apiGraphUserPermissions];
    } else {
        // Processing permissions information
//        HackbookAppDelegate *delegate = (HackbookAppDelegate *)[[UIApplication sharedApplication] delegate];
        [appDelegate setUserPermissions:[[result objectForKey:@"data"] objectAtIndex:0]];
    }
}

/**
 * Called when an error prevents the Facebook API request from completing
 * successfully.
 */
- (void)request:(FBRequest *)request didFailWithError:(NSError *)error {
    NSLog(@"Err message: %@", [[error userInfo] objectForKey:@"error_msg"]);
    NSLog(@"Err code: %d", [error code]);
}

- (void)apiGraphUserPermissions {
//    SoundPathAppDelegate *delegate = (HackbookAppDelegate *)[[UIApplication sharedApplication] delegate];
    [[appDelegate facebook] requestWithGraphPath:@"me/permissions" andDelegate:self];
}

- (void)apiFQLIMe {
    // Using the "pic" picture since this currently has a maximum width of 100 pixels
    // and since the minimum profile picture size is 180 pixels wide we should be able
    // to get a 100 pixel wide version of the profile picture
    NSMutableDictionary *params = [NSMutableDictionary dictionaryWithObjectsAndKeys:
                                   @"SELECT uid, name, pic FROM user WHERE uid=me()", @"query",
                                   nil];

    [[appDelegate facebook] requestWithMethodName:@"fql.query"
                                     andParams:params
                                 andHttpMethod:@"POST"
                                   andDelegate:self];
}

@end
