//
// Created by julia on 18.12.23.
//

#ifndef BALLISTICS2023_DUTCONTAINER_HPP
#define BALLISTICS2023_DUTCONTAINER_HPP
#include "Time.hpp"
#include "ballistics/types/types.hpp"
#include <vector>
namespace Ballistics::Time {

class DutContainer {
  std::vector<scalar> dut_;

  int start_;

public:
  explicit DutContainer(const std::vector<scalar> &dut, const int start)
      : dut_(dut), start_(start){};
  [[nodiscard]] double dut(const Time<Scale::UTC> utc)
                           const {const scalar mjd = utc.mjd();
  for (int i = 0; i < dut_.size() - 1; ++i) {

    if (mjd >= dut_[i] && mjd < dut_[i + 1]) {
      const scalar y = dut_[i + 1] - dut_[i];
      const scalar leapAdd = y > static_cast<scalar>(0.6)? -static_cast<scalar>(1)
          : y < static_cast<scalar>(-0.6) ? static_cast<scalar>(1)
                                                      : static_cast<scalar>(0);

      const scalar y_ = dut_[i + 1] - dut_[i] + leapAdd;
      const scalar x = 1;

      return dut_[i] + y_ / x * (mjd - start_);
    }
  }

  if (mjd == dut_[dut_.size() - 1]) {
    return dut_[dut_.size() - 1];
  };
}

};

};

#endif // BALLISTICS2023_DUTCONTAINER_HPP
