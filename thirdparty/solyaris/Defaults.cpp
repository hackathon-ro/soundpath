//
//  Defaults.cpp
//  Solyaris
//
//  Created by CNPP on 19.8.2011.
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

#include "Defaults.h"


#pragma mark -
#pragma mark Defaults Object

/**
 * Defaults.
 */
Defaults::Defaults() {
}



#pragma mark -
#pragma mark Defaults Accessors

/**
 * Set/get default.
 */
void Defaults::setDefault(string key, string value) {
   
    // add
    defaults.insert(make_pair(key, Default(key,value)));
}
Default Defaults::getDefault(string key) {
    
    // search
    map<string,Default>::iterator it = defaults.find(key);
    if(it != defaults.end()) {
        return it->second;
    }
    return Default();
}



#pragma mark -
#pragma mark Default Object

/**
 * Default.
 */
Default::Default() {
    set = false;
}
Default::Default(string k, string v) {
    
    // fields
    key = k;
    value = v;
    set = true;
}


#pragma mark -
#pragma mark Default Accessors

/**
 * Check.
 */
bool Default::isSet() {
    return set;
}

/**
 * Returns the value.
 */
string Default::stringVal() {
    return value;
}
int Default::intVal() {
    return boost::lexical_cast<int>(value);
}
float Default::floatVal() {
    return boost::lexical_cast<float>(value);
}
double Default::doubleVal() {
    return boost::lexical_cast<double>(value);
}
bool Default::boolVal() {
    return boost::lexical_cast<bool>(value);
}
