//
//  Tooltip.h
//  Solyaris
//
//  Created by CNPP on 4.8.2011.
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
#include "cinder/gl/gl.h"
#include "cinder/gl/Texture.h"
#include "cinder/Color.h"
#include "cinder/Font.h"
#include "cinder/Text.h"
#include "cinder/CinderMath.h"
#include "Configuration.h"


// namespace
using namespace std;
using namespace ci;


// constants
const int tooltipTimeout = 12;

/**
 * Graph Tooltip.
 */
class Tooltip {
    
    
    // public
    public:
    
    // Tooltip
    Tooltip();
    Tooltip(Vec2d b);
    
    // Cinder
    void config(Configuration c);
    void resize(int w, int h);
    
    // Sketch
    void update();
    void draw();
    
    // Business
    void hide();
    void show();
    void activate();
    void position(Vec2d p);
    void offset(double o);
    void renderText(vector<string> txts);
    
    // Public Fields
    string text;
    
    // private
    private:
    
    // config
    bool redux;
    bool retina;
    
    // States
    bool active;
    int timeout;

    
    // position
    Vec2d pos;
    Vec2d dpos;
    Vec2d border;
    Vec2d bounds;
    
    // color
    ColorA cbg;
    ColorA ctxt;

    
    // Font
    Font font;
    Font sfont;
    Vec2d off;
    Vec2d inset;
    Vec2d size;
    Vec2i maxed;
    gl::Texture	textureText;
    
};