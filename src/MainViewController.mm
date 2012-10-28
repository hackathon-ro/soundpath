//
//  SoundPath MainViewController
//  SoundPath
//
//  Created by Leonard Chioveanu on 10/27/12.
//
//

#import <QuartzCore/QuartzCore.h>
#import "cinder/app/CinderViewCocoaTouch.h"

#import "MainViewController.h"
#import "SoundPathAppDelegate.h"
#import "SPUtils.h"
#import "Graph.h"

#import "BrowserViewController.h"

#import "SPHTTPClient.h"
#import "Band.h"

@interface MainViewController ()
{
    SoundPathApp *app;

    CinderViewCocoaTouch * cinderView;
}

@end

/**
 * Gesture Stack.
 */
@interface MainViewController (GestureHelpers)
- (void)pinched:(UIPinchGestureRecognizer*)recognizer; 
@end

@implementation MainViewController
@synthesize popOver;
@synthesize data;

#pragma mark -
#pragma mark View lifecycle

- (void)loadView
{
    [super loadView];
    FLog();
    
    if(self.view == nil)
    {
        CGRect frame = iPad ? CGRectMake(0, 0, 768, 1024) : CGRectMake(0, 0, 320, 480);
        if (UIDeviceOrientationIsLandscape([[UIDevice currentDevice] orientation])) {
            frame = iPad ? CGRectMake(0, 0, 1024, 768) : CGRectMake(0, 0, 480, 320);
        }
        
        self.view = [[UIView alloc] initWithFrame:frame];
        self.view.autoresizingMask = UIViewAutoresizingFlexibleWidth|UIViewAutoresizingFlexibleHeight;
        self.view.multipleTouchEnabled = YES;
        self.view.userInteractionEnabled = YES;
        // make sure we can see the CinderView through our view
        self.view.opaque = NO;
        self.view.backgroundColor = [UIColor clearColor];
    }
    
    UIWindow *window = [[UIApplication sharedApplication] keyWindow];
    
    // get CinderView
    for (UIView *subview in window.subviews) {
        if ([subview isKindOfClass:NSClassFromString(@"CinderViewCocoaTouch")]) {
            cinderView = (CinderViewCocoaTouch*) subview;
            break;
        }
    }
}

- (void) showPopover {
    NSURL *url = [NSURL URLWithString:@"http://www.youtube.com/results?search_query=metallica+unforgiven"];
    BrowserViewController *bvc = [[BrowserViewController alloc] initWithUrls:url];
    
//    [self.navigationController pushViewController:bvc animated:YES];
    if(!popOver) {
        popOver = [[UIPopoverController alloc] initWithContentViewController:bvc];
        popOver.delegate = self;
    }
        [popOver setPopoverContentSize:CGSizeMake(300, 300)];
        [popOver presentPopoverFromBarButtonItem:self.navigationItem.rightBarButtonItem permittedArrowDirections:UIPopoverArrowDirectionUp animated:YES];
    
   
}


- (void) showInfo {
//    NSURL *url = [NSURL URLWithString:@"http://www.youtube.com/results?search_query=metallica+unforgiven"];
//    BrowserViewController *bvc = [[BrowserViewController alloc] initWithUrls:url];
    
//    [self.navigationController pushViewController:bvc animated:YES];
//    if(!popOver)
//        popOver = [[UIPopoverController alloc] initWithContentViewController:bvc];
//
//    [popOver presentPopoverFromBarButtonItem:self.navigationItem.rightBarButtonItem permittedArrowDirections:UIPopoverArrowDirectionDown animated:YES];
}


-(void)viewDidLoad {
	[super viewDidLoad];
	FLog();
   
    UIBarButtonItem * btn = [[UIBarButtonItem alloc] initWithBarButtonSystemItem:UIBarButtonSystemItemCompose target:self action:@selector(showPopover)];
    self.navigationItem.rightBarButtonItem = btn;

    UIBarButtonItem * btn2 = [[UIBarButtonItem alloc] initWithBarButtonSystemItem:UIBarButtonSystemItemBookmarks target:self action:@selector(showInfo)];
    self.navigationItem.leftBarButtonItem = btn2;

    
    // pinch gesture recognizer
    UIPinchGestureRecognizer *pinchGesture = [[UIPinchGestureRecognizer alloc] initWithTarget:self action:@selector(pinched:)];
    [self.view addGestureRecognizer:pinchGesture];
    
    // notifications
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(activate) name: UIApplicationDidBecomeActiveNotification object:nil];
    
    data = [[NSMutableDictionary  alloc] init];
    
}

- (void) viewWillAppear:(BOOL)animated
{
    [super viewWillAppear:animated];
    
    app = (SoundPathApp*)cinder::app::AppCocoaTouch::get();
    app->interopDelegate = self;
    app->initMe();
    
}

- (void) viewDidAppear:(BOOL)animated
{
    [super viewDidAppear:animated];
    
    NSUserDefaults *defaults = [NSUserDefaults standardUserDefaults];
    NSString * token = [defaults objectForKey:@"FBAccessTokenKey"];
    NSString * uid = [defaults objectForKey:kUid];
    
    NSDictionary * params = [NSDictionary dictionaryWithObjectsAndKeys:uid,@"uid",token,@"token", nil];
    
    [SPHTTPClient getBands:params andBlock:^(NSArray *response) {
        if (response) {
            DLog();
            NSLog(@"%@",response);
            
            if([response count] > 0){
                
                for(NSDictionary * d  in response)
                {
                    NSString * key = [NSString stringWithFormat:@"%@",[d objectForKey:@"id"]];
                    [data setObject:d forKey:key];
                }
                
                app->loaded(-1, data);
            }
        }
    }];
    
}

