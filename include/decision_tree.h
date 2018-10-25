#ifndef DECISION_TREE
#define DECISION_TREE

#include <assert.h>
#include <Eigen/Dense>
#include <iostream>
#include <vector>

// rename solve least squares
// find model to work with vectors
Eigen::VectorXd solve_least_squares(Eigen::MatrixXd& A, Eigen::VectorXd& b)
{
  assert(b.rows() == A.rows());
  Eigen::VectorXd m;
  m = A.bdcSvd(Eigen::ComputeThinU | Eigen::ComputeThinV).solve(b);
  assert(m.rows() == A.cols());
  return m;
}

template <typename Label>
std::vector<double> fit_model(std::vector<Point<Label>>& dpoints)
{
  std::cout << "fitting model...\n";

  // cp coords in Eigen matrices
  Eigen::MatrixXd X(dpoints.size(), 2);
  Eigen::VectorXd y(dpoints.size());
  for(std::size_t i = 0; i < dpoints.size(); ++i)
  {
    int idx = static_cast<int>(i);
    X(idx, 0) = 1;
    X(idx, 1) = dpoints[i].x;
    y[idx] = dpoints[i].y;
  }

  Eigen::VectorXd w = solve_least_squares(X, y);

  // cp solution and return a std::vector
  // quick solution
  std::vector<double> m;
  m.push_back(w[0]);
  m.push_back(w[1]);
  return m;
}





template <typename Label>
class Decision_tree
{

public:
  Decision_tree(Node<Label>* node) : root(node) {}

  void fit()
  {
    partition(root);
  }

  void predict()
  {

  }

private:

  using Points = std::vector<Point<Label>>;

  // divides points based on the model, independent of their label
  void divide(Points& parentdata,
              std::vector<double>& m,
              Points& left,
              Points& right)
  {
    std::cout << "dividing...\n";

    // ypred = b0 + b1 * x
    for(std::size_t i = 0; i < parentdata.size(); ++i)
    {
      double ypred = m[0] + m[1] * parentdata[i].x;
      if (ypred > parentdata[i].y)
        left.push_back(parentdata[i]); // fix needed
      else
        right.push_back(parentdata[i]);
    }
  }

  void partition(Node<Label>* node)
  {

    assert(!node->is_pure());

    // fit model
    std::vector<double> m = fit_model(node->data);

    // divide points based on model
    Points pleft, pright;
    divide(node->data, m, pleft, pright);
    assert(!pleft.empty());
    assert(!pright.empty());

    // asssign points to child nodes
    Node<Label>* l_child = new Node<Label>;
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

  Node<Label>* root;
};





#endif

