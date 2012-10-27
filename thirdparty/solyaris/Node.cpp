//
//  Node.cpp
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

#include "Node.h"


#pragma mark -
#pragma mark Object

/**
 * Creates a Node.
 */
Node::Node() {
    Node("nid",0,0);
}
Node::Node(string idn, double x, double y) {
    GLog();
    
    // node
    nid = idn;
    parent = NodeWeakPtr();
    label = "";
    meta = "";
    type = "Node";
    category = "";
    action = "";
    
    
    // fields
    perimeter = 420;
    zone = perimeter / 9.0;
    dist = 480;
    damping = 0.5;
    strength = -1;
    stiffness = 0.05;
    distraction = 0.3;
    ramp = 1.2;
    mvelocity = 15;
    speed = 30;
    initial = 12;
    fcount = 0;
    retina = false;
    redux = false;
    
    
    // position
    pos.set(x,y);
    ppos.set(x,y);
    mpos.set(x,y);
    
    // state
    selected = false;
    active = false;
    closed = false;
    grow = false;
    shrink = false;
    loading = false;
    visible = false;
    
    // radius / mass
    core = 9;
    radius = 9;
    maxr = 90;
    minr = 60;
    mass = calcmass();
    
    // inc
    rincg = 1.5;
    rincs = 1.8;
    
    // velocity
    velocity.set(0,0);
    
    // color
    ctxt = Color(0.3,0.3,0.3);
    ctxta = Color(0.2,0.2,0.2);
    ctxts = Color(0.15,0.15,0.15);
    
    // alpha
    anode = 0.8;
    asnode = 0.9;
    acore = 0.8;
    ascore = 0.9;
    aglow = 0.3;
    asglow = 0.39;
    
    // textures
    textureNode = gl::Texture(1,1);
    textureCore = gl::Texture(1,1);
    textureGlow = gl::Texture(1,1);
    
    // font
    font = Font("Helvetica",12);
    textureLabel = gl::Texture(1,1);
    loff.set(0,5);

}

/**
 * Node movie.
 */
NodeMovie::NodeMovie(): Node::Node()  {    
}
NodeMovie::NodeMovie(string idn, double x, double y): Node::Node(idn, x, y) {
    
    // type
    this->updateType(nodeMovie);

}

/**
 * Node person.
 */
NodePerson::NodePerson(): Node::Node()  {    
}
NodePerson::NodePerson(string idn, double x, double y): Node::Node(idn, x, y) {
    
    // type
    this->updateType(nodePerson);
}



#pragma mark -
#pragma mark Cinder

/**
 * Applies the configuration.
 */
void Node::config(Configuration c) {
    
    // device
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
    
    // init redux
    if (redux) {
        minr *= 0.75;
        maxr *= 0.75;
    }
    
    // init retina
    if (retina) {
        
        // params
        core *= 2;
        radius *= 2;
        maxr *= 2;
        minr *= 2;
        
        // inc
        rincg *= 2;
        rincs *= 2;
        
        // fonts
        font = Font("Helvetica",24);
        loff *= 2;
    }
}


/**
 * Applies the defaults.
 */
void Node::defaults(Defaults d) {
    
    // children
    initial = redux ? 5 : 8;
    Default graphNodeInitial = d.getDefault(dGraphNodeInitial);
    if (graphNodeInitial.isSet()) {
        initial = (int) graphNodeInitial.doubleVal();
    }
    
    
    // distance
    double length = redux ? 300 : 480;
    Default graphEdgeLength = d.getDefault(dGraphEdgeLength);
    if (graphEdgeLength.isSet()) {
        length = graphEdgeLength.doubleVal();
    }
    
    // props
    dist = length*1.11;
    perimeter = length*0.9;
    zone = length / 9.0;
    
    // scale retina
    if (retina) {
        dist *= 2;
        perimeter *= 2;
        zone *= 2;
    }

}


#pragma mark -
#pragma mark Sketch

/**
* Updates the node.
*/
void Node::update() {
    
    // count
    fcount++;
    
    
    // limit
    velocity.limit(mvelocity);
    
    // threshold
    float thresh = 0.1;
    if (abs(velocity.x) < thresh && abs(velocity.y) < thresh) {
        velocity.x = 0;
        velocity.y = 0;
    }

    // damping
    float damp = active ? damping : (damping * 1.75);
    velocity *= (1 - damp);
    
    // add vel to moving position
    mpos += velocity;
    
    // update position
    Vec2d dm = mpos - pos;
    ppos = pos;
    pos += dm/speed;

    // grow
    if (grow) {
        
        // radius
        radius += rincg;
        
        // mass
        mass = calcmass();
        
        // grown
        if (radius >= growr) {
            this->grown();
        }
        
    }
    
    // shrink
    if (shrink) {
        
        // radius
        radius -= rincs;
        
        // mass
        mass = calcmass();
        
        // shrunk
        if (radius <= shrinkr) {
            this->shrinked();
        }
        
        
    }

    
}




/**
* Draws the node.
*/
void Node::draw() {
    
    
    // blend
    gl::enableAlphaBlending();
    
    // node expanded
    if (active || loading) {
        
        // core
        float ca = selected ? ascore : acore;
        gl::color( ColorA(1.0f, 1.0f, 1.0f, ca) ); // alpha channel
        gl::draw(textureCore, Rectf(pos.x-core,pos.y-core,pos.x+core,pos.y+core));
        
        // glow
        float ga = selected ? asglow : aglow;
        if (loading && ! grow) {
            ga *= (1.15+sin((fcount*1.15*M_PI)/180));
            ga = fmin(0.79,ga);
        }
        gl::color( ColorA(1.0f, 1.0f, 1.0f, ga) ); // alpha channel
        gl::draw(textureGlow, Rectf(pos.x-radius,pos.y-radius,pos.x+radius,pos.y+radius));
        
    }
    else {
        
        // node
        float na = selected ? asnode : anode;
        gl::color( ColorA(1.0f, 1.0f, 1.0f, na) ); // alpha channel
        gl::draw(textureNode, Rectf(pos.x-core,pos.y-core,pos.x+core,pos.y+core));
        
    }
    
    
    // label
    if (active || ! closed) {
        
        // unblend
        gl::enableAlphaBlending(true);
        
        // drawy thingy
        selected ? gl::color(ctxts) : (active ? gl::color(ctxta) : gl::color(ctxt));
        gl::draw(textureLabel, Vec2d(pos.x+loff.x, pos.y+core+loff.y));
    }

    
    // reset
    gl::disableAlphaBlending();

}




#pragma mark -
#pragma mark Business

/**
* Node attraction.
*/
void Node::attract(NodePtr node) {
    

    // distance
    double d = pos.distance((*node).pos);
    if (d > 0 && d < perimeter) {
        
        // force
        double s = pow(d / perimeter, 1 / ramp);
        double m = selected ? mass*2 : mass;
        double force = s * 9 * strength * (1 / (s + 1) + ((s - 3) / 4)) / d;
        Vec2d df = (pos - (*node).pos) * (force/m);
        
        // velocity
        (*node).velocity += df;
    }

}

/**
 * Node distraction.
 */
void Node::distract(NodePtr node) {
    
    
    // distance
    double d = pos.distance((*node).pos);
    if (d > 0 && d < zone) {
        
        // force
        double s = pow(d / zone, 1 / ramp);
        double force = s * 9 * strength * (1 / (s + 1) + ((s - 3) / 4)) / d;
        Vec2d df = (pos - (*node).pos) * (force/mass) * distraction;
        
        // velocity
        (*node).velocity += df;
    }
    
}

/**
 * Node repulsion.
 */
void Node::repulse(Vec2d p, float dist, float dir) {
    
    // distance vector
    Vec2d diff = p - this->pos;
    
    // normalize / length
    diff.safeNormalize();
    diff *= dist;
    
    // target
    Vec2d target = p + diff;
    
    // force
    Vec2d force = this->pos - target;
    force *= stiffness;

    // update velocity
    this->velocity += force*dir;
}


/**
 * Move.
 */
