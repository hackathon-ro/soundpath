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

#include "Graph.h"

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
    
    Vec3d nodeCoordinates(int nid);
    
    void expand(NodePtr parent, NodeVectorPtr nodes);
    void hideSubChildren(NodePtr parent);
    
    Graph graph;
    
    void initMe();
    void loaded(int nid, NSArray* records);
    
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
