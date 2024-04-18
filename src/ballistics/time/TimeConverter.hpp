//
// Created by julia on 18.12.23.
//

#ifndef BALLISTICS2023_TIMECONVERTER_HPP
#define BALLISTICS2023_TIMECONVERTER_HPP
#include "Time.hpp"
#include "ballistics/types/types.hpp"
namespace Ballistics::Time{
template<typename DutContainer>
class TimeConverter{

  DutContainer dutContainer_;

  using UT1 = Time<Scale::UT1>;
  using UTC = Time<Scale::UTC>;
  using TAI = Time<Scale::TAI>;
  using TT = Time<Scale::TT>;
  using TCG = Time<Scale::TCG>;
  using TCB = Time<Scale::TCB>;
  using TDB = Time<Scale::TDB>;

  [[nodiscard]] double dtr(const Time<Scale::TT> &tt) const noexcept{
    return static_cast<double>(iauDtdb(static_cast<double >(tt.jdInt()), static_cast<double>(tt.jdFrac()), 0,
                                       0, 0, 0));

  }
  static constexpr bool AlwaysFalse = false;
public:

  explicit TimeConverter(const DutContainer& dut_Container): dutContainer_(dut_Container) {};

  template <Scale To, Scale From>
  [[nodiscard]] Time<To> convert(const Time<From>& time) const;

  [[nodiscard]] UT1 convertTAI_UT1(const TAI& tai ) const noexcept;
  [[nodiscard]] UTC convertTAI_UTC(const TAI& tai ) const noexcept;
  [[nodiscard]]  TT  convertTAI_TT(const TAI& tai ) const noexcept;
  [[nodiscard]] TCG convertTAI_TCG(const TAI& tai ) const noexcept;
  [[nodiscard]] TCB convertTAI_TCB(const TAI& tai ) const noexcept;
  [[nodiscard]] TDB convertTAI_TDB(const TAI& tai ) const noexcept;

  [[nodiscard]] UT1 convertUTC_UT1(const UTC& utc ) const noexcept;
  [[nodiscard]] TAI convertUTC_TAI(const UTC& utc ) const noexcept;
  [[nodiscard]]  TT convertUTC_TT(const UTC& utc ) const noexcept;
  [[nodiscard]] TCG convertUTC_TCG(const UTC& utc ) const noexcept;
  [[nodiscard]] TCB convertUTC_TCB(const UTC& utc ) const noexcept;
  [[nodiscard]] TDB convertUTC_TDB(const UTC& utc ) const noexcept;

  [[nodiscard]] UTC convertUT1_UTC(const UT1& ut1 ) const noexcept;
  [[nodiscard]] TAI convertUT1_TAI(const UT1& ut1 ) const noexcept;
  [[nodiscard]]  TT  convertUT1_TT(const UT1& ut1 ) const noexcept;
  [[nodiscard]] TCG convertUT1_TCG(const UT1& ut1 ) const noexcept;
  [[nodiscard]] TCB convertUT1_TCB(const UT1& ut1 ) const noexcept;
  [[nodiscard]] TDB convertUT1_TDB(const UT1& ut1 ) const noexcept;

  [[nodiscard]] UT1 convertTT_UT1(const TT& tt ) const noexcept;
  [[nodiscard]] UTC convertTT_UTC(const TT& tt ) const noexcept;
  [[nodiscard]] TAI convertTT_TAI(const TT& tt ) const noexcept;
  [[nodiscard]] TCG convertTT_TCG(const TT& tt ) const noexcept;
  [[nodiscard]] TCB convertTT_TCB(const TT& tt ) const noexcept;
  [[nodiscard]] TDB convertTT_TDB(const TT& tt ) const noexcept;

  [[nodiscard]] UT1 convertTCG_UT1(const TCG& tcg ) const noexcept;
  [[nodiscard]] UTC convertTCG_UTC(const TCG& tcg ) const noexcept;
  [[nodiscard]] TAI convertTCG_TAI(const TCG& tcg ) const noexcept;
  [[nodiscard]]  TT  convertTCG_TT(const TCG& tcg ) const noexcept;
  [[nodiscard]] TCB convertTCG_TCB(const TCG& tcg ) const noexcept;
  [[nodiscard]] TDB convertTCG_TDB(const TCG& tcg ) const noexcept;

