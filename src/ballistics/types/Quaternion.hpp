//
// Created by julia on 20.12.23.
//

#ifndef BALLISTICS2023_QUATERNION_HPP
#define BALLISTICS2023_QUATERNION_HPP
#include <Eigen/Geometry>

namespace Ballistics {

using Quaterniond = Eigen::Quaterniond;

template <typename T>
using Quaternion = Eigen::Quaternion<T>;
}

#endif // BALLISTICS2023_QUATERNION_HPP
