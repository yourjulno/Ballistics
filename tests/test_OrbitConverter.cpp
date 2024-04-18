//
// Created by julia on 28.03.24.
//
#include "gtest/gtest.h"
#include "ballistics/orbit_converter/OrbitConverter.hpp"
using namespace Ballistics;

const auto eps = std::numeric_limits<scalar>::epsilon();
TEST(KEPLERIAN_AND_CARTESIAN, SINGULAR_1) {
  const scalar gravParam = 3.9860044158e14;
  const scalar semimajor = 6800e3;
  const scalar ecc = 0.;
  const scalar incl = 0;
  const scalar argPerig = 0;
  const scalar ascNode = 0;
  const scalar anomaly = 0;

  const Orbit::KeplerianOrbit keplerianTrueInitial{semimajor, ecc, argPerig,incl,  ascNode, anomaly};
  const auto cart = Orbit::convertKeplerianToCartesian(keplerianTrueInitial, gravParam);
  const Vector3d referencePosition(semimajor, 0, 0);
  const Vector3d referenceVelocity(0, std::sqrt(gravParam / referencePosition.norm()), 0);
  ASSERT_NEAR((cart.position - referencePosition).norm() / referencePosition.norm(), 0,
              10 * eps);
  ASSERT_NEAR((cart.velocity - referenceVelocity).norm() / referenceVelocity.norm(), 0,
              10 *  eps);
  const auto resultKeplerian = Orbit::convertCartesianToKeplerian(cart, gravParam);
  const auto resultCartesian = Orbit::convertKeplerianToCartesian(resultKeplerian, gravParam);
  ASSERT_NEAR((resultCartesian.position - cart.position).norm() / resultCartesian.position.norm(), 0.,
              100 * eps);
}
