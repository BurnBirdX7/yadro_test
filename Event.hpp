#pragma once
#include <string>
#include "EventTime.hpp"

enum class EventType {
    // incoming:
    CLIENT_CAME_IN = 1,
    CLIENT_SAT = 2,
    CLIENT_WAITS = 3,
    CLIENT_CAME_OUT = 4,

    // outgoing:
    FORCED_CAME_OUT = 11,
    FORCED_SAT = 12,
    ERROR = 13

};

class Event {
public:
    [[nodiscard]] EventType   type()    const;
    [[nodiscard]] int         id()      const;
    [[nodiscard]] std::string client()  const;
    [[nodiscard]] std::string error()   const;
    [[nodiscard]] int         table()   const;

private:
    EventType type_;
    EventTime time_;
    std::string text_;
    int table_;

};
