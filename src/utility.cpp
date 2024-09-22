#include "utility.h"
#include <sstream>
#include <iomanip>
#include <stdexcept>
#include <vector>

std::string join_many_to_one_str(std::vector<std::string>::iterator start, std::vector<std::string>::iterator end,
                                 std::string delimiter) {
  std::string concatenated_str = ""; 
  for (auto it = start; it != end; it++)
    concatenated_str += *it + delimiter; 
  if (start != end)
    for (int index = 0; index < delimiter.length(); index++)
      concatenated_str.pop_back();
  return concatenated_str;
}

std::string join_many_to_one_str(std::vector<std::string> list, std::string delimiter) {
  return join_many_to_one_str(list.begin(), list.end(), delimiter); 
}

std::vector<std::string> split_into_substrings(std::string input_text, char delimiter) {
  std::string extracted_substring; 
  std::vector<std::string> split_substrings;
  std::stringstream ss(input_text);
  while (std::getline(ss, extracted_substring, delimiter))
    split_substrings.push_back(extracted_substring);
  return split_substrings; 
}

float str_to_float(std::string input_str) {
  float f;
  try {
    f = std::stof(input_str);
  } catch (...) {
    throw std::runtime_error("Expected float, got " + input_str);
  }
  return f;
}

std::string format_float(float f, int precision) {
  std::stringstream ss;
  ss << std::fixed << std::setprecision(precision) << f;
  return ss.str();
}