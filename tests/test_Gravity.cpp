//
// Created by julia on 20.12.23.
//

#include "fstream"
#include <gtest/gtest.h>
#include "ballistics/ephemeris/Ephemeris.hpp"
#include "ballistics/forces/EarthGravityForce.hpp"
#include "ballistics/forces/ThirdBodyGravityForce.hpp"
using namespace Ballistics::Forces;
using namespace Ballistics::Ephemeris;
using namespace Ballistics::Time;

//TEST(GRAVITY, EARTH_COMPARISON) {

//  const double tolerance = 1e-4;
//
//  EarthGravityForce earthGravity4x4("/home/julia/ballistics2023/resources/gravity",
//                                                       "egm96", 4, 4);
//
//  const std::string ephemerisPath = "../../resources/ephemeris/de405.bin";
//  const Ephemeris ephemerisCalculator(ephemerisPath);
//
//  const double R = 42164e3;
//  const double mass = 100;
//
//  const int target = 3;
//  const double gravParameter = ephemerisCalculator.calcGravParameter(target);
//  const Time<Scale::TT> tt(0, 0);
//  const ThirdBodyGravityForce::SatelliteParams satParams{};
//  const ThirdBodyGravityForce celestialGravity(gravParameter);
//
//  for (int m = 0; m < 100; m++) {
//    for (int n = 0; n < 100; n++) {
//
//      double Tet = M_1_PI / 100 * m;
//      double Phi = 2 * M_1_PI / 100 * n;
//
//      double x = R * std::sin(Tet) * std::cos(Phi);
//      double y = R * std::sin(Tet) * std::sin(Phi);
//      double z = R * std::cos(Tet);
//
//      const Ballistics::Vector3d position = {x, y, z};
//      const Ballistics::Vector3d velocity = Ballistics::Vector3d::Zero();
//
//      const Ballistics::Vector3d egmAcceleration = earthGravity4x4.calcAccelerationECEF({x, y, z});
//      const Ballistics::Vector3d celestialAcceleration = celestialGravity.calcAcceleration(tt, position, velocity,
//                                                                                           mass, satParams);
//
//      ASSERT_NEAR((egmAcceleration - celestialAcceleration).norm(), 0, tolerance);
//    }
//  }
//}


TEST(CALCFORCES, TEST) {

//  const std::string currentFile = __FILE__;
  const std::string modelPath = "/home/julia/ballistics2023/resources/gravity/";
  const std::string outputPath = "/home/julia/ballistics2023/resources/gravity/data.txt";

  std::ofstream myfile(outputPath, std::ios::trunc);

  EarthGravityForce earthGravity1x1( "egm2008", modelPath, 1, 1);

  EarthGravityForce earthGravity2x2( "egm2008", modelPath, 2, 2);

  EarthGravityForce earthGravity3x3( "egm2008", modelPath,3, 3);

  EarthGravityForce earthGravity128x128("egm2008", modelPath, 128, 128);

  EarthGravityForce earthGravity64x64( "egm2008", modelPath, 64, 64);

  EarthGravityForce earthGravity32x32("egm2008",modelPath, 32, 32);

  EarthGravityForce earthGravity16x16("egm2008", modelPath, 16, 16);

  EarthGravityForce earthGravity4x4( "egm2008",modelPath, 4, 4);

  for (int i = 0; i < 177; i++) {

    std::cout << i << std::endl;

    double R = (200 * i + 6600) * 1e3;

    Ballistics::scalar meanDiffByR1122 = 0;
    Ballistics::scalar meanDiffByR2233 = 0;

    Ballistics::scalar meanDiffByR128x64 = 0;
    Ballistics::scalar meanDiffByR128x32 = 0;
    Ballistics::scalar meanDiffByR128x16 = 0;
    Ballistics::scalar meanDiffByR128x4 = 0;
    for (int m = 0; m < 100; m++) {
      for (int n = 0; n < 100; n++) {

        double Tet = M_1_PI / 100 * m;
        double Phi = 2 * M_1_PI / 100 * n;

        double x = R * std::sin(Tet) * std::cos(Phi);
        double y = R * std::sin(Tet) * std::sin(Phi);
        double z = R * std::cos(Tet);

        const Ballistics::Vector3d acceleration1x1 =
            earthGravity1x1.calcAccelerationECEF({x, y, z});
        const Ballistics::Vector3d acceleration2x2 =
            earthGravity2x2.calcAccelerationECEF({x, y, z});
        const Ballistics::Vector3d acceleration3x3 =
            earthGravity3x3.calcAccelerationECEF({x, y, z});

        const Ballistics::Vector3d acceleration128x128 =
            earthGravity128x128.calcAccelerationECEF({x, y, z});
        const Ballistics::Vector3d acceleration64x64 =
            earthGravity64x64.calcAccelerationECEF({x, y, z});
        const Ballistics::Vector3d acceleration32x32 =
            earthGravity32x32.calcAccelerationECEF({x, y, z});
        const Ballistics::Vector3d acceleration16x16 =
            earthGravity16x16.calcAccelerationECEF({x, y, z});
        const Ballistics::Vector3d acceleration4x4 =
            earthGravity4x4.calcAccelerationECEF({x, y, z});

        meanDiffByR1122 += (acceleration1x1 - acceleration2x2).norm();
        meanDiffByR2233 += (acceleration3x3 - acceleration2x2).norm();

        meanDiffByR128x64 += (acceleration128x128 - acceleration64x64).norm();
        meanDiffByR128x32 += (acceleration128x128 - acceleration32x32).norm();
        meanDiffByR128x16 += (acceleration128x128 - acceleration16x16).norm();
        meanDiffByR128x4 += (acceleration128x128 - acceleration4x4).norm();

        //                myfile << R << " " << x << " " << y << " " << z << " " << acceleration.x() << " " << acceleration.y() << " "
        //                       << acceleration.z() << std::endl;
      }
    }

    myfile << " " << R << " " << meanDiffByR1122 / 10000 << " "
           << meanDiffByR2233 / 10000 << " " << meanDiffByR128x64 / 10000 << " "
           << meanDiffByR128x32 / 10000 << " " << meanDiffByR128x16 / 10000
           << " " << meanDiffByR128x4 / 10000 << std::endl;
  }
}