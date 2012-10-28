//
//  SoundPathApp.mm
//  SoundPath
//
//  Created by Leonard Chioveanu on 10/27/12.
//
//

#include "SoundPathApp.h"
#include "SPUtils.h"
#import "Band.h"

#pragma mark -
#pragma mark Cinder

/*
 * Cinder Launch.
 */
void SoundPathApp::launch( const char *title, int argc, char * const argv[] ) {
    DLog();
    
    // custom app delegate
	::UIApplicationMain( argc, const_cast<char**>( argv ), nil, @"SoundPathAppDelegate" );
}

/*
 * Cinder Setup.
 */
void SoundPathApp::setup() {
    DLog();
    
    // redux
    redux = (UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad) ? false : true;
    
    // display
    retina = false;
    dwidth = redux ? 320 : 768;
    dheight = redux ? 480 : 1024;
    
    // retina
    if ([SPUtils isRetina]) {
        retina = true;
        dwidth = redux ? 640 : 1536;
        dheight = redux ? 960 : 2048;
    }
    
    
    // sketch
    bg = Color(30.0/255.0, 30.0/255.0, 30.0/255.0);
    
    // graph
    graph = Graph(dwidth,dheight,UIDeviceOrientationPortrait);
    
    // vars
    pscale = 1.0;
    ppinch = CGPointMake(0,0);
    
    // app
    this->applyDeviceOrientation(UIDeviceOrientationPortrait);
    
    // configuration
    Configuration conf = Configuration();
    conf.setConfiguration(cDeviceRedux,redux ? "1" : "0");
    conf.setConfiguration(cDisplayRetina,retina ? "1" : "0");
    graph.config(conf);
}


/*
 * Cinder settings.
 */
void SoundPathApp::prepareSettings(Settings *settings) {
    FLog();
    
    // stage
    settings->setWindowSize(dwidth, dheight);
    
    // device
    settings->enableMultiTouch();
}

void SoundPathApp::applyDeviceOrientation(int dorientation) {
    GLog();
    
    // orientation
    orientation = dorientation;
    
    // orientation
    if (orientation == UIDeviceOrientationLandscapeLeft || orientation == UIDeviceOrientationLandscapeRight) {
        this->setWindowSize(dheight, dwidth);
        graph.resize(dheight,dwidth,orientation);
    }
    else {
        this->setWindowSize(dwidth, dheight);
        graph.resize(dwidth,dheight,orientation);
    }
}

#pragma mark -
#pragma mark Sketch

/*
 * Cinder update.
 */
void SoundPathApp::update() {
    // graph
    graph.update();
}

/*
 * Cinder draw.
 */
void SoundPathApp::draw() {
    
    // prepare
    gl::setMatricesWindow( getWindowSize() );
	gl::setViewport( getWindowBounds() );
    glEnable(GL_LINE_SMOOTH);
    
    // clear
	gl::clear(bg);
    
    // graph
    graph.draw();
}

/*
 * Cinder reset.
 */
void SoundPathApp::reset() {
    
    // app
    pscale = 1.0;
    
    // graph
    graph.reset();
}

/*
 * init
 */

void SoundPathApp::initMe() {
    
    // source
    int nid = -1;
    NodePtr root = graph.getNode(nid);
    if (root == NULL) {
        root = graph.createNode(nid,[@"node" UTF8String]);
        root->renderLabel([@"Me" UTF8String]);
    }
    
    // load
    root->load();
}

void SoundPathApp::loaded(int nid, NSDictionary * artists)
{
    unsigned int recordCount = [[artists allKeys] count];
    
    
    NodePtr source = graph.getNode(nid);
    source->loaded();


    NodeVectorPtr nodes;

    hideSubChildren(source);

    NSArray * keys = [artists allKeys];
    
    for(int i=1; i<recordCount; i++)
    {
        NSDictionary * band = [artists objectForKey:[keys objectAtIndex:i]];

        int nid = [[band objectForKey:@"id"] integerValue];
        
        NodePtr node = graph.getNode(nid);
        if(node == NULL)
        {
            node = graph.createNode(nid, [@"node" UTF8String]);
        }
        
        node->renderLabel([[NSString stringWithFormat:@"%@", [band objectForKey:@"name" ]] UTF8String]);

        if (!node->isLoading()) {
            node->load();
        }

        nodes.push_back(node);

    }
    
    graph.expand(source, nodes);
}


