#include "Matrix.h"
#include <vector>

void validate_result(std::vector<std::vector<double> > a, std::vector<std::vector<double> > b)
{
  if (a == b)
    std::cout << "PASS\n";
  else
    std::cout << "FAIL\n";
}

int main()
{
  std::vector<std::vector<double> > m1_vals = {
    {2.0, 3.0, 4.0},
    {5.0, 6.0, 7.0}
  };

  std::vector<std::vector<double> > m2_vals = {
    {8.0, 9.0, 10.0},
    {11.0, 12.0, 13.0}
  };

  Matrix m1(m1_vals);
  Matrix m2(m2_vals);

  /* Test scalar multiply */
  {
    std::vector<std::vector<double> > expected_result = {
      {4.0, 6.0, 8.0},
      {10.0, 12.0, 14.0}
    };
    Matrix res = m1.multiply(2.0);
    std::cout << "Scalar Multiply: ";
    validate_result(res.get_vector_matrix(), expected_result);
  }
  /* Test add */
  {
    std::vector<std::vector<double> > expected_result = {
      {10.0, 12.0, 14.0},
      {16.0, 18.0, 20.0}
    };
    Matrix res = m1.add(m2);
    std::cout << "Addition: ";
    validate_result(res.get_vector_matrix(), expected_result);
  }
  /* Test sub */
  {
    std::vector<std::vector<double> > expected_result = {
      {-6.0, -6.0, -6.0},
      {-6.0, -6.0, -6.0}
    };
    Matrix res = m1.subtract(m2);
    std::cout << "Subtraction: ";
    validate_result(res.get_vector_matrix(), expected_result);
  }
  /* Test hadamard multiply */
  {
    std::vector<std::vector<double> > expected_result = {
      {16.0, 27.0, 40.0},
      {55.0, 72.0, 91.0}
    };
    Matrix res = m1.multiply(m2);
    std::cout << "Hadamard Multiply: ";
    validate_result(res.get_vector_matrix(), expected_result);
  }
  /* Test dot product */
  {  
    std::vector<std::vector<double> > m3_vals = {
      {8.0, 9.0},
      {10.0, 11.0},
      {12.0, 13.0}
    };
    std::vector<std::vector<double> > expected_result = {
      {94.0, 103.0},
      {184.0, 202.0}
    };
    Matrix res = m1.dot(Matrix(m3_vals));
    std::cout << "Dot Product: ";
    validate_result(res.get_vector_matrix(), expected_result);
  }
  /* Test transpose */
  {
    std::vector<std::vector<double> > expected_result = {
      {2.0, 5.0},
      {3.0, 6.0},
      {4.0, 7.0}
    };
    Matrix res = m1.transpose();
    std::cout << "Transpose Matrix: ";
    validate_result(res.get_vector_matrix(), expected_result);
  }
  /* Test apply function */
  {
    std::vector<std::vector<double> > expected_result = {
      {4.0, 6.0, 8.0},
      {10.0, 12.0, 14.0}
    };
    Matrix res = m1.applyFunction([](double x) {return x * 2;});
    std::cout << "Apply Function: ";
    validate_result(res.get_vector_matrix(), expected_result);
  }
}