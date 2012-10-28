//
//  Graph.h
//  Solyaris
//
//  Created by CNPP on 22.5.2011.
//  Copyright 2011 Beat Raess. All rights reserved.
//
//  This file is part of Solyaris.
//  
//  Solyaris is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//  
//  Solyaris is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//  
//  You should have received a copy of the GNU General Public License
//  along with Solyaris.  If not, see www.gnu.org/licenses/.

#include "cinder/app/AppCocoaTouch.h"
#include "cinder/app/TouchEvent.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Texture.h"
#include "cinder/ImageIo.h"
#include "cinder/Rand.h"
#include "cinder/audio/Output.h"
#include "cinder/audio/Io.h"
#include "Node.h"
#include "Connection.h"
#include "Action.h"
#include "Configuration.h"
#include "Resources.h"
#include <vector>
#include <map>



// namespace
using namespace ci;
using namespace ci::app;
using namespace std;


/**
 * Graph.
 */
class Graph {
        
    // public
    public:
    
    // Graph
    Graph();
    Graph(int w, int h, int o);

    // Cinder
    void resize(int w, int h, int o);
    void config(Configuration c);
    void setBackground(string bg);
    
    
    // Sketch
    void reset();
    void update();
    void draw();
    
    // Touch
	NodePtr touchBegan(Vec2d tpos, int tid);
    void touchMoved(Vec2d tpos, Vec2d ppos, int tid);
    void touchEnded(Vec2d tpos, int tid);
    
    // Tap
    NodePtr doubleTap(Vec2d tpos, int tid);
    
    // Gestures
    void pinched(Vec2d p, Vec2d pp, double s, double ps);
    
    
    // Business
    void attract();
    void repulse();
    void move(Vec2d d);
    void drag(Vec2d d);
    void shift(Vec2d d);
    Vec3d coordinates(double px, double py, double d);
    NodePtr createNode(unsigned int nid, string type);
    NodePtr createNode(unsigned int nid, string type, double x, double y);
    NodePtr getNode(unsigned int nid);
    ConnectionPtr createConnection(NodePtr n1, NodePtr n2);
    ConnectionPtr getConnection(unsigned int nid1, unsigned int nid2);
    ConnectionPtr getConnection(unsigned int nid);
    void removeNode(unsigned int nid);
    void load(NodePtr n);
    void unload(NodePtr n);
    bool onStage(NodePtr n);
    void action(int tid);
    void sample(int s);
    
    void expand(NodePtr parent, NodeVectorPtr nodes);
    void hideChildren(NodePtr parent);
    void hideSubChildren(NodePtr parent);
    void hideConnections(unsigned int nid);
    
    // private
    private:
    
    // size
    int width;
    int height;
    int orientation;
    bool retina;
    bool redux;
    bool portrait;
    float dpr;
    float speed;
    float friction;
    float harea;
    
    // background
    gl::Texture bg_portrait, bg_landscape;
    
    
    // data
    NodeVectorPtr nodes;
    ConnectionVectorPtr connections;
    
    // virtual offset
    Vec2d voff;
    Vec2d vpoff;
    Vec2d vmoff;
    
    // virtual position
    Vec2d vpos;
    Vec2d vppos;
    Vec2d vmpos;
    
    // bound
    float mbound;
    
    // zoom
    Vec2d translate;
    double scale;

    
    // touched nodes
    int nbtouch;
    map<int, NodePtr> touched;
    
    // actions
    map<int, Action> actions;
    
    // configuration & settings
    Configuration conf;
    
    // samples
    audio::SourceRef audioSampleClick;
    
};

