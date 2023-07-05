#include "Event.hpp"

EventType Event::type() const {
    return type_;
}

int Event::id() const {
    return static_cast<int>(type_);
}

std::string Event::client() const {
    if (type_ == EventType::ERROR) {
        throw std::runtime_error("client() called on error event");
    }
    return text_;
}

std::string Event::error() const {
    if (type_ != EventType::ERROR) {
        throw std::runtime_error("error() called on non-error event");
    }

    return text_;
}

int Event::table() const {
    if (type_ != EventType::CLIENT_SAT && type_ != EventType::FORCED_SAT) {
        throw std::runtime_error("table() called on non-table event");
    }

    return table_;
}
