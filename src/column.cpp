#include "column.h"

Column::Column(std::string header, std::string subheader, rapidjson::GenericValue<rapidjson::UTF8<>> *start, JSONT type)
    : header(header), subheader(subheader), start(start), type(type) {}

std::string Column::format_value(int index) {
    rapidjson::GenericValue<rapidjson::UTF8<>> *it = (start + index);
    
    switch (type) {
    case JSONT::s:
        return it->GetString();
    case JSONT::f:
        return format_float(it->GetFloat(), 1);
    case JSONT::i:
        return std::to_string(it->GetInt());
    }
    throw std::runtime_error("column type not set");
}
