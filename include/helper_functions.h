#ifndef HELPER_FUNCTIONS
#define HELPER_FUNCTIONS

bool almost_equal(double a, double b)
{
  double eps = 1e-15;
  return (a < b + eps && a > b - eps) ? true : false;
}



#endif // HELPER_FUNCTIONS

