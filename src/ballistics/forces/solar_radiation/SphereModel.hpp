//
// Created by julia on 23.11.23.
//

#ifndef BALLISTICS2023_SPHEREMODEL_HPP
#define BALLISTICS2023_SPHEREMODEL_HPP

#include "ballistics/types/Vector.hpp"
namespace Ballistics::Forces{

  class SphereModel{
  public:
      struct SatParams{
        double area;
      };

      inline static Vector3d calcForce(const Vector3d& solarPressure,
                         const SatParams& satelliteParams) noexcept{
        return solarPressure * satelliteParams.area;
      }

  };
}
#endif // BALLISTICS2023_SPHEREMODEL_HPP
