#ifndef NODE
#define NODE


#include "point.h"
#include <vector>

// needs base class??

// represents partitions of the space
template <typename Label>
class Node
{
public:

  Node() {} // for pointer
  Node(std::vector<Point<Label>>& points) : data(points) {}

  // todo: lambda
  bool is_pure()
  {
    Label l = data[0].label;
    for(std::size_t i = 1; i < data.size(); ++i)
    {
      if (data[i].label != l) // label type must support == operator
        return false;
    }
    return true;
  }

  // data
  std::vector<Point<Label>> data; // unordered map??

  // children
  Node* left_child;
  Node* right_child;
};


#endif
