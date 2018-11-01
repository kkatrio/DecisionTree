#include <iostream>
#include <vector>
#include <Eigen/Dense>
#include "point.h"
#include "node.h"
#include "decision_tree.h"
#include <fstream>



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


void test_decision_tree2()
{
  std::cout << "test_decision_tree\n";

  using Label = int;
  std::vector<Point<Label>> datapoints {{0, 1, 0},
                                        {1, 0, 1}};

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
  // todo: assert line b1 = 1, b0 = 0
}


void test_decision_tree_outlier()
{
  std::cout << "test_decision_tree_outlier\n";

  using Label = int;
  std::vector<Point<Label>> datapoints {{-2, 1.5, 0}, //0
                                       {1, 1, 0},  //1
                                       {0, 1.5, 0}, //2
                                       {0.5, -0.5, 1}, //3
                                       {1, -1, 1}, //4
                                       {-1, -2, 1}, //5
                                       {1, 2, 0}, //6
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

  //test_decision_tree2();
  test_decision_tree_outlier();
  return 0;
}

