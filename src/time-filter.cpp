#include "time-filter.h"
#include <iomanip>

bool is_valid_date(std::string s) {
    std::tm d;
    std::stringstream ss;
    ss << s;
    ss >> std::get_time(&d, "%Y-%m-%d");
    return !ss.fail();
}

int stoi_bound(int min, int max, std::string _value) {
    int value;
    try {
        value = std::stoi(_value);
    } catch (...) {
        throw std::runtime_error("expected a number, got: " + std::to_string(value));
    }
    if (min <= value && value <= max)
        return value;
    throw std::runtime_error(std::to_string(value) + " not in expected range");
}

TimeFilter::TimeFilter() {
    kind = TimeFilterKinds::relative;
    relative_before = 0;
    relative_after = 1;
}

void TimeFilter::set_relative(std::string _before, std::string _after) {
    kind = TimeFilterKinds::relative;
    relative_before = stoi_bound(0, 92, _before);
    relative_after = stoi_bound(0, 16, _after);
}

void TimeFilter::set_absolute(std::string _start, std::string _end) {
    kind = TimeFilterKinds::absolute;
    if (!is_valid_date(_start))
        throw std::runtime_error(_start + " is not a valid date");
    if (!is_valid_date(_end))
        throw std::runtime_error(_end + " is not a valid date");
    absolute_start = _start;
    absolute_end = _end;
}

void TimeFilter::parse(TimeFilterKinds kind, std::string raw) {
    if (raw.length() == 0)
        return;

    std::vector<std::string> s = split_into_substrings(raw, ',');
    if (s.size() != 2)
        throw std::runtime_error("unable to parse time filter");

    if (kind == TimeFilterKinds::absolute)
        set_absolute(s[0], s[1]);
    else if (kind == TimeFilterKinds::relative)
        set_relative(s[0], s[1]);
}

std::string TimeFilter::to_url_params() {
    if (kind == TimeFilterKinds::absolute)
        return "&start_date=" + absolute_start + "&end_date=" + absolute_end;
    else if (kind == TimeFilterKinds::relative)
        return "&forecast_days=" + std::to_string(relative_after) + "&past_days=" + std::to_string(relative_before);
    else
        throw std::runtime_error("unreachable");
}
