#include <iostream>
#include <cmath>
#include <limits>

#include "RNum.h"

int main()
{
  RNum gen(1, 62831);
  std::cout << std::numeric_limits<int>::max() << '\n';

  for(unsigned int i = 0; i < 10; i++)
  {
    double val = (double)(gen.next()) / 10000;
    std::cout << val << " - sin: "
      << std::sin(val) << " - cos: "
      << std::cos(val) << '\n';
  }

  return 0;
}
