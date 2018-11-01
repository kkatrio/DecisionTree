#ifndef DECISION_TREE
#define DECISION_TREE

#include <assert.h>
#include <Eigen/Dense>
#include <iostream>
#include <vector>
#include <fstream>
#include <utility>
#include "splitter.h"
#include <algorithm>


template <typename Label>
class Decision_tree
{

public:
  Decision_tree(Node<Label>* node) : root(node) {}

  void fit()
  {
    partition(root);

    stream_out_lines();
  }

  void predict()
  {

  }


private:

  using Points = std::vector<Point<Label>>;

  void partition(Node<Label>* node)
  {

    save_xrange(node->data);

    std::cout << "partitioning...\n";

    assert(!node->is_pure());

    Splitter<Points> splitter;
    std::pair<double, double> split_line = splitter(node->data);
    std::cout << "split line: y = " << split_line.first << " + " <<
                 split_line.second << "x\n";

    lines.push_back(split_line.first);
    lines.push_back(split_line.second);

    // divide points based on the splitting line
    Points pleft, pright;
    divide_points(node->data, split_line, pleft, pright);


    if(pleft.empty() || pright.empty())
    {
      std::cerr << "empty set, returning\n";
      return;
    }

    assert(!pleft.empty());
    assert(!pright.empty());

    // asssign points to child nodes
    Node<Label>* l_child = new Node<Label>; // todo: delete
    Node<Label>* r_child = new Node<Label>;
    l_child->data = pleft;
    r_child->data = pright;

    // link parent with children
    node->left_child = l_child;
    node->right_child = r_child;

    // partition recursively if necessary
    if(!stopping_criterion_met(l_child))
    {
      std::cout << "into left child\n";
      partition(l_child);
    }
    if(!stopping_criterion_met(r_child))
    {
      std::cout << "into right child\n";
      partition(r_child);
    }
    return;
  }

  bool stopping_criterion_met(Node<Label>* node)
  {
    return node->is_pure();
  }

  void save_xrange(Points data)
  {
    // sort based on x
    using Point = typename Points::value_type;
    auto based_on_x = [](Point p1, Point p2)
    {
      return p1.x < p2.x;
    };

    std::sort(data.begin(), data.end(), based_on_x);

    xrange.push_back(data[0].x); //xmin
    xrange.push_back(data[data.size()-1].x); //xmax
  }

  void stream_out_lines()
  {
    assert(lines.size() % 2 == 0); // even, 2 coeff per model
    std::ofstream linesout("data/lines.txt");
    for(int i = 0; i < lines.size(); i = i + 2)
    {
      linesout << lines[i] << " " << lines[i+1] << " "
               << xrange[i] << " " << xrange[i+1]
               << "\n";
    }
  }

  // data
  std::vector<double> lines; // save to stream out
  std::vector<double> xrange; // save to stream out
  Node<Label>* root;
};





#endif

