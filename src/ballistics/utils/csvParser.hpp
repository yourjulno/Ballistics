//
// Created by julia on 19.12.23.
//

#ifndef BALLISTICS2023_CSVPARSER_HPP
#define BALLISTICS2023_CSVPARSER_HPP

#include "ballistics/types/types.hpp"
#include "fast-cpp-csv-parser/csv.h"
#include "ballistics/types/types.hpp"
namespace Ballistics::Utils{

  std::pair<std::vector<scalar>, int> timeConverterParser(const std::string &filename) {

    io::CSVReader<2> data(filename);
    int a;
    scalar b;
    std::vector<int> mjd;
    std::vector<scalar> dUT;
    data.read_header(io::ignore_extra_column, "mjd", "UT1-UTC s");
    while (data.read_row(a, b)) {
      mjd.emplace_back(a);
      dUT.emplace_back(b);
    }

    return std::make_pair(dUT, mjd[0]);
  }

  std::pair<std::pair<std::vector<scalar>, std::vector<scalar>>,
      std::vector<int>>frameConverterParser(const std::string& filename){
    io::CSVReader<3> data(filename);
    int mjd;
    double x, y;
    std::vector<int> mjd_;
    std::vector<scalar> x_arcsec_;
    std::vector<scalar> y_arcsec_;
    data.read_header(io::ignore_extra_column, "mjd", "x arcsec", "y arcsec");
    while (data.read_row(mjd, x, y)) {
      mjd_.emplace_back(mjd);
      x_arcsec_.emplace_back(x);
      y_arcsec_.emplace_back(y);
    }
    return std::make_pair(std::make_pair(x_arcsec_, y_arcsec_), mjd_);
  }


}
#endif // BALLISTICS2023_CSVPARSER_HPP
