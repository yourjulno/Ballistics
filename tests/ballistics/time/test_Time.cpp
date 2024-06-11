//
// Created by julia on 19.12.23.
//
#include "gtest/gtest.h"
#include "ballistics/time/Time.hpp"
#include <limits>
#include <iostream>
using namespace Ballistics::Time;
using TimeTT = Time<Scale::TT>;
constexpr double epsilon = std::numeric_limits<double>::epsilon();

TEST(TIME_TEST, COMPARISON_OPERATORS) {
  const auto time1 = TimeTT(53005, 0.5);
  const auto time2 = TimeTT(53005, 0.5);



  EXPECT_FALSE(time1 < time2);
  EXPECT_TRUE(time1 <= time2);

  EXPECT_FALSE(time1 > time2);
  EXPECT_TRUE(time1 >= time2);

  const auto time3 = TimeTT(53005, 0.5);
  const auto time4 = TimeTT(53005, 0.5 + epsilon * 2);
  EXPECT_FALSE(time3 == time4);
  EXPECT_FALSE(time1 != time2);

  EXPECT_TRUE(time3 < time4);
  EXPECT_TRUE(time3 <= time4);

  EXPECT_FALSE(time3 > time4);
  EXPECT_FALSE(time3 >= time4);

  const auto time5 = TimeTT(530051, 0.5);
  const auto time6 = TimeTT(53005, 0.5);
  EXPECT_FALSE(time5 == time6);
  EXPECT_FALSE(time1 != time2);

  EXPECT_FALSE(time5 < time6);
  EXPECT_FALSE(time5 <= time6);

  EXPECT_TRUE(time5 > time6);
  EXPECT_TRUE(time5 >= time6);
}
