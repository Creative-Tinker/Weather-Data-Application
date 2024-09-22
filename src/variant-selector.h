#pragma once

#include <string>
#include <vector>
#include <algorithm>
#include <stdexcept>
#include "utility.h"

/*Select different options such as humidity/windspeed/precipitation*/
class VariantSelection {
public:
    std::string selected;

    VariantSelection() {}

    void init(std::string _name, std::vector<std::string> _variants);

    bool includes(std::string needle);

    void try_modify(std::string value);

private:
    std::string name;
    std::vector<std::string> variants;

    std::string fmt_list();
};
