//
// Created by julia on 08.02.24.
//

#ifndef BALLISTICS2023_EXPLICIT_RUNGE_KUTTA_HPP
#define BALLISTICS2023_EXPLICIT_RUNGE_KUTTA_HPP
#include "../types/types.hpp"
#include "ballistics/rhs/CentralGravity.hpp"
#include <array>
namespace Ballistics::NumericalMethods {

struct RK4 {
  static constexpr std::size_t stages = 4;
  static constexpr std::array<scalar, stages> column = {0, 0.5, 0.5, 1};
  static constexpr std::array<scalar, stages> row = {1 / 6., 1 / 3., 1 / 3.,
                                                     1 / 6.};
  static constexpr std::array<scalar, stages *(stages - 1) / 2> matrix = {
      0.5, 0, 0.5, 0, 0, 1};
};

struct IntegrationParameters {
  scalar step;
};

template <typename ButcherTable, typename RHS>
typename RHS::State
integrateOneStep(const RHS &rhs, const typename RHS::State &initial,
                 const IntegrationParameters &integrationParameters) {
  auto currentIntegrationState = rhs.toIntegrationState(initial);
  std::array<typename RHS::IntegrationVector, ButcherTable::stages> k;
  for (std::size_t i = 0; i < ButcherTable::stages; i++) {
    typename RHS::IntegrationVector vec = RHS::IntegrationVector::Zero();
    for (std::size_t j = 0; j < i; j++) {
      vec = vec + k[j] * ButcherTable::matrix[i * (i - 1) / 2 + j];
    }
    vec = vec *  integrationParameters.step;
    vec = vec +  currentIntegrationState.vector;
    k[i] = rhs.calc(
        {vec, currentIntegrationState.argument +
                  ButcherTable::column[i] * integrationParameters.step});
  }

  typename RHS::IntegrationVector result = RHS::IntegrationVector::Zero();
  for (std::size_t i = 0; i < ButcherTable::stages; ++i) {
    result = result +  ButcherTable::row[i] * k[i];
  }
  result = result *  integrationParameters.step;
  result = result + currentIntegrationState.vector;
  return RHS::toState({result, initial.argument + integrationParameters.step});
}

/**
 *
 * @tparam RHS - тип правой части уравнения x' = f(t,x)
 * @tparam ButcherTable
 * @param rhs - правая часть f(t,x)
 * @param initialState - н.у.
 * @param integrationParameters - шаг интегрирования
 * @param endTime - время, до которого надо интегрироваться
 * @return State в момент времени endTime
 */
template <typename ButcherTable, typename RHS>
typename RHS::State rungeKutta(const RHS &rhs,
                         const typename RHS::State &initialState,
                         const IntegrationParameters &integrationParameters,
                         const typename RHS::ArgumentType &endTime) {

  typename RHS::State currentState = initialState;

  while (currentState.argument + integrationParameters.step < endTime) {


    currentState = integrateOneStep<ButcherTable>(rhs, currentState,
                                                  integrationParameters);
  }


  return integrateOneStep<ButcherTable>(rhs, currentState,
                                        {endTime - currentState.argument});
}

} // namespace Ballistics::NumericalMethods
#endif // BALLISTICS2023_EXPLICIT_RUNGE_KUTTA_HPP
