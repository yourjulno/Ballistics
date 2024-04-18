//
// Created by julia on 20.12.23.
//

#ifndef BALLISTICS2023_EPHEMERIS_HPP
#define BALLISTICS2023_EPHEMERIS_HPP
#include <string>
#include "ballistics/time/Time.hpp"
#include "../../../third_party/calceph/calceph.h"
#include "ballistics/types/Vector.hpp"
namespace Ballistics::Ephemeris
{


enum class Body
{
  MercuryBarycenter     = 1,
  VenusBarycenter       = 2,
  Earth                 = 3,
  MarsBarycenter        = 4,
  JupiterBarycenter     = 5,
  SaturnBarycenter      = 6,
  UranusBarycenter      = 7,
  NeptuneBarycenter     = 8,
  PlutoBarycenter       = 9,
  Moon                  = 10,
  Sun                   = 11,
  SolarSystemBarycenter = 12,
  EarthMoonBarycenter   = 13
};


class Ephemeris
{
  scalar AU;
  scalar EMRAT;

public:
  /** Constructor of a Ephemeris
         * @param fileLocation location of the binary file with information about ephemeris
   */
  explicit Ephemeris(const std::string& path)
  {

    const int statusOpen = calceph_sopen(path.c_str());
    const int statusConstant = calceph_sgetconstant("AU", &AU);
    const int statusEMRAT = calceph_sgetconstant("EMRAT", &EMRAT);

  }


  /** Method for calculating the coordinates of the target body relative to the central one
         * at a certain point in time
         * @param jdDate date in jd format (possible scales TCB or TDB only)
         * @param target the body whose coordinates we want to get
         * @param center the body around which the target body rotates
         * @return coordinates of the target body in kilometres
   */
  template<Time::Scale T>
  [[nodiscard]] Vector3d calcCoordinates(const int target,
                                         const int center,
                                         const Time::Time<T> &time)
      const{
    scalar PV[6];

    const auto jdInt = static_cast<scalar>(time.jdInt());
    const auto jdFrac = static_cast<scalar>(time.jdFrac());

    const int status = calceph_scompute(jdInt, jdFrac, target, center, PV);
    const scalar AUe3 = AU * 1e3;
    const scalar AUe3divDay = AUe3 / 86400;
    const auto auToMeters = [&](const scalar distance) { return distance * AUe3; };
    return {auToMeters(PV[0]),
            auToMeters(PV[1]),
            auToMeters(PV[2])};
  }

  /** Method for calculating the velocities of the target body relative to the central one
        * at a certain point in time
        * @param jdDate date in jd format (possible scales TCB or TDB only)
        * @param target the body whose velocities we want to get
        * @param center the body around which the target body rotates
        * @return velocities of the target body in kilometres per second
   */
  template<Time::Scale T>
  [[nodiscard]] Vector3d calcVelocities(const int target,
                                        const int center,
                                        const Time::Time<T> &time)
      const
  {
    scalar posAndVel[6];
    scalar PV[6];

    const auto jdInt = static_cast<scalar>(time.jdInt());
    const auto jdFrac = static_cast<scalar>(time.jdFrac());

    const int status = calceph_scompute(jdInt, jdFrac, target, center, PV);
    const scalar AUe3 = AU * 1e3;
    const scalar AUe3divDay = AUe3 / 86400;

    const auto auPerDayToMetersPerSecond = [&](const scalar speed) { return speed * AUe3divDay; };

   return {auPerDayToMetersPerSecond(PV[3]),
           auPerDayToMetersPerSecond(PV[4]),
           auPerDayToMetersPerSecond(PV[5])};
  }

  [[nodiscard]] scalar calcGravParameter(const int targetIndex) const {

    if (targetIndex >= 1 && targetIndex <= 11) {
      scalar gravParameter;
      char nameconstant[CALCEPH_MAX_CONSTANTNAME];

      const scalar multiplyFactor = (targetIndex == 3) ? EMRAT / (EMRAT + 1)
                                    : (targetIndex == 10) ? 1 / (EMRAT + 1)
                                                          : 1;

      const int target = (targetIndex == 10) ? 3 : (targetIndex == 11) ? 10 : targetIndex;

      calceph_sgetconstantindex(8 + target, nameconstant, &gravParameter);

      const scalar AUe3 = AU * 1e3;
      const scalar AUe3divDaySec = AUe3 / 86400;
      const scalar AUe3divDaySecSqr = AUe3divDaySec * AUe3divDaySec;

      return gravParameter * AUe3 * AUe3divDaySecSqr * multiplyFactor;

    }
  }
};
};

#endif // BALLISTICS2023_EPHEMERIS_HPP
