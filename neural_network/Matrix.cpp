#include "Matrix.h"
#include <assert.h>
#include <sstream>

Matrix::Matrix() {}

Matrix::Matrix(int height, int width)
{
  this->mHeight = height;
  this->mWidth = width;
  this->mArray = std::vector<std::vector<double> >(height, std::vector<double>(width));
}

Matrix::Matrix(std::vector<std::vector<double> > const & array)
{
  assert(array.size() != 0);
  this->mHeight = array.size();
  this->mWidth = array[0].size();
  this->mArray = array;
}

/* scalar multiplication */
Matrix Matrix::multiply(double const &value)
{
  Matrix result(mHeight, mWidth);

  for (unsigned int row = 0; row < mHeight; ++row) {
    for (unsigned int col = 0; col < mWidth; ++col)
      result.mArray[row][col] = mArray[row][col] * value;
  }

  return result;
}

/* addition */
Matrix Matrix::add(Matrix const &matrix) const
{
  assert(mHeight == matrix.mHeight && mWidth == matrix.mWidth);

  Matrix result(mHeight, mWidth);

  for (unsigned int row = 0; row < mHeight; ++row) {
    for (unsigned int col = 0; col < mWidth; ++col)
      result.mArray[row][col] = mArray[row][col] + matrix.mArray[row][col];
  }

  return result;
}

/* subtract */
Matrix Matrix::subtract(Matrix const &matrix) const
{
  assert(mHeight == matrix.mHeight && mWidth == matrix.mWidth);
  
  Matrix result(mHeight, mWidth);

  for (unsigned int row = 0; row < mHeight; ++row) {
    for (unsigned int col = 0; col < mWidth; ++col)
      result.mArray[row][col] = mArray[row][col] - matrix.mArray[row][col];
  }

  return result;
}

/* hadamard multiply */
Matrix Matrix::multiply(Matrix const &matrix) const
{
  assert(mHeight == matrix.mHeight && mWidth == matrix.mWidth);
  
  Matrix result(mHeight, mWidth);

  for (unsigned int row = 0; row < mHeight; ++row) {
    for (unsigned int col = 0; col < mWidth; ++col)
      result.mArray[row][col] = mArray[row][col] * matrix.mArray[row][col];
  }

  return result;
}

/* dot product */
Matrix Matrix::dot(Matrix const &matrix) const
{
  
}

/* transpose matrix */
Matrix transpose() const;

/* apply a function to every element of the matrix */
Matrix applyFunction(double (*function)(double)) const;
/* print of the matrix */
void print(std::ostream &flux) const;