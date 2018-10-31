#ifndef SPLITTER
#define SPLITTER

#include <utility>
#include <iostream>
#include <cassert>
#include <limits>
#include "vector.h"
#include <Eigen/Dense>


unsigned factorial(unsigned n)
{
  return n == 0 || n == 1 ? 1 : factorial(n-1) * n;
}


template <typename Points>
class Splitter
{

  using Point = typename Points::value_type;

public:

  std::pair<double, double> operator()(const Points& points)
  {
    std::pair<Point, Point> best_points = find_best(points);

    // find line that is perpendicular to the segment between them
    return perpendicular_line(best_points);
  }

  // brute force : consider all and take the best
  // all (unsorted) binary(k=2) combinations of n points
  // are n! / ((n-k)! k!)

  std::pair<Point, Point> find_best(const Points& points)
  {
    int n = static_cast<int>(points.size());
    unsigned count = 0;

    double min = std::numeric_limits<double>::max();
    int min_i, min_j;

    for(int i = 0; i < n-1; ++i)
    {
      for(int j = i + 1; j < n; ++j)
      {

        double dist = measure(points[i],points[j]);
        if (dist < min)
        {
          min = dist;
          min_i = i;
          min_j = j;
        }

        ++count;
      }
    }

    unsigned n_combinations = factorial(n) / (factorial(n- 2) * 2);// implicit convestion ??
    assert(count == n_combinations);


    return std::make_pair(points[min_i], points[min_j]);

  }

  std::pair<double, double> perpendicular_line(const std::pair<Point, Point>& two_points)
  {
    // find line perpendicular to AB which passes through its middle.

    Point A = two_points.first;
    Point B = two_points.second;

    Vector<Point> v(A, B);
    v.normalize(); // for cross product

    // vector n = perpendicular to v
    // 1 way:
    // v * n  = 0
    // v x n = 1

    // 2 way:
    // v * n  = 0
    // |n| = 1 (find both points on the circle)

    // any way we won't avoid the sqrt.

    // solving
    // vx * nx + vy * ny = 0
    // vx * ny - vy * nx = 1

    Eigen::Matrix2d V;
    V(0,0) = v.x;
    V(0,1) = v.x;
    V(1,0) = v.y;
    V(1,1) = - v.y;
    Eigen::Vector2d b;
    b[0] = 0;
    b[1] = 1;

    Eigen::Vector2d n = V.colPivHouseholderQr().solve(b);
    double nx = n[0];
    double ny = n[1];

    if (nx == 0)
    {
      std::cerr << "returning 0,0 instead of a line!\n";
      return std::make_pair(0, 0); // needs thinking. what if line is x = 1
    }

    // Line(x,y) = M + t * n, M is the middle of v,  -Inf < t < Inf
    // x = Mx + t * nx
    // y = My + t * ny
    // (x - Mx) / nx = (y - My) / ny
    // y = My - (ny/nx) Mx  + (ny/nx) x

    // find middle
    double mx = (A.x + B.x) / 2.0;
    double my = (A.y + B.y) / 2.0;

    double b0 = my - (ny / nx) * mx;
    double b1 = ny / nx;

    return std::make_pair(b0, b1);
  }

  double measure(const Point& pi, const Point& pj)
  {
    // measures squared distance
    return (pj.x - pi.x) * (pj.x - pi.x) + (pj.y - pi.y) * (pj.y - pi.y);

  }





};





#endif
