//
//  SoundPathApp.h
//  SoundPath
//
//  Created by Leonard Chioveanu on 10/27/12.
//
//

#import <UIKit/UIKit.h>

#import "SPInteropDelegate.h"

#include "cinder/Cinder.h"
#include "cinder/gl/gl.h"
#include "cinder/app/AppCocoaTouch.h"
#include "cinder/System.h"

// namespace
using namespace std;
using namespace ci;
using namespace ci::app;

class SoundPathApp : public AppCocoaTouch {
    // Methods
public:
    
    // Cinder
    void launch( const char *title, int argc, char * const argv[] );
    void prepareSettings(Settings *settings);
	void setup();
    void applyDeviceOrientation(int dorientation);
    void applySettings();
    
    
    // Sketch
	void update();
	void draw();
    void reset();
    
    // Touch
    void touchesBegan( TouchEvent event );
	void touchesMoved( TouchEvent event );
	void touchesEnded( TouchEvent event );
    
    // Gestures
    void pinched(UIPinchGestureRecognizer* recognizer);
    
    id<SPInteropDelegate> interopDelegate;
    
    // Fields
private:
    
    int orientation;
    double dwidth;
    double dheight;
    bool retina;
    bool redux;
    double pscale;
    CGPoint ppinch;
    
    Color bg;

};
