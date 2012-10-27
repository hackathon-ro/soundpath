//
//  I18N.h
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

#pragma once
#include <iostream>
#include <string> 
#include <map>

// namespace
using namespace std;

// constants
const string i18nTooltipActor1 = "i18n_tooltip_actor_1";
const string i18nTooltipActor2 = "i18n_tooltip_actor_2";
const string i18nTooltipCrew1 = "i18n_tooltip_crew_1";
const string i18nTooltipCrew2 = "i18n_tooltip_crew_2";


/**
 * I18N.
 */
class I18N {
    
    // public
    public:
    
    // Setting
    I18N();
    
    // Accessors
    void setTranslation(string key, string value);
    string getTranslation(string key, string def);
    string translate(string msg, string key, string value);
    
    // private
private:
    
    // Fields
    map<string,string>translations;
};