void Node::move(double dx, double dy) {
    mpos.x += dx;
    mpos.y += dy;
}
void Node::move(Vec2d d) {
    mpos += d;
}
void Node::moveTo(double x, double y) {
    mpos.x = x;
    mpos.y = y;
}
void Node::moveTo(Vec2d p) {
    mpos.set(p);
}

/**
 * Translate.
 */
void Node::translate(Vec2d d) {
    pos += d;
    mpos += d;
}


/**
* Adds a child.
*/
void Node::addChild(NodePtr child) {
    GLog();
    
    // push
    children.push_back(child);
}

/**
 * Node is grown.
 */
void Node::grown() {
    FLog();
    
    // state
    loading = false;
    grow = false;

    // mass
    mass = calcmass();
    
    // state
    if (! active) {
        
        // born
        this->born();
    }
    else {
        
        // unfold
        this->unfold();
    }

      
}

/**
 * Node is shrinked.
 */
void Node::shrinked() {
    FLog();
    
    // state
    shrink = false;
    
    // mass
    mass = calcmass();
    
    // fold
    this->fold();
    
}


/**
 * Born.
 */
void Node::born() {
    FLog();
    
    // state
    active = true;
    closed = false;
    
    // children
    int nb = initial;
    NodeVectorPtr cnodes;
    for (NodeIt child = children.begin(); child != children.end(); ++child) {
        
        // filter existing
        if (! (*child)->isActive()) {
            
            // parent
            if ((*child)->parent.lock()) {
                
                // unhide
                (*child)->show(true);
                
            }
            
            // adopt child
            else  {
                
                // adopt child
                (*child)->parent = sref;
                
                // director
                if ((*child)->type == nodePersonDirector) {
                    (*child)->show(false);
                    cnodes.push_back(*child);
                }
                
                // actor / movie
                if (((*child)->type == nodePersonActor || (*child)->type == nodeMovie)) {
                    
                    // show
                    if (nb > 0) {
                        (*child)->show(false);
                        cnodes.push_back(*child);
                        nb--;
                    }
                    else if (! (*child)->isActive()) {
                        (*child)->hide(); 
                    }
                }
                
                
            }
            
        }
        
        
    }
    
    // position children
    this->cposition(cnodes);
    
}

/**
 * Unfold.
 */
void Node::unfold() {
    FLog();
    
    // children
    NodeVectorPtr cnodes;
    for (NodeIt child = children.begin(); child != children.end(); ++child) {
        
        // open child
        if (this->isNodeChild(*child)) {
            
            // radius & position
            float rx = Rand::randFloat(radius * nodeUnfoldMin,radius * nodeUnfoldMax) + 0.1;
            rx *= (Rand::randFloat(1) > 0.5) ? 1.0 : -1.0;
            float ry = Rand::randFloat(radius * nodeUnfoldMin,radius * nodeUnfoldMax) + 0.1;
            ry *= (Rand::randFloat(1) > 0.5) ? 1.0 : -1.0;
            Vec2d p = Vec2d(pos.x+rx,pos.y+ry);
            
            // open & push
            (*child)->open();
            cnodes.push_back(*child);
        }
        
    }
    
    // position children
    this->cposition(cnodes);
    
}

/**
 * Fold.
 */
void Node::fold() {
    FLog();
    
    // children
    NodeVectorPtr cnodes;
    for (NodeIt child = children.begin(); child != children.end(); ++child) {
        
        // move it
        if (this->isNodeChild(*child)) {
            
            // push
            cnodes.push_back(*child);
            
        }
    }
    
    // position children
    this->cposition(cnodes);

}



/**
 * Load noad.
 */
