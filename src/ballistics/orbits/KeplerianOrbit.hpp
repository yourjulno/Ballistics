//
// Created by julia on 01.02.24.
//

#ifndef BALLISTICS2023_KEPLERIANORBIT_HPP
#define BALLISTICS2023_KEPLERIANORBIT_HPP
#include "ballistics/types/types.hpp"
namespace Ballistics::Orbit{
  struct KeplerianOrbit{
    scalar semiMajor;
    scalar eccentricity;
    scalar inclination;
    scalar periapsisArg; //w
    scalar ascNode;
    scalar trueAnomaly; //nu
  };
}
#endif // BALLISTICS2023_KEPLERIANORBIT_HPP
