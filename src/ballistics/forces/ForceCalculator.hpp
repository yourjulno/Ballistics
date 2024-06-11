//
// Created by julia on 20.12.23.
//

#ifndef BALLISTICS2023_FORCECALCULATOR_HPP
#define BALLISTICS2023_FORCECALCULATOR_HPP

#include "../time/Time.hpp"
#include "../types/Quaternion.hpp"
#include "../types/Vector.hpp"
#include "ballistics/frame_converter/FrameConverter.hpp"
#include "ballistics/time/TimeConverter.hpp"
namespace Ballistics::Forces {

template <typename TimeConverter, typename FrameConverter,
          typename EphemerisCalculator, typename EarthGravity,
          typename... OtherForces>
class ForceCalculator {

public:
  struct allSatParams : OtherForces::SatParams... {};

private:
  TimeConverter timeConverter_;
  FrameConverter frameConverter_;
  EphemerisCalculator ephemerisCalculator_;
  std::tuple<OtherForces...> otherForces_;
  const EarthGravity &earthGravity_;

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

//    const auto stdToVector3d = [](const std::vector<double> &vector3) {
//      return Vector3d{vector3[0], vector3[1], vector3[2]};
//    };

    const int centerBody = 3;
    const Vector3d moonPositionITRS =
        ephemerisCalculator_.calcCoordinates(10, centerBody, timeTDB);

    const Vector3d jupiterPositionITRS =
        ephemerisCalculator_.calcCoordinates(5, centerBody, timeTDB);

    const Vector3d sunR =
        ephemerisCalculator_.calcCoordinates(11, centerBody, timeTDB);
    const Vector3d sunV =
        ephemerisCalculator_.calcVelocities(11, centerBody, timeTDB);

    const Vector3d sunPositionITRS = sunR;
    const Vector3d sunVelocityITRS = sunV;

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
  auxFunction(const Time::Time<Time::Scale::TT> timeTT,
              const Vector3d &positionGCRS, const Vector3d &velocityGCRS,
              double mass, const allSatParams &allSatParams,
              const InputParams &inputParams,
              std::integer_sequence<unsigned int, Is...>) const noexcept {

    return (std::get<Is>(otherForces_)
                .calcForce(timeTT, positionGCRS, velocityGCRS, mass,
                           allSatParams, inputParams) +
            ...);
  }

public:
  ForceCalculator(const EarthGravity &earthGravity,
                  const TimeConverter &timeConverter,
                  const FrameConverter &frameConverter,
                  const EphemerisCalculator &ephemerisCalculator,
                  const std::tuple<OtherForces...> &otherForces) noexcept
      : timeConverter_(timeConverter), frameConverter_(frameConverter),
        ephemerisCalculator_(ephemerisCalculator), earthGravity_(earthGravity),
        otherForces_(otherForces){};

  [[nodiscard]] Vector3d
  calcForce(const Time::Time<Time::Scale::TT> timeTT,
            const Vector3d &positionGCRS, const Vector3d &velocityGCRS,
            double mass, const allSatParams &allSatParams) const noexcept {
    const Time::Time<Time::Scale::UT1> ut1 = timeConverter_.convertTT_UT1(
        timeTT);
    const Time::Time<Time::Scale::UTC> utc = timeConverter_.convertTT_UTC(
        timeTT);
    const Quaternion<double> quaternionGCRStoITRS =
        frameConverter_.template calcQuaternion<Frame::Frame::GCRS, Frame::Frame::ITRS>(timeTT, ut1, utc);
    const Quaternion<double> quaternionITRStoGCRS =
        quaternionGCRStoITRS.inverse();

    const InputParams inputParams =
        calcInputParam(timeTT, quaternionGCRStoITRS);

    const Vector3d positionITRS =
        quaternionGCRStoITRS._transformVector(positionGCRS);

    const Vector3d earthGravAccelerationITRS =
        earthGravity_.calcForce(positionITRS);

    const Vector3d otherForcesAcceleration = auxFunction(
        timeTT, positionGCRS, velocityGCRS, mass, allSatParams, inputParams,
        std::make_integer_sequence<unsigned int, sizeof...(OtherForces)>{});

    return earthGravAccelerationITRS + otherForcesAcceleration;
  };
};
}; // namespace Ballistics::Forces

#endif // BALLISTICS2023_FORCECALCULATOR_HPP
