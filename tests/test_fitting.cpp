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

  // todo: test function fit_model instead of solve_lease_squares
  Eigen::VectorXd m = solve_least_squares(X, y);
  std::cout << "m= " << m << std::endl;


  // stream out
  std::ofstream out("data/data.txt");
  for(std::size_t i = 0; i < datapoints.size(); ++i)
  {
    out << datapoints[i].x << " " << datapoints[i].y << " "
        << datapoints[i].label << "\n";
  }
  out.close();

  std::ofstream mout("data/model.txt");
  mout << m << "\n";
  mout.close();


}

void test_decision_tree()
{
  std::cout << "test_decision_tree\n";

  using Label = int;
  std::vector<Point<Label>> datapoints {{-2, 1.5, 0},
                                       {1, 1, 0},
                                       {0, 1.5, 0},
                                       {0.5, -0.5, 1},
                                       {1, -1, 1},
                                       {-1, -2, 1},
                                       {1, 2, 0}};


  Node<Label> root(datapoints);
  Decision_tree<Label> tree(&root);
  tree.fit();
}

void test_decision_tree1()
{
  std::cout << "test_decision_tree\n";

  using Label = int;
  std::vector<Point<Label>> datapoints {{-2, 1.5, 0},
                                       {1, 1, 0},
                                       {0, 1.5, 0},
                                       {0.5, -0.5, 1},
                                       {1, -1, 1},
                                       {-1, -2, 1},
                                       {1, 2, 0},
                                       {-1, 2, 1}}; // outlier


  // stream out
  std::ofstream out("data/data.txt");
  for(std::size_t i = 0; i < datapoints.size(); ++i)
  {
    out << datapoints[i].x << " " << datapoints[i].y << " "
        << datapoints[i].label << "\n";
  }
  out.close();

  Node<Label> root(datapoints);
  Decision_tree<Label> tree(&root);
  tree.fit();
}



int main()
{

  test_decision_tree1();
  return 0;
}

