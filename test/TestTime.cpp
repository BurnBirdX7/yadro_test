#include <gtest/gtest.h>

#include "Time.hpp"

using namespace club;

TEST(Time, Basic) {
    ASSERT_NO_THROW(auto t = Time());
    ASSERT_NO_THROW(auto t = Time(12, 42));

    auto t = Time(13, 24);
    ASSERT_EQ(13, t.hours());
    ASSERT_EQ(24, t.minutes());
    ASSERT_EQ(13 * 60 + 24, t.units());
}

TEST(Time, OkParsing) {
    Time t1;
    ASSERT_NO_THROW(t1 = Time("12:42"));
    ASSERT_EQ(Time(12, 42), t1);

    Time t2;
    ASSERT_NO_THROW(t2 = Time("01:35"));
    ASSERT_EQ(Time(1, 35), t2);

    Time t3;
    ASSERT_NO_THROW(t3 = Time("23:07"));
    ASSERT_EQ(Time(23, 7), t3);
}

TEST(Time, ThrowingParsing) {
    ASSERT_THROW(Time("13:5"), std::system_error);
    ASSERT_THROW(Time("5:05"), std::system_error);
    ASSERT_THROW(Time("15-05"), std::system_error);
    ASSERT_THROW(Time("23_07"), std::system_error);
    ASSERT_THROW(Time("23:007"), std::system_error);
    ASSERT_THROW(Time("023:07"), std::system_error);
}

TEST(Time, Addition) {
    auto t1 = Time(01, 25);
    auto t2 = Time(02, 43);
    auto res = Time(4, 8);

    ASSERT_EQ(res, t1 + t2);

    t1 += t2;
    ASSERT_EQ(t1, res);
}

TEST(Time, AdditionCanOverflow) {
    auto t1 = Time(10, 12);
    auto t2 = Time(17, 12);

    auto res = t1 + t2;
    ASSERT_GT(res.hours(), 24);
}

TEST(Time, HoursCanOverflow) {
    auto t = Time(30, 2);
    ASSERT_GT(t.hours(), 24);
}

TEST(Time, MinutesCannotOverflow) {
    auto t = Time(1, 132);
    ASSERT_LT(t.minutes(), 60);
}

TEST(Time, Subtraction) {
    auto t2 = Time(17, 24);
    auto t1 = Time(10, 12);

    auto res = t2 - t1;
    ASSERT_EQ(res.hours(), 7);

    ASSERT_THROW(t1 - t2, std::underflow_error);
}

TEST(Time, Stringify) {
    auto t1 = Time(1, 2);
    ASSERT_EQ("01:02", t1.toString());

    auto t2 = Time(13, 8);
    ASSERT_EQ("13:08", t2.toString());

    auto t3 = Time(1, 61);
    ASSERT_NE("1:61", t3.toString());
    ASSERT_NE("01:61", t3.toString());
    ASSERT_EQ("02:01", t3.toString());

    auto t4 = Time(30, 20);
    ASSERT_EQ("30:20", t4.toString());
}