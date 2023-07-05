#include "EventTime.hpp"
#include <format>


int int_from_2_digits(char const* data) {
    int i;

    auto ret = std::from_chars(data, data + 2, i, 10);
    if (ret.ptr != data + 2) {
        throw std::system_error(std::make_error_code(ret.ec));
    }

    return i;
}

EventTime::EventTime(const std::string &str) {
    char const* data = str.c_str();

    int minutes = int_from_2_digits(data);
    int hours = int_from_2_digits(data + 3);

    this->units_ = hours * 60 + minutes;
}

EventTime::EventTime(int units)
    : units_(units)
{}

EventTime::EventTime(int hours, int minutes)
    : units_(hours * 60 + minutes)
{}

std::string EventTime::toString() const {
    return std::format("{:02}:{:02}", hours(), minutes());
}
