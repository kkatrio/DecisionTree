#ifndef VECTOR
#define VECTOR

#include <math.h>

template <typename Point>
struct Vector
{
  double x, y;
  Vector() = default; // doubles initialize to 0
  Vector(Point& A, Point& B) : x(B.x - A.x), y(B.y - A.y) {}

  void normalize()
  {
    // not great. Assert it's not been normalized before?
    double nrm = norm();
    x /= nrm;
    y /= nrm;
  }

  double norm()
  {
    return std::sqrt(x * x + y * y);
  }

};



#endif
