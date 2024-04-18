//
// Created by julia on 23.11.23.
//

#ifndef BALLISTICS2023_SPHEREMODEL_HPP
#define BALLISTICS2023_SPHEREMODEL_HPP

#include "ballistics/types/Vector.hpp"
namespace Ballistics::Force{


  class SphereModel{
  public:
      struct SatelliteParams{
        double area;
      };

      inline static Vector3d calcForce(const Vector3d& solarPressure,
                         const SatelliteParams& satelliteParams) noexcept{
        return solarPressure * satelliteParams.area;
      }

  };
}
#endif // BALLISTICS2023_SPHEREMODEL_HPP
