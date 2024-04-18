//
// Created by julia on 20.12.23.
//

#ifndef BALLISTICS2023_DRAGFORCE_HPP
#define BALLISTICS2023_DRAGFORCE_HPP
#include "ballistics/types/Vector.hpp"
#include "ballistics/types/types.hpp"
#include "ballistics/time/Time.hpp"
namespace Ballistics::Forces
{
template <class DensityCalculator>
class DragForce{
DensityCalculator density_;
scalar EarthRadius = 6.371e6;
;
public:
  /**
   * @param S - площадь
   * @param Cd - коэффициент трения об атмосферу
   */
  struct SatelliteParams{
    double S;
    double Cd;
  };



  explicit DragForce(const DensityCalculator densityCalculator): density_(densityCalculator){}
  // @return вектор силы трения
  template <typename InputParams>
  Vector3d calcForce(const Time::Time<Time::Scale::UTC> & /*time*/,
                     const Vector3d &pos, const Vector3d & velocity,
                     const double mass,
                     const SatelliteParams & satelliteParams,
                     const InputParams /*&data*/) const {
    const double velocityNorm = velocity.norm();
    const double height = pos.norm() - EarthRadius;
    const double density = density_.calcDensity(height);
    return - density * velocityNorm * satelliteParams.S * satelliteParams.Cd * velocity / mass ;
  };
};
}

#endif // BALLISTICS2023_DRAGFORCE_HPP
