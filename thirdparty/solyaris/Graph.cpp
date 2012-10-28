//
//  Graph.cpp
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

#include "Graph.h"


#pragma mark -
#pragma mark Object

/**
 * Creates a graph.
 */
Graph::Graph() {
}
Graph::Graph(int w, int h, int o) {
    
    // fields
    width = w;
    height = h;
    orientation = o;
    portrait = (h > w) ? true : false; 
    
    // retina
    retina = false;
    dpr = 1.0;
    
    // virtual offset
    voff.set(0,0);
    vpoff.set(0,0);
    vmoff.set(0,0);
    mbound = 90.0;
    
    // virtual position
    vpos.set(0,0);
    vppos.set(0,0);
    
    // movement
    speed = 45;
    friction = 5.1;
    
    // hitarea
    harea = 15;
    
    // action
    nbtouch = 10;
    for (int t = 1; t <= nbtouch; t++) {
        actions[t] = Action();
    }
    
    // zoom
    scale = 1.0;
    translate.set(0,0);
    
    // background
    bg_portrait = gl::Texture(1,1);
    bg_landscape = gl::Texture(1,1);
    
    // sample
    audioSampleClick = audio::load(loadResource(SAMPLE_CLICK));
}


#pragma mark -
#pragma mark Cinder

/**
 * Resize.
 */
void Graph::resize(int w, int h, int o) {
    FLog();
    
    // size
    width = w;
    height = h;
    
    // orientation
    orientation = o;
    portrait = (h > w) ? true : false;
    
}

/**
 * Applies the settings.
 */
void Graph::config(Configuration c) {
    
    // reference
    conf = c;
    
    // device
    redux = false;
    Config confDeviceRedux = c.getConfiguration(cDeviceRedux);
    if (confDeviceRedux.isSet()) {
        redux = confDeviceRedux.boolVal();
    }
    
    // display retina
    retina = false;
    Config confDisplayRetina = conf.getConfiguration(cDisplayRetina);
    if (confDisplayRetina.isSet()) {
        retina = confDisplayRetina.boolVal();
    }
    
    // action
    for (int t = 1; t <= nbtouch; t++) {
        actions[t].config(conf);
    }
    
    // device pixel ratio
    dpr = retina ? 2.0 : 1.0;
    
    // retina stuff
    if (retina) {
        
        // bound
        mbound *= 2;
        
        // hitarea
        harea *= 2;
        
    }
    
    
    // background
    string bg = redux ? "bg_graph_redux" : "bg_graph";
    bg += retina ? "@2x.png" : ".png";
    
    // surface
    Surface surface_portrait = loadImage(loadResource(bg));
    int surface_w = surface_portrait.getSize().x;
    int surface_h = surface_portrait.getSize().y;
    Surface surface_landscape = Surface( surface_h, surface_w, true, SurfaceChannelOrder::RGBA );
    for (int x = 0; x < surface_w; x++) {
        for (int y = 0; y < surface_h; y++) {
            surface_landscape.setPixel(Vec2i(y,x), surface_portrait.getPixel(Vec2i(x,y)));
        }
    }
    
    // textures
    bg_portrait = gl::Texture(surface_portrait);
    bg_landscape = gl::Texture(surface_landscape);
    
}

void Graph::setBackground(string bg)
{
    bg += retina ? "@2x.png" : ".png";
    
    // surface
    Surface surface_portrait = loadImage(loadResource(bg));
    int surface_w = surface_portrait.getSize().x;
    int surface_h = surface_portrait.getSize().y;
    Surface surface_landscape = Surface( surface_h, surface_w, true, SurfaceChannelOrder::RGBA );
    for (int x = 0; x < surface_w; x++) {
        for (int y = 0; y < surface_h; y++) {
            surface_landscape.setPixel(Vec2i(y,x), surface_portrait.getPixel(Vec2i(x,y)));
        }
    }
    
    // textures
    bg_portrait = gl::Texture(surface_portrait);
    bg_landscape = gl::Texture(surface_landscape);
}

#pragma mark -
#pragma mark Sketch


/**
 * Updates the graph.
 */
