#pragma once
#include <string>
#include <iostream>
#include "Time.hpp"

namespace club {

    enum class EventType {
        // incoming:
        CLIENT_WALKED_IN = 1,
        CLIENT_SAT = 2,
        CLIENT_WAITS = 3,
        CLIENT_WALKED_OUT = 4,

        // generated:
        FORCED_WALKED_OUT = 11,
        FORCED_SAT = 12,
        ERROR = 13,

        // dummy
        DUMMY = 21
    };

    class Event {
    public:
        /**
         * Creates event with specified parameters
         * @param time time of the event
         * @param type type of the event
         * @param text text that represents name of the client or error message depending on event type
         */
        Event(Time time, EventType type, std::string text = "");

        /**
         * Creates event with specified parameters
         * Same as Event(Time, EventType, std::string)
         * @param type type of the event, should be *_SAT
         * @param table ID of the table
         */
        Event(Time time, EventType type, std::string name, int table);

        /**
         * Creates event from string
         * Sting must be in a following format:
         * <HH>:<MM> <Event ID> <Text (name or error message)> [table number]
         *      - table number can be added only if Event ID = 2 or 12
         * @param input string to parse
         * @return event described by the string
         * @throws std::system_error when there's parsing error
         */
        static Event from_string(std::string const& input);

        // Getters:
        [[nodiscard]] EventType   type()    const;
        [[nodiscard]] int         id()      const;
        [[nodiscard]] Time        time()    const;
        [[nodiscard]] std::string client()  const;
        [[nodiscard]] std::string error()   const;
        [[nodiscard]] int         table()   const;


        [[nodiscard]]
        std::string toString() const;

    private:
        Time time_;
        EventType type_;
        std::string text_ = {};
        int table_        = {};
    };

}


