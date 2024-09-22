#include "location.h" /* specifying full path is unecessary due to target_include_directories in cmake*/
#include <string>
#include <stdexcept>
#include <algorithm>

Location::Location(std::string _latitude, std::string _longitude, std::string _name) {
    if (std::all_of(_name.begin(), _name.end(), ::isdigit))
        throw std::runtime_error("location names can't be exclusively numeric");

    name = _name;
    latitude = str_to_float(_latitude);
    longitude = str_to_float(_longitude);
}

std::string Location::serialise() {
    return format_float(latitude, 4) + " " + format_float(longitude, 4) + " " + name;
}
