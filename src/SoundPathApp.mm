//
//  SoundPathApp.mm
//  SoundPath
//
//  Created by Leonard Chioveanu on 10/27/12.
//
//

#include "SoundPathApp.h"
#include "SPUtils.h"

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
                unsigned int nid = node->nid;
                
                
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
                unsigned int nid = node->nid;
                
                // load
                if (! node->isActive() && ! node->isLoading()) {
                    
                    // node load
                    [interopDelegate nodeLoad:nid];
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


/*
 * Creates a node.
 */
NodePtr SoundPathApp::createNode(unsigned int nid, string type) {
    GLog();
    
    // graph
    return graph.createNode(nid,type);
}
NodePtr SoundPathApp::createNode(unsigned int nid, string type, double x, double y) {
    GLog();
    
    // graph
    return graph.createNode(nid,type,x,y);
}

/*
 * Gets a node.
 */
NodePtr SoundPathApp::getNode(unsigned int nid) {
    GLog();
    
    // graph
    return graph.getNode(nid);
}

/*
 * Creates a connection.
 */
ConnectionPtr SoundPathApp::createConnection(NodePtr n1, NodePtr n2) {
    GLog();
    
    // graph
    return graph.createConnection(n1,n2);
}

/*
 * Gets a connection.
 */
ConnectionPtr SoundPathApp::getConnection(unsigned int nid1, unsigned int nid2) {
    GLog();
    
    // graph
    return graph.getConnection(nid1,nid2);
}

void SoundPathApp::load(NodePtr n) {
    GLog();
    
    // graph
    graph.load(n);
}

/**
 * Unloads a node.
 */
void SoundPathApp::unload(NodePtr n) {
    GLog();
    
    // graph
    graph.unload(n);
}

/**
 * Shifts the graph.
 */
void SoundPathApp::graphShift(double mx, double my) {
    GLog();
    
    // graph
    graph.shift(Vec2d(mx,my));
}

/**
 * Calculates a node's real world coordinates.
 */
Vec3d SoundPathApp::nodeCoordinates(NodePtr n) {
    GLog();
    
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
