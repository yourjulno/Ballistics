//
// Created by julia on 19.12.23.
//
#include "ballistics/time/DutContainer.hpp"
#include "ballistics/time/Time.hpp"
#include "ballistics/time/TimeConverter.hpp"
#include "ballistics/utils/csvParser.hpp"
#include "gtest/gtest.h"
#include <iostream>
#include <limits>
using namespace Ballistics::Time;
using namespace Ballistics::Utils;
using TimeTT = Time<Scale::TT>;
constexpr double epsilon = std::numeric_limits<double>::epsilon();



Time<Scale::TAI> TAI_t = Time<Scale::TAI>::fromCalendar(2010, 1, 20, 21, 25, 10.5);
Time<Scale::UT1> UT1_t = Time<Scale::UT1>::fromCalendar(2010, 01, 20, 21, 24, 37.834100);
Time<Scale::UTC> UTC_t = Time<Scale::UTC>::fromCalendar(2010, 1, 20, 21, 24, 37.5);
Time<Scale::TT> TT_t = Time<Scale::TT>::fromCalendar(2010, 01, 20, 21, 25, 42.684);
Time<Scale::TCB> TCB_t = Time<Scale::TCB>::fromCalendar(2010, 01, 20, 21, 25, 56.893952);
Time<Scale::TDB> TDB_t = Time<Scale::TDB>::fromCalendar(2010, 01, 20, 21, 25, 42.684373);
Time<Scale::TCG> TCG_t = Time<Scale::TCG>::fromJD(TT_t.jd()+6.969290134*std::pow(10, -10)*(TT_t.jd()-2443144.5003725));

const std::string filename = "../../resources/rotation/earth_rotation.csv";
const auto result = timeConverterParser(filename);

DutContainer container(result.first, result.second);

TimeConverter<DutContainer> TimeConverter_1(container);

double error_1 = 0.005;
TEST(TIME_CONVERTER_TEST, CONVERT_METHOD_UT1){
  ASSERT_EQ((TimeConverter_1.convert<Scale::UT1>(UT1_t)).jdInt(), UT1_t.jdInt());
  ASSERT_EQ((TimeConverter_1.convert<Scale::UTC>(UT1_t)).jdInt(), UTC_t.jdInt());
  ASSERT_EQ((TimeConverter_1.convert<Scale::TAI>(UT1_t)).jdInt(), TAI_t.jdInt());
  ASSERT_EQ((TimeConverter_1.convert<Scale::TT>(UT1_t)).jdInt(), TT_t.jdInt());

  ASSERT_EQ((TimeConverter_1.convert<Scale::TCB>(UT1_t)).jdInt(), TCB_t.jdInt());
  ASSERT_EQ((TimeConverter_1.convert<Scale::TDB>(UT1_t)).jdInt(), TDB_t.jdInt());

  ASSERT_NEAR((TimeConverter_1.convert<Scale::UT1>(UT1_t)).jdFrac(), UT1_t.jdFrac(), error_1);
  ASSERT_NEAR((TimeConverter_1.convert<Scale::UTC>(UT1_t)).jdFrac(), UTC_t.jdFrac(), error_1);
 // ASSERT_NEAR((TimeConverter_1.convert<Scale::TAI>(UT1_t)).jdFrac(), TAI_t.jdFrac(), error_1);
  ASSERT_NEAR((TimeConverter_1.convert<Scale::TT>(UT1_t)).jdFrac(), TT_t.jdFrac(), error_1);

  ASSERT_NEAR((TimeConverter_1.convert<Scale::TCB>(UT1_t)).jdFrac(), TCB_t.jdFrac(), error_1);
  ASSERT_NEAR((TimeConverter_1.convert<Scale::TDB>(UT1_t)).jdFrac(), TDB_t.jdFrac(), error_1);

}

TEST(TIME_CONVERTER_TEST, CONVERT_METHOD_UTC){

  ASSERT_EQ((TimeConverter_1.convert<Scale::UT1>(UTC_t)).jdInt(), UT1_t.jdInt());
  ASSERT_EQ((TimeConverter_1.convert<Scale::UTC>(UTC_t)).jdInt(), UTC_t.jdInt());
  ASSERT_EQ((TimeConverter_1.convert<Scale::TAI>(UTC_t)).jdInt(), TAI_t.jdInt());
  ASSERT_EQ((TimeConverter_1.convert<Scale::TT>(UTC_t)).jdInt(), TT_t.jdInt());
  ASSERT_EQ((TimeConverter_1.convert<Scale::TCG>(UTC_t)).jdInt(), TCG_t.jdInt());
  ASSERT_EQ((TimeConverter_1.convert<Scale::TCB>(UTC_t)).jdInt(), TCB_t.jdInt());
  ASSERT_EQ((TimeConverter_1.convert<Scale::TDB>(UTC_t)).jdInt(), TDB_t.jdInt());

  ASSERT_NEAR((TimeConverter_1.convert<Scale::UT1>(UTC_t)).jdFrac(), UT1_t.jdFrac(), error_1);
  ASSERT_NEAR((TimeConverter_1.convert<Scale::UTC>(UTC_t)).jdFrac(), UTC_t.jdFrac(), error_1);
  ASSERT_NEAR((TimeConverter_1.convert<Scale::TAI>(UTC_t)).jdFrac(), TAI_t.jdFrac(), error_1);
  ASSERT_NEAR((TimeConverter_1.convert<Scale::TT>(UTC_t)).jdFrac(), TT_t.jdFrac(), error_1);
  ASSERT_NEAR((TimeConverter_1.convert<Scale::TCG>(UTC_t)).jdFrac(), TCG_t.jdFrac(), error_1);
  ASSERT_NEAR((TimeConverter_1.convert<Scale::TCB>(UTC_t)).jdFrac(), TCB_t.jdFrac(), error_1);
  ASSERT_NEAR((TimeConverter_1.convert<Scale::TDB>(UTC_t)).jdFrac(), TDB_t.jdFrac(), error_1);
}


