#pragma once

#include <string>

class Unit {
public:
    std::string technical, display; /* technical: m/s, display : metres per second*/
    Unit(std::string technical, std::string display);

};