#include <iostream>
#include <vector>
#include <Eigen/Dense>
#include "point.h"
#include "node.h"
#include "decision_tree.h"
#include <fstream>


void test_pointset()
{
  std::cout << "test_pointset\n";

  using Label = int;
  std::vector<Point<Label>> datapoints {{-2, 1.5, 0},
                                       {1, 1, 0},
                                       {0, 1.5, 0},
                                       {0.5, -0.5, 1},
                                       {1, -1, 1},
                                       {-1, -2, 1},
                                       {1, 2, 0}};

  Node<Label> root(datapoints);

  Eigen::MatrixXd X(datapoints.size(), 2);
  Eigen::VectorXd y(datapoints.size());
  for(std::size_t i = 0; i < datapoints.size(); ++i)
  {
    int idx = static_cast<int>(i);
    X(idx, 0) = 1;
    X(idx, 1) = datapoints[i].x;

    y[idx] = datapoints[i].y;
  }

  Eigen::VectorXd m = find_model(X, y);
  std::cout << "m= " << m << std::endl;


  // stream out
  std::ofstream out("data/data.txt");
  for(std::size_t i = 0; i < datapoints.size(); ++i)
  {
    out << datapoints[i].x << " " << datapoints[i].y << " "
        << datapoints[i].data << "\n";
  }
  out.close();


  std::ofstream mout("data/model.txt");
  mout << m << "\n";
  mout.close();



}







int main()
{

  test_pointset();
  return 0;
}

