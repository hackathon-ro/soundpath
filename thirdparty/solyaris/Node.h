//
//  Node.h
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

#pragma once
#include "cinder/app/AppCocoaTouch.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Texture.h"
#include "cinder/ImageIo.h"
#include "cinder/Vector.h"
#include "cinder/Color.h"
#include "cinder/Font.h"
#include "cinder/Text.h"
#include "cinder/Rand.h"
#include "cinder/CinderMath.h"
#include <boost/ptr_container/ptr_vector.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include "Configuration.h"


// namespace
using namespace std;
using namespace ci;
using namespace ci::app;

// declarations
class Node;

// typedef
typedef boost::shared_ptr<Node> NodePtr;
typedef boost::weak_ptr<Node> NodeWeakPtr;
typedef std::vector<NodePtr> NodeVectorPtr;
typedef NodeVectorPtr::iterator NodeIt;

// lengths
const float nodeUnfoldMin = 0.9f;
const float nodeUnfoldMax = 1.53f;
const float nodeFoldMin = 0.3f;
const float nodeFoldMax = 0.9f;


/**
 * Graph Node.
 */
class Node {
    
    
    // public
    public:
    
    // Node
    Node();
    Node(int idn, double x, double y);
    
    // Cinder
    void config(Configuration c);
    
    // Sketch
    void update();
    void draw();
    
    
    // Business
    void attract(NodePtr node);
    void distract(NodePtr node);
    void repulse(Vec2d p, float dist, float dir);
    void moveTo(double x, double y);
    void moveTo(Vec2d p);
    void move(double dx, double dy);
    void move(Vec2d d);
    void translate(Vec2d d);
    void grown();
    void shrinked();
    void load();
    void unload();
    void loaded();
    void close();
    void open();
    void hide();
    void born();
    void show(bool animate);
    void touched();
    void untouched();
    void tapped();
    void connect(NodePtr n);
    void renderLabel(string lbl);
    void renderNode();
    void updateType(string t);
    void updateMeta(string m);
    void setAction(string a);
    bool isActive();
    bool isClosed();
    bool isInactive();
    bool isVisible();
    bool isSelected();
    bool isLoading();
    
    // Children
    void setChildren(NodeVectorPtr children);
    void clearChildren();
    void addChild(NodePtr child);
    void removeChild(NodePtr child);
    
    // Public Fields
    int nid;
    string label;
    string meta;
    string type;
    string action;
    NodeWeakPtr sref;
    NodeWeakPtr parent;
    Vec2d pos;
    Vec2d ppos;
    Vec2d mpos;
    float core;
    float radius,growr,shrinkr;
    float mass;
	Vec2d velocity;
    
    
    double dist;
    
    NodeVectorPtr children;
    
    // private
    private:
    
    // States
    bool selected;
    bool active;
    bool closed;
    bool visible;
    bool grow,shrink;
    bool loading;
    
    // Helpers
    float calcmass();

    
    // Parameters
    double perimeter;
    double zone;
    double damping;
    double strength;
    float stiffness;
    float distraction;
    double ramp;
    double mvelocity;
    double speed;
    int initial;
    int fcount;
    int minr,maxr;
    double rincg,rincs;
    bool retina;
    bool redux;
    
    // Textures
    gl::Texture textureNode;
    gl::Texture textureGlow;
    
    
    // Color
    Color ctxt;
    Color ctxta;
    Color ctxts;
    float anode,asnode;
    float acore,ascore;
    float aglow,asglow;
    
    // Font
    Font font;
    Vec2d loff;
    gl::Texture	textureLabel;

};