void Graph::update() {
    

    // randomize
    Rand::randomize();
    
    // virtual position
    Vec2d dd = vmpos - vpos;
    vppos = vpos;
    vpos += dd / friction;
    translate += (vpos - vppos);
    
    
    // virtual offset
    Vec2d dm = vmoff - voff;
    vpoff = voff;
    voff += dm/speed;
    Vec2d vmove = (voff - vpoff);
    
    
    // nodes
    for (NodeIt node = nodes.begin(); node != nodes.end(); ++node) {
        
        // active
        if ((*node)->isActive() || (*node)->isLoading()) {
            
            // global movement
            (*node)->move(vmove);
            
            // update
            (*node)->update();

        }
        
    }
    
    // connections
    for (ConnectionIt connection = connections.begin(); connection != connections.end(); ++connection) {
        if ((*connection)->isVisible()) {
            // update
            (*connection)->update();
        }
    }
    
    // actions
    for (int t = 1; t <= nbtouch; t++) {
        actions[t].update();
    }

}

/**
 * Draws the graph.
 */
void Graph::draw() {
    
    
    // background
    gl::color( ColorA(1.0f, 1.0f, 1.0f, 1.0f) ); // alpha channel
    gl::draw(portrait ? bg_portrait : bg_landscape);
    
    // push it
    gl::pushMatrices();
    gl::translate(translate);
    gl::scale(Vec2d(scale,scale));
    
    // connections
    for (ConnectionIt connection = connections.begin(); connection != connections.end(); ++connection) {;
        
        if ((*connection)->isVisible()) {
            // draw
            (*connection)->draw();
        }
    }
    
    // nodes
    for (NodeIt node = nodes.begin(); node != nodes.end(); ++node) {
        
        // draw if visible and on stage
        if ((*node)->isVisible() && this->onStage(*node)) {
            (*node)->draw();
        }
    }
    
    // actions
    for (int t = 1; t <= nbtouch; t++) {
        actions[t].draw();
    }
    
    // pop
    gl::popMatrices();

}


/**
 * Resets the graph.
 */
void Graph::reset() {
    DLog();
    
    // clear
    connections.clear();
    nodes.clear();
    
    // zoom
    scale = 1.0;
    translate.set(0,0);
    
}



#pragma mark -
#pragma mark Touch

/**
 * Touch.
 */
NodePtr Graph::touchBegan(Vec2d tpos, int tid) {
    GLog();
    
    // zoomed
    Vec2d ztpos = (tpos - translate)*(1.0/scale);
    
    
    // actions
    for (int t = 1; t <= nbtouch; t++) {
        if (actions[t].isActive()) {
            
            // check
            if (actions[t].action(ztpos)) {
                
                // node
                NodePtr n = actions[t].node.lock();
                if (n) {
                    n->setAction(actions[t].act);
                }
                
                // deactivate actions
                actions[t].deactivate();
                
                // enough for today
                return n;
            }
        }
    }
    
    // nodes
    for (NodeIt node = nodes.begin(); node != nodes.end(); ++node) {
        
        // visible
        if ((*node)->isVisible()) {
            
            // distance
            float d = (*node)->pos.distance(ztpos);
            if (d < (*node)->core+harea) {
                
                // touched
                GLog("tid = %d, node = ",tid);
                touched[tid] = NodePtr(*node); 
                
                // state
                touched[tid]->touched();
                
                // set the action
                this->action(tid);
                
                // sample
                this->sample(sampleClick);
                
                // have a break
                return *node;
            }
        }
    }
    
    // empty
    return NodePtr();

    
}
void Graph::touchMoved(Vec2d tpos, Vec2d ppos, int tid){
    GLog();

    // node
    if (touched[tid]) {
        GLog("tid = %d, node = ",tid);
        
        // zoom
        Vec2d ztpos = (tpos - translate)*(1.0/scale);
        
        // move
        touched[tid]->moveTo(ztpos);


    }
    // graph
    else {
        
        // drag
        this->drag(tpos-ppos);
    }
    
}
void Graph::touchEnded(Vec2d tpos, int tid){
    GLog();
    
    // node
    if (touched[tid]) {
        
        // state
        GLog("tid = %d, node = ",tid);
        touched[tid]->untouched();
        
        // hide action
        actions[tid].hide();
    }
    // graph
    else {
        
        // undrag
        this->drag(Vec2d(0,0));
    }
    
    // reset
    touched.erase(tid);

}


#pragma mark -
#pragma mark Taps

/**
 * Tapped.
 */
NodePtr Graph::doubleTap(Vec2d tpos, int tid) {
    GLog();
    
    // zoom
    Vec2d ztpos = (tpos - translate)*(1.0/scale);
    
    // nodes
    for (NodeIt node = nodes.begin(); node != nodes.end(); ++node) {
        
        // visible
        if ((*node)->isVisible()) {
            
            // distance
            float d = (*node)->pos.distance(ztpos);
            if (d < (*node)->core+harea) {
                
                // tapped
                (*node)->tapped();
                
                // sample
                this->sample(sampleClick);
              
                // return
                return (*node);
            }
        }
    }
    
    // nop
    return NodePtr();
    
}



