#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <stdexcept>
#include "location.h" /*indirectly includes utility.h*/

/* Necessary for data validation in parse_entry function*/
std::optional<unsigned int> parse_uint(std::string s);

/* Standardises comparison within list_filtered function */
std::string to_lowercase(std::string s);

class LocationList {
public:
    LocationList(std::stringstream &ss);
    LocationList(std::string path);
    void load_entry(std::string raw);

      /* Parses raw string representing location
  entry into separate parts(ID, lat, lon,  name)
  & returns as a pair. Converting to unsigned int is necessary
  for error checking*/
    std::pair<unsigned int, Location> parse_entry(std::string raw);

      /* Converting data of each location object into string */
    std::string serialise();
    bool remove_entry(std::string raw);

      /* Filtering locations based on provided filter
  string & returning a string containing filtered 
  location data */
    std::string list_filtered(std::string filter);

      /* Iterates through each value in map & attempts
  to match parameter by either id or name*/
    std::optional<unsigned int> match_entry(std::string raw);

    /*Attempts to recieve location object associated with provided id */
    Location get_entry(std::string id);

private:
    std::map<unsigned int, Location> locations;

    /*Adds location entries after initialising location list object */ 
    void init(std::stringstream &s);
};