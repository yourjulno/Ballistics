//
// Created by julia on 16.11.23.
//

#ifndef BALLISTICS2023_SOLARPRESSURE_HPP
#define BALLISTICS2023_SOLARPRESSURE_HPP

#include "ballistics/forces/solar_radiation/IndependentShadow.hpp"
#include "ballistics/forces/solar_radiation/TSI.hpp"
#include "ballistics/time/Time.hpp"
#include "ballistics/types/Vector.hpp"
namespace Ballistics {

template <typename TSIModel, typename ShadowModel> class SolarPressure {
  TSIModel tsiModel_;
  ShadowModel shadowModel_;

public:
  SolarPressure(const TSIModel &tsiModel, const ShadowModel &shadowModel)
      : tsiModel_(tsiModel), shadowModel_(shadowModel){};

  // считает поток энергии, деленный на скорость света
  [[nodiscard]] inline Vector3d
  calcSolarPressure(const Time::Time<Time::Scale::UTC> &time, const Vector3d &pos,
                    const Vector3d &velocity, const Vector3d &sunPos,
                    const Vector3d &moonPos,
                    const Vector3d &sunVelocity) const {
    constexpr double AU = 149597870700;
    constexpr double c = 299792458;
    const double deltaRadiusNorm = (pos - sunPos).norm();
    const Vector3d vRel = velocity - sunVelocity;
    const Vector3d normVector = pos - sunPos / deltaRadiusNorm;
    const Vector3d energyFlow =
        tsiModel_.TSI() * AU * AU / (deltaRadiusNorm * deltaRadiusNorm) *
        (normVector * (1 - normVector.dot(vRel) / c) - vRel / c);
    return energyFlow.norm() / c *
           shadowModel_.calcFlux(pos, sunPos, moonPos, energyFlow);
  }
};
} // namespace Ballistics
#endif // BALLISTICS2023_SOLARPRESSURE_HPP
