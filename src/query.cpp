#include "query.h"

size_t curl_write_res(void *content, size_t member_size, size_t length, std::string *output) {
    size_t total_size = member_size * length;
    output->append(static_cast<char *>(content), total_size);
    return total_size; 
}

Query::Query(const Location &_location) : location(_location) {
    temperature.init("temperature", "Temperature", {Unit("celsius", "C"), Unit("fahrenheit", "F")}, JSONT::f);
    precipitation.init("precipitation", "Precipitation", {Unit("mm", "mm"), Unit("inch", "inches")}, JSONT::f);
    pressure.init("surface_pressure", "Pressure", {Unit("hpa", "hPa")}, JSONT::f);
    humidity.init("relative_humidity", "Humidity", {Unit("percent", "%")}, JSONT::i);
    radiation.init("shortwave_radiation", "Radiation", {Unit("wm2", "W/m^2")}, JSONT::f);
    cloudiness.init("cloud_cover", "Cloudiness", {Unit("percent", "%")}, JSONT::i);
    wind_speed.init("windspeed", "Wind speed",
                    {Unit("kmh", "km/h"), Unit("ms", "m/s"), Unit("mph", "miles/h"), Unit("kn", "knots")}, JSONT::f);
    wind_direction.init("wind_direction", "Wind direction", {Unit("deg", "Degree")}, JSONT::i);
    temperature_max.init("temperature_2m_max", "Temperature (max)", temperature.units, JSONT::f);
    temperature_min.init("temperature_2m_min", "Temperature (min)", temperature.units, JSONT::f);
    uv.init("uv_index", "UV", {Unit("index", "index")}, JSONT::f);
    rain_sum.init("rain_sum", "Rain sum", {Unit("mm", "mm")}, JSONT::f);

    timeformat.init("timeformat", {"iso8601", "unixtime"});
    set_time_datatype();
    sunrise.init("sunrise", "Sunrise", {Unit("time", "Timestamp")}, time_datatype);
    sunset.init("sunset", "Sunset", {Unit("time", "Timestamp")}, time_datatype);

    model.init("model", {"best_match", "ecmwf_ifs04", "cma_grapes_global", "bom_access_global", "metno_nordic",
                         "gfs_seamless", "jma_seamless", "icon_seamless", "gem_seamless", "meteofrance_seamless"});
    type.init("type", {"daily", "hourly"});
    // Empty string disables pressure level variance
    pressure_level.init("pressure_level", {
                                              "",    "1000", "975", "950", "925", "900", "850", "800", "700", "600",
                                              "500", "400",  "300", "250", "200", "150", "100", "70",  "50",  "30",
                                          });
    timezone = "auto";
}

void Query::set_time_datatype() {
    time_datatype = timeformat.selected == "unixtime" ? JSONT::i : JSONT::s;
}

void Query::set_timezone(std::string raw) {
    if (raw.length() == 0)
        return;
    try {
        std::chrono::locate_zone(raw);
    } catch (...) {
        throw std::runtime_error(raw + " is not a valid timezone");
    }
    timezone = raw;
}

void Query::apply_suffixes() {
    for (WeatherVariable &v : vars)
        v.technical_suffix = "";    
    
    if (type.selected == "hourly") {
        if (pressure_level.selected == "") {
            temperature.technical_suffix = "_2m";
            wind_speed.technical_suffix = "_10m";
            humidity.technical_suffix = "_2m";
        } else {
            for (WeatherVariable &v : vars)
                v.technical_suffix = "_" + pressure_level.selected + "hPa";
        }
    }
}

std::string Query::get_url() {
    std::string url = "https://api.open-meteo.com/v1/forecast?"
                      "latitude=" +
                      format_float(location.latitude, 4) + "&longitude=" + format_float(location.longitude, 4) +
                      "&temperature_unit=" + temperature.selected_unit.technical +
                      "&wind_speed_unit=" + wind_speed.selected_unit.technical +
                      "&precipitation_unit=" + precipitation.selected_unit.technical +
                      "&timeformat=" + timeformat.selected + "&timezone=" + timezone + "&models=" + model.selected;

    set_vars();
    apply_suffixes();
    url += "&" + type.selected + "=";
    for (WeatherVariable &v : vars)
        url += v.full_technical_name() + ",";
    url.pop_back();
    url += time_filter.to_url_params();
    return url;
}

rapidjson::Document Query::send() {
    curl_global_init(CURL_GLOBAL_ALL); 
    
    CURL *handle = curl_easy_init();

    std::string raw_response = "", url = get_url(); 
    curl_easy_setopt(handle, CURLOPT_URL, url.c_str());
    curl_easy_setopt(handle, CURLOPT_WRITEDATA, &raw_response);
    curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, curl_write_res);
    curl_easy_setopt(handle, CURLOPT_BUFFERSIZE, CURL_MAX_READ_SIZE);
    
    CURLcode success = curl_easy_perform(handle);

    if (success != CURLE_OK) 
        throw std::runtime_error("HTTP error. This is a bug with wda.");

    rapidjson::Document response;
    response.Parse(raw_response.c_str());
    if (response.HasMember("error") == false)
        return response; 
    std::string reason = response["reason"].GetString(), no_data_msg = "No data is available for this location";
    if (reason == no_data_msg)
        throw std::runtime_error(no_data_msg);
    throw std::runtime_error("API returned an error. This is a bug with wda. \nURL:\n" + url + "\nDetails:\n" + reason);
}

std::string Query::run() {
    rapidjson::Document response = send();
    Table table; 

    rapidjson::Value &data_table = response[type.selected.c_str()];
    
    table.init(data_table["time"].Capacity());

    std::string time_column_name = type.selected == "daily" ? "Date" : "Time";
    
    table.add(Column(time_column_name, "", data_table["time"].Begin(), time_datatype));
    sunset.datatype = sunrise.datatype = time_datatype;
   
    for (WeatherVariable v : vars)
        table.add(v.build_column(data_table));

    return table.format_table();
}

void Query::set_vars() {
    if (type.selected == "daily")
        vars = {temperature_max, temperature_min, sunrise, sunset, rain_sum};
    else if (type.selected == "hourly") {
        if (pressure_level.selected == "")
            vars = {temperature, humidity, precipitation, pressure, cloudiness, wind_speed, radiation, uv};
        else
            vars = {temperature, humidity, cloudiness, wind_speed, wind_direction};
    } else
        vars = {};
}
