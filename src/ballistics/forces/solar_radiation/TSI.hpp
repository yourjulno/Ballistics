//
// Created by julia on 16.11.23.
//

#ifndef BALLISTICS2023_TSI_HPP
#define BALLISTICS2023_TSI_HPP
#include "ballistics/time/Time.hpp"
namespace Ballistics{
    class TSIConstant{
        static constexpr double TSI_ = 1366;
      public:
        [[nodiscard]] consteval double static TSI(const Time::Time<Time::Scale::UTC> & /*tt*/)
            noexcept { return TSI_; }
    };
}
#endif //BALLISTICS2023_TSI_HPP
