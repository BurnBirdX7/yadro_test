#pragma once
#include <string>
#include <cstdint>
#include <charconv>
#include <system_error>
#include <iostream>

namespace club {

    /**
     * Class that stores time
     * Represents timestamp and doubles as addable duration
     */
    class Time {
    public:
        /**
         * Default time: 00:00
         */
        Time() = default;

        /**
         * Parses string as time
         * String must be in HH:MM format
         * @param str string to parse
         * @throws std::system_error if error occurred during parsing
         */
        explicit Time(std::string const& str);

        /**
         * @param units minutes since 0:00, shouldn't be more than 1440
         */
        explicit Time(int units);

        /**
         * @param hours shouldn't be more than
         * @param minutes shouldn't be more than 60
         */
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
        inline int units() const {
            return units_;
        }

        /**
         * @return string in format HH:MM
         */
        [[nodiscard]]
        std::string toString() const;

        /**
         * Rounds the time up
         * For example 6:15 will be rounded to 7:00 and 8:00 will be rounded to 8:00 (stays the same)
         * @return new object with rounded time
         */
        [[nodiscard]]
        Time roundUp() const;

        // Arithmetic operations (work as if Time is duration):
        Time& operator+=(Time const& other);
        Time& operator-=(Time const& other);
        Time operator+(Time const& other) const;
        Time operator-(Time const& other) const;

        auto operator<=>(Time const& time) const = default;

    private:
        std::uint16_t units_;

    };
}
