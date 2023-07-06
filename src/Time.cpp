#include <sstream>
#include <iomanip>
#include "Time.hpp"

using namespace club;

int int_from_2_digits(char const* data) {
    int i = -1;

    auto ret = std::from_chars(data, data + 2, i, 10);
    if (ret.ptr != data + 2) {
        throw std::system_error(std::make_error_code(ret.ec));
    }

    return i;
}

Time::Time(const std::string &str) {
    if (str.length() != 5) {
        throw std::system_error(std::make_error_code(std::errc::invalid_argument),
                                "Wrong time string length");
    }

    char const* data = str.c_str();

    if (str[2] != ':') {
        throw std::system_error(std::make_error_code(std::errc::invalid_argument), "Wrong time delimiter");
    }

    int hours = int_from_2_digits(data);
    int minutes = int_from_2_digits(data + 3);

    this->units_ = hours * 60 + minutes;
}

Time::Time(int units)
    : units_(units)
{}

Time::Time(int hours, int minutes)
    : units_(hours * 60 + minutes)
{}

std::string Time::toString() const {
    std::ostringstream ss;
    ss << std::setfill('0') << std::setw(2) << hours() << ':' << std::setw(2) << minutes();
    return ss.str();
}

Time &Time::operator+=(const Time &other) {
    units_ += other.units_;
    return *this;
}

Time Time::operator+(const Time &other) const {
    return Time(units_ + other.units_);
}

Time &Time::operator-=(const Time &other) {
    if (other.units_ > units_) {
        throw std::underflow_error("Tried to subtract later time from earlier");
    }
    units_ -= other.units_;
    return *this;
}

Time Time::operator-(const Time &other) const {
    if (other.units_ > units_) {
        throw std::underflow_error("Tried to subtract later time from earlier");
    }
    return Time(units_ - other.units_);
}

Time Time::roundUp() const {
    return {hours() + (minutes() > 0 ? 1 : 0), 0};
}
