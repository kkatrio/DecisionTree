#ifndef NODE
#define NODE


#include "point.h"
#include <vector>


// represents partitions in the space
template <typename Label>
class Node
{
public:

  Node() = default;
  Node(std::vector<Point<Label>>& points) : data(points) {}


  // data
  std::vector<Point<Label>> data;

  // is pure predicate
};


#endif
