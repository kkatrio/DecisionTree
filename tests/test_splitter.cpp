#include "splitter.h"
#include "point.h"
#include <iostream>
#include <vector>
#include <cassert>
#include <utility>


void test_factorial()
{
  assert(factorial(4) == 24);
  assert(factorial(3) == 6);
  assert(factorial(2) == 2);
  assert(factorial(1) == 1);
  assert(factorial(0) == 1);
}

void test_perpendicular_line()
{
  using Label = int;
  using Point = Point<Label>;
  std::pair<Point, Point> twopoints{{1, 1, 0},
                                    {1.5, 0.5, 1}};


  std::pair<double, double> perp_line =
      Splitter<std::vector<Point>>().get_line(twopoints);

  double e = 1e-15;
  double b0 = perp_line.first;
  double b1 = perp_line.second;
  assert(b0 > -0.5 - e && b0 < -0.5 + e);
  assert(b1 > 1 - e && b1 < 1 + e);

}


void test_gini_index()
{
  using Label = int;
  using Points = std::vector<Point<Label>>;
  Points first {{0, 0, 0},
             {0, 2, 0},
             {2.01, 0, 1}};
  Points second {{0, 0, 1},
             {0, 2, 1},
             {2.01, 0, 1}};

  Points third {{0, 0, 1},
             {0, 2, 1},
             {2.01, 0, 0},
             {2.01, 0, 0}  };

  double e = 1e-6;
  double g1 = measure_gini(first);
  assert(g1 > 0.444444 - e && g1 < 0.444444 + e);

  double g2 = measure_gini(second);
  assert(g2 > 0 - e && g2 < 0 + e);

  double g3 = measure_gini(third);
  assert(g3 > 0.5 - e && g3 < 0.5 + e);

  assert(measure_weighted_average_index(first, third) > 0.47619 - e
      && measure_weighted_average_index(first, third) < 0.47619 + e );
}

void test_splitter1()
{

  using Label = int;
  using Points = std::vector<Point<Label>>;
  Points datapoints {{-2, 1.5, 0},
                     {1, 1, 0},
                     {0, 1.5, 0},
                     {0.5, -0.5, 1},
                     {1, -1, 1}};
  Splitter<Points> splitter;
  std::pair<double, double> ps = splitter(datapoints); // returns a line
  double e = 1e-15;
  assert(ps.first < 0.5 + e
         && ps.first > 0.5 - e);
  assert(ps.second < -0.333333333333333 + e
         && ps.second > -0.333333333333333 - e);

}


void test_splitter2()
{

  using Label = int;
  using Points = std::vector<Point<Label>>;
  Points datapoints {{-2, 1.5, 0}, //0
                     {1, 1, 0},    //1
                     {0, 1.5, 0},  //2
                     {1.5, 0.5, 1},//3
                     {2, -1, 1},   //4
                     {0, -2, 1},  //5
                     {0.7, 2, 0}};   //6
  Splitter<Points> splitter;
  std::pair<double, double> ps = splitter(datapoints); // returns a line

  double e = 1e-15;
  assert(ps.first < 0.3214285714285712 + e
         && ps.first > 0.3214285714285712 - e);
  assert(ps.second < 0.5714285714285712 + e
         && ps.second > 0.5714285714285712 - e);

}


int main()
{
  test_gini_index();
  test_factorial();
  test_perpendicular_line();
  test_splitter1();
  test_splitter2();
  return 0;
}
