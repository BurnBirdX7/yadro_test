#include "Time.hpp"
#include <format>

using namespace club;

int int_from_2_digits(char const* data) {
    int i;

    auto ret = std::from_chars(data, data + 2, i, 10);
    if (ret.ptr != data + 2) {
        throw std::system_error(std::make_error_code(ret.ec));
    }

    return i;
}

Time::Time(const std::string &str) {
    char const* data = str.c_str();

    int minutes = int_from_2_digits(data);
    int hours = int_from_2_digits(data + 3);

    this->units_ = hours * 60 + minutes;
}

Time::Time(int units)
    : units_(units)
{}

Time::Time(int hours, int minutes)
    : units_(hours * 60 + minutes)
{}

std::string Time::toString() const {
    return std::format("{:02}:{:02}", hours(), minutes());
}
