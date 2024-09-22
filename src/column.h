#pragma once
#include <string>
#include "rapidjson/document.h"
#include "utility.h"

enum JSONT { s, f, i }; // String, float, int

class Column {
public:
    std::string header, subheader;

    Column(std::string header, std::string subheader, rapidjson::GenericValue<rapidjson::UTF8<>> *start, JSONT type);

    std::string format_value(int index);

private:
    rapidjson::GenericValue<rapidjson::UTF8<>> *start;
    JSONT type; 
};