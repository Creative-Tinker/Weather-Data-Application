#include "weather-variable.h"

WeatherVariable::WeatherVariable() : selected_unit("", "") {}

void WeatherVariable::init(std::string _technical_name, std::string _display_name, std::vector<Unit> _units, JSONT _datatype) {
    technical_name = _technical_name;
    display_name = _display_name;
    units = _units;
    datatype = _datatype;
    if (units.size() == 0)
        throw std::runtime_error("unit list can not be empty");
    selected_unit = units[0]; 
}

std::string WeatherVariable::full_technical_name() {
    return technical_name + technical_suffix; 
}

Column WeatherVariable::build_column(rapidjson::Value &data_table) {
    return Column(display_name, selected_unit.display, data_table[full_technical_name().c_str()].Begin(), datatype);
}

void WeatherVariable::try_select_unit(std::string technical) {
    if (technical == "") {
        selected_unit = units[0];
        return;
    }

    for (Unit unit : units) {
        if (unit.technical == technical) {
            selected_unit = unit; 
            return;
        }
    }

    throw std::runtime_error("Invalid value \"" + technical + "\" for unit of \"" + display_name + "\"");
}
