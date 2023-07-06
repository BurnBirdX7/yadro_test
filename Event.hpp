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

        // outgoing:
        FORCED_WALKED_OUT = 11,
        FORCED_SAT = 12,
        ERROR = 13,

        // dummy
        DUMMY = 21
    };

    class Event {
    public:
        Event(Time time, EventType type, std::string text = "");
        Event(Time time, EventType type, std::string name, int table);
        static Event from_string(std::string const& input);

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


