//
//  Configuration.h
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

#pragma once
#include <boost/lexical_cast.hpp>
#include <iostream>
#include <string> 
#include <map>

// namespace
using namespace std;

// declare
class Config;


// constants
const string  cDeviceRedux          = "conf_device_redux";
const string  cDisplayRetina        = "conf_display_retina";



/**
 * Graph Configuration.
 */
class Configuration {
    
    // public
    public:
    
    // Setting
    Configuration();
    
    // Accessors
    void setConfiguration(string key, string value);
    Config getConfiguration(string key);
    
    // private
    private:
    
    // Fields
    map<string,Config>configurations;
};


/**
 * Graph Config.
 */
class Config {
    
    // public
    public:
    
    // Setting
    Config();
    Config(string k, string v); 
    
    // Accessors
    bool isSet();
    string stringVal();
    int intVal();
    float floatVal();
    double doubleVal();
    bool boolVal();
    
    // private
    private:
    
    // Fields
    string key;
    string value;
    bool set;
};
