#ifndef DECISION_TREE
#define DECISION_TREE

#include <assert.h>
#include <Eigen/Dense>
#include <iostream>

Eigen::VectorXd find_model(Eigen::MatrixXd& A, Eigen::VectorXd& b)
{
  assert(b.rows() == A.rows());
  Eigen::VectorXd m;
  m = A.bdcSvd(Eigen::ComputeThinU | Eigen::ComputeThinV).solve(b);
  assert(m.rows() == A.cols());
  return m;
}





/*
void test_feature()
{

}



void partition(Node* node)
{

  find_model


  test_feature



  if (stopping criterion met)
      return;

  if (left partition != pure)
    partition(left child)

  if (right partition != pure)
    partition(right child)


  return;

}



//main algorithm
void build_tree(all data)
{
  

  // create root node

  // attach all data to root

  partition(root);

}

*/


#endif

