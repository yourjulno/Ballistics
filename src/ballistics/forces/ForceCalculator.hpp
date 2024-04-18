//
// Created by julia on 20.12.23.
//

#ifndef BALLISTICS2023_FORCECALCULATOR_HPP
#define BALLISTICS2023_FORCECALCULATOR_HPP

#include "../time/Time.hpp"
#include "../types/Quaternion.hpp"
#include "../types/Vector.hpp"

namespace Ballistics{

template <typename TimeConverter, typename FrameConverter,
          typename EphemerisCalculator, typename EarthGravity,
          typename... OtherForces>
class AccelerationCalculator {

public:
  struct allSatParams : OtherForces::SatParams... {};

private:
  TimeConverter timeConverter_;
  FrameConverter frameConverter_;
  EphemerisCalculator ephemerisCalculator_;

  struct InputParams {
    Vector3d moonPositionGCRS;
    Vector3d jupiterPositionGCRS;
    Vector3d sunPositionGCRS;
    Vector3d sunVelocityGCRS;

    double moonGravParameter;
    double jupiterGravParameter;
    double sunGravParameter;
  };

  [[nodiscard]] InputParams calcInputParam(
      const Time::Time<Time::Scale::TT> timeTT,
      const Quaternion<double> &quaternionITRStoGCRS) const noexcept {

    const Time::Time<Time::Scale::TDB> timeTDB =
        timeConverter_.convertTT_TDB(timeTT);

    const auto stdToVector3d = [](const std::vector<double> &vector3) {
      return Vector3d{vector3[0], vector3[1], vector3[2]};
    };

    const int centerBody = 3;
    const Vector3d moonPositionITRS = stdToVector3d(
        ephemerisCalculator_.calcCoordinates(10, centerBody, timeTDB));

    const Vector3d jupiterPositionITRS = stdToVector3d(
        ephemerisCalculator_.calcCoordinates(5, centerBody, timeTDB));

    const std::vector<double> sunR =
        ephemerisCalculator_.calcCoordinates(11, centerBody, timeTDB);
    const std::vector<double> sunV =
        ephemerisCalculator_.calcVelocities(11, centerBody, timeTDB);

    const Vector3d sunPositionITRS = {sunR[0], sunR[1], sunR[2]};
    const Vector3d sunVelocityITRS = {sunV[0], sunV[1], sunV[3]};


    const double muMoon = ephemerisCalculator_.calcGravParameter(10);
    const double muJupiter = ephemerisCalculator_.calcGravParameter(5);
    const double muSun = ephemerisCalculator_.calcGravParameter(11);

    return {moonPositionITRS,
            jupiterPositionITRS,
            sunPositionITRS,
            sunVelocityITRS,
            muMoon,
            muJupiter,
            muSun};
  }

  template <unsigned int... Is>
  [[nodiscard]] Vector3d
  auxFunction(const std::tuple<OtherForces...> &otherForces,
              const Time::Time<Time::Scale::TT> timeTT,
              const Vector3d &positionGCRS, const Vector3d &velocityGCRS,
              double mass, const allSatParams &allSatParams,
              const InputParams &inputParams,
              std::integer_sequence<unsigned int, Is...>) const noexcept {

    return (std::get<Is>(otherForces)
                .calcForce(timeTT, positionGCRS, velocityGCRS, mass,
                                  allSatParams, inputParams) +
            ...);
  }

public:
  AccelerationCalculator(const TimeConverter &timeConverter,
                         const FrameConverter &frameConverter,
                         const EphemerisCalculator &ephemerisCalculator,
                         EarthGravity &,
                         const std::tuple<OtherForces...> &) noexcept
      : timeConverter_(timeConverter), frameConverter_(frameConverter),
        ephemerisCalculator_(ephemerisCalculator){};

  [[nodiscard]] Vector3d
  calcAcceleration(const EarthGravity &earthGravity,
                   const std::tuple<OtherForces...> &otherForces,
                   const Time::Time<Time::Scale::TT> timeTT,
                   const Vector3d &positionGCRS, const Vector3d &velocityGCRS,
                   double mass,
                   const allSatParams &allSatParams) const noexcept {

    const Quaternion<double> quaternionGCRStoITRS =
        frameConverter_.quaternionGCRStoITRS(timeTT);
    const Quaternion<double> quaternionITRStoGCRS =
        quaternionGCRStoITRS.inverse();

    const InputParams inputParams =
        calcInputParam(timeTT, quaternionGCRStoITRS);

    const Vector3d positionITRS =
        quaternionGCRStoITRS._transformVector(positionGCRS);
    const Vector3d earthGravAccelerationITRS =
        earthGravity.calcAccelerationECEF(positionITRS);

    const Vector3d otherForcesAcceleration = auxFunction(
        otherForces, timeTT, positionGCRS, velocityGCRS, mass, allSatParams,
        inputParams,
        std::make_integer_sequence<unsigned int, sizeof...(OtherForces)>{});

    return earthGravAccelerationITRS + otherForcesAcceleration;
  };
};
};


#endif // BALLISTICS2023_FORCECALCULATOR_HPP
