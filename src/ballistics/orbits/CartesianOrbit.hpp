//
// Created by julia on 01.02.24.
//

#ifndef BALLISTICS2023_ORBITS_HPP
#define BALLISTICS2023_ORBITS_HPP
#include "ballistics/types/Vector.hpp"
namespace Ballistics::Orbit{

struct CartesianOrbit{
  Vector3d position;
  Vector3d velocity;
};

}
#endif // BALLISTICS2023_ORBITS_HPP
