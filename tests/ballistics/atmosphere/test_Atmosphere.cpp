#include "../../../resources/atmosphere/GOST4401_81.h"
#include "ballistics/atmosphere/Density.hpp"
#include "ballistics/forces/DragForce.hpp"
#include "ballistics/time/Time.hpp"
#include "ballistics/types/types.hpp"
#include <gtest/gtest.h>
using namespace Ballistics;
using namespace Ballistics::Forces;
using namespace Ballistics::Time;
TEST(DENSITY, GOST) {

  const auto tolerance = static_cast<Ballistics::scalar>(1e-12);

  std::vector<scalar> x;
  std::vector<scalar> y;

  for (Ballistics::indexType i = 0;
       i < Ballistics::Atmosphere::GOST4401_81Raw::N; ++i) {
    x.push_back(Ballistics::Atmosphere::GOST4401_81Raw::height[i]);
    y.push_back(Ballistics::Atmosphere::GOST4401_81Raw::density[i]);
  }

  const Density density_(x, y);

  for (Ballistics::indexType i = 0;
       i < Ballistics::Atmosphere::GOST4401_81Raw::N - 1; ++i) {

    const Ballistics::scalar point =
        (Ballistics::Atmosphere::GOST4401_81Raw::height[i] +
         Ballistics::Atmosphere::GOST4401_81Raw::height[i + 1]) /
        2;

    const Ballistics::scalar density = density_.calcDensity(point);

    const Ballistics::scalar referenceDensity =
        (Ballistics::Atmosphere::GOST4401_81Raw::density[i] +
         Ballistics::Atmosphere::GOST4401_81Raw::density[i + 1]) /
        2;

    ASSERT_NEAR(density, referenceDensity, tolerance);
  }
}

  TEST(DRAG, SET1) {

    const auto tolerance = static_cast<Ballistics::scalar>(1e-12);

    std::vector<scalar> x;
    std::vector<scalar> y;

    for (Ballistics::indexType i = 0;
         i < Ballistics::Atmosphere::GOST4401_81Raw::N; ++i) {
      x.push_back(Ballistics::Atmosphere::GOST4401_81Raw::height[i]);
      y.push_back(Ballistics::Atmosphere::GOST4401_81Raw::density[i]);
    }

    const Density density_(x, y);

    const DragForce<Density> dragForce(density_);

    const Ballistics::Time::Time<Scale::TT> timeTT(2458119, 0.5);
    const Ballistics::Vector3d position = {6800e3, 0, 0};
    const Ballistics::Vector3d velocity = {0, std::sqrt(398600e9 / 6800e3), 0};
    const double mass = 1;
    decltype(dragForce)::SatelliteParams satParams = {1, 1};
    const double inputParams = 0;

    const Vector3d drag = dragForce.calcForce(timeTT, position, velocity, mass, satParams, inputParams);

    std::cout << drag;
  }
