#ifndef MATRIX
#define MATRIX

#include <vector>
#include <iostream>

class Matrix {
public:
  Matrix();
  Matrix(int height, int width);
  Matrix(std::vector<std::vector<double> > const & array);

  /* scalar multiplication */
  Matrix multiply(double const &value);
  /* addition */
  Matrix add(Matrix const &matrix) const;
  /* subtract */
  Matrix subtract(Matrix const &matrix) const;
  /* hadamard multiply */
  Matrix multiply(Matrix const &matrix) const;

  /* dot product */
  Matrix dot(Matrix const &matrix) const;
  /* transpose matrix */
  Matrix transpose() const;

  /* apply a function to every element of the matrix */
  Matrix applyFunction(double (*function)(double)) const;
  /* print of the matrix */
  void print(std::ostream &flux) const;
  std::vector<std::vector<double> > get_vector_matrix();

private:
  std::vector<std::vector<double> > mArray;
  unsigned int mHeight;
  unsigned int mWidth;
};

/* print matrix to terminal using cout */
std::ostream& operator<<(std::ostream &os, const Matrix &matrix);

#endif