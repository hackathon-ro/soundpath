//
//  I18N.cpp
//  Solyaris
//
//  Created by Beat Raess on 15.12.2011.
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

#include "I18N.h"


#pragma mark -
#pragma mark I18N Object

/**
 * I18N.
 */
I18N::I18N() {
}



#pragma mark -
#pragma mark Accessors

/**
 * Set/get default.
 */
void I18N::setTranslation(string key, string value) {
    
    // add
    translations.insert(make_pair(key, value));
}
string I18N::getTranslation(string key, string def) {
    
    // search
    map<string,string>::iterator it = translations.find(key);
    if(it != translations.end()) {
        return it->second;
    }
    return def;
}


#pragma mark -
#pragma mark Business

/**
 * Translates.
 */
string I18N::translate(string msg, string key, string value) {
    
    // search
    string term = this->getTranslation(key, value);
    
    // replace
    int ind = msg.find(value);
    int len = value.length();
    if (ind >= 0 && len > 0) {
        return msg.replace(ind,len,term);
    }
    return msg;
}