  [[nodiscard]] UT1 convertTCB_UT1(const TCB& tcb ) const noexcept;
  [[nodiscard]] UTC convertTCB_UTC(const TCB& tcb ) const noexcept;
  [[nodiscard]] TAI convertTCB_TAI(const TCB& tcb ) const noexcept;
  [[nodiscard]]  TT  convertTCB_TT(const TCB& tcb ) const noexcept;
  [[nodiscard]] TCG convertTCB_TCG(const TCB& tcb ) const noexcept;
  [[nodiscard]] TDB convertTCB_TDB(const TCB& tcb ) const noexcept;

  [[nodiscard]] UT1 convertTDB_UT1(const TDB& tdb ) const noexcept;
  [[nodiscard]] UTC convertTDB_UTC(const TDB& tdb ) const noexcept;
  [[nodiscard]] TAI convertTDB_TAI(const TDB& tdb ) const noexcept;
  [[nodiscard]]  TT  convertTDB_TT(const TDB& tdb ) const noexcept;
  [[nodiscard]] TCG convertTDB_TCG(const TDB& tdb ) const noexcept;
  [[nodiscard]] TCB convertTDB_TCB(const TDB& tdb ) const noexcept;

};


////TAI////
template<typename DutContainer>
Time<Scale::UTC> TimeConverter<DutContainer>::convertTAI_UTC(const Time<Scale::TAI> &tai) const noexcept{

  scalar jdIntTAI, jdFracTAI;
  const int status = iauTaiutc(static_cast<double>(tai.jdInt()), static_cast<double>(tai.jdFrac()), &jdIntTAI, &jdFracTAI);

  return {jdIntTAI, jdFracTAI};

}

template<typename DutContainer>
Time<Scale::UT1> TimeConverter<DutContainer>::convertTAI_UT1(const Time<Scale::TAI> &tai) const noexcept{

  scalar jdIntTAI = tai.jdInt();
  scalar jdFracTAI = tai.jdFrac();

  const int status = iauTaiut1(static_cast<double>(tai.jdInt()), dutContainer_.dut(Time<Scale::TAI>(jdIntTAI, jdFracTAI)),
                               static_cast<double>(tai.jdFrac()),
                               &jdIntTAI, &jdFracTAI);


  return {jdIntTAI, jdFracTAI};

}


template<typename DutContainer>
Time<Scale::TT>
TimeConverter<DutContainer>::convertTAI_TT(const Time<Scale::TAI> &tai) const noexcept{

  scalar jdIntTT, jdFracTT;

  const int status = iauTaitt(tai.jdInt(), tai.jdFrac(), &jdIntTT, &jdFracTT);

  return {jdIntTT, jdFracTT};

}


template<typename DutContainer>
Time<Scale::TCG>
TimeConverter<DutContainer>::convertTAI_TCG(const Time<Scale::TAI> &tai) const noexcept{

  const Time<Scale::TT> tt = convertTAI_TT(tai);

  return convertTT_TCG(tt);
}


template<typename DutContainer>
Time<Scale::TCB>
TimeConverter<DutContainer>::convertTAI_TCB(const Time<Scale::TAI> &tai) const noexcept{

  const Time<Scale::TT> tt = convertTAI_TT(tai);

  return convertTT_TCB(tt);
}


template<typename DutContainer>
Time<Scale::TDB>
TimeConverter<DutContainer>::convertTAI_TDB(const Time<Scale::TAI> &tai) const noexcept{

  const Time<Scale::TT> tt = convertTAI_TT(tai);

  return convertTT_TDB(tt);
}





/////UTC////
template<typename DutContainer>
Time<Scale::TAI>
TimeConverter<DutContainer>::convertUTC_TAI(const Time<Scale::UTC> &utc) const noexcept{

  scalar jdIntTAI, jdFracTAI;

  const int status = iauUtctai(static_cast<double>(utc.jdInt()), static_cast<double>(utc.jdFrac()),
                               &jdIntTAI, &jdFracTAI);

  return {jdIntTAI, jdFracTAI};
}


template<typename DutContainer>
Time<Scale::UT1>
TimeConverter<DutContainer>::convertUTC_UT1(const Time<Scale::UTC> &utc) const noexcept{

  scalar jdIntUT1, jdFracUT1;

  const int status = iauUtcut1(static_cast<double>(utc.jdInt()),
                               static_cast<double>(utc.jdFrac()), dutContainer_.dut(utc), &jdIntUT1, &jdFracUT1);

  return {jdIntUT1, jdFracUT1};

}


template<typename DutContainer>
Time<Scale::TT>
TimeConverter<DutContainer>::convertUTC_TT(const Time<Scale::UTC> &utc) const noexcept{

  const Time<Scale::TAI> tai = convertUTC_TAI(utc);

  return convertTAI_TT(tai);
}


template<typename DutContainer>
Time<Scale::TCG>
TimeConverter<DutContainer>::convertUTC_TCG(const Time<Scale::UTC> &utc) const noexcept{

  const Time<Scale::TT> tt = convertUTC_TT(utc);

  return convertTT_TCG(tt);
}


template<typename DutContainer>
Time<Scale::TCB>
TimeConverter<DutContainer>::convertUTC_TCB(const Time<Scale::UTC> &utc) const noexcept{

  const Time<Scale::TDB> tdb = convertUTC_TDB(utc);

  return convertTDB_TCB(tdb);
}


template<typename DutContainer>
Time<Scale::TDB>
TimeConverter<DutContainer>::convertUTC_TDB(const Time<Scale::UTC> &utc) const noexcept{

  const Time<Scale::TT> tt = convertUTC_TT(utc);

  return convertTT_TDB(tt);
}





////UT1////
template<typename DutContainer>
Time<Scale::UTC>
TimeConverter<DutContainer>::convertUT1_UTC(const Time<Scale::UT1> &ut1) const noexcept{

  double jdIntUTC = ut1.jdInt(), jdFracUTC = ut1.jdFrac();
  for (indexType i = 0; i < 3; ++i) {

    const int status = iauUt1utc(static_cast<double>(ut1.jdInt()), static_cast<double>(ut1.jdFrac()),
                                 static_cast<double>(dutContainer_.dut(Time<Scale::UTC>(jdIntUTC, jdFracUTC))),
                                 &jdIntUTC,
                                 &jdFracUTC);


  }

  return {static_cast<scalar>(jdIntUTC), static_cast<scalar>(jdFracUTC)};
}



template<typename DutContainer>
Time<Scale::TAI>
TimeConverter<DutContainer>::convertUT1_TAI(const Time<Scale::UT1> &ut1) const noexcept{

  const Time<Scale::UTC> utc = convertUT1_UTC(ut1);

  return convertUTC_TAI(utc);
}


template<typename DutContainer>
Time<Scale::TT>
TimeConverter<DutContainer>::convertUT1_TT(const Time<Scale::UT1> &ut1) const noexcept{

  const Time<Scale::TAI> tai = convertUT1_TAI(ut1);

  return convertTAI_TT(tai);
}


template<typename DutContainer>
Time<Scale::TCG>
TimeConverter<DutContainer>::convertUT1_TCG(const Time<Scale::UT1> &ut1) const noexcept{

  const Time<Scale::TT> tt = convertUT1_TT(ut1);

  return convertTT_TCG(tt);
}


template<typename DutContainer>
Time<Scale::TDB>
TimeConverter<DutContainer>::convertUT1_TDB(const Time<Scale::UT1> &ut1) const noexcept{

  const Time<Scale::TT> tt = convertUT1_TT(ut1);

  return convertTT_TDB(tt);
}


template<typename DutContainer>
Time<Scale::TCB>
TimeConverter<DutContainer>::convertUT1_TCB(const Time<Scale::UT1> &ut1) const noexcept{

  const Time<Scale::TDB> tdb = convertUT1_TDB(ut1);

  return convertTDB_TCB(tdb);
}





////TT////
template<typename DutContainer>
Time<Scale::UT1>
TimeConverter<DutContainer>::convertTT_UT1(const Time<Scale::TT> &tt) const noexcept{

  const Time<Scale::UTC> utc = convertTT_UTC(tt);

  return convertUTC_UT1(utc);
}


template<typename DutContainer>
Time<Scale::TAI>
TimeConverter<DutContainer>::convertTT_TAI(const Time<Scale::TT> &tt) const noexcept{

  scalar jdIntTAI, jdFracTAI;

  const int status = iauTttai(tt.jdInt(), tt.jdFrac(), &jdIntTAI, &jdFracTAI);

  return {jdIntTAI, jdFracTAI};

}


template<typename DutContainer>
Time<Scale::TCG>
TimeConverter<DutContainer>::convertTT_TCG(const Time<Scale::TT> &tt) const noexcept{

  scalar jdIntTCG, jdFracTCG;


  const int status = iauTttcg(tt.jdInt(), tt.jdFrac(), &jdIntTCG, &jdFracTCG);

  return {jdIntTCG, jdFracTCG};
}


template<typename DutContainer>
Time<Scale::TCB>
TimeConverter<DutContainer>::convertTT_TCB(const Time<Scale::TT> &tt) const noexcept{

  const Time<Scale::TDB> tdb = convertTT_TDB(tt);
  return convertTDB_TCB(tdb);
}


template<typename DutContainer>
Time<Scale::TDB>
TimeConverter<DutContainer>::convertTT_TDB(const Time<Scale::TT> &tt) const noexcept{

  scalar jdIntTDB, jdFracTDB;
  const int status = iauTttdb(tt.jdInt(), tt.jdFrac(), dtr(tt), &jdIntTDB, &jdFracTDB);

  return {jdIntTDB, jdFracTDB};
}

template<typename DutContainer>
Time<Scale::UTC>
TimeConverter<DutContainer>::convertTT_UTC(const Time<Scale::TT> &tt) const noexcept{

  const Time<Scale::TAI> tai = convertTT_TAI(tt);

  return convertTAI_UTC(tai);
}




////TCG////

template<typename DutContainer>
Time<Scale::UTC>
TimeConverter<DutContainer>::convertTCG_UTC(const Time<Scale::TCG> &tcg) const noexcept{

  const Time<Scale::TT> tt = convertTCG_TT(tcg);

  return convertTT_UTC(tt);
}


template<typename DutContainer>
Time<Scale::UT1>
TimeConverter<DutContainer>::convertTCG_UT1(const Time<Scale::TCG> &tcg) const noexcept{

  const Time<Scale::TT> tt = convertTCG_TT(tcg);

  return convertTT_UT1(tt);
}


template<typename DutContainer>
Time<Scale::TAI>
TimeConverter<DutContainer>::convertTCG_TAI(const Time<Scale::TCG> &tcg) const noexcept{

  const Time<Scale::TT> tt = convertTCG_TT(tcg);

  return convertTT_TAI(tt);
}


template<typename DutContainer>
Time<Scale::TT>
TimeConverter<DutContainer>::convertTCG_TT(const Time<Scale::TCG> &tcg) const noexcept{

  scalar jdIntTT, jdFracTT;

  const int status = iauTcgtt(tcg.jdInt(), tcg.jdFrac(), &jdIntTT, &jdFracTT);

  return {jdIntTT, jdFracTT};
}


template<typename DutContainer>
Time<Scale::TCB>
TimeConverter<DutContainer>::convertTCG_TCB(const Time<Scale::TCG> &tcg) const noexcept{

  const Time<Scale::TT> tt = convertTCG_TT(tcg);

  return convertTT_TCB(tt);
}


template<typename DutContainer>
Time<Scale::TDB>
TimeConverter<DutContainer>::convertTCG_TDB(const Time<Scale::TCG> &tcg) const noexcept{

  const Time<Scale::TT> tt = convertTCG_TT(tcg);

  return convertTT_TDB(tt);
}


////TCB////
template<typename DutContainer>
Time<Scale::UTC>
TimeConverter<DutContainer>::convertTCB_UTC(const Time<Scale::TCB> &tcb) const noexcept{

  const Time<Scale::TDB> tdb = convertTCB_TDB(tcb);

  return convertTDB_UTC(tdb);
}


template<typename DutContainer>
Time<Scale::UT1>
TimeConverter<DutContainer>::convertTCB_UT1(const Time<Scale::TCB> &tcb) const noexcept{

  const Time<Scale::TDB> tdb = convertTCB_TDB(tcb);

  return convertTDB_UT1(tdb);
}


template<typename DutContainer>
Time<Scale::TAI>
TimeConverter<DutContainer>::convertTCB_TAI(const Time<Scale::TCB> &tcb) const noexcept{

  const Time<Scale::TDB> tdb = convertTCB_TDB(tcb);

  return convertTDB_TAI(tdb);
}


template<typename DutContainer>
Time<Scale::TT>
TimeConverter<DutContainer>::convertTCB_TT(const Time<Scale::TCB> &tcb) const noexcept{

  const Time<Scale::TDB> tdb = convertTCB_TDB(tcb);

  return convertTDB_TT(tdb);
}


template<typename DutContainer>
Time<Scale::TCG>
TimeConverter<DutContainer>::convertTCB_TCG(const Time<Scale::TCB> &tcb) const noexcept{

  const Time<Scale::TDB> tdb = convertTCB_TDB(tcb);

  return convertTDB_TCG(tdb);
}


template<typename DutContainer>
Time<Scale::TDB>
TimeConverter<DutContainer>::convertTCB_TDB(const Time<Scale::TCB> &tcb) const noexcept{

  scalar jdIntTDB, jdFracTDB;

  const int status = iauTcbtdb(tcb.jdInt(), tcb.jdFrac(), &jdIntTDB, &jdFracTDB);

  return {jdIntTDB, jdFracTDB};

}



////TDB///

template<typename DutContainer>
Time<Scale::UTC>
TimeConverter<DutContainer>::convertTDB_UTC(const Time<Scale::TDB> &tdb) const noexcept{

  const Time<Scale::TT> tt = convertTDB_TT(tdb);

  return convertTT_UTC(tt);
}


template<typename DutContainer>
Time<Scale::UT1>
TimeConverter<DutContainer>::convertTDB_UT1(const Time<Scale::TDB> &tdb) const noexcept{

  const Time<Scale::TT> tt = convertTDB_TT(tdb);

  return convertTT_UT1(tt);
}


template<typename DutContainer>
Time<Scale::TAI>
TimeConverter<DutContainer>::convertTDB_TAI(const Time<Scale::TDB> &tdb) const noexcept{

  const Time<Scale::TT> tt = convertTDB_TT(tdb);

  return convertTT_TAI(tt);
}


template<typename DutContainer>
Time<Scale::TT>
TimeConverter<DutContainer>::convertTDB_TT(const Time<Scale::TDB> &tdb) const noexcept{

  scalar jdIntTT = tdb.jdInt(), jdFracTT = tdb.jdFrac();

    const int status = iauTdbtt(tdb.jdInt(), tdb.jdFrac(),
                                dtr(Time<Scale::TT>(jdIntTT, jdFracTT)),
                                &jdIntTT,
                                &jdFracTT);


  return {jdIntTT, jdFracTT};

}


template<typename DutContainer>
Time<Scale::TCG>
TimeConverter<DutContainer>::convertTDB_TCG(const Time<Scale::TDB> &tdb) const noexcept{

  const Time<Scale::TT> tt = convertTDB_TT(tdb);

  return convertTT_TCG(tt);
}


template<typename DutContainer>
Time<Scale::TCB>
TimeConverter<DutContainer>::convertTDB_TCB(const Time<Scale::TDB> &tdb) const noexcept{

  scalar jdIntTCB, jdFracTCB;

  const int status = iauTdbtcb(tdb.jdInt(), tdb.jdFrac(), &jdIntTCB, &jdFracTCB);
  return {jdIntTCB, jdFracTCB};

}

#define CREATE_FUNC_NAME(A, B, C, D) A##B##C##D

#define CREATE_CONVERSION(SCALE_FROM)   \
  if constexpr (To == Scale::UT1) {         \
    return CREATE_FUNC_NAME(convert, SCALE_FROM, _, UT1)(time);         \
  } else if constexpr (To == Scale::UTC) {  \
    return CREATE_FUNC_NAME(convert, SCALE_FROM, _, UTC)(time);         \
  } else if constexpr (To == Scale::TAI) {  \
    return CREATE_FUNC_NAME(convert, SCALE_FROM, _, TAI)(time);         \
  } else if constexpr (To == Scale::TT) {   \
    return CREATE_FUNC_NAME(convert, SCALE_FROM, _, TT)(time);          \
  } else if constexpr (To == Scale::TCG) {  \
    return CREATE_FUNC_NAME(convert, SCALE_FROM, _, TCG)(time);         \
  } else if constexpr (To == Scale::TCB) {  \
    return CREATE_FUNC_NAME(convert, SCALE_FROM, _, TCB)(time);         \
  } else if constexpr (To == Scale::TDB) {  \
    return CREATE_FUNC_NAME(convert, SCALE_FROM, _, TDB)(time);         \
  }

template <typename DutContainer>
template <Scale To, Scale From>
Time<To> TimeConverter<DutContainer>::convert(
    const Time<From> &time) const{
  if constexpr (From == To) {
    return time;
  } else if constexpr (From == Scale::UT1) {
    CREATE_CONVERSION(UT1)
  } else if constexpr (From == Scale::UTC) {
    CREATE_CONVERSION(UTC)
  } else if constexpr (From == Scale::TAI) {
    CREATE_CONVERSION(TAI)
  } else if constexpr (From == Scale::TT) {
    CREATE_CONVERSION(TT)
  } else if constexpr (From == Scale::TCG) {
    CREATE_CONVERSION(TCG)
  } else if constexpr (From == Scale::TCB) {
    CREATE_CONVERSION(TCB)
  } else if constexpr (From == Scale::TDB) {
    CREATE_CONVERSION(TDB)
  }
}




}
#endif // BALLISTICS2023_TIMECONVERTER_HPP
