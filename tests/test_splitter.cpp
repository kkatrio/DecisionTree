#include "splitter.h"
#include "point.h"
#include <iostream>
#include <vector>
#include <cassert>
#include <utility>

void test_splitter()
{

  using Label = int;
  using Points = std::vector<Point<Label>>;
  Points datapoints {{0, 0, 0},
                     {0, 2, 0},
                     {2.01, 0, 0}};

  Splitter<Points> splitter;
  std::pair<double, double> ps = splitter(datapoints); // returns a line
  double e = 1e-15;
  assert(ps.first > 1 - e && ps.first < 1 + e);
  assert(ps.second > 0 - e && ps.second < 0 + e);

}

void test_factorial()
{
  assert(factorial(4) == 24);
  assert(factorial(3) == 6);
  assert(factorial(2) == 2);
  assert(factorial(1) == 1);
  assert(factorial(0) == 1);
}




int main()
{
  test_factorial();
  test_splitter();
  return 0;
}