#pragma mark -
#pragma mark Touch

/*
 * Cinder touch events.
 */
void SoundPathApp::touchesBegan( TouchEvent event ) {
    GLog();
    
    // touch
    for( vector<TouchEvent::Touch>::const_iterator touch = event.getTouches().begin(); touch != event.getTouches().end(); ++touch ) {
        
        // taps
        int taps = [(__bridge UITouch*)touch->getNative() tapCount];
        
        // touch
        if (taps == 1) {
            
            // touch graph
            NodePtr node = graph.touchBegan(touch->getPos(),touch->getId());
            if (node != NULL) {
                
                // touch controller
                int nid = node->nid;
                
                
                // info
                if (node->action == actionInfo) {
                    FLog("action info");
                    
                    // info
                    [interopDelegate nodeInfo:nid];
                } else {
                    [interopDelegate nodeTapped:nid];
                }
                
                // reset
                node->setAction("");
                
            }
        }
        
        // double tap
        if (taps == 2) {
            
            // tap graph
            NodePtr node = graph.doubleTap(touch->getPos(),touch->getId());
            if (node != NULL) {
                
                // tap controller
                int nid = node->nid;
                
                // load
                if (! node->isLoading()) {
                    
                }
                // open
                if (node->isClosed()) {
                    node->open();
                }
                // information
                else {
                    // node info
                    [interopDelegate nodeInfo:nid];
                }
            }
        }
        
    }
}
void SoundPathApp::touchesMoved( TouchEvent event ){
    GLog();
    
    // touch
    for( vector<TouchEvent::Touch>::const_iterator touch = event.getTouches().begin(); touch != event.getTouches().end(); ++touch ) {
        
        // graph
        graph.touchMoved(touch->getPos(),touch->getPrevPos(),touch->getId());
        
    }
}
void SoundPathApp::touchesEnded( TouchEvent event ){
    GLog();
    
    // touch
    for( vector<TouchEvent::Touch>::const_iterator touch = event.getTouches().begin(); touch != event.getTouches().end(); ++touch ) {
        
        // graph
        graph.touchEnded(touch->getPos(),touch->getId());
    }
}

#pragma mark -
#pragma mark Gestures

/*
 * Pinched.
 */
void SoundPathApp::pinched(UIPinchGestureRecognizer* recognizer) {
    GLog();
    
    // states
    if (recognizer.state == UIGestureRecognizerStateBegan) {
        
        // reset
        pscale = 1.0;
        ppinch = [recognizer locationInView:recognizer.view];
    }
    if (recognizer.state == UIGestureRecognizerStateEnded) {
        return;
    }
    
    // check
    if ([recognizer numberOfTouches] == 2) {
        
        // position
        CGPoint pinch = [recognizer locationInView:recognizer.view];
        double scale = recognizer.scale;
        
        // graph
        graph.pinched(Vec2d(pinch.x,pinch.y), Vec2d(ppinch.x,ppinch.y), scale, pscale);
        
        // value
        pscale = scale;
        ppinch = pinch;
    }
}

#pragma mark -
#pragma mark Business

/**
 * Calculates a node's real world coordinates.
 */
Vec3d SoundPathApp::nodeCoordinates(int nid) {
    GLog();
    
    NodePtr n = graph.getNode(nid);
    
    // calculate real world position
    return graph.coordinates(n->pos.x, n->pos.y, n->radius);
}


void SoundPathApp::expand(NodePtr parent, NodeVectorPtr nodes)
{
    graph.expand(parent, nodes);
}

void SoundPathApp::hideSubChildren(NodePtr parent)
{
    graph.hideSubChildren(parent);
}
CINDER_APP_COCOA_TOUCH( SoundPathApp, RendererGl )
