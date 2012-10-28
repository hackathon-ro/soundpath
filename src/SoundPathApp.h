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
    
    // Business
    NodePtr createNode(unsigned int nid, string type);
    NodePtr createNode(unsigned int nid, string type, double x, double y);
    NodePtr getNode(unsigned int nid);
    ConnectionPtr createConnection(NodePtr n1, NodePtr n2);
    ConnectionPtr getConnection(unsigned int nid1, unsigned int nid2);
    
    void load(NodePtr n);
    void unload(NodePtr n);
    void graphShift(double mx, double my);
    Vec3d nodeCoordinates(NodePtr n);
    
    
    
    void expand(NodePtr parent, NodeVectorPtr nodes);
    void hideSubChildren(NodePtr parent);
    
    Graph graph;
    
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
