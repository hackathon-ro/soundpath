//
//  Action.cpp
//  Solyaris
//
//  Created by Beat Raess on 28.10.2011.
//  Copyright (c) 2011 Beat Raess. All rights reserved.
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

#include "Action.h"


#pragma mark -
#pragma mark Object

/**
 * Creates an action object.
 */
Action::Action() {
    
    // state
    active = false;
    
    // position
    pos.set(0,0);
    pos_info.set(0,0);
    asize.set(44,44);
    
    // time
    timeout = -1;
    reminder = -1;
    
    // textures
    textureActionInfo = gl::Texture(1,1);
    
    // actions
    action_info = false;
    
    // hide
    this->hide();
}


#pragma mark -
#pragma mark Cinder

/*
 * Configuration.
 */
void Action::config(Configuration c) {
    
    // display retina
    retina = false;
    Config confDisplayRetina = c.getConfiguration(cDisplayRetina);
    if (confDisplayRetina.isSet()) {
        retina = confDisplayRetina.boolVal();
    }
    
    // retina stuff
    if (retina) {
        
        // size
        asize *= 2;
    }
    
    // render
    this->renderAction();
    
}



#pragma mark -
#pragma mark Sketch

/**
 * Updates the Action.
 */
void Action::update() {
    
    // timeout (avoids flickering)
    if (timeout > 0) {
        timeout--;
    }
    else if (timeout == 0) {
        this->activate();
    }
    
    // reminder
    if (reminder > 0) {
        reminder--;
    }
    else if (reminder == 0) {
        this->deactivate();
    }
    
    // position
    if (active) {
        
        // lock & loll
        NodePtr n = this->node.lock();
        if (n) {
    
            // position
            pos = (n->pos);
        }
        
    }
    
    
}

/**
 * Draws the Action.
 */
void Action::draw() {
    
    // test
    //gl::enableAlphaBlending();
    //glColor4f(1,0,0,0.9);
    //gl::drawSolidRect(Rectf(pos.x+bounds.x1,pos.y+bounds.y1,pos.x+bounds.x2,pos.y+bounds.y2), false);
    
    // active
    if (active) {
        
        // unblend
        gl::enableAlphaBlending(true);
        
        // draw textures
        gl::color( ColorA(1.0f, 1.0f, 1.0f, 1.0f) ); // alpha channel
        
        // info
        gl::draw(textureActionInfo, pos+pos_info);
        
        // reset
        gl::disableAlphaBlending();
    }
}



#pragma mark -
#pragma mark Business

/**
 * It's hammer time.
 */
bool Action::action(Vec2d tpos) {
    GLog();
    
    // action
    if (Rectf(pos.x+bounds.x1,pos.y+bounds.y1,pos.x+bounds.x2,pos.y+bounds.y2).contains(tpos)) {
        
        // info
        if (action_info && Rectf(pos.x+pos_info.x,pos.y+pos_info.y,pos.x+pos_info.x+asize.x,pos.y+pos_info.y+asize.y).contains(tpos)) {
            act = actionInfo;
            return true;
        }
    }
    
    // miss
    act = actionNone;
    return false;
}


/**
 * States.
 */
bool Action::isActive() {
    return active;
}


/**
 * Show / Hide.
 */
void Action::show() {
    GLog();
    
    // state
    active = false;
    
    // props
    timeout = actionTimeout;
    reminder = -1;
    
}
void Action::hide() {
    GLog();
    
    // reminder
    timeout = -1;
    reminder = actionReminder;

}
void Action::activate() {
    GLog();
    
    // state
    active = true;
    
    
    // reset
    timeout = -1;
    reminder = -1;
    
    // current
    act = actionNone;

}
void Action::deactivate() {
    GLog();
    
    // state
    active = false;
    
    // fluff
    pos.set(-10000,-10000);
    
    // reset
    timeout = -1;
    reminder = -1;
    
    // no no node
    node = NodePtr();
    
    // reset
    action_info = false;
    
    // current
    act = actionNone;
}


/**
 * Assign node.
 */
void Action::assignNode(NodePtr n) {
    
    // ref
    node = n;
    
    // position (shift -90°)
    pos_info.set(n->radius * cos(-2.617993878), n->radius * sin(-2.617993878)); // -150°
    
    pos_info -= asize/2.0;
    
    // actions
    action_info = true;
    
    // bounds
    bounds.x1 = pos_info.x;
    bounds.x2 = pos_info.x+asize.x;
    bounds.y1 = pos_info.y;
    bounds.y2 = pos_info.y+asize.y;
}


/*
 * Renders the action.
 */
void Action::renderAction() {
    GLog();
    
    // suffix
    string sfx = retina ? "@2x.png" : ".png";
    
    // textures
    textureActionInfo = gl::Texture(loadImage(loadResource("node_action_info"+sfx)));
}


