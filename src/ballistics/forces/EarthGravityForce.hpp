//
// Created by julia on 19.10.23.
//

#ifndef BALLISTICS2023_EARTHGRAVITYFORCE_HPP
#define BALLISTICS2023_EARTHGRAVITYFORCE_HPP
#include "GeographicLib/GravityModel.hpp"
#include "ballistics/types/Vector.hpp"
#include "ballistics/time/Time.hpp"
namespace Ballistics::Forces{
    class EarthGravityForce{
        GeographicLib::GravityModel model_;
    public:
      struct SatParams{

      };
      /**
             * Конструктор класса гравитационной силы
             * @param model - название модели гравитационного потенциала
             * @param path - путь к файлу с данными модели
             * @param maxN - степень разложения гравитационного потенциала
             * @param maxM - порядок разложения потенциала
             * @return вектор силы в системе GCRS
       */

       EarthGravityForce(const std::string& model, const std::string &path,
                                   int maxN, int maxM): model_(model, path, maxN, maxM){}

       //ECEF
        [[nodiscard]] Vector3d calcForce(const Vector3d& positionECEF) const{
          Vector3d result;
          model_.V(positionECEF.x(), positionECEF.y(), positionECEF.z(),
                   result.x(), result.y(), result.z());
          return result;
        }



    };

}

#endif //BALLISTICS2023_EARTHGRAVITYFORCE_HPP
