#include <gtest/gtest.h>

#include <sstream>

#include "Parse.hpp"

using namespace club;


class ParseFixture : public ::testing::Test {
protected:
    int tables_expected;
    int tables;

    Time start_expected;
    Time start;

    Time end_expected;
    Time end;

    int price_expected;
    int price;

    void set_expected(int tables, Time start, Time end, int price) {
        tables_expected = tables;
        start_expected = start;
        end_expected = end;
        price_expected = price;
    }

    void TearDown() override {
        ASSERT_EQ(tables_expected, tables);
        ASSERT_EQ(start_expected, start);
        ASSERT_EQ(end_expected, end);
        ASSERT_EQ(price_expected, price);
    }

};


TEST_F(ParseFixture, Basic) {
    auto header =
            "12\n"
            "09:00 17:00\n"
            "5\n";

    std::istringstream iss(header);

    ASSERT_NO_THROW(std::tie(tables, start, end, price) = Parse::header(iss));

    set_expected(12, Time(9, 0), Time(17, 0), 5);
}

TEST_F(ParseFixture, ExcessiveSpaces) {
    auto header =
            "   12  \n"
            "  09:00      17:00 \n"
            "   5   \n";

    std::istringstream iss(header);

    ASSERT_NO_THROW(std::tie(tables, start, end, price) = Parse::header(iss));

    set_expected(12, Time(9, 0), Time(17, 0), 5);
}

TEST_F(ParseFixture, ExcessiveNewlines) {
    auto header =
            "12\n\n"
            "09:00 17:00\n\n\n"
            "5\n";

    std::istringstream iss(header);

    ASSERT_THROW(std::tie(tables, start, end, price) = Parse::header(iss), PrintLineError);
}

TEST_F(ParseFixture, NoTables) {
    auto header =
            "09:00 17:00 \n"
            "5\n";

    std::istringstream iss(header);

    ASSERT_THROW(std::tie(tables, start, end, price) = Parse::header(iss), PrintLineError);
}

TEST_F(ParseFixture, NoStartTime) {
    auto header =
            "12\n"
            "17:00\n"
            "5\n";

    std::istringstream iss(header);

    ASSERT_THROW(std::tie(tables, start, end, price) = Parse::header(iss), PrintLineError);
}

TEST_F(ParseFixture, NoTime) {
    auto header =
            "12\n"
            "5\n";

    std::istringstream iss(header);

    ASSERT_THROW(std::tie(tables, start, end, price) = Parse::header(iss), PrintLineError);
}

TEST_F(ParseFixture, NoPrice) {
    auto header =
            "12\n"
            "09:00 17:00\n";

    std::istringstream iss(header);

    ASSERT_THROW(std::tie(tables, start, end, price) = Parse::header(iss), PrintLineError);
}

TEST_F(ParseFixture, FloatingPointPrice) {
    auto header =
            "12\n"
            "09:00 17:00\n"
            "7.9";

    std::istringstream iss(header);

    ASSERT_NO_THROW(std::tie(tables, start, end, price) = Parse::header(iss));

    set_expected(12, Time(9, 0), Time(17, 0), 7);
}



