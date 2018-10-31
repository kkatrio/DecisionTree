#ifndef SPLITTER
#define SPLITTER

#include <utility>
#include <iostream>
#include <cassert>
#include "vector.h"
#include <Eigen/Dense>
#include <vector>
#include <algorithm>
#include <numeric>


unsigned factorial(unsigned n)
{
  return n == 0 || n == 1 ? 1 : factorial(n-1) * n;
}

// free copy of Decision_tree's member
template <typename Point>
void divide_points(const std::vector<Point>& points,
                   const std::pair<double, double>& line,
                   std::vector<Point>& top_set,
                   std::vector<Point>& bottom_set)
{
  //std::cout << "dividing...\n";

  // y_split = b0 + b1 * x
  for(std::size_t i = 0; i < points.size(); ++i)
  {
    double y_splitting_line = line.first + line.second * points[i].x;
    double y_point = points[i].y;

    if (y_point > y_splitting_line)
      top_set.push_back(points[i]); // fix needed
    else // equal too
      bottom_set.push_back(points[i]);
  }

  /*
  std::cout << "points size: " << points.size() << "\n";
  std::cout << "left_set size: " << left_set.size();
  for(int i = 0; i < left_set.size(); ++i)
    std::cout << " (" << left_set[i].x << "," << left_set[i].y << ") ";
  std::cout << std::endl;
  std::cout << "right_set size: " << right_set.size();
  for(int i = 0; i < right_set.size(); ++i)
    std::cout << " (" << right_set[i].x << "," << right_set[i].y << ") ";
  std::cout << std::endl;
  */
}

template <typename Point>
struct Has_label
{
  Has_label(int l) : label(l) {}
  bool operator()(Point p)
  {
    return p.label == label;
  }
  int label;
};



template <typename Point>
double measure_gini(std::vector<Point>& node_points)
{
  // gini index = 1 - Σ_i (p_i)^2
  // p_i is the fraction of points belonging to class i

  // todo: have a vector of labels, maybe in the class as data
  // temporarily:
  using Label = int;
  std::vector<Label> plabels{0, 1};
  //

  std::vector<double> pis; // to save the fractions
  pis.reserve(plabels.size());
  for(int plabel : plabels)
  {

    auto has_label = [plabel](Point p)
    {
      return p.label == plabel;
    };

    //int c = std::count_if(node_points.begin(), node_points.end(), Has_label<Point>(plabel));
    int c = std::count_if(node_points.begin(), node_points.end(), has_label);

    double pi = static_cast<double>(c) / static_cast<double>(node_points.size());
    pis.push_back(pi * pi); // store the square
  }

  return 1 - std::accumulate(pis.begin(), pis.end(), 0.0);
}


template <typename Point>
double measure_weighted_average_index(std::vector<Point>& points_left, std::vector<Point>& points_right)
{

  // tree is binary, so ok
  double n1 = static_cast<double>(points_left.size());
  double n2 = static_cast<double>(points_right.size());
  double n = n1 + n2; // points in the parent node
  double g1 = measure_gini(points_left);
  double g2 = measure_gini(points_right);
  return  (n1 / n) * g1 + (n2 / n) * g2;
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

  // just for testing
  std::pair<double, double> get_line(const std::pair<Point, Point>& two_points)
  {
    return perpendicular_line(two_points);
  }


private:

  // brute force : consider all and take (one of) the best
  // all (unsorted) binary(k=2) combinations of n points
  // are n! / ((n-k)! k!)

  std::pair<Point, Point> find_best(const Points& points)
  {
    int n = static_cast<int>(points.size());
    unsigned count = 0;

    double min_gini_index = 0.5;
    int min_i, min_j;

    for(int i = 0; i < n-1; ++i)
    {
      for(int j = i + 1; j < n; ++j)
      {

        // for every pair, find perpendicular line
        std::pair<double, double> line =
            perpendicular_line(std::make_pair(points[i], points[j]));

        // based on the line, split points
        std::vector<Point> left_set;
        std::vector<Point> right_set;
        divide_points(points, line, left_set, right_set);

        // must maximize information gain
        // Δ = I(parent) - Σ_j (N_i / N) I_j   ,
        // j the children (binary = 2),
        // N_j number of points in the child node
        // N number of points in the parent node
        // I_j impurity of the node

        // we find the min weighted average
        double impurity = measure_weighted_average_index(left_set, right_set);
        //std::cout << "imp = " << impurity << " i,j= " << i << "," << j << "\n";

        if (impurity < min_gini_index)
        {
          min_gini_index = impurity;
          min_i = i;
          min_j = j;
        }

        ++count;
      }
    }

    //std::cout << "min_gini_index = " << min_gini_index << "  i, j = " << min_i << " " << min_j << "\n";

    unsigned n_combinations = factorial(n) / (factorial(n - 2) * 2);// implicit convestion ??
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
    V(0,1) = v.y;
    V(1,0) = v.x;
    V(1,1) = - v.y;
    Eigen::Vector2d b;
    b[0] = 0;
    b[1] = 1;

    Eigen::Vector2d n = V.colPivHouseholderQr().solve(b);
    double nx = n[0];
    double ny = n[1];

    if (std::abs(nx) < 1e-15)
    {
      //std::cerr << "vertical line! returning b0 = 0, b1 = 1000!\n";
      return std::make_pair(0, 1000); // needs thinking. what if line is x = 1
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


};





#endif
