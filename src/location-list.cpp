#include "location-list.h"

#include <limits>

const unsigned int UINT_MAX = std::numeric_limits<unsigned int>::max();

std::optional<unsigned int> parse_uint(std::string s) {
    try {
        long l = std::stol(s);

        if (0 <= l && l <= UINT_MAX)
            return static_cast<unsigned int>(l);
    } catch (...) {
    }
    return std::nullopt;
}

std::string to_lowercase(std::string s) {
    std::transform(s.begin(), s.end(), s.begin(), ::tolower);
    return s;
}


LocationList::LocationList(std::stringstream &ss) {
    init(ss);
}

LocationList::LocationList(std::string path) {
    std::ifstream f(path);
    std::stringstream read_content;

    if (f.good())
        read_content << f.rdbuf();
    init(read_content);
    f.close();
}

void LocationList::load_entry(std::string raw) {
    auto [id, location] = parse_entry(raw);
    locations.insert({id, location});
}

std::pair<unsigned int, Location> LocationList::parse_entry(std::string raw) {
    std::vector<std::string> words = split_into_substrings(raw, ' ');

    if (words.size() < 4)
        throw std::runtime_error("Invalid location string. Expected 4 values, got " + std::to_string(words.size()));

    std::vector<std::string>::iterator it = words.begin();

    std::string raw_id = *(it++);
    std::optional<unsigned int> maybe_id = parse_uint(raw_id);

    unsigned int id;
    if (maybe_id.has_value())
        id = maybe_id.value();
    else
        throw std::runtime_error("Expected unsigned int, got " + raw_id);

    if (locations.count(id) == 1)
        throw std::runtime_error("ID " + std::to_string(id) + " is already used.");

    std::string latitude = (*(it++)), longitude = (*(it++)), name = join_many_to_one_str(it, words.end(), " ");

    return {id, Location(latitude, longitude, name)};
}

std::string LocationList::serialise() {
    std::string s = "";
    for (auto [id, location] : locations)
        s += std::to_string(id) + " " + location.serialise() + "\n";
    return s;
}

bool LocationList::remove_entry(std::string raw) {
    if (auto maybe_location = match_entry(raw); maybe_location.has_value())
        return locations.erase(maybe_location.value());
    return false;
}

std::string LocationList::list_filtered(std::string filter) {
    if (std::optional<unsigned int> maybe_id = parse_uint(filter); maybe_id.has_value()) {
        try {
            Location location = locations.at(maybe_id.value());
            return filter + " " + location.serialise() + "\n";
        } catch (...) {
        }
    }

    filter = to_lowercase(filter);

    std::string filtered_data = "";

    for (auto [id, location] : locations)
        if (to_lowercase(location.name).find(filter) != std::string::npos)
            filtered_data += std::to_string(id) + " " + location.serialise() + "\n";
    return filtered_data;
}

std::optional<unsigned int> LocationList::match_entry(std::string raw) {
    for (auto it = locations.begin(); it != locations.end(); it++) {
        auto [id, location] = *it;

        if (std::to_string(id) == raw || location.name == raw)
            return id;
    }
    return std::nullopt;
}

Location LocationList::get_entry(std::string id) {
    std::optional<unsigned int> maybe_id = match_entry(id);
    if (!maybe_id.has_value())
        throw std::runtime_error("no location found with id " + id);
    return locations.at(maybe_id.value());
}

void LocationList::init(std::stringstream &s) {
    for (std::string line; getline(s, line);)
        if (!line.empty())
            load_entry(line);
}
