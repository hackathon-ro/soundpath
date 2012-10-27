//
//  Configuration.cpp
//  Solyaris
//
//  Created by Beat Raess on 28.3.2012.
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

#include "Configuration.h"


#pragma mark -
#pragma mark Configuration Object

/**
 * Configuration.
 */
Configuration::Configuration() {
}



#pragma mark -
#pragma mark Configuration Accessors

/**
 * Set/get configuration.
 */
void Configuration::setConfiguration(string key, string value) {
    
    // add
    configurations.insert(make_pair(key, Config(key,value)));
}
Config Configuration::getConfiguration(string key) {
    
    // search
    map<string,Config>::iterator it = configurations.find(key);
    if(it != configurations.end()) {
        return it->second;
    }
    return Config();
}



#pragma mark -
#pragma mark Config Object

/**
 * Default.
 */
Config::Config() {
    set = false;
}
Config::Config(string k, string v) {
    
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
bool Config::isSet() {
    return set;
}

/**
 * Returns the value.
 */
string Config::stringVal() {
    return value;
}
int Config::intVal() {
    return boost::lexical_cast<int>(value);
}
float Config::floatVal() {
    return boost::lexical_cast<float>(value);
}
double Config::doubleVal() {
    return boost::lexical_cast<double>(value);
}
bool Config::boolVal() {
    return boost::lexical_cast<bool>(value);
}
