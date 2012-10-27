//
//  Edge.cpp
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

#include "Edge.h"


#pragma mark -
#pragma mark Object

/**
 * Creates an Edge.
 */
Edge::Edge() {
}
Edge::Edge(string ide, NodePtr n1, NodePtr n2) {
    
    // fields
    eid = ide;
    length = 480;
    stiffness = 0.6;
    damping = 0.9;
    redux = false;
    retina = false;
    
    // nodes
    wnode1 = NodeWeakPtr(n1);
    wnode2 = NodeWeakPtr(n2);
    
    // state
    active = false;
    visible = false;
    selected = false;
    
    // position
    pos.set(0,0);
    
    // color
    cstroke = Color(0.85,0.85,0.85);
    cstrokea = Color(0.85,0.85,0.85);
    cstrokes = Color(0.75,0.75,0.75);
    
    ctxt = Color(0.5,0.5,0.5);
    ctxta = Color(0.5,0.5,0.5);
    ctxts = Color(0.15,0.15,0.15);
    
    // label
    label = "";
    
    // font
    font = Font("Helvetica",12);
    loff.set(0,-12);
    textureLabel = gl::Texture(1,1);
}


/**
 * Edge movie.
 */
EdgeMovie::EdgeMovie(): Edge::Edge()  {    
}
EdgeMovie::EdgeMovie(string ide, NodePtr n1, NodePtr n2): Edge::Edge(ide,n1,n2) {
    
    // type
    type = edgeMovie;

}

/**
 * Edge person.
 */
EdgePerson::EdgePerson(): Edge::Edge()  {    
}
EdgePerson::EdgePerson(string ide, NodePtr n1, NodePtr n2): Edge::Edge(ide,n1,n2) {
    
    // type
    type = edgePerson;

}



#pragma mark -
#pragma mark Cinder

/**
 * Applies the configuration.
 */
void Edge::config(Configuration c) {
    
    // device redux
    redux = false;
    Config confDeviceRedux = c.getConfiguration(cDeviceRedux);
    if (confDeviceRedux.isSet()) {
        redux = confDeviceRedux.boolVal();
    }
    
    // display retina
    retina = false;
    Config confDisplayRetina = c.getConfiguration(cDisplayRetina);
    if (confDisplayRetina.isSet()) {
        retina = confDisplayRetina.boolVal();
    }
    
    
    // init retina
    if (retina) {
        
        // fonts
        font = Font("Helvetica",24);
        loff *= 2;
    }
}

/**
 * Applies the settings.
 */
void Edge::defaults(Defaults d) {
    
    // length
    length = redux ? 300 : 480;
    Default graphEdgeLength = d.getDefault(dGraphEdgeLength);
    if (graphEdgeLength.isSet()) {
        length = graphEdgeLength.doubleVal();
    }
    
    // retina
    if (retina) {
        length *= 2;
    }
}



#pragma mark -
#pragma mark Sketch

/**
 * Updates the edge.
 */
void Edge::update() {
    
    
    // nodes
    NodePtr node1 = wnode1.lock();
    NodePtr node2 = wnode2.lock();
    if (node1 && node2) {
        
        // selected
        selected = false;
        if (node1->isSelected() || node2->isSelected()) {
            selected = true;
        }
        
        // position
        pos = node1->pos + ((node2->pos - node1->pos) / 2.0);
    }
    
}

/**
 * Draws the edge.
 */
void Edge::draw() {
    
    // nodes
    NodePtr node1 = wnode1.lock();
    NodePtr node2 = wnode2.lock();
    if (node1 && node2) {
        
        // unblend
        gl::enableAlphaBlending(true);
        
        // color
        active ? gl::color(cstrokea) : gl::color(cstroke);
        if (selected) {gl::color(cstrokes);}
        
        // line
        glLineWidth(retina ? 2 : 1);
        gl::drawLine(node1->pos, node2->pos);
        
        // label
        if (active || selected) {
            
            // color
            selected ? gl::color(ctxts) : (active ? gl::color(ctxta) : gl::color(ctxt));
            
            // angle 
            float ar = cinder::math<float>::atan2(node2->pos.x - node1->pos.x, node2->pos.y - node1->pos.y);
            float ad = cinder::toDegrees(-ar);
            ad += (ad < 0) ? 90 : 270;
            
            // push, translate & rotate
            gl::pushMatrices();
            gl::translate(pos);
            gl::rotate(Vec3f(0, 0,ad));
            
            // draw
            gl::draw( textureLabel, loff);
            
            // and pop it goes
            gl::popMatrices();
        }
        
        // reset
        gl::disableAlphaBlending();
    }
    

}