#pragma mark -
#pragma mark Gestures


/**
 * Pinched.
 * @url: http://www.gamedev.net/topic/560485-how-can-you-calculate-translation-while-zooming-in-with-zoom-gestures/
 */
void Graph::pinched(Vec2d p, Vec2d pp, double s, double ps) {
    GLog();
    
    // scale pinch point
    Vec2d pt = p*dpr;
    pt -= translate;
    pt *= (1.0/scale);
    
    // value
    double cs = s - ps;
    
    // scale
    scale += (cs*0.5);
    scale = min(scale, 1.3); 
    scale = max(scale, 0.3);
    
    // translate
    translate = -1 * (pt * scale - p*dpr); 
    
    // drag
    this->drag((p-pp) * scale * dpr);
    
}




#pragma mark -
#pragma mark Business


/**
 * Attraction.
 */
void Graph::attract() {
    
    // nodes
    for (NodeIt n1 = nodes.begin(); n1 != nodes.end(); ++n1) {
        
        // attract others
        for (NodeIt n2 = nodes.begin(); n2 != nodes.end(); ++n2) {
            if ((*n1)->isActive() && (*n2)->isActive() && ! (*n1)->isClosed() && ! (*n2)->isClosed() && (*n1) != (*n2)) {
                (*n1)->attract(*n2);
            }
        }
    }
    
}

/**
 * Repulsion.
 */
void Graph::repulse() {
    
}


/**
 * Move.
 */
void Graph::move(Vec2d d) {
    vmoff += d;
}

/**
 * Drag.
 */
void Graph::drag(Vec2d d) {
    
    // translate
    vmpos += d;
    
    // threshold
    float thresh = 1.0;
    if (abs(vmpos.x) < thresh && abs(vmpos.y) < thresh) {
        vmpos.set(0,0);
    }
}

/**
 * Shift.
 */
void Graph::shift(Vec2d d) {
    
    // zoom
    Vec2d zd = d*(1.0/scale)*dpr;
    
    // off
    vmoff += zd;
}


/**
 * Calculates the real world coordinates.
 */
Vec3d Graph::coordinates(double px, double py, double d) {
    
    // coordinates    
    Vec3d gpos = Vec3d(px,py,0);
    Vec3d rpos = gpos * scale + Vec3d(translate.x,translate.y,0);
    
    // distance
    rpos.z = d * scale;
    
    // retina
    rpos *= (retina ? 0.5 : 1.0);
    
    // back
    return rpos;
}




/**
 * Creates a node.
 */
NodePtr Graph::createNode(unsigned int nid, string type) {
    GLog();
    
    // scale
    double sf = (1.0/scale);
    
    // position
    int b = 150*sf;
    Vec2d np = Vec2d( b, (height*sf/2.0) );
    
    // create
    return createNode(nid,type,np.x,np.y);
}
NodePtr Graph::createNode(unsigned int nid, string type, double x, double y) {
    GLog();
    
    boost::shared_ptr<Node> node(new Node(nid,x,y));
    node->sref = node;
    node->config(conf);
    nodes.push_back(node);
    return node;
}

/**
 * Gets a node.
 */
NodePtr Graph::getNode(unsigned int nid) {
    GLog();
    
    for (NodeIt node = nodes.begin(); node != nodes.end(); ++node) {
        if ((*node)->nid == nid) {
            return *node;
        }
    }
    
    // nop
    return NodePtr();
}

/**
 * Creates a connection.
 */
ConnectionPtr Graph::createConnection(NodePtr n1, NodePtr n2) {
    GLog();
    
    boost::shared_ptr<Connection> connection(new Connection(n1,n2));
    connection->config(conf);
    connections.push_back(connection);
    return connection;
    
}

/**
 * Gets a connection.
 */
ConnectionPtr Graph::getConnection(unsigned int nid) {
    GLog();
    
    for (ConnectionIt connection = connections.begin(); connection != connections.end(); ++connection) {
        
        unsigned int cnid1 = (*connection)->nid1;
        unsigned int cnid2 = (*connection)->nid2;
        
        if (cnid1 == nid || cnid2  == nid) {
            return *connection;
        }
    }
    
    // no connection
    return ConnectionPtr();
}

/**
 * Gets a connection.
 */
