//
//  Edge.h
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
#include "Node.h"
#include "Configuration.h"
#include "Defaults.h"
#include "cinder/gl/gl.h"
#include "cinder/Text.h"
#include "cinder/CinderMath.h"
#include "cinder/Color.h"
#include "I18N.h"
#include <boost/ptr_container/ptr_vector.hpp>
#include <boost/shared_ptr.hpp>


// namespace
using namespace std;
using namespace ci;

// declarations
class Edge;

// typedef
typedef boost::shared_ptr<Edge> EdgePtr;
typedef std::vector<EdgePtr> EdgeVectorPtr;
typedef EdgeVectorPtr::iterator EdgeIt;


// constants
const string edgeMovie = "movie";	
const string edgePerson = "person";
const string edgePersonActor = "person_actor";
const string edgePersonDirector = "person_director";
const string edgePersonCrew = "person_crew";



/**
 * Graph Edge.
 */
class Edge {
    
    // public
    public:
    
    // Edge
    Edge();
    Edge(string ide, NodePtr n1, NodePtr n2); 
    
    // Cinder
    void config(Configuration c);
    void defaults(Defaults s);
    
    // Sketch
    void update();
    void draw();
    
    // Business
    void repulse();
    void hide();
    void show();
    void renderLabel(string lbl);
    void updateType(string t);
    bool isActive();
    bool isVisible();
    bool isTouched(NodePtr n);
    string info(I18N translations);
    
    
    // Public Fields
    string eid;
    NodeWeakPtr wnode1;
    NodeWeakPtr wnode2;
    string label;
    string type;
    
    
    // private
    private:
    
    // States
    bool active;
    bool visible;
    bool selected;
    
    // parameters
    double length;
    double stiffness;
    double damping;
    
    // config
    bool redux;
    bool retina;
    
    // position
    Vec2d pos;
    
    // color
    Color cstroke;
    Color cstrokea;
    Color cstrokes;
    Color ctxt;
    Color ctxta;
    Color ctxts;
    
    // Font
    Font font;
    Vec2d loff;
    gl::Texture	textureLabel;

};

class EdgeMovie: public Edge {
    
    // public
    public:
    
    // Node
    EdgeMovie();
    EdgeMovie(string ide, NodePtr n1, NodePtr n2);
};
class EdgePerson: public Edge {
    
    // public
    public:
    
    // Node
    EdgePerson();
    EdgePerson(string ide, NodePtr n1, NodePtr n2);
};