#pragma mark -
#pragma mark Business

/**
 * Edge repulsion.
 */
void Edge::repulse() {
    
    // nodes
    NodePtr node1 = wnode1.lock();
    NodePtr node2 = wnode2.lock();
    if (node1 && node2) {
        
        // distance vector
        Vec2d diff = node2->pos - node1->pos;
      
        // normalize / length
        diff.safeNormalize();
        diff *= length;
        
        // target
        Vec2d target = node1->pos + diff;
        
        // force
        Vec2d force = target - node2->pos;
        force *= 0.5;
        force *= stiffness;
        force *= (1 - damping);
        
        // update velocity
        node1->velocity -= force;
        node2->velocity += force;
    }

}



/**
 * Shows/Hides the edge.
 */
void Edge::show() {
    GLog();
    
    // nodes
    NodePtr node1 = wnode1.lock();
    NodePtr node2 = wnode2.lock();
    if (node1 && node2) {
        
        // check if active
        if ((node1->isActive() && node2->isActive()) 
            || (node1->isActive() && node2->isLoading())
            || (node2->isActive() && node1->isLoading())) {
            
            // label
            font = Font("Helvetica-Bold",retina ? 24 : 12);
            //loff.y = -15;
            this->renderLabel(label);
            
            // state
            active = true;
        }
        
        // state
        visible = true;
    }
    
}
void Edge::hide() {
    GLog();
    
    // state
    visible = false;
    
}

/**
 * States.
 */
bool Edge::isActive() {
    bool a = false;
    
    // nodes
    NodePtr node1 = wnode1.lock();
    NodePtr node2 = wnode2.lock();
    if (node1 && node2) {
        
        // active?
        a = active && ! ( (node1->isClosed() || node2->isClosed()) ); 
    }
    return a;
}
bool Edge::isVisible() {
    bool v = false;
    
    // nodes
    NodePtr node1 = wnode1.lock();
    NodePtr node2 = wnode2.lock();
    if (node1 && node2) {
        
        // visible?
        v = active
        || (visible && ( (node1->isVisible() && ! node2->isLoading()) && (node2->isVisible() && ! node1->isLoading())) ) 
        || ( (node1->isLoading() && node2->isActive()) || (node2->isLoading() && node1->isActive()) ) 
        || ( (node1->isActive() || node1->isSelected()) & (node2->isActive() || node2->isSelected()));
    }
    return v;
}
bool Edge::isTouched(NodePtr n) {
    bool touched = false;
    
    // nodes
    NodePtr node1 = wnode1.lock();
    NodePtr node2 = wnode2.lock();
    if (node1 && node2) {
        
        // selected
        if (this->isVisible() && (node1->isSelected() || node2->isSelected()) && (node1 == n || node2 == n)) {
            touched = true;
        }
    }
    return touched;
}

/**
 * Info.
 */
string Edge::info(I18N translations) {
    string nfo = "";
    
    // nodes
    NodePtr node1 = wnode1.lock();
    NodePtr node2 = wnode2.lock();
    if (node1 && node2) {
        
        // person / movie
        string person = node1->label;
        string movie = node2->label;
        if (node1->type == nodeMovie) {
            person = node2->label;
            movie = node1->label;
        }
        
        // movie
        if (this->type == edgeMovie) {
            nfo = person + translations.getTranslation(i18nTooltipActor1," is ") + this->label + translations.getTranslation(i18nTooltipActor2," in ") + movie;
        }
        
        // actor
        else if (this->type == edgePersonActor) {
            nfo = person + translations.getTranslation(i18nTooltipActor1," is ") + this->label + translations.getTranslation(i18nTooltipActor2," in ") + movie;
        }
        
        // director / crew
        else if (this->type == edgePersonDirector || this->type == edgePersonCrew) {
            nfo = person + translations.getTranslation(i18nTooltipCrew1," is the ") + this->label + translations.getTranslation(i18nTooltipCrew2," of ") + movie;
        }
    }
    return nfo;
}



/**
 * Renders the label.
 */
void Edge::renderLabel(string lbl) {
    GLog();
    
    // field
    label = (lbl == "") ? " " : lbl;
    
    // text
    TextLayout tlLabel;
	tlLabel.clear(ColorA(0, 0, 0, 0));
	tlLabel.setFont(font);
	tlLabel.setColor(ctxt);
	tlLabel.addCenteredLine(label);
	Surface8u rendered = tlLabel.render(true, true);
	textureLabel = gl::Texture(rendered);

    
    // offset
    loff.x = - textureLabel.getWidth() / 2.0;
    
}

/**
 * Updates the type.
 */
void Edge::updateType(string t) {
    
    // type
    type = t;
}


