//
// Created by julia on 16.11.23.
//

#ifndef BALLISTICS2023_INDEPENDENTSHADOW_HPP
#define BALLISTICS2023_INDEPENDENTSHADOW_HPP
#include "ballistics/types/Vector.hpp"
#include "PenumbraShadow.hpp"
namespace Ballistics{

    template <typename EarthShadow, typename MoonShadow>
    class IndependentShadow{
        EarthShadow earthShadow_;
        MoonShadow moonShadow_;

    public:
        IndependentShadow(const EarthShadow& earthShadow, const MoonShadow& moonShadow):
        earthShadow_(earthShadow), moonShadow_(moonShadow){};


        //Earth pos = 0 (GCRS)
        [[nodiscard]] Vector3d  calcFlux(const Vector3d& position,
                           const Vector3d& sunPosition,
                           const Vector3d& moonPosition,
                           const Vector3d& zeroFlux) const{
            const double F = std::min(earthShadow_.calcShadow(position, sunPosition, Vector3d::Zero()),
                                      moonShadow_.calcShadow(position, sunPosition, Vector3d::Zero()));
            return zeroFlux * F;
        }
    };
}
#endif //BALLISTICS2023_INDEPENDENTSHADOW_HPP