ConnectionPtr Graph::getConnection(unsigned int nid1, unsigned int nid2) {
    GLog();
    
    for (ConnectionIt connection = connections.begin(); connection != connections.end(); ++connection) {
        
        unsigned int cnid1 = (*connection)->nid1;
        unsigned int cnid2 = (*connection)->nid2;
        
        if (cnid1 == nid1 || cnid1  == nid2 || cnid2 == nid1 || cnid2 == nid2) {
            return *connection;
        }
    }
    
    // no connection
    return ConnectionPtr();
}


/**
 * Removes a node.
 */
void Graph::removeNode(unsigned int nid) {
    FLog();
    
    // erase from nodes
    int eraser = -1;
    int index = 0;
    for (NodeIt node = nodes.begin(); node != nodes.end(); ++node) {
        if ((*node)->nid == nid) {
            eraser = index;
        }
        index++;
    }
    if (eraser >= 0) {
        nodes.erase(nodes.begin()+eraser); 
    }
}


/**
 * Spawn subnodes
 */

void Graph::expand(NodePtr parent, NodeVectorPtr nodes)
{
        
        parent->setChildren(nodes);
        
        unsigned int nCount = nodes.size();
        
        int index = 0;
        for (NodeIt node = nodes.begin(); node != nodes.end(); ++node) {
            
            ConnectionPtr connection = createConnection(parent,*node);
            connection->show();
            
            float yDistrib = nCount * (parent->radius*3.0 + 0.1);

            float rx = (parent->radius + 0.1)*nCount;
            float ry = (parent->radius*3.0 + 0.1)*index - yDistrib/2.0;
            
            Vec2d finalPos = Vec2d(parent->pos.x+rx,parent->pos.y+ry);

            // set parent position
            (*node)->pos.set(Vec2d(parent->pos.x,parent->pos.y));

            // distance
            Vec2d cdist =  finalPos - parent->pos;
            
            (*node)->show(TRUE);
            
            // move
            (*node)->moveTo(parent->pos+cdist);
        
            
            index++;
        }
}

void Graph::hideChildren(NodePtr parent)
{
    NodeVectorPtr nodes = parent->children;
    
    for (NodeIt node = nodes.begin(); node != nodes.end(); ++node) {
        unsigned int nid = (*node)->nid;
        
        hideChildren((*node));
        
        ConnectionPtr connection = getConnection(nid);
        
        if(connection != NULL)
        {
            connection->hide();
        }
        
        (*node)->hide();
    }
}


void Graph::hideSubChildren(NodePtr parent)
{
    NodeVectorPtr nodes = parent->children;
    for (NodeIt node = nodes.begin(); node != nodes.end(); ++node) {
        hideChildren(*node);
    }
}

/**
 * Prepares the graph for loading.
 */
void Graph::load(NodePtr n) {
    FLog();
    
    // scale
    float sf = (1.0/scale);
    Vec2d p = (n)->mpos + (translate * sf);
    
    // bounds
    Vec2d d = Vec2d(0,0);
    if (p.x < mbound*sf) {
        d.x = mbound*sf - p.x;
    }
    else if (p.x > (width-mbound)*sf) {
        d.x = - (p.x - (width-mbound)*sf);
    }
    if (p.y < mbound*sf) {
        d.y = mbound*sf - p.y;
    }
    else if (p.y > (height-mbound)*sf) {
        d.y = - (p.y - (height-mbound)*sf);
    }
    
    // move it
    this->move(d);
}

/**
 * Unloads a node.
 */
void Graph::unload(NodePtr n) {
    FLog();
    
    // parent
    NodePtr pp = n->parent.lock();
    if (pp) {
        
        // unload
        n->unload();
    }
    else {
        this->removeNode(n->nid);
    }

}

/**
 * Indicates if a node is on stage.
 */
bool Graph::onStage(NodePtr n) {
    
    // scale
    float sf = (1.0/scale);
    Vec2d p = (n)->pos + (translate * sf);
    
    // borderline
    float b = 300;
    return p.x > -b*sf && p.x < (width+b)*sf && p.y > -b*sf && p.y < (height+b)*sf;
    
}

/**
 * Sets the action.
 */
void Graph::action(int tid) {
    GLog();
    
    // showtime
    if (touched[tid]->isActive() && ! touched[tid]->isClosed()) {
        actions[tid].assignNode(touched[tid]);
        actions[tid].show();
    }
    
}


/**
 * Sample player.
 */
void Graph::sample(int s) {
    
    // play it again sam
    switch(s) {
            // click
        case sampleClick:
            try {
                audio::Output::play(audioSampleClick);
            }
            catch (...) {
                // ignore
            }
            break;
        default:
            break;
    }

}


