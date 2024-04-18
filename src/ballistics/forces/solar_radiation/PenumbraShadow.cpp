//
// Created by julia on 16.11.23.
//
#include "PenumbraShadow.hpp"
namespace Ballistics{

double PenumbraShadow::calcShadow(const Vector3d &position,
                                  const Vector3d &bodyPosition,
                                  const Vector3d& sunPosition) const {

  const double theta_b = std::asin(bodyRadius_ / (position - bodyPosition).norm());
  const double theta_s = std::asin(sunRadius_ / (position - sunPosition).norm());
  const double theta_bs = std::acos((bodyPosition - position).dot(sunPosition - position) /
                                    ((bodyPosition - position).norm() * (sunPosition - position).norm()));
  if (theta_bs >= theta_b + theta_s){ // Umbra
    return 0;

  } else if (theta_bs <= theta_b - theta_s){ // On Light
    return 1;

  } else {
    const double theta_s2 = theta_s * theta_s;
    const double theta_bs2 = theta_bs * theta_bs;
    const double theta_b2 = theta_b * theta_b;
    const double cosCAF = (theta_s2 + theta_bs2 - theta_b2) /
                          (2 * theta_s * theta_bs);
    const double cosCBD = (theta_b2 + theta_bs2 - theta_s2) /
                          (2 * theta_b * theta_bs);
    const double CAF = std::acos(cosCAF);
    const double CBD = std::acos(cosCBD);
    const double squareAEC = (theta_s2 * std::sin(CAF) * cosCAF) / 2;
    const double squareBEC = (theta_b2 * std::sin(CBD) * cosCBD) / 2;
    const double squareCBD = CBD * theta_b2 / 2;
    const double squareAFC = CAF * theta_s2 / 2;
    const double resultSquare = 2 * (squareAFC - squareAEC + squareCBD - squareBEC);
    return 1 - resultSquare / (M_PI * theta_s2);

  }



}
}