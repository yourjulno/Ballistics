//
// Created by julia on 20.12.23.
//

#ifndef BALLISTICS2023_DENSITY_HPP
#define BALLISTICS2023_DENSITY_HPP
#include "ballistics/types/types.hpp"
#include <vector>
namespace Ballistics{
  class Density{
    std::vector<scalar> height_;
    std::vector<scalar> density_;

  public:
    Density(const std::vector<scalar>& height, const std::vector<scalar>& density):
          height_(height), density_(density){};

    [[nodiscard]] double calcDensity(const scalar height) const{
      for (indexType i = 0; i < height_.size() - 1; ++i) {
        
        if (height >= height_[i] && height < height_[i + 1]) {
          const scalar y = density_[i + 1] - density_[i];
          const scalar x = height_[i + 1] - height_[i];

          return density_[i] + y / x * (height - height_[i]);
        }
      }

      if (height == height_[height_.size() - 1]) {
        return density_[density_.size() - 1];
      };

    }
  };
}
#endif // BALLISTICS2023_DENSITY_HPP
