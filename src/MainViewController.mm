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

-(void)viewDidLoad {
	[super viewDidLoad];
	FLog();
   
    // pinch gesture recognizer
    UIPinchGestureRecognizer *pinchGesture = [[UIPinchGestureRecognizer alloc] initWithTarget:self action:@selector(pinched:)];
    [self.view addGestureRecognizer:pinchGesture];
    
    // notifications
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(activate) name: UIApplicationDidBecomeActiveNotification object:nil];
}

- (void) viewWillAppear:(BOOL)animated
{
    [super viewWillAppear:animated];
    
    app = (SoundPathApp*)cinder::app::AppCocoaTouch::get();
    app->interopDelegate = self;
}

- (void) viewDidAppear:(BOOL)animated
{
    [super viewDidAppear:animated];
    [self populate];
}

- (void) viewDidDisappear:(BOOL)animated
{
    [super viewDidDisappear:animated];
}

#pragma mark - testing

-(void) populate {
    
    // source
    NSString *sid = [self makeNodeId:[NSNumber numberWithInt:0] type:@"artist"];
    NodePtr source = app->getNode([sid UTF8String]);
    if (source == NULL) {
        source = app->createNode([sid UTF8String],[@"artist" UTF8String]);
        source->renderLabel([@"title 1" UTF8String]);
    }
    
    // active
    if (! (source->isActive() || source->isLoading())) {
        
        // load
        source->load();
    }
    
    source->numConnections = 5;
    
    for(int i=1; i<5; i++)
    {
        // node
        NSString *nid = [self makeNodeId:[NSNumber numberWithInt:i] type:@"artist"];
        NodePtr node = app->getNode([nid UTF8String]);
        if (node == NULL) {
            node = app->createNode([nid UTF8String],[@"artist" UTF8String]);
            node->renderLabel([[NSString stringWithFormat:@"title%d", i] UTF8String]);
        }
        
        // connection
        ConnectionPtr connection = app->getConnection([sid UTF8String], [nid UTF8String]);
        if (connection == NULL) {
            
            // create
            NSString *cid = [self makeConnectionId:sid to:nid];
            connection = app->createConnection([cid UTF8String],[@"related" UTF8String],source,node);
            
            // connect it
            source->connect(node);
        }
        
        // active
        if (! (node->isActive() || node->isLoading())) {
            
            // load
            node->load();
        }
    }
}

#pragma mark - SoundPathInteractionDelegate
- (void)nodeInfo:(NSString*)nid
{
    DLog();
    
    // node
    NodePtr node = app->getNode([nid UTF8String]);
    
    // info
    if (node->isActive()) {
        
    }
}

- (void)nodeRelated:(NSString*)nid
{
    DLog();
    
    // node
    NodePtr node = app->getNode([nid UTF8String]);
    if (node->isActive()) {
        
    }
}

- (void)nodeClose:(NSString*)nid
{
    DLog();
    
    // node
    NodePtr node = app->getNode([nid UTF8String]);
    
    // close
    if (node->isActive()) {
        node->close();
    }
}

- (void)nodeLoad:(NSString*)nid
{
    DLog();
    
    // node
    NodePtr node = app->getNode([nid UTF8String]);
    if (node && ! node->isLoading()) {
        
        // flag
        node->load();
        
        // solyaris
        app->load(node);
    }

}

- (void)nodeInformation:(NSString*)nid
{

}

#pragma mark -
#pragma mark Helpers

/*
 * Node id.
 */
- (NSString*)makeNodeId:(NSNumber*)nid type:(NSString*)type {
    
    // make it so
    return [NSString stringWithFormat:@"%@_%i",type,[nid intValue]];
}

/*
 * Edge id.
 */
- (NSString*)makeEdgeId:(NSString*)pid to:(NSString *)cid {
    
    // theres an edge
    return [NSString stringWithFormat:@"%@_edge_%@",pid,cid];
}

/*
 * Connection id.
 */
- (NSString*)makeConnectionId:(NSString *)sid to:(NSString *)nid {
    
    // connect this
    return [NSString stringWithFormat:@"%@_connection_%@",sid,nid];
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
    
    return UIInterfaceOrientationIsLandscape(interfaceOrientation);
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

@end
