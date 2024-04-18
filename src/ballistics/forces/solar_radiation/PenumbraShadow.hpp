//
// Created by julia on 16.11.23.
//

#ifndef BALLISTICS2023_PENUMBRASHADOW_HPP
#define BALLISTICS2023_PENUMBRASHADOW_HPP

#include "ballistics/types/Vector.hpp"

namespace Ballistics{

    class PenumbraShadow{
        double sunRadius_;
        double bodyRadius_;

    public:
        PenumbraShadow(double sunRadius, double bodyRadius): sunRadius_(sunRadius),
        bodyRadius_(bodyRadius){};

        [[nodiscard]] double calcShadow(const Vector3d& position, const Vector3d& bodyPosition,
                          const Vector3d& sunPosition) const;


    };
}
#endif //BALLISTICS2023_PENUMBRASHADOW_HPP
