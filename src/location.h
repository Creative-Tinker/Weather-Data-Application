#pragma once

#include <string>
#include <stdexcept>
#include <algorithm>
#include "utility.h" 

class Location {
public:
    std::string name; 
    float latitude;
    float longitude;

    Location(std::string _latitude, std::string _longitude, std::string _name);
    std::string serialise();
};