void Node::load() {
    FLog();
    
    // state
    visible = true;
    loading = true;
    
    // radius
    core = retina ? 30 : 15;
    radius = retina ? 72 : 36;
    
    // color
    ctxt = Color(0.6,0.6,0.6);
    
    // font
    font = Font("Helvetica-Bold",retina ? 30 : 15);
    loff.y = retina ? 12 : 6;
    this->renderLabel(label);
    this->renderNode();
    
}
void Node::unload() {
    FLog();
    
    // state
    visible = true;
    loading = false;
    
    // radius
    core = retina ? 18 : 9;
    radius = retina ? 18 : 9;
    
    // color
    ctxt = Color(0.75,0.75,0.75);
    
    // font
    font = Font("Helvetica",retina ? 24 : 12);
    loff.y = retina ? 10 : 5;
    this->renderLabel(label);
    
    // parent
    NodePtr pp = this->parent.lock();
    if (pp) {
        
        // radius & position
        Vec2d back = pp->pos + ((this->pos - pp->pos) / 2.0);
        this->moveTo(back);
    }
    
    
}
void Node::loaded() {
    FLog();

    // state
    growr = ((int)children.size()) > 1 ? min(minr+(int)children.size(),maxr) : minr * 0.75;
    grow = true;
  
}

/**
 * Close / open node.
 */
void Node::close() {
    GLog();
    
    // state
    closed = true;
    
    // active
    if (active) {
        
        // shrink
        shrinkr = minr * 0.5;
        shrink = true;
        
        // children
        for (NodeIt child = children.begin(); child != children.end(); ++child) {
            
            // close child
            if (this->isNodeChild(*child)) {
                (*child)->close();
            }
            
        }
    }
    
}


/**
 * Reactivate node.
 */
void Node::open() {
    GLog();
    
    // state
    closed = false;
    
    // active
    if (active) {
        
        // state
        growr = ((int)children.size()) > 1 ? min(minr+(int)children.size(),maxr) : minr * 0.75;
        grow = true;
        
    }
    
}


/**
 * Shows/Hides the node.
 */
void Node::show(bool position) {
    GLog();
    
    // show it
    if (! visible) {
        
        // parent
        NodePtr pp = this->parent.lock();
        if (pp) {
            
            // base position
            this->pos.set(pp->pos);
            this->mpos.set(pp->pos);
            
            // position
            if (position) {
                
                // radius & position
                float rx = Rand::randFloat(pp->radius * nodeUnfoldMin,pp->radius * nodeUnfoldMax) + 0.1;
                rx *= (Rand::randFloat(1) > 0.5) ? 1.0 : -1.0;
                float ry = Rand::randFloat(pp->radius * nodeUnfoldMin,pp->radius * nodeUnfoldMax) + 0.1;
                ry *= (Rand::randFloat(1) > 0.5) ? 1.0 : -1.0;
                Vec2d p = Vec2d(pp->pos.x+rx,pp->pos.y+ry);
                
                // set
                this->pos.set(p);
                this->mpos.set(p);
            }
            

        }
        
    }
    
    // state
    visible = true;
    
}
void Node::hide() {
    GLog();
    
    // state
    visible = false;
    
}


/**
 * Positions the children.
 */
void Node::cposition(NodeVectorPtr cnodes) {
    GLog();
    
    // randomize
    Rand::randomize();
    
    // number
    float cnb = cnodes.size();
    
    // radius
    float rmin = closed ? radius * 0.25 : radius * nodeUnfoldMin * 0.75;
    float rmax = radius * nodeUnfoldMax * 0.75;
    
    // angle
    float a = 360.0/cnb; 
    float ca = Rand::randFloat(-130.0,-110.0);
    
    // child nodes
    for (NodeIt cnode = cnodes.begin(); cnode != cnodes.end(); ++cnode) {
        
        // randomize radius / angle
        float rr = Rand::randFloat(rmin,rmax) + 0.1;
        float ra = Rand::randFloat(ca-a/2.0,ca+a/2.0);
        
        // position
        Vec2d p = Vec2d(pos.x+(rr * cos(ra * M_PI / 180.0)),pos.y+(rr * sin(ra * M_PI / 180.0)));
        
        // move
        (*cnode)->moveTo(p);
        
        // angle
        ca += a;
    }
    
}


/**
 * Child.
 */
bool Node::isNodeChild(NodePtr n) {
    
    // parent
    NodePtr cp = n->parent.lock();
    
    // active
    bool available = ! (n->isActive() || n->isLoading()) && n->isVisible();
    return available && cp->nid == this->nid;
}


/**
 * Touched.
 */
