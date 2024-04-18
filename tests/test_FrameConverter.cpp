//
// Created by julia on 11.12.23.
//

#include "gtest/gtest.h"
#include "ballistics/frame_converter/FrameConverter.hpp"
#include "ballistics/time/Time.hpp"
#include "ballistics/time/DutContainer.hpp"
#include "ballistics/utils/csvParser.hpp"
#include "ballistics/time/TimeConverter.hpp"
#include "ballistics/utils/earth_rotation_result.hpp"
#include "ballistics/types/Vector.hpp"
using namespace Ballistics;
using namespace Ballistics::Time;
using namespace Ballistics::Utils;

const std::string filename = "../../resources/rotation/earth_rotation.csv";

const auto result = frameConverterParser(filename);

const std::string filename2 = "../../resources/rotation/earth_rotation.csv";
const auto result2 = timeConverterParser(filename);

DutContainer container(result2.first, result2.second);

TimeConverter<DutContainer> TimeConverter_1(container);
TEST(FRAME, CONVERT){

  struct Line
  {
    scalar mjd;
    Vector3d v1;
    Vector3d v2;
    Vector3d v3;
  };

  Frame::FrameConverter FrameConverter(result.second, result.first.first, result.first.second);
  TimeConverter<DutContainer> TimeConverter = TimeConverter_1;
  Vector3d initVec1 = {6.7e+6, 0., 0.};
  Vector3d initVec2 = {0., 6.7e+6, 0.};
  Vector3d initVec3 = {0., 0., 6.7e+6};
  scalar tolerance = 1.e-8;

  for (const auto el : earthRotationResult)
  {
    Line line = {el[0], {el[1], el[2], el[3]}, {el[4], el[5], el[6]}, {el[7], el[8], el[9]}};
    auto const timeTT = Ballistics::Time::Time<Ballistics::Time::Scale::TT>(line.mjd, 0.);
    auto const timeUTC = TimeConverter.convert<Ballistics::Time::Scale::UTC>(timeTT);
    auto const timeUT1 = TimeConverter.convert<Ballistics::Time::Scale::UT1>(timeTT);

    Quaterniond Q = FrameConverter.calcQuaternion<Frame::Frame::GCRS, Frame::Frame::ITRS>(timeTT,
                                                                                          timeUT1,
                                                                                          timeUTC);
    Vector3d vec1 = Q * initVec1;
    Vector3d vec2 = Q * initVec2;
    Vector3d vec3 = Q * initVec3;

    ASSERT_NEAR(vec1.norm(), line.v1.norm(), tolerance);
    ASSERT_NEAR(vec2.norm(), line.v2.norm(), tolerance);
    ASSERT_NEAR(vec3.norm(), line.v3.norm(), tolerance);

  }
}

