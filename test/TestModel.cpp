#include <gtest/gtest.h>

#include "Model.hpp"

using namespace club;

/**
 * Creates model with following parameters:
 * Tables: 5
 * Work time: 9:00-19:00
 * Table price per hour: 10
 * @param queue incoming events
 * @return model
 */
Model createModel(Model::queue_t queue) {
    return {5,
            {9, 00}, {19, 00},
            10,
            std::move(queue)};
}

/**
 * Checks if expected_events in model's output are equal to expected expected_events
 * @param stream model's output
 * @param expected_events
 */
template <class Cont = std::vector<Event>>
void assertEqualEvents(std::istream& stream, Cont const& expected_events)
requires std::same_as<Event, typename Cont::value_type>
{
    std::string line;
    std::getline(stream, line);
    ASSERT_EQ("09:00", line);
    for (auto const& expect : expected_events) {
        std::getline(stream, line);
        ASSERT_EQ(expect.toString(), line);
    }
    std::getline(stream, line);
    ASSERT_EQ("19:00", line);
}

/**
 * Exhausts all events from model's output
 * Handy for testing table statistics at the end of the output
 * @param stream model's output
 */
void exhaustAllEvents(std::istream& stream) {
    std::string line;
    std::getline(stream, line);
    while (line != "19:00") {
        std::getline(stream, line);
    }
}


TEST(Model, NoInNoOut) {
    std::string client = "ginger";

    Event walkIn = {{8, 30}, EventType::CLIENT_WALKED_IN, client};
    Event expectedErrorIn = {{8, 30}, EventType::ERROR, "NotOpenYet"};
    Event walkOut = {{10, 30}, EventType::CLIENT_WALKED_OUT, client};
    Event expectedErrorOut = {{10, 30}, EventType::ERROR, "ClientUnknown"};

    Model model = createModel({walkIn, walkOut});

    std::stringstream stream;
    model.run(stream); // pass as output stream

    assertEqualEvents(stream, {walkIn, expectedErrorIn, walkOut, expectedErrorOut});
}

TEST(Model, WalkInTwice) {
    std::string client = "brown";

    Event w1 = {{9, 5}, EventType::CLIENT_WALKED_IN, client};
    Event w2 = {{9, 15}, EventType::CLIENT_WALKED_IN, client};
    Event e1 = {{9, 15}, EventType::ERROR, "YouShallNotPass"};
    Event f1 = {{19, 0}, EventType::FORCED_WALKED_OUT, client};

    Model model = createModel({w1, w2});

    std::stringstream stream;
    model.run(stream);

    assertEqualEvents(stream, {w1, w2, e1, f1});
}

TEST(Model, UnknownClient) {
    std::string client = "alien";

    Event cr1 = {{9, 15}, EventType::CLIENT_SAT, client, 2};
    Event er1 = {{9, 15}, EventType::ERROR, "ClientUnknown"};
    Event cr2 = {{9, 17}, EventType::CLIENT_WALKED_OUT, client};
    Event er2 = {{9, 17}, EventType::ERROR, "ClientUnknown"};

    Event in3 = {{11, 0}, EventType::CLIENT_WALKED_IN, client};
    Event fo3 = {{19, 0}, EventType::FORCED_WALKED_OUT, client};

    // Same errors but after closing:
    Event in4 = {{20, 15}, EventType::CLIENT_SAT, client, 2};
    Event er4 = {{20, 15}, EventType::ERROR, "ClientUnknown"};
    Event in5 = {{20, 17}, EventType::CLIENT_WALKED_OUT, client};
    Event er5 = {{20, 17}, EventType::ERROR, "ClientUnknown"};


    Model model = createModel({cr1, cr2, in3, in4, in5});

    std::stringstream stream;
    model.run(stream);

    assertEqualEvents(stream, {cr1, er1, cr2, er2, in3, fo3, in4, er4, in5, er5});
}

TEST(Model, ClientCannotWait) {
    std::string client = "they";

    // Client implicitly walks in, as specification does not describe error for this situation
    Event ev1 = {{9, 10}, EventType::CLIENT_WAITS, client};
    Event er2 = {{9, 10}, EventType::ERROR, "ICanWaitNoLonger!"};
    Event fo3 = {{19, 0}, EventType::FORCED_WALKED_OUT, client}; // Waited a lot :(

    Model model = createModel({ev1});

    std::stringstream stream;
    model.run(stream);

    assertEqualEvents(stream, {ev1, er2, fo3});
}

TEST(Model, ClientCanWait) {
    std::vector<std::string> clients = {"c1", "c2", "c3", "c4", "c5", "waits"};

    std::deque<Event> events{};
    for (auto i = 0; i < 5; ++i) {
        events.emplace_back(Time{9, 10 + 2 * i}, EventType::CLIENT_WALKED_IN, clients[i]);
        events.emplace_back(Time{9, 10 + 2 * i + 1}, EventType::CLIENT_SAT, clients[i], i + 1);
    }
    events.emplace_back(Time{10, 0}, EventType::CLIENT_WAITS, clients[5]);

    Model model = createModel(events);
    std::stringstream stream;
    model.run(stream);

    for (auto const& client : clients) {
        events.emplace_back(Time{19, 0}, EventType::FORCED_WALKED_OUT, client);
    }

    assertEqualEvents(stream, events);
}

