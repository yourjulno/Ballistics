//
// Created by julia on 16.11.23.
//

#ifndef BALLISTICS2023_TIME_HPP
#define BALLISTICS2023_TIME_HPP
#include "ballistics/types/types.hpp"
#include <string>
#include <array>
#include "../third_party/sofa/sofa.h"

namespace Ballistics::Time{
    // 7 шкал времён
    enum class Scale {
        UT1 = 0,
        UTC,
        TAI,
        TT,
        TCG,
        TCB,
        TDB
    };

    inline constexpr std::string_view UT1 = "ut1";
    inline constexpr std::string_view UTC = "UTC";
    inline constexpr std::string_view TAI = "TAI";
    inline constexpr std::string_view TT = "tt";
    inline constexpr std::string_view TCG = "tcg";
    inline constexpr std::string_view TCB = "tcb";
    inline constexpr std::string_view TDB = "tdb";
    inline constexpr std::array<std::string_view, 7> SOFA_SCALES = {UT1, UTC, TAI, TT, TCG, TCB, TDB};
template <Scale T>
class Time {
  double jdInt_;
  double jdFrac_;

public:

  static constexpr double JD_SUB_MJD = 2400000.5;
  static constexpr double SECS_PER_DAY = 86400;

  constexpr Time(): jdInt_(0), jdFrac_(0) {};
  constexpr Time(double jdInt, double jdFrac) noexcept: jdInt_(jdInt), jdFrac_(jdFrac){};

  [[nodiscard]] inline double jdInt() const noexcept { return jdInt_; }
  [[nodiscard]] inline double jdFrac() const noexcept { return jdFrac_; }
  [[nodiscard]] inline double jd() const noexcept { return jdInt_ + jdFrac_; }
  [[nodiscard]] inline double mjd() const noexcept { return jdInt_ - JD_SUB_MJD + jdFrac_; }

  [[nodiscard]] constexpr static Time fromJD(double jd) noexcept{
    return Time<T>(jd, static_cast<double >(0));
  }
  [[nodiscard]] constexpr static Time fromMJD(double mjd) noexcept{
    return Time<T>(JD_SUB_MJD, mjd);
  }
  [[nodiscard]] static Time fromCalendar(int year, int month, int day, int hour, int minute, double seconds){

    scalar jdInt;
    scalar jdFrac;
    const int status = iauDtf2d(static_cast<std::string>(SOFA_SCALES[static_cast<int>(T)]).c_str(),
                                year,month, day, hour, minute,seconds, &jdInt, &jdFrac);

      return {jdInt, jdFrac};
  }

  [[nodiscard]] constexpr double operator-(const Time& rhs) const noexcept;
  [[nodiscard]] constexpr Time operator-(double seconds) const noexcept;
  [[nodiscard]] constexpr Time operator+(double seconds) const noexcept;

  [[nodiscard]] auto operator<=>(const Time& timeOther) const noexcept = default;
};


template <Scale scale>
constexpr Time<scale> Time<scale>::operator-(const double seconds) const noexcept {
  return Time<scale>(jdInt_, jdFrac_-(seconds/SECS_PER_DAY));

}

template <Scale scale>
constexpr Time<scale> Time<scale>::operator+(const double seconds) const noexcept {
  return Time<scale>(jdInt_, jdFrac_+(seconds/SECS_PER_DAY));
}

template <Scale scale>
constexpr double Time<scale>::operator-(const Time<scale>& rhs) const noexcept {
  return ((jdInt_ - rhs.jdInt()) + (jdFrac_ - rhs.jdFrac()))*SECS_PER_DAY;
}


}
#endif //BALLISTICS2023_TIME_HPP
