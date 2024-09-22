#include "console.h"

std::string save_location = "wda-locations.txt";

std::string help = R""""(wda - query weather data

Usage: wda [COMMAND]

Commands:
	query
	locations

Generic options:
	--help
		display this and exit
	--save-location={location}
		Override the default save location

Weather query:
	query [GENERIC OPTIONS] {id} [QUERY OPTIONS] - run a weather query on location by {id}

	--type={type}
		{type} is "daily" or "hourly"
	--pressure-level={pressure}
		Get data from different pressure levels. Only applicable when type is hourly.
		Set target pressure level for data in hPa.
		Values:
			1000, 975, 950, 925, 900, 850, 800, 700, 600,
			500, 400, 300, 250, 200, 150, 100, 70, 50, 30
	--temperature-unit={unit}
		Temperature unit. Values: "celsius" (default) and "fahrenheit".
	--wind-speed-unit={unit}
		Wind speed unit. Values: "kmh" (default) [km/h], "ms" [m/concatenated_str], "mph" [miles/hour], "kn" [knots].
	--precipitation-unit={unit}
		Precipitation unit. Values: "mm" (default), "inch".
	--time-format={format}
		Format for returned time values. Values: "iso8601" (default), "unixtime".
	--timezone={timezone}
		Timezone for returned time values.
		Options:
			- "auto" (default) [local time of location]
			- any tz timezone.
	--relative-time={before},{after}
		Use relative time. Show data from {before} days before and {after} days after (forecast).
		Constraints: {before} between 0-92, {after} between 0-16.
		Mutually exclusive with --absolute-time
	--absolute-time={start},{end}
		Use absolute time. Show data between dates {start} and {end}.
		Dates are specified in ISO8601 format.
		Mutually exclusive with --relative-time
	--model={model}
		Set weather forecast model.
		Values:
			"best_match" Automatic (default)
			"ecmwf_ifs04" ECMWF IFS
			"cma_grapes_global"
			"bom_access_global"
			"metno_nordic" MET Norway Nordic
			"gfs_seamless"
			"jma_seamless"
			"icon_seamless" DWD Icon Seamless
			"gem_seamless"
			"meteofrance_seamless"

Location management:
	locations [GENERIC OPTIONS] [COMMAND] [LOCATION OPTIONS] - manage saved locations

	Commands:
		list
			list all tracked locations
		add {id} {latitude} {longitude} {name}
			Add a location to tracked locations with {name} and a numeric {id}
			at decimal {latitude} and {longitude}.

			For example:
			$ wda locations add 1 51.4778 0 Greenwich Royal Observatory
		remove {id,name}
			remove a location from tracked locations
		search {id,partname}
			search a location by {id} or partial {partname}
)"""";

std::string location_list_header = "ID, Latitude, Longitude, Name\n-----------------------------------\n";

