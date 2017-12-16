#include "Matrix.h"
// #include "Test.h"

int main()
{
  std::vector<std::vector<double> > m1_vals = {
    {7.5, 8.0, 9.0},
    {10.0, 11.0, 12.0}
  };

  Matrix m1(m1_vals);
  std::cout << m1;
  
  return 0;
}