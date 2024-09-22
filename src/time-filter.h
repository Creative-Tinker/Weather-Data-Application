#pragma once

#include <string>
#include <stdexcept>
#include <sstream>
#include <ctime>
#include <vector>

/* Verifies if a string conforms to iso8601 date */
bool is_valid_date(std::string s);
int stoi_bound(int min, int max, std::string _value);
std::vector<std::string> split_into_substrings(std::string input_text, char delimiter);

enum TimeFilterKinds { absolute, relative };

/* Specify time interval for data request */
class TimeFilter {
private:
    int relative_before, relative_after;
    std::string absolute_start, absolute_end;
    TimeFilterKinds kind;

public:
    TimeFilter();

  /* Number of days before & after current date*/
    void set_relative(std::string _before, std::string _after);
    void set_absolute(std::string _start, std::string _end);
      /* Set time filter accordingly based on user input*/
    void parse(TimeFilterKinds kind, std::string raw);
      /* Paramaters allows URL construction for HTTP request to API
  as API endpoint's expect certain paramters to be included*/
    std::string to_url_params();
};