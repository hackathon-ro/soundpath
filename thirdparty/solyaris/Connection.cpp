//
//  Connection.cpp
//  Solyaris
//
//  Created by Beat Raess on 30.5.2012.
//  Copyright (c) 2012 Beat Raess. All rights reserved.
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

#include "Connection.h"


#pragma mark -
#pragma mark Object

/**
 * Creates a Connection.
 */
Connection::Connection() {
}
Connection::Connection(string idc, NodePtr n1, NodePtr n2) {
    
    // fields
    cid = idc;
    redux = false;
    retina = false;
    
    // nodes
    wnode1 = NodeWeakPtr(n1);
    wnode2 = NodeWeakPtr(n2);
    
    // state
    selected = false;
    
    // color
    cstroke = Color(0.75,0.75,0.75);
    cstrokes = Color(0.6,0.6,0.6);
    
    // size
    s = 1.0;
    d = 4.0;

}


/**
 * Connection related.
 */
ConnectionRelated::ConnectionRelated(): Connection::Connection()  {    
}
ConnectionRelated::ConnectionRelated(string idc, NodePtr n1, NodePtr n2): Connection::Connection(idc,n1,n2) {
    
    // type
    type = connectionRelated;
}



#pragma mark -
#pragma mark Cinder

/**
 * Applies the configuration.
 */
void Connection::config(Configuration c) {
    
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
    
    // size
    s = retina ? 2.0 : 1.0;
    d = retina ? 8.0 : 4.0;
    
}

/**
 * Applies the settings.
 */
void Connection::defaults(Defaults d) {
}


#pragma mark -
#pragma mark Sketch

/**
 * Updates the connection.
 */
void Connection::update() {
    
    
    // nodes
    NodePtr node1 = wnode1.lock();
    NodePtr node2 = wnode2.lock();
    if (node1 && node2) {
        
        // selected
        selected = false;
        if (node1->isSelected() || node2->isSelected()) {
            selected = true;
        }

    }
    
}

/**
 * Draws the connection.
 */
void Connection::draw() {
    
    // nodes
    NodePtr node1 = wnode1.lock();
    NodePtr node2 = wnode2.lock();
    if (node1 && node2) {
        
        // unblend
        gl::enableAlphaBlending(true);
        
        // color
        selected ? gl::color(cstrokes) : gl::color(cstroke);
        
        // params
        int nb = 100;
        float ox = node1->pos.x;
        float oy = node1->pos.y;
        float dx = (node2->pos.x - node1->pos.x) / (float)nb;
        float dy = (node2->pos.y - node1->pos.y) / (float)nb;
        for (int i = 0; i < nb; i++) {
            gl::drawSolidRect( Rectf(ox+dx*i, oy+dy*i, ox+dx*i+s, oy+dy*i+s) );
        }
         
        // reset
        gl::disableAlphaBlending();
    }
    
    
}


