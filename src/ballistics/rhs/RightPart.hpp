//
// Created by julia on 04.06.24.
//

#ifndef BALLISTICS2023_RIGHTPART_HPP
#define BALLISTICS2023_RIGHTPART_HPP

#include "ballistics/atmosphere/Density.hpp"
#include "ballistics/ephemeris/Ephemeris.hpp"
#include "ballistics/forces/DragForce.hpp"
#include "ballistics/forces/EarthGravityForce.hpp"
#include "ballistics/forces/ForceCalculator.hpp"
#include "ballistics/forces/ThirdBodyGravityForce.hpp"
#include "ballistics/forces/solar_radiation/SolarForce.hpp"
#include "ballistics/forces/solar_radiation/SolarPressure.hpp"
#include "ballistics/forces/solar_radiation/SphereModel.hpp"
#include "ballistics/forces/solar_radiation/TSI.hpp"
#include "ballistics/frame_converter/FrameConverter.hpp"
#include "ballistics/time/DutContainer.hpp"
#include "ballistics/time/Time.hpp"
#include "ballistics/time/TimeConverter.hpp"
#include "ballistics/types/Vector.hpp"
namespace Ballistics::RHS {
// dr/dt = v

// dv/dt = F(t)/m

template <typename ForceCalculator>
class RightPartForces {
  ForceCalculator forceCalculator_;
  ForceCalculator::allSatParams allSatParams_;
  scalar mass_;

public:
  RightPartForces(const ForceCalculator &forceCalculator,
                  const ForceCalculator::allSatParams &allSatParams,
                  const scalar mass): forceCalculator_(forceCalculator), allSatParams_(allSatParams),
  mass_(mass){}

  using IntegrationVector = Vector6d;
  using ArgumentType = Time::Time<Time::Scale::TT>;

  struct IntegrationState {
    IntegrationVector vector;
    Time::Time<Time::Scale::TT> argument;
  };

  struct State {
    Vector3d position;
    Vector3d velocity;

    Time::Time<Time::Scale::TT> argument;
  };

  [[nodiscard]] IntegrationVector
  calc(const IntegrationState &integrationState) const noexcept {

    const Vector3d satPosGCRS = integrationState.vector.template segment<3>(0);
    const Vector3d satVel = integrationState.vector.template segment<3>(3);
    const Vector3d resForce = forceCalculator_.calcForce(integrationState.argument, satPosGCRS,
        satVel, mass_, allSatParams_);
    IntegrationVector result;

    result.segment<3>(0) = integrationState.vector.template segment<3>(3);
    result.segment<3>(3) = resForce;

    return result;
  }

  [[nodiscard]] static IntegrationState
  toIntegrationState(const State &state) noexcept {

    IntegrationVector integrationVector;
    integrationVector.segment<3>(0) = state.position;
    integrationVector.segment<3>(3) = state.velocity;

    return {integrationVector, state.argument};
  }

  [[nodiscard]] static State
  toState(const IntegrationState &integrationState) noexcept {

    const Vector3d position = integrationState.vector.template segment<3>(0);
    const Vector3d velocity = integrationState.vector.template segment<3>(3);

    return {position, velocity, integrationState.argument};
  }
};
} // namespace Ballistics::RHS
#endif // BALLISTICS2023_RIGHTPART_HPP
