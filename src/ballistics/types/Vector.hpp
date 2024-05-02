//
// Created by julia on 19.10.23.
//

#ifndef BALLISTICS2023_VECTOR_HPP
#define BALLISTICS2023_VECTOR_HPP
#include "Eigen/Geometry"
#include "types.hpp"

namespace Ballistics {

    template<typename T>
    using Vector3 = Eigen::Vector3<T>;
    using Vector3d = Vector3<scalar>;
    using Vector6d = Eigen::Matrix<scalar, 6, 1>;  // Вектор размера 6 с типом scalar
}
#endif //BALLISTICS2023_VECTOR_HPP
