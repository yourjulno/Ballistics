//
// Created by julia on 08.02.24.
//

#ifndef BALLISTICS2023_CENTRALGRAVITY_HPP
#define BALLISTICS2023_CENTRALGRAVITY_HPP
#include "ballistics/types/Vector.hpp"
#include "ballistics/time/Time.hpp"
namespace Ballistics::Rhs{


class CentralGravity{
static constexpr scalar gravityParam = 9;

public:
  using ArgumentType = Time::Time<Time::Scale::TT>;
  using InegrationVector = Eigen::Vector<scalar,6>;

  struct IntegrationState{
    Time::Time<Time::Scale::TT> argument;
    InegrationVector  vector;
  };

  struct State{
    Vector3d position;
    Vector3d velocity;
  };

  [[nodiscard]] static inline InegrationVector calc(const IntegrationState& state) {
    InegrationVector result;
    result.segment<3>(0) = state.vector.segment<3>(0);
    const Vector3d pos = state.vector.segment<3>(0);
    const scalar radiusSqr = pos.squaredNorm();
    const Vector3d acceleration = - gravityParam / radiusSqr / std::sqrt(radiusSqr) * pos;
    result.segment<3>(3) = acceleration;
    return result;
  }


};
}
#endif // BALLISTICS2023_CENTRALGRAVITY_HPP
