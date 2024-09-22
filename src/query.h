#pragma once

#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
#include <chrono>
#include <curl/curl.h>

#include "location.h"
#include "time-filter.h"
#include "table.h"
#include "weather-variable.h"
#include "variant-selector.h"

/* Handle response data recieved from HTTP request*/
size_t curl_write_res(void *content, size_t member_size, size_t length, std::string *output);

class Query {
public:

  // Default settings for querying various weather variables and options
    Query(const Location &_location);

    void set_time_datatype();

    void set_timezone(std::string raw);

  /* Ensures technical names of variables are adjusted correctly
  based on query context*/ 
    void apply_suffixes();

  /* Construct URL for querying API */
    std::string get_url();

  /*RapidJson: Represents response from API*/
    rapidjson::Document send();

  /* Creates table with relevant data based off API response*/
    std::string run();

/* Different aspects that can be queried & manipulated  */
    WeatherVariable temperature, precipitation, pressure, humidity, radiation, cloudiness, wind_speed, wind_direction,
                    temperature_max, temperature_min, sunrise, sunset, uv, rain_sum;
    VariantSelection timeformat, model, type, pressure_level;
    TimeFilter time_filter;
    JSONT time_datatype;

private:
    Location location;
    std::string timezone;
     /* References weather variable objects
  storing variables that will be retreived
  from the API for the specified location*/
    std::vector<std::reference_wrapper<WeatherVariable>> vars;

    void set_vars();
};
