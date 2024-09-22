#pragma once

#include <string>
#include <vector>
#include "unit.h"
#include "column.h"

class WeatherVariable {
public:
    std::string technical_name, technical_suffix, display_name;
    Unit selected_unit;
    std::vector<Unit> units;
    JSONT datatype; 

    WeatherVariable();

    void init(std::string _technical_name, std::string _display_name, std::vector<Unit> _units, JSONT _datatype);

    std::string full_technical_name();

    Column build_column(rapidjson::Value &data_table);

    void try_select_unit(std::string technical);
};