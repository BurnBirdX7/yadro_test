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
void assertEqualEvents(std::istream& stream, std::vector<Event> const& expected_events) {
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