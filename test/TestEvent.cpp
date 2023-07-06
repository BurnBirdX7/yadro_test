#include <gtest/gtest.h>

#include <random>

#include "Event.hpp"

using namespace club;


static Time generate_time() {
    static std::minstd_rand generator{};
    static std::uniform_int_distribution hours_dist(0, 23);
    static std::uniform_int_distribution minutes_dist(0, 59);

    int hours = hours_dist(generator);
    int minutes = minutes_dist(generator);

    return {hours, minutes};
}


static constexpr std::string client_name = "client1";


TEST(Event, Basic) {
    auto time = generate_time();
    auto event = Event(time, EventType::DUMMY, client_name);

    ASSERT_EQ(EventType::DUMMY, event.type());
    ASSERT_EQ(21, event.id());
    ASSERT_EQ(time, event.time());

    auto str = time.toString() + " 21 client1";
    ASSERT_EQ(str, event.toString());
}

TEST(Event, ClientWalkedIn) {
    auto time = generate_time();
    auto event = Event(time, EventType::CLIENT_WALKED_IN, client_name);

    ASSERT_EQ(client_name, event.client());
    ASSERT_THROW((void)event.error(), std::runtime_error);
    ASSERT_THROW((void)event.table(), std::runtime_error);
}

TEST(Event, Sat) {
    auto time = generate_time();

    for (auto type : {EventType::CLIENT_SAT, EventType::FORCED_SAT}) {

        auto event = Event(time, type, client_name, 8);

        ASSERT_EQ(client_name, event.client());
        ASSERT_THROW((void)event.error(), std::runtime_error);
        ASSERT_EQ(8, event.table());
    }
}

TEST(Event, ClientWaits) {
    auto time = generate_time();
    auto event = Event(time, EventType::CLIENT_WAITS, client_name);

    ASSERT_EQ(client_name, event.client());
    ASSERT_THROW((void)event.error(), std::runtime_error);
    ASSERT_THROW((void)event.table(), std::runtime_error);
}

TEST(Event, WalkedOut) {
    auto time = generate_time();

    for (auto type : {EventType::CLIENT_WALKED_OUT, EventType::FORCED_WALKED_OUT}) {
        auto event = Event(time, type, client_name);

        ASSERT_EQ(client_name, event.client());
        ASSERT_THROW((void)event.error(), std::runtime_error);
        ASSERT_THROW((void)event.table(), std::runtime_error);
    }
}

TEST(Event, Error) {
    auto time = generate_time();
    std::string msg = "actually, error message";
    auto event = Event(time, EventType::ERROR, msg);

    ASSERT_THROW((void)event.client(), std::runtime_error);
    ASSERT_EQ(msg, event.error());
    ASSERT_THROW((void)event.table(), std::runtime_error);
}


TEST(Event, ToString) {
    auto event = Event(Time(14, 15), EventType::FORCED_SAT, "client_1", 8);
    std::string str = "14:15 12 client_1 8";

    ASSERT_EQ(str, event.toString());
}

