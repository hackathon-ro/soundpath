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
#import "LoginViewController.h"

#import "SPHTTPClient.h"
#import "BandApi.h"



static NSString* kFacebookAppId = @"465005836872056";

@implementation SoundPathAppDelegate

@synthesize navigationController;
@synthesize mainViewController;

@synthesize facebook;
@synthesize userPermissions;
@synthesize login;


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
    
    navigationController = [[UINavigationController alloc] initWithRootViewController:mainViewController];
    
    [navigationController.navigationBar setBarStyle:UIBarStyleBlackTranslucent];
    
    [window addSubview:navigationController.view];
    
    if(!login)
        self.login = [[LoginViewController alloc] initWithNibName:@"LoginViewController" bundle:nil];
    
    facebook = [[Facebook alloc] initWithAppId:kFacebookAppId andDelegate:self];
    
    // Check and retrieve authorization information
    NSUserDefaults *defaults = [NSUserDefaults standardUserDefaults];
    if ([defaults objectForKey:@"FBAccessTokenKey"] && [defaults objectForKey:@"FBExpirationDateKey"]) {
        facebook.accessToken = [defaults objectForKey:@"FBAccessTokenKey"];
        facebook.expirationDate = [defaults objectForKey:@"FBExpirationDateKey"];
    }
    
    userPermissions = [[NSArray alloc] initWithObjects: @"user_likes",@"friends_likes", nil];
    
    if (![facebook isSessionValid]) {
        
//        self.navigationController.modalPresentationStyle = UIModalPresentationFormSheet;
        [self.navigationController presentModalViewController:login animated:NO];
//        login.view.superview.autoresizingMask =
//        UIViewAutoresizingFlexibleTopMargin |
//        UIViewAutoresizingFlexibleBottomMargin;
//        login.view.frame = CGRectMake(
//                                                         login.view.superview.frame.origin.x,
//                                                         login.view.superview.frame.origin.y,
//                                                         300.0f,
//                                                         200.0f
//                                                         );
//        login.view.center = window.center;
    } else{
        NSUserDefaults *defaults = [NSUserDefaults standardUserDefaults];
        NSLog(@"token:%@",[defaults objectForKey:@"FBAccessTokenKey"]);
        [self apiFQLIMe];
        

    }
    
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
    [[self facebook] extendAccessTokenIfNeeded];
    
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

- (BOOL)application:(UIApplication *)application handleOpenURL:(NSURL *)url {
    
    return [self.facebook handleOpenURL:url];
}

- (BOOL)application:(UIApplication *)application openURL:(NSURL *)url sourceApplication:(NSString *)sourceApplication annotation:(id)annotation {
    
    return [self.facebook handleOpenURL:url];
}

- (void)fbDidLogin {
    
    DLog();
    
    [self apiFQLIMe];
    [self storeAuthData:[facebook accessToken] expiresAt:[facebook expirationDate]];
}

-(void)fbDidNotLogin:(BOOL)cancelled {
    NSLog(@"did not login");
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
    
    NSUserDefaults *defaults = [NSUserDefaults standardUserDefaults];
    
    // This callback can be a result of getting the user's basic
    // information.
    if ([result objectForKey:@"name"]) {
        
        [defaults setObject:[result objectForKey:@"name"] forKey:kName];
        
         if([result objectForKey:@"uid"]){
            [defaults setObject:[result objectForKey:@"uid"] forKey:kUid];
        }
        if([result objectForKey:@"pic"]){
            [defaults setObject:[result objectForKey:@"pic"] forKey:kImage];
        }
        
        [defaults synchronize];
        [self testBands];
        
//        [self ApiTest];
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

    [[self facebook] requestWithGraphPath:@"me/permissions" andDelegate:self];
}

- (void)apiFQLIMe {

    NSMutableDictionary *params = [NSMutableDictionary dictionaryWithObjectsAndKeys:
                                   @"SELECT uid, name, pic FROM user WHERE uid=me()", @"query",
                                   nil];
    
    [[self facebook] requestWithMethodName:@"fql.query"
                                        andParams:params
                                    andHttpMethod:@"POST"
                                      andDelegate:self];
}


- (void)storeAuthData:(NSString *)accessToken expiresAt:(NSDate *)expiresAt {
    NSUserDefaults *defaults = [NSUserDefaults standardUserDefaults];
    [defaults setObject:accessToken forKey:@"FBAccessTokenKey"];
    [defaults setObject:expiresAt forKey:@"FBExpirationDateKey"];
    [defaults synchronize];
}


#pragma mark AFTest
- (void) testBands {

    // test afnetworking - bands
    NSUserDefaults *defaults = [NSUserDefaults standardUserDefaults];
    NSString * token = [defaults objectForKey:@"FBAccessTokenKey"];
    
    NSDictionary * params = [NSDictionary dictionaryWithObjectsAndKeys:[defaults objectForKey:kUid],@"uid",token,@"token", nil];
    
    [SPHTTPClient getBands:params andBlock:^(NSArray *response) {
        if (response) {
            DLog();
            NSLog(@"%@",response);
            
            if([response count] > 0){
                
//                [SPHTTPClient getRelatedBands:params withId:[(NSDictionary*)[response objectAtIndex:0] valueForKey:@"page_id"] andBlock:^(NSArray *response) {
//                    if (response) {
//                        DLog();
//                        NSLog(@"%@",response);
//                        
//                    }
//                }];

//                [SPHTTPClient getBandInfo:params withId:[(NSDictionary*)[response objectAtIndex:0] valueForKey:@"page_id"] andBlock:^(NSDictionary *response) {
//                    if (response) {
//                        DLog();
//                        NSLog(@"%@",response);
//                        
//                    }
//                }];
                
                BandApi * bApi = [[BandApi alloc] init];
                bApi.delegate = nil;
                
                for(NSDictionary * d in response){
                    [bApi getBand:[d valueForKey:@"page_id"]];
                    
                }
                
            }
        }
    }];

}

- (void) testRelatedBands:(NSString *) page_id {
    
    // test afnetworking - bands
    NSUserDefaults *defaults = [NSUserDefaults standardUserDefaults];
    NSString * token = [defaults objectForKey:@"FBAccessTokenKey"];
    
    NSDictionary * params = [NSDictionary dictionaryWithObjectsAndKeys:[defaults objectForKey:kUid],@"uid",token,@"token", nil];
    
    [SPHTTPClient getRelatedBands:params withId:page_id andBlock:^(NSArray* response) {
        if (response) {
            NSLog(@"%@",response);
        }
    }];
    
}

- (void) testBandInfo:(NSString *) page_id {
    
    // test afnetworking - bands
    NSUserDefaults *defaults = [NSUserDefaults standardUserDefaults];
    NSString * token = [defaults objectForKey:@"FBAccessTokenKey"];
    
    NSDictionary * params = [NSDictionary dictionaryWithObjectsAndKeys:[defaults objectForKey:kUid],@"uid",token,@"token", nil];
    
    [SPHTTPClient getBandInfo:params withId:page_id andBlock:^(NSDictionary* response) {
        if (response) {
            NSLog(@"%@",response);
        }
    }];
    
}

- (void) ApiTest {
    
    BandApi * bApi = [[BandApi alloc] init];
    bApi.delegate = nil;
    
    [bApi fetchRecords];
    
}


@end