void Node::touched() {
    GLog();
    
    // state
    selected = true;
    
}
void Node::untouched() {
    GLog();
    
    // state
    selected = false;
    
}


/**
 * Tapped.
 */
void Node::tapped() {
    FLog();
    
    // state
    selected = false;
    
    // show
    if (! visible) {
        this->show(true);
    }
    
    // reposition
    if (! active && ! loading) {
        
        // parent
        NodePtr pp = this->parent.lock();
        if (pp) {
            
            // distance to parent
            Vec2d pdist =  pos - pp->pos;
            if (pdist.length() < dist) {
                
                // unity vector
                pdist.safeNormalize();
                
                // move
                this->moveTo(pp->pos+pdist*dist);
            }
        }
    
    }
    
}

/**
 * Connect.
 */
void Node::connect(NodePtr n) {
    FLog();
    
    // reposition
    if (! n->active && ! n->loading) {
        
        // randomize position
        float rx = Rand::randFloat(this->radius * nodeUnfoldMin,this->radius * nodeUnfoldMax) + 0.1;
        rx *= (Rand::randFloat(1) > 0.5) ? 1.0 : -1.0;
        float ry = Rand::randFloat(this->radius * nodeUnfoldMin,this->radius * nodeUnfoldMax) + 0.1;
        ry *= (Rand::randFloat(1) > 0.5) ? 1.0 : -1.0;
        
        // set
        n->pos.set(Vec2d(this->pos.x+rx,this->pos.y+ry));
        
        // distance
        Vec2d cdist =  n->pos - this->pos;
        if (cdist.length() < dist) {
            
            // unity vector
            cdist.safeNormalize();
            
            // move
            n->moveTo(this->pos+cdist*dist*0.75);
        }
        
    }
    
}


/**
 * States.
 */
bool Node::isActive() {
    return active;
}
bool Node::isClosed() {
    return closed;
}
bool Node::isVisible() {
    return visible;
}
bool Node::isSelected() {
    return selected;
}
bool Node::isLoading() {
    return loading;
}



/**
 * Renders the label.
 */
void Node::renderLabel(string lbl) {
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

/*
 * Renders the node.
 */
void Node::renderNode() {
    GLog();
    
    // suffix
    string sfx = retina ? "@2x.png" : ".png";
    
    // movie
    if (type == nodeMovie) {
        
        // category
        string cat = (category.length()) > 0 ? ("_" + category) : "";
        
        // texture
        textureNode = gl::Texture(loadImage(loadResource("node_movie"+sfx)));
        if (active || loading) {
            textureCore = gl::Texture(loadImage(loadResource("node_movie_core"+cat+sfx)));
            textureGlow = gl::Texture(loadImage(loadResource("node_movie_glow"+cat+sfx)));
        }
        
    }
    // director / crew
    else if (type == nodePersonDirector || type == nodePersonCrew) {
        
        // texture
        textureNode = gl::Texture(loadImage(loadResource("node_crew"+sfx)));
        if  (active || loading) {
            textureCore = gl::Texture(loadImage(loadResource("node_crew_core"+sfx)));
            textureGlow = gl::Texture(loadImage(loadResource("node_crew_glow"+sfx)));
        }
        
    }
    // person
    else {
        
        // texture
        textureNode = gl::Texture(loadImage(loadResource("node_person"+sfx)));
        if (active || loading) {
            textureCore = gl::Texture(loadImage(loadResource("node_person_core"+sfx)));
            textureGlow = gl::Texture(loadImage(loadResource("node_person_glow"+sfx)));
        }
    }
}

/**
 * Updates the type.
 */
void Node::updateType(string t) {
    
    // type
    type = t;
    
    // render
    this->renderNode();
}

/**
 * Updates the category.
 */
void Node::updateCategory(string c) {
    
    // category
    category = c;
    
    // render
    this->renderNode();
}


/**
 * Updates the meta.
 */
void Node::updateMeta(string m) {
    meta = m;
}

/**
 * Sets the action.
 */
void Node::setAction(string a) {
    action = a;
}

/*
 * Calculates the mass.
 */
float Node::calcmass() {
    return radius * radius * 0.0001f + 0.01f;
}




