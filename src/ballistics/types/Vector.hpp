//
// Created by julia on 19.10.23.
//

#ifndef BALLISTICS2023_VECTOR_HPP
#define BALLISTICS2023_VECTOR_HPP
#include "Eigen/Geometry"

namespace Ballistics {

    template<typename T>
    using Vector3 = Eigen::Vector3<T>;
    using Vector3d = Vector3<double>;
}
#endif //BALLISTICS2023_VECTOR_HPP