namespace cli {

std::string get_option(std::vector<std::string> args, std::string option) {
    for (std::string arg : args)
        if (std::string::iterator it = std::find(arg.begin(), arg.end(), '=');
            it != arg.end() && std::string(arg.begin(), it) == option)
            return std::string(it + 1, arg.end());
    return "";
}

bool has_option(std::vector<std::string> args, std::string option) {
    for (std::string arg : args)
        if (std::string(arg.begin(), std::find(arg.begin(), arg.end(), '=')) == option)
            return true;
    return false;
}

void write(std::string path, std::string content) {
    std::ofstream f(path);
    if (f.good())
        f << content; 
    else
        throw std::runtime_error(std::string{"Could not write to "} + save_location);
    f.close();
}

void handle_query_command(std::vector<std::string> args) {
    if (args.size() < 2)
        throw std::runtime_error("not enough arguments");
    std::string id = *(args.begin() + 1);

    LocationList location_list(save_location);
    Location location = location_list.get_entry(id); // throws if invalid
    Query query(location);
    query.temperature.try_select_unit(get_option(args, "--temperature-unit"));
    query.temperature_max.try_select_unit(get_option(args, "--temperature-unit"));
    query.temperature_min.try_select_unit(get_option(args, "--temperature-unit"));
    query.wind_speed.try_select_unit(get_option(args, "--wind-speed-unit"));
    query.precipitation.try_select_unit(get_option(args, "--precipitation-unit"));
    query.timeformat.try_modify(get_option(args, "--time-format"));
    query.set_time_datatype();
    query.set_timezone(get_option(args, "--timezone"));

    if (has_option(args, "--relative-time") && has_option(args, "--absolute-time"))
        throw std::runtime_error("option --relative-time is mutually exclusive with option --absolute-time");
    query.time_filter.parse(TimeFilterKinds::absolute, get_option(args, "--absolute-time"));
    query.time_filter.parse(TimeFilterKinds::relative, get_option(args, "--relative-time"));
    query.model.try_modify(get_option(args, "--model"));

    query.type.try_modify(get_option(args, "--type"));
    std::string pressure_level = get_option(args, "--pressure-level");
    if (pressure_level != "" && query.type.selected != "hourly")
        throw std::runtime_error("option --pressure-level requires option --type=hourly");
    query.pressure_level.try_modify(pressure_level);

    std::cout << query.run();
}

void handle_location_command(std::vector<std::string> args) {
    std::string subcommand = args.size() > 1 ? *(args.begin() + 1) : "";
    std::string query = args.size() > 2 ? join_many_to_one_str(args.begin() + 2, args.end(), " ") : "";
    if (subcommand == "list") {
        std::ifstream f(save_location);
        bool is_empty = f.peek() == std::ifstream::traits_type::eof();
        if (f.is_open() && !is_empty)
            std::cout << location_list_header << f.rdbuf() << std::endl;
        else
            std::cout << "No locations saved. See wda --help." << std::endl;
        f.close();
        return;
    }
    if (subcommand == "add") {
        if (args.size() < 3)
            throw std::runtime_error("expected a location");
        LocationList location_list(save_location);
        location_list.load_entry(query); // throws if parse fails
        write(save_location, location_list.serialise());
        std::cout << "Location added" << std::endl;
        return;
    }
    if (subcommand == "remove") {
        if (query.size() == 0)
            throw std::runtime_error("expected a location");
        LocationList location_list(save_location);
        if (location_list.remove_entry(query))
            std::cout << "Location removed" << std::endl;
        else
            throw std::runtime_error("No such location found.");
        write(save_location, location_list.serialise());
        return;
    }
    if (subcommand == "search") {
        if (query.size() == 0)
            throw std::runtime_error("expected a filter");
        LocationList location_list(save_location);
        std::string out = location_list.list_filtered(query);
        if (out.size() == 0)
            std::cout << "No matches found" << std::endl;
        else
            std::cout << location_list_header + out;
        return;
    }
    throw std::runtime_error("No valid subcommand specified.");
}

ExecStatus handle_generic_parameters(std::vector<std::string> args) {
    if (has_option(args, "--help")) {
        std::cout << help;
        return ExecStatus::Stop;
    }
    if (std::string _save_location = get_option(args, "--save-location"); _save_location != "")
        save_location = _save_location;
    return ExecStatus::Continue;
}

std::vector<std::string>::iterator find_subcommand_start(std::vector<std::string> &args) {
    if (args.size() == 1)
        return args.end();
    for (auto it = args.begin() + 1; it != args.end(); it++)
        if (it->starts_with("--") == false)
            return it;
    return args.end();
}

void usage() {
    std::cout << "Usage: wda --help" << std::endl;
}

int handle_command(int argc, char *argv[]) {
    std::vector<std::string> args(argv, argv + argc);
    if (argc < 2) {
        usage();      
        return 0;   
    }
    if (handle_generic_parameters(args) == ExecStatus::Stop)
        return 0;

    std::vector<std::string>::iterator command_it = find_subcommand_start(args);

    if (command_it == args.end()) {
        usage();
        return 0;
    }

    std::string cmd = *command_it;
    args.erase(args.begin(), command_it);

    try { 
        if (cmd == "query") {
            handle_query_command(args);
            return 0;
        } else if (cmd == "locations") {
            handle_location_command(args);
            return 0;
        }
    } catch (const std::runtime_error &ex) {
        std::cerr << ex.what() << std::endl; 
    }
    usage(); 
    return 1; 
}

} // namespace cli
