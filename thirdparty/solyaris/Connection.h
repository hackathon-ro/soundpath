//
//  Connection.h
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

#pragma once
#include "Node.h"
#include "Configuration.h"
#include "Defaults.h"
#include <boost/ptr_container/ptr_vector.hpp>
#include <boost/shared_ptr.hpp>


// namespace
using namespace std;
using namespace ci;

// declarations
class Connection;

// typedef
typedef boost::shared_ptr<Connection> ConnectionPtr;
typedef std::vector<ConnectionPtr> ConnectionVectorPtr;
typedef ConnectionVectorPtr::iterator ConnectionIt;


// constants
const string connectionRelated = "related";	


/**
 * Graph Connection.
 */
class Connection {
    
    // public
    public:
    
    // Connection
    Connection();
    Connection(string idc, NodePtr n1, NodePtr n2); 
    
    // Cinder
    void config(Configuration c);
    void defaults(Defaults s);
    
    // Sketch
    void update();
    void draw();
    
    
    // Public Fields
    string cid;
    NodeWeakPtr wnode1;
    NodeWeakPtr wnode2;
    string type;
    
    
    // private
    private:
    
    // States
    bool selected;
    
    // config
    bool redux;
    bool retina;
    
    // color
    Color cstroke;
    Color cstrokes;
    
    // size
    float s;
    float d;
    
};

class ConnectionRelated: public Connection {
    
    // public
    public:
    
    // Connection
    ConnectionRelated();
    ConnectionRelated(string idc, NodePtr n1, NodePtr n2);
};



