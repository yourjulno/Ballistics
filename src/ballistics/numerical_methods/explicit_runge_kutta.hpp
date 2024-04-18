//
// Created by julia on 08.02.24.
//

#ifndef BALLISTICS2023_EXPLICIT_RUNGE_KUTTA_HPP
#define BALLISTICS2023_EXPLICIT_RUNGE_KUTTA_HPP
#include "../types/types.hpp"

#include <array>
namespace Ballistics::NumericalMethods{

struct RK4 {
  static constexpr std::size_t stages = 4;
  static constexpr std::array<scalar, stages> column = {0, 0.5, 0.5, 1};
  static constexpr std::array<scalar, stages> row = {1 / 6., 1 / 3., 1 / 3., 1 / 6.};
  static constexpr std::array<scalar, stages *(stages - 1) / 2> matrix = {
      0.5, 0, 0.5, 0, 0, 1};
};

struct IntegrationParameters {
   scalar step;
};

template<typename ButcherTable, typename RHS>
typename RHS::State integrateOneStep(const RHS &rhs,
                                     const typename RHS::State &initial,
                                     const IntegrationParameters &integrationParameters) {
  auto currentIntegrationState = rhs.toIntegrationState(initial);
  std::array<typename RHS::IntegrationVector, ButcherTable::stages> k;
  for (std::size_t i = 0; i < ButcherTable::stages; i++) {
    auto vec = RHS::IntegrationVector::Zeros();
    for (std::size_t j = 0; j < i; j++) {
      vec += ButcherTable::matrix[i * (i - 1) / 2 + j] * k[j];
    }
    vec *= integrationParameters.step;
    vec += currentIntegrationState.vector;
    k[i] = rhs.calc(
        {vec,
         currentIntegrationState.argument + ButcherTable::column[i] * integrationParameters.step});
  }

  auto result = RHS::IntegrationVector::Zeros();
  for (std::size_t i = 0; i < ButcherTable::stages; ++i) {
    result += ButcherTable::row[i] * k[i];
  }
  result *= integrationParameters.step;
  result += currentIntegrationState.vector;
  return RHS::toState({result, initial.argument + integrationParameters.step});
}




}
#endif // BALLISTICS2023_EXPLICIT_RUNGE_KUTTA_HPP
