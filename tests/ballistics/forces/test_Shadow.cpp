//
// Created by julia on 20.12.23.
//
#include "ballistics/forces/solar_radiation/PenumbraShadow.hpp"
#include "ballistics/time/Time.hpp"
#include "rapidcsv/src/rapidcsv.h"
#include "gtest/gtest.h"
#include <iostream>
#include <limits>
using namespace Ballistics;
TEST(SHADOW, TEST)
{
  std::string filename = "/home/julia/ballistics2023/src/ballistics/utils/penumbra_shadow.csv";
  rapidcsv::Document doc(filename);
  std::vector<scalar> xCol = doc.GetColumn<scalar>("x");
  std::vector<scalar> yCol = doc.GetColumn<scalar>("y");
  std::vector<scalar> zCol = doc.GetColumn<scalar>("z");
  std::vector<scalar> shadowCol = doc.GetColumn<scalar>("shadow");

  int n = xCol.size();
  scalar tolerance = 5e-14;

  for (int i = 0; i < n; ++i)
  {
    PenumbraShadow penumbraShadow(695.700e6,  6.371e6);
    ASSERT_TRUE(abs(penumbraShadow.calcShadow({xCol[i], yCol[i], zCol[i]}, {-1.47098291e10, 0., 0.}, Vector3d::Zero())
                    - shadowCol[i]) <= tolerance);
  }
}