- (void) viewDidDisappear:(BOOL)animated
{
    [super viewDidDisappear:animated];
}



#pragma mark - SoundPathInteractionDelegate
- (void)nodeInfo:(int)nid
{
    DLog();
    
}

- (void) nodeTapped:(int)nid
{
    NSDictionary * artist = [data objectForKey:[NSString stringWithFormat:@"%d", nid]];
                              
    NSString * page_id = [artist valueForKey:@"page_id"];

    NSUserDefaults *defaults = [NSUserDefaults standardUserDefaults];
    NSString * token = [defaults objectForKey:@"FBAccessTokenKey"];
    NSString * uid = [defaults objectForKey:kUid];
    
    NSDictionary * params = [NSDictionary dictionaryWithObjectsAndKeys:uid,@"uid",token,@"token", nil];
    
    [SPHTTPClient getRelatedBands:params withId:page_id andBlock:^(NSArray *response) {
        
        if (response) {
            DLog();
            NSLog(@"%@",response);
            
            if([response count] > 0){
                NSMutableDictionary * result = [[NSMutableDictionary  alloc] init];
                
                for(NSDictionary * d  in response)
                {
                    NSString * key = [NSString stringWithFormat:@"%@",[d objectForKey:@"id"]];
                    
                    [result setObject:d forKey:key];
                    [data setObject:d forKey:key];
                }
                
                app->loaded(nid, result);
            }
        }
    }];
}

#pragma mark -
#pragma mark Application

/*
 * Activates the controller.
 */
- (void)activate {
    FLog();
}

/**
 * Quit Cinder App.
 */
- (void)quit {
    NSLog(@"Aus die Maus.");
    
    // quit app
    app->quit();
    abort();
}

- (void)reset {
    DLog();
    
    // app reset
    app->reset();
}

#pragma mark -
#pragma mark Rotation support

- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation {
    
    return YES;
}

/*
 * Prepare rotation animation.
 */
- (void)willAnimateRotationToInterfaceOrientation:(UIInterfaceOrientation)toInterfaceOrientation duration:(NSTimeInterval)duration {
    
    // app
    app->applyDeviceOrientation(toInterfaceOrientation);
 
    // animate cinder
    [UIView beginAnimations:@"flip" context:nil];
    [UIView setAnimationDuration:0]; // animation distorts view
    
    // flip 
    if (toInterfaceOrientation == UIInterfaceOrientationPortrait) {      
        cinderView.transform = CGAffineTransformIdentity;
        cinderView.transform = CGAffineTransformMakeRotation(0);
        cinderView.bounds = iPad ? CGRectMake(0.0, 0.0, 768, 1024) : CGRectMake(0.0, 0.0, 320, 480);
    }
    else if (toInterfaceOrientation == UIInterfaceOrientationLandscapeRight) {      
        cinderView.transform = CGAffineTransformIdentity;
        cinderView.transform = CGAffineTransformMakeRotation(M_PI * 0.5);
        cinderView.bounds = iPad ? CGRectMake(0.0, 0.0, 1024, 768) : CGRectMake(0.0, 0.0, 480, 320);
    }
    else if (toInterfaceOrientation == UIInterfaceOrientationPortraitUpsideDown) {      
        cinderView.transform = CGAffineTransformIdentity;
        cinderView.transform = CGAffineTransformMakeRotation(M_PI);
        cinderView.bounds = iPad ? CGRectMake(0.0, 0.0, 768, 1024) : CGRectMake(0.0, 0.0, 320, 480);
    }
    else if (toInterfaceOrientation == UIInterfaceOrientationLandscapeLeft) {      
        cinderView.transform = CGAffineTransformIdentity;
        cinderView.transform = CGAffineTransformMakeRotation(- M_PI * 0.5);
        cinderView.bounds = iPad ? CGRectMake(0.0, 0.0, 1024, 768) : CGRectMake(0.0, 0.0, 480, 320);
    }
    [UIView commitAnimations];
}

#pragma mark -
#pragma mark Touch

/*
 * Touches.
 */
- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event {
    GLog();
    
    // forward to cinder
    [cinderView touchesBegan:touches withEvent:event]; 
}
- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event {
    GLog();
    
    // forward to cinder
    [cinderView touchesMoved:touches withEvent:event]; 
}
- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event{
    GLog();
    
    // forward to cinder
    [cinderView touchesEnded:touches withEvent:event]; 
}
- (void)touchesCancelled:(NSSet *)touches withEvent:(UIEvent *)event {
    GLog();
    
    // forward to cinder
    [cinderView touchesCancelled:touches withEvent:event]; 
}

#pragma mark -
#pragma mark Gestures

/*
 * Pinched.
 */
- (void)pinched:(UIPinchGestureRecognizer *)recognizer {
    GLog();
    
    // forward to cinder
    app->pinched(recognizer);
}


- (void) popoverControllerDidDismissPopover:(UIPopoverController *)popoverController {
    self.popOver = nil;
}

#pragma mark -
#pragma mark Memory management

- (void)didReceiveMemoryWarning
{
    DLog();
}

- (void)dealloc {
	GLog();
    
    // notifications
    [[NSNotificationCenter defaultCenter] removeObserver:self];
	
	// release global
}


#pragma mark - API


@end
