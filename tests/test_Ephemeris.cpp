//
// Created by julia on 20.12.23.
//
#include "ballistics/time/Time.hpp"
#include "ballistics/time/TimeConverter.hpp"
#include "ballistics/types/types.hpp"
#include "gtest/gtest.h"
#include "ballistics/utils/csvParser.hpp"
#include "ballistics/utils/ephems.hpp"
#include "../third_party/fast-cpp-csv-parser/csv.h"
#include "ballistics/time/DutContainer.hpp"
#include "ballistics/ephemeris/Ephemeris.hpp"
using namespace Ballistics;
using namespace Ballistics::Utils;
using namespace Ballistics::Time;
bool isEqualByRelativeError(const Eigen::Vector3d &l, const Eigen::Vector3d &r, const scalar tolerance)
{
  return (l - r).norm()  <= tolerance * r.norm();
}
const std::string filename = "../../resources/rotation/earth_rotation.csv";
const auto result = timeConverterParser(filename);

DutContainer container(result.first, result.second);

TimeConverter<DutContainer> TimeConverter_1(container);

TEST(EPHEMERIS, TEST1){
  std::vector<int> bodyNumCol;
  std::vector<scalar> jdDayCol;
  std::vector<scalar> jdPartCol;
  std::vector<scalar> xCol;
  std::vector<scalar> yCol;
  std::vector<scalar> zCol;
  std::vector<scalar> vxCol;
  std::vector<scalar> vyCol;
  std::vector<scalar> vzCol;
  for (auto& set: ephemeris_result) {
    bodyNumCol.push_back(set[0]);
    jdDayCol.push_back(set[1]);
    jdPartCol.push_back(set[2]);
    xCol.push_back(set[3]);
    yCol.push_back(set[4]);
    zCol.push_back(set[5]);
    vxCol.push_back(set[6]);
    vyCol.push_back(set[7]);
    vzCol.push_back(set[8]);
  }
  const std::string fileLoc = "/home/julia/ballistics2023/resources/ephemeris/de405.bin";
    auto converter = TimeConverter_1;
    Ballistics::Ephemeris::Ephemeris Ephems(fileLoc);

    int n = xCol.size();
    scalar tolerance = 1e-14;

    for (int i = 0; i < n; ++i) {
      const Ballistics::Time::Time<Ballistics::Time::Scale::TT> timeTT(
          jdDayCol[i], jdPartCol[i]);
      const Ballistics::Time::Time<Ballistics::Time::Scale::TDB> timeTDB =
          converter.convertTT_TDB(timeTT);
      const Vector3d position = {xCol[i], yCol[i], zCol[i]};
      const Vector3d velocity = {vxCol[i], vyCol[i], vzCol[i]};

      ASSERT_TRUE(isEqualByRelativeError(
          Ephems.calcCoordinates(timeTDB, bodyNumCol[i], 3), position,
          tolerance));

      ASSERT_TRUE(isEqualByRelativeError(
          Ephems.calcVelocities(timeTDB, bodyNumCol[i], 3), velocity,
          tolerance));
    }
  }


