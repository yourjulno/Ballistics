//
// Created by julia on 01.02.24.
//

#ifndef BALLISTICS2023_ORBITCONVERTER_HPP
#define BALLISTICS2023_ORBITCONVERTER_HPP
#include "ballistics/orbits/CartesianOrbit.hpp"
#include "ballistics/orbits/KeplerianOrbit.hpp"
#include "ballistics/types/types.hpp"
namespace Ballistics::Orbit{

CartesianOrbit convertKeplerianToCartesian(const KeplerianOrbit& orbit, scalar gravityParameter) {
  /** formulas from GMATMathSpec 3.1.3, p. 50-51*/

  const scalar semimajor = orbit.semiMajor;
  const scalar eccentricity = orbit.eccentricity;
  const scalar inclination = orbit.inclination;
  const scalar argumentPeriapsis = orbit.periapsisArg;
  const scalar ascendingNode = orbit.ascNode;
  const scalar trueAnomaly = orbit.trueAnomaly;

  const scalar focalParameter = semimajor * (1 - eccentricity * eccentricity);
  const scalar distance = focalParameter / (1 + eccentricity * std::cos(trueAnomaly));

  const scalar cosArgumentPeriapsisAndTrueAnomaly = std::cos(argumentPeriapsis + trueAnomaly);
  const scalar sinArgumentPeriapsisAndTrueAnomal = std::sin(argumentPeriapsis + trueAnomaly);

  const scalar cosInclination = std::cos(inclination);
  const scalar sinInclination = std::sin(inclination);

  const scalar cosAscendingNode = std::cos(ascendingNode);
  const scalar sinAscendingNode = std::sin(ascendingNode);

  const scalar cosTrueAnomaly = std::cos(trueAnomaly);
  const scalar sinTrueAnomaly = std::sin(trueAnomaly);

  const Vector3d position = Vector3d((cosArgumentPeriapsisAndTrueAnomaly * cosAscendingNode -
                                      cosInclination * sinArgumentPeriapsisAndTrueAnomal * sinAscendingNode),
                                     (cosArgumentPeriapsisAndTrueAnomaly * sinAscendingNode +
                                      cosInclination * sinArgumentPeriapsisAndTrueAnomal * cosAscendingNode),
                                     (sinArgumentPeriapsisAndTrueAnomal * sinInclination)) *
                            distance;

  const scalar sqrtGravParamByFocalParameter = std::sqrt(gravityParameter / focalParameter);
  const scalar cosTrueAnomalyPlusEcc = cosTrueAnomaly + eccentricity;
  const scalar cosArgumentPeriapsis = std::cos(argumentPeriapsis);
  const scalar sinArgumentPeriapsis = std::sin(argumentPeriapsis);

  const Vector3d velocity(
      sqrtGravParamByFocalParameter * cosTrueAnomalyPlusEcc *
              (-sinArgumentPeriapsis * cosAscendingNode - cosInclination * sinAscendingNode * cosArgumentPeriapsis) -
          sqrtGravParamByFocalParameter * sinTrueAnomaly *
              (cosArgumentPeriapsis * cosAscendingNode - cosInclination * sinAscendingNode * sinArgumentPeriapsis),
      sqrtGravParamByFocalParameter * cosTrueAnomalyPlusEcc *
              (-sinArgumentPeriapsis * sinAscendingNode + cosInclination * cosAscendingNode * cosArgumentPeriapsis) -
          sqrtGravParamByFocalParameter * sinTrueAnomaly *
              (cosArgumentPeriapsis * sinAscendingNode + cosInclination * cosAscendingNode * sinArgumentPeriapsis),
      sqrtGravParamByFocalParameter * (cosTrueAnomalyPlusEcc * sinInclination * cosArgumentPeriapsis -
                                       sinTrueAnomaly * sinInclination * sinArgumentPeriapsis));
  return CartesianOrbit{position, velocity};
}

  inline scalar normalize(scalar number) {
    const scalar n = std::fmod(number, 2 * M_PI);
    return n >= 0 ? n : n + 2 * M_PI;
  }


  inline KeplerianOrbit convertCartesianToKeplerian(const CartesianOrbit& orbit, scalar gravityParameter) {

    const Vector3d& position = orbit.position;
    const Vector3d& velocity = orbit.velocity;

    const scalar velocityNormSqr = velocity.squaredNorm();
    const scalar positionNorm = position.norm();

    const Vector3d orbitalMomentum = position.cross(velocity);  // орбитальный момент
    const Vector3d orbitalMomentumNormalized = orbitalMomentum.normalized();
    const Vector3d nodeVectorExp = Vector3d::UnitZ().cross(orbitalMomentum);  // вектор линии узлов
    const scalar nodeVectorExpNorm = nodeVectorExp.norm();
    const Vector3d e1 = nodeVectorExpNorm > 0 ? Vector3d(nodeVectorExp / nodeVectorExpNorm) : Vector3d::UnitX();
    const Vector3d e2 = orbitalMomentumNormalized.cross(e1);

    const Vector3d eccentricityExp =
        ((velocityNormSqr - gravityParameter / positionNorm) * position - position.dot(velocity) * velocity) /
        gravityParameter;  // эксцентриситет(вектор)

    const scalar eccentricityExpNorm = eccentricityExp.norm();
    const Vector3d g1 = eccentricityExpNorm > 0 ? Vector3d(eccentricityExp / eccentricityExpNorm) : e1;
    const Vector3d g2 = orbitalMomentumNormalized.cross(g1);
    const scalar energyIntegral = velocityNormSqr / 2 - gravityParameter / positionNorm;  // интеграл энергии
    const scalar semiMajorAxis = -gravityParameter / (2 * energyIntegral);  // большая полуось

    const Vector3d vectorForInclination = e1.cross(Vector3d::UnitZ());  // e1.cross(Vector3d::UnitZ())
    const scalar inclinationX = orbitalMomentum.z();
    const scalar inclinationY = orbitalMomentum.dot(vectorForInclination);
    const scalar inclination = std::atan2(inclinationY, inclinationX);
    const scalar ascNode = std::atan2(e1.y(), e1.x());
    const scalar ex = g1.dot(e1);
    const scalar ey = g1.dot(e2);
    const scalar argPerig = std::atan2(ey, ex);
    const scalar posX = position.dot(g1);
    const scalar posY = position.dot(g2);
    const scalar anomaly = std::atan2(posY, posX);

    return KeplerianOrbit{semiMajorAxis,       eccentricityExpNorm, inclination,
                                     normalize(argPerig), normalize(ascNode),  normalize(anomaly)};
  }


}
#endif // BALLISTICS2023_ORBITCONVERTER_HPP
