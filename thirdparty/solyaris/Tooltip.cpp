//
//  Tooltip.cpp
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

#include "Tooltip.h"


#pragma mark -
#pragma mark Object

/**
 * Creates a Tooltip object.
 */
Tooltip::Tooltip() {
}
Tooltip::Tooltip(Vec2d b) {
    
    // config
    redux = false;
    retina = false;
    
    // state
    active = false;
    
    // position
    pos.set(0,0);
    border.set(10,50);
    bounds = b;
    
    // color
    cbg = ColorA(0.9,0.9,0.9,0.9);
    ctxt = ColorA(0,0,0,1);
    
    // font
    font = Font("Helvetica",12);
    sfont = Font("Helvetica",3);
    size.set(0,0);
    off.set(0,-75);
    inset.set(6,6);
    maxed.set(30,100);
    textureText = gl::Texture(0,0);
    
    // hide
    this->hide();
}


#pragma mark -
#pragma mark Cinder

/*
 * Configuration.
 */
void Tooltip::config(Configuration c) {
    
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
    
    
    // retina stuff
    if (retina) {
        
        // position
        border *= 2;
        
        // font
        font = Font("Helvetica",24);
        sfont = Font("Helvetica",6);
        off *= 2;
        inset *= 2;
        maxed *= 2;
    }
   
}

/*
 * Resize.
 */
void Tooltip::resize(int w, int h) {
    
    // fields
    bounds = Vec2d(w,h);
}


#pragma mark -
#pragma mark Sketch

/**
 * Updates the Tooltip.
 */
void Tooltip::update() {
   
    // timeout (avoids flickering)
    if (timeout > 0) {
        timeout--;
    }
    else if (timeout == 0) {
        this->activate();
    }
    
    // position
    if (active) {
        
        // bound
        float px = max(border.x,(pos.x-size.x/2.0+off.x));
        float py = max(border.y,(pos.y-size.y+off.y));
        px += min(0.0,bounds.x-(px+size.x+border.x));
        py += min(0.0,bounds.y-(py+size.y+border.y));

        // set
        dpos.set(px, py);
        
    }
    
    
}

/**
 * Draws the Tooltip.
 */
void Tooltip::draw() {
    
    // blend
    gl::enableAlphaBlending();
    
    // background
    Rectf rect = Rectf(dpos.x,dpos.y,dpos.x+size.x,dpos.y+size.y);
    glColor4f(cbg.r,cbg.g,cbg.b,cbg.a);
    gl::drawSolidRect(rect, false);
        
    // unblend
    gl::enableAlphaBlending(true);
        
    // draw
    gl::draw(textureText, Vec2d(dpos.x,dpos.y)+inset);
    
    // reset
    gl::disableAlphaBlending();
    
}


#pragma mark -
#pragma mark Business



/**
 * Renders the text.
 */
void Tooltip::renderText(vector<string> txts) {
    GLog();
    
    
    // text
    TextLayout tlText;
	tlText.clear(ColorA(0, 0, 0, 0));
	tlText.setFont(font);
	tlText.setColor(ctxt);
    
    // lines
    for (vector<string>::iterator txt = txts.begin(); txt != txts.end(); ++txt) {
        
        // line
        tlText.setFont(sfont);
        tlText.addLine(" ");
        tlText.setFont(font);
        tlText.addLine((*txt));
    }
    
    // render
	Surface8u rendered = tlText.render(true, true);
    
    
    // resize
    int resized_w = min(rendered.getWidth(),(int)bounds.x-maxed.x);
    int resized_h = min(rendered.getHeight(),(int)bounds.y-maxed.y);
    Surface resized = Surface( resized_w, resized_h, true, SurfaceChannelOrder::RGBA );
    for (int x = 0; x < resized_w; x++) {
        for (int y = 0; y < resized_h; y++) {
            resized.setPixel(Vec2i(x,y), rendered.getPixel(Vec2i(x,y)));
        }
    }
    
    // texture
	textureText = gl::Texture(resized);
    
    
    // off
    size.x = textureText.getWidth() + 2*abs(inset.x);
    size.y = textureText.getHeight()+ 2*abs(inset.y);
    
}

/**
 * Show / Hide.
 */
void Tooltip::show() {
    
    // props
    timeout = tooltipTimeout;
    
}
void Tooltip::hide() {
    
    // state
    active = false;
    
    // nirvana it is
    timeout = -1;
    dpos.set(-100000,-100000);

}


/**
 * Activate.
 */
void Tooltip::activate() {
    
    // state
    active = true;
    
    // reset timeout
    timeout = -1;
}

/**
 * Position.
 */
void Tooltip::position(Vec2d p) {
    pos = p;
}

/**
 * Offset.
 */
void Tooltip::offset(double o) {
    off.set(0,- max(o,retina ? 150.0 : 75.0));
}



