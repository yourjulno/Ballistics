//
// Created by julia on 16.11.23.
//

#ifndef BALLISTICS2023_SOLARFORCE_HPP
#define BALLISTICS2023_SOLARFORCE_HPP
#include "SolarPressure.hpp"
#include "SphereModel.hpp"
#include "ballistics/types/Vector.hpp"
namespace Ballistics {

template <typename SolarPressure, typename InteractionModel> class SolarForce {
  SolarPressure solarPressure_;

public:
  using SatelliteParams = typename InteractionModel::SatelliteParams;
  Vector3d calcForce(const Time::Time<Time::Scale::UTC> &time, const Vector3d &pos,
                     const Vector3d &velocity, const Vector3d &sunPos,
                     const Vector3d &sunVelocity, const Vector3d &moonPos,
                     const SatelliteParams &satelliteParams) {
    const Vector3d solarPressure = solarPressure_.calcSolarPressure(
        time, pos, velocity, sunPos, sunVelocity);
    // домножить на площадь
    return InteractionModel::calcForce(solarPressure, satelliteParams);
  }

  template <typename T>
  Vector3d calcForce(const Time::Time<Time::Scale::UTC> &time, const Vector3d &pos,
                     const Vector3d &velocity, const double /*mass*/,
                     const SatelliteParams &satelliteParams, const T &data) {
    return calcForce(time, pos, velocity, data.sunPos, data.sunVelocity,
                     data.moonPos, satelliteParams);
  }
};
} // namespace Ballistics
#endif // BALLISTICS2023_SOLARFORCE_HPP
