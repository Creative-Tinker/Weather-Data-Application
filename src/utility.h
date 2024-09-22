#pragma once

#include <sstream>
#include <iomanip>
#include <stdexcept>
#include <vector>

std::string join_many_to_one_str(std::vector<std::string>::iterator start, std::vector<std::string>::iterator end,
                                 std::string delimiter);

std::string join_many_to_one_str(std::vector<std::string> list, std::string delimiter);

std::vector<std::string> split_into_substrings(std::string input_text, char delimiter);

float str_to_float(std::string input_str);

std::string format_float(float f, int precision);