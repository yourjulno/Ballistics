//
// Created by julia on 02.05.24.
//
#include "ballistics/numerical_methods/explicit_runge_kutta.hpp"
#include "ballistics/rhs/CentralGravity.hpp"
#include "ballistics/orbits/CartesianOrbit.hpp"
#include "ballistics/orbits/KeplerianOrbit.hpp"
#include "ballistics/orbit_converter/OrbitConverter.hpp"
#include "gtest/gtest.h"
using namespace Ballistics;
using namespace RHS;
using namespace NumericalMethods;
using namespace Orbit;

constexpr scalar gravParam = 398600441500000;
using State = EarthGravity::State;

scalar calcM(const KeplerianOrbit& o){
  const scalar cosE = (o.eccentricity + std::cos(o.trueAnomaly)) /
                        (1 + o.eccentricity * std::cos(o.trueAnomaly));
  const scalar sinE = std::sqrt(1 - cosE * cosE);
  const scalar E = asin(sinE);
  const scalar M = E - o.eccentricity * sinE;
  return M;
}
TEST(first, Intergrator) {
  const std::string modelPath = "/home/julia/ballistics2023/resources/gravity/";

  const EarthGravity rhs( modelPath, "egm2008", 1, 1);
  const RK4 butcherTable;

  const scalar T = 10;
  const scalar a = 42e6;
  const scalar M0 = 0 ;
  const scalar n = std::sqrt(gravParam/ (a * a * a));

  const scalar time = 2 * M_PI / n;

  const Vector3d r0 = {a * std::cos(n * time + M0), a * std::sin(n * time + M0), 0};
  const Vector3d v0 = {- a * n * std::sin(n * time + M0), a * n * std::cos(n * time + M0), 0};

  const State initialState = {
      r0, v0, Time::Time<Time::Scale::TT>()};
  const std::vector<scalar> h = {10000, 1000, 100, 10};
  std::vector<scalar> errors;
  std::vector<scalar> maxError(h.size());
  std::vector<scalar> lnH(h.size());         // Логарифм шага
  std::vector<scalar> lnMaxError(h.size());  // Логарифм ошибки

  const EarthGravity::ArgumentType endTime = initialState.argument + time;
  for (int i = 0; i < h.size(); ++i) {
    const auto result = rungeKutta<RK4, EarthGravity>(rhs, initialState, {h[i]}, endTime);
    maxError[i] = 0;
    scalar t;
    scalar error;
    error = (result.position - r0).norm();
    errors.push_back(error);
    maxError[i] = std::max(maxError[i], error);

    // Логарифмы шага и ошибок
    lnH[i] = log(h[i]);
    lnMaxError[i] = log(sqrt(maxError[i]));
  }
  for(const auto& error : errors){
    std::cout << error << std::endl;
  }
const scalar t = 10;
////  const IntegrationParameters p = {h};
////  const EarthGravity::ArgumentType endTime = initialState.argument + time;
//  const auto result = rungeKutta<EarthGravity, RK4>(rhs, initialState, p, endTime);

}