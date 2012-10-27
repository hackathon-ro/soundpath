//
//  Action.h
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

#pragma once
#include "cinder/gl/gl.h"
#include "cinder/gl/Texture.h"
#include "cinder/Color.h"
#include "cinder/CinderMath.h"
#include "Configuration.h"
#include "Node.h"


// namespace
using namespace std;
using namespace ci;


// constants
const int actionTimeout = 12;
const int actionReminder = 90;

// constants
const string actionNone = "action_none";
const string actionInfo = "action_info";
const string actionRelated = "action_related";
const string actionClose = "action_close";


/**
 * Graph Action.
 */
class Action {
    
    
    // public
    public:
    
    // Action
    Action();
    
    // Cinder
    void config(Configuration c);
    
    // Sketch
    void update();
    void draw();
    
    // Business
    void hide();
    void show();
    void activate();
    void deactivate();
    bool isActive();
    bool action(Vec2d tpos);
    void assignNode(NodePtr n);
    void renderAction();
    
    // node
    NodeWeakPtr node;
    
    // current action
    string act;
    
    
    // private
    private:
    
    // states
    bool active;
    int timeout;
    int counter;
    int reminder;
    
    // config
    bool retina;
    
    // position
    Vec2d pos;
    Rectd bounds;
    Vec2d pos_info;
    Vec2d pos_related;
    Vec2d pos_close;
    Vec2d asize;
    
    // textures
    gl::Texture textureActionInfo;
    gl::Texture textureActionRelated;
    gl::Texture textureActionClose;
    
    // actions
    bool action_info;
    bool action_related;
    bool action_close;
    
    
};
