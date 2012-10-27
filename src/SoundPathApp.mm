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
    
    // vars
    pscale = 1.0;
    ppinch = CGPointMake(0,0);
    
    // app
    this->applyDeviceOrientation(UIDeviceOrientationPortrait);
    this->applySettings();
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
    }
    else {
        this->setWindowSize(dwidth, dheight);
    }
}

/**
 * Applies the user defaults.
 */
void SoundPathApp::applySettings() {
    GLog();
}

#pragma mark -
#pragma mark Sketch

/*
 * Cinder update.
 */
void SoundPathApp::update() {
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
}

/*
 * Cinder reset.
 */
void SoundPathApp::reset() {
    
    // app
    pscale = 1.0;
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
            
        }
        
        // double tap
        if (taps == 2) {
            
        }
        
    }
}
void SoundPathApp::touchesMoved( TouchEvent event ){
    GLog();
    
    // touch
    for( vector<TouchEvent::Touch>::const_iterator touch = event.getTouches().begin(); touch != event.getTouches().end(); ++touch ) {
        
    }
}
void SoundPathApp::touchesEnded( TouchEvent event ){
    GLog();
    
    // touch
    for( vector<TouchEvent::Touch>::const_iterator touch = event.getTouches().begin(); touch != event.getTouches().end(); ++touch ) {
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
        
        // value
        pscale = scale;
        ppinch = pinch;
    }
}

CINDER_APP_COCOA_TOUCH( SoundPathApp, RendererGl )
