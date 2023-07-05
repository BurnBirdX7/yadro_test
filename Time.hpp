#pragma once
#include <string>
#include <cstdint>
#include <charconv>
#include <system_error>
#include <iostream>

namespace club {

    class Time {
    public:
        Time() = default;
        explicit Time(std::string const& str);
        explicit Time(int units);
        Time(int hours, int minutes);

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

        auto operator<=>(Time const& time) const = default;

    private:
        std::uint16_t units_;

    };
}


