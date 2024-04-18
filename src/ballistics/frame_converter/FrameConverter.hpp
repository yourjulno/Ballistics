//
// Created by julia on 19.12.2023.
//

#ifndef BALLISTICS_CONVERTER_H
#define BALLISTICS_CONVERTER_H

#include "ballistics/time/Time.hpp"
#include "sofa.h"
#include "../third_party/fast-cpp-csv-parser/csv.h"
#include "ballistics/types/Quaternion.hpp"
#include "ballistics/types/types.hpp"

namespace Ballistics::Frame {

enum Frame {
  ITRS = 0,
  GCRS = 1
};

struct Pole {
  scalar xp;
  scalar yp;
};


class FrameConverter {
private:
  std::vector<int> mjd_;
  std::vector<scalar> x_arcsec_;
  std::vector<scalar> y_arcsec_;
public:

  inline FrameConverter(const std::vector<int> &mjd, const std::vector<scalar> &x_arcsec,
                        const std::vector<scalar> &y_arcsec) : mjd_(mjd), x_arcsec_(x_arcsec),
                                                               y_arcsec_(y_arcsec) {}

  [[nodiscard]] Pole interpolate(const scalar mjd) const noexcept {
    scalar xp = 0.;
    scalar yp = 0.;
    for (int i = 0; i < mjd_.size(); ++i) {
      if (mjd_[i] <= mjd && mjd_[i + 1] > mjd) {
        xp = ((mjd - mjd_[i]) / (mjd_[i + 1] - mjd_[i])) * (x_arcsec_[i + 1] - x_arcsec_[i]) + x_arcsec_[i];
        yp = ((mjd - mjd_[i]) / (mjd_[i + 1] - mjd_[i])) * (y_arcsec_[i + 1] - y_arcsec_[i]) + y_arcsec_[i];
      }
    }
    return {xp, yp};
  }

  [[nodiscard]] Pole getPole(const scalar mjd) const noexcept {
    return interpolate(mjd);
  }

  template<Frame From, Frame To>
  [[nodiscard]] Quaterniond calcQuaternion(const Time::Time<Time::Scale::TT> &TT,
                                           const Time::Time<Time::Scale::UT1> &UT1,
                                           const Time::Time<Time::Scale::UTC> &UTC) const noexcept;
};

template<>
inline Quaterniond FrameConverter::calcQuaternion<Frame::GCRS, Frame::ITRS>(const Time::Time<Time::Scale::TT> &TT,
                                                                            const Time::Time<Time::Scale::UT1> &UT1,
                                                                            const Time::Time<Time::Scale::UTC> &UTC)
    const noexcept {
  //CIP and CIO, IAU 2000A
  scalar x, y, s;
  iauXys06a(TT.jdInt(), TT.jdFrac(), &x, &y, &s);
  //GCRS to CIRS matrix.
  scalar rc2i[3][3];
  iauC2ixys(x, y, s, rc2i);


  //Earth rotation angle.
  const scalar era = iauEra00(UT1.jdInt(), UT1.jdFrac());
  //Form celestial-terrestrial matrix (no polar motion yet).
  scalar rc2ti[3][3];
  iauCr(rc2i, rc2ti);
  iauRz(era, rc2ti);

  const scalar sp = iauSp00(TT.jdInt(), TT.jdFrac());
  scalar rpom[3][3];
  Pole pole = getPole(UTC.mjd());
  //Polar motion matrix (TIRS->ITRS, IERS 2003).
  iauPom00(pole.xp, pole.yp, sp, rpom);
  scalar rc2it[3][3];
  //Form celestial-terrestrial matrix (including polar motion).
  iauRxr(rpom, rc2ti, rc2it);

  Eigen::Matrix<scalar, 3, 3> M{{rc2it[0][0], rc2it[0][1], rc2it[0][2]},
                                {rc2it[1][0], rc2it[1][1], rc2it[1][2]},
                                {rc2it[2][0], rc2it[2][1], rc2it[2][2]}};

  return Quaterniond(M);
}

template<>
inline Eigen::Quaterniond
FrameConverter::calcQuaternion<Frame::ITRS, Frame::GCRS>(const Time::Time<Time::Scale::TT> &TT,
                                                         const Time::Time<Time::Scale::UT1> &UT1,
                                                         const Time::Time<Time::Scale::UTC> &UTC)
    const noexcept {
  return calcQuaternion<Frame::GCRS, Frame::ITRS>(TT, UT1, UTC).conjugate();
}

}
#endif //BALLISTICS_CONVERTER_H
