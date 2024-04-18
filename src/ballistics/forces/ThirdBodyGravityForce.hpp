//
// Created by julia on 20.12.23.
//

#ifndef BALLISTICS2023_THIRDBODYGRAVITYFORCE_HPP
#define BALLISTICS2023_THIRDBODYGRAVITYFORCE_HPP
#include "ballistics/types/Vector.hpp"
#include "ballistics/types/types.hpp"
#include "ballistics/time/Time.hpp"
namespace Ballistics::Forces {

class ThirdBodyGravityForce {


  [[nodiscard]] static Vector3d
  calcAccelerationSingleBody(const Vector3d &position,
                             const double gravParameter) noexcept {

    const double distanceSqr = position.squaredNorm();
    const double distance = std::sqrt(distanceSqr);
    const double distanceCube = distanceSqr * distance;

    return (-gravParameter / distanceCube) * position;
  }

public:
  struct SatelliteParams {};
  template <typename InputParams>
  Vector3d calcForce(const Time::Time<Time::Scale::UTC> & /*time*/,
                     const Vector3d &pos, const Vector3d & /*velocity*/,
                     const double mass,
                     const SatelliteParams & /*satelliteParams*/,
                     const InputParams &data) const {
    const Vector3d moonAcc =
        calcAccelerationSingleBody(pos - data.moonPos, data.moonGravParam);
    const Vector3d jupiterAcc = calcAccelerationSingleBody(
        pos - data.jupiterPos, data.jupiterGravParam);
    const Vector3d sunAcc =
        calcAccelerationSingleBody(pos - data.sunPos, data.sunGravParam);

    return moonAcc + jupiterAcc + sunAcc;
  };
};
}
#endif // BALLISTICS2023_THIRDBODYGRAVITYFORCE_HPP
