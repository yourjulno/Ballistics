//
// Created by yourjulno on 28.03.24.
//

#ifndef BALLISTICS2023_EARTHGRAVITY_H
#define BALLISTICS2023_EARTHGRAVITY_H


#include "ballistics/types/Vector.hpp"
#include "ballistics/forces/EarthGravityForce.hpp"
#include "ballistics/time/Time.hpp"

namespace Ballistics::RHS {

class EarthGravity {

  Ballistics::Force::EarthGravityForce earthGravityForce;

public:

  EarthGravity(const std::string &path, const std::string &gravityModelName, int maxN, int maxM)
      : earthGravityForce(gravityModelName, path, maxN, maxM) {};

  using ArgumentType = Time::Time<Time::Scale::TT>;

  using IntegrationVector = Vector6d;

  struct IntegrationState {
    IntegrationVector vector;
    Time::Time<Time::Scale::TT> argument;
  };

  struct State {
    Vector3d position;
    Vector3d velocity;

    Time::Time<Time::Scale::TT> argument;
  };

  [[nodiscard]] inline IntegrationVector calc(const IntegrationState &integrationState) const noexcept {

    const Vector3d acceleration = earthGravityForce.calcAccelerationECEF(integrationState.vector.segment<3>(0));

    IntegrationVector result;

    result.segment<3>(0) = integrationState.vector.segment<3>(3);
    result.segment<3>(3) = acceleration;

    return result;
  }

  [[nodiscard]] static IntegrationState toIntegrationState(const State &state) noexcept {

    IntegrationVector integrationVector;
    integrationVector.segment<3>(0) = state.position;
    integrationVector.segment<3>(3) = state.velocity;

    return {integrationVector, state.argument};
  }

  [[nodiscard]] static State toState(const IntegrationState &integrationState) noexcept {

    const Vector3d position = integrationState.vector.segment<3>(0);
    const Vector3d velocity = integrationState.vector.segment<3>(3);

    return {position, velocity, integrationState.argument};
  }

};
}

#endif //BALLISTICS2023_EARTHGRAVITY_H