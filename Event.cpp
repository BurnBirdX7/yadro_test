#include <sstream>
#include <format>
#include "Event.hpp"

using namespace club;

EventType Event::type() const {
    return type_;
}

int Event::id() const {
    return static_cast<int>(type_);
}

Time Event::time() const {
    return time_;
}

std::string Event::error() const {
    if (type_ != EventType::ERROR) {
        throw std::runtime_error("error() called on non-error event");
    }

    return text_;
}

std::string Event::client() const {
    if (type_ == EventType::ERROR) {
        throw std::runtime_error("client() called on error event");
    }
    return text_;
}

int Event::table() const {
    if (type_ != EventType::CLIENT_SAT && type_ != EventType::FORCED_SAT) {
        throw std::runtime_error("table() called on non-table event");
    }

    return table_;
}

Event Event::from_string(std::string const& string) {
    auto ss = std::stringstream(string);
    std::string time_str, id_str;

    ss >> time_str >> id_str;

    auto time = Time(time_str);
    auto type = EventType(std::stoi(id_str));

    auto event = Event(time, type);

    std::string client_or_error;
    ss >> client_or_error;
    event.text_ = client_or_error;

    if (type == EventType::CLIENT_SAT || type == EventType::FORCED_SAT) {
        int table_id;
        ss >> table_id;
        event.table_ = table_id;
    }

    return event;
}

Event::Event(Time time, EventType type, std::string const& text)
    : time_(time)
    , type_(type)
    , text_(text)
{}

std::string Event::toString() const {
    std::string str = time_.toString() + ' ' + std::to_string(static_cast<int>(type_)) + ' ' + text_;
    if (type_ == EventType::CLIENT_SAT || type_ == EventType::FORCED_SAT) {
        str += ' ' + std::to_string(table_);
    }
    return str;
}

