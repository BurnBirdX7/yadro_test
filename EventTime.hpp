#pragma once
#include <string>
#include <cstdint>
#include <charconv>
#include <system_error>
#include <iostream>

class EventTime {
public:
    explicit EventTime(std::string const& str);
    explicit EventTime(int units);
    EventTime(int hours, int minutes);

    [[nodiscard]]
    inline int hours() const {
        return units_ / 60;
    }

    [[nodiscard]]
    inline int minutes() const {
        return units_ % 60;
    }

    [[nodiscard]]
    std::string toString() const;

private:
    std::uint16_t units_;


};
