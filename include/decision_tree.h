#ifndef DECISION_TREE
#define DECISION_TREE

#include <assert.h>
#include <Eigen/Dense>
#include <iostream>
#include <vector>
#include <fstream>
#include <utility>
#include "splitter.h"



template <typename Label>
class Decision_tree
{

public:
  Decision_tree(Node<Label>* node) : root(node) {}

  void fit()
  {
    partition(root);

    stream_out_models();
  }

  void predict()
  {

  }

private:

  using Points = std::vector<Point<Label>>;

  // should this be free?
  // divides points based on the split line
  void divide(const Points& parentdata,
              const std::pair<double, double>& line,
              Points& left,
              Points& right)
  {
    std::cout << "dividing...\n";

    // ypred = b0 + b1 * x
    for(std::size_t i = 0; i < parentdata.size(); ++i)
    {
      double ypred = line.first + line.second * parentdata[i].x;
      //figure out y_split

      if (ypred > parentdata[i].y)
        left.push_back(parentdata[i]); // fix needed
      else
        right.push_back(parentdata[i]);
    }

    std::cout << "parent size: " << parentdata.size() << "\n";
    std::cout << "left size: " << left.size();
    for(int i = 0; i < left.size(); ++i)
      std::cout << " (" << left[i].x << "," << left[i].y << ") ";
    std::cout << std::endl;
    std::cout << "right size: " << right.size();
    for(int i = 0; i < right.size(); ++i)
      std::cout << " (" << right[i].x << "," << right[i].y << ") ";
    std::cout << std::endl;
  }

  void partition(Node<Label>* node)
  {

    std::cout << "partitioning...\n";

    assert(!node->is_pure());

    Splitter<Points> splitter;
    std::pair<double, double> split_line = splitter(node->data);
    std::cout << "split line: y = " << split_line.first << " + " <<
                 split_line.second << "x\n";


    // divide points based on the splitting line
    Points pleft, pright;
    divide(node->data, split_line, pleft, pright);

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

  void stream_out_models()
  {
    assert(models.size() % 2 == 0); // even, probably 2 coeff per model
    std::ofstream mout("data/models.txt");
    for(int i = 0; i < models.size(); i = i + 2)
    {
      mout << models[i] << " " << models[i+1] << "\n";
    }
  }

  // data

  std::vector<double> models; // save to stream out
  Node<Label>* root;
};





#endif

