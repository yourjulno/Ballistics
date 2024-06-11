//
// Created by julia on 06.06.24.
//
#include "gtest/gtest.h"
#include "fstream"
#include "ballistics/rhs/RightPart.hpp"
#include "ballistics/utils/csvParser.hpp"
#include "../../../resources/atmosphere/GOST4401_81.h"
#include "ballistics/atmosphere/Density.hpp"
#include "ballistics/numerical_methods/explicit_runge_kutta.hpp"
using namespace Ballistics;
using namespace Forces;
using namespace Time;
using namespace Utils;
using namespace Frame;
using namespace RHS;
using namespace NumericalMethods;

TEST(RK_FORCE_CALCULATOR, ORBIT1) {

  const std::string currentFile = __FILE__;

  const auto begin = static_cast<Ballistics::scalar>(0);
  const auto end = static_cast<Ballistics::scalar>(100000);


  const std::string filename = "../../resources/rotation/earth_rotation.csv";
  const auto resultTimeParser = timeConverterParser(filename);
  const auto resultFrameParser = frameConverterParser(filename);
  const DutContainer container(resultTimeParser.first, resultTimeParser.second);

  const TimeConverter<DutContainer> timeConverter(container);


  const FrameConverter frameConverter(resultFrameParser.second, resultFrameParser.first.first, resultFrameParser.first.second);


  const std::string ephemerisPath = currentFile.substr(0, currentFile.size() - 56) + "data/ephemeris/de405.bin";
  const std::string fileLoc = "/home/julia/ballistics2023/resources/ephemeris/de405.bin";

  Ephemeris::Ephemeris ephemerisCalculator(fileLoc);


  const std::string modelPath = "/home/julia/ballistics2023/resources/gravity/";
  EarthGravityForce earthGravity( "egm2008", modelPath, 1, 1);

  std::vector<scalar> x;
  std::vector<scalar> y;

  for (Ballistics::indexType i = 0;
       i < Atmosphere::GOST4401_81Raw::N; ++i) {
    x.push_back(Atmosphere::GOST4401_81Raw::height[i]);
    y.push_back(Atmosphere::GOST4401_81Raw::density[i]);
  }

  const Density density_(x, y);

  const DragForce<Density> dragForce(density_);

  /******************************************************/

  auto forcesTuple = std::tuple(dragForce);

  const double mass = 1;

  const ForceCalculator forceCalculator(earthGravity, timeConverter, frameConverter,
                                                                         ephemerisCalculator,
                                                                         forcesTuple);

  decltype(forceCalculator)::allSatParams allSatParams;
  allSatParams.Cd = 1;
  allSatParams.S = 0.001;

  const RightPartForces RP(forceCalculator, allSatParams, mass);

  const scalar gravParameter = 3.986e14;
  const Ballistics::Time::Time<Ballistics::Time::Scale::TT> timeTT(2452793.5, 0);
  const Ballistics::scalar initialRho = 6.7e6;
  const Ballistics::Vector3d position = {initialRho, 0, 0};
  const Ballistics::Vector3d velocity = {0, std::sqrt(gravParameter / position.norm()), 0};
  const decltype(RP)::State initialState = {position, velocity, timeTT};

  const Ballistics::scalar T = 2 * std::numbers::pi * initialRho * std::sqrt(initialRho / gravParameter);

  const auto step = 1;

  const auto endTimeTT = timeTT + 2 * T;

  const auto result = rungeKutta<RK4>(RP, initialState, {step}, endTimeTT);

  std::cout << result.position.norm() - initialRho;
}