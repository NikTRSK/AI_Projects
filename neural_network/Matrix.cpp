#include "Matrix.h"
#include <assert.h>
#include <sstream>
#include <iomanip>

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
  assert(mWidth == matrix.mHeight);

  double dot_product_item = 0.0;
  Matrix result(mHeight, matrix.mWidth);

  for (unsigned int row = 0; row < mHeight; ++row) {
    for (unsigned int col = 0; col < matrix.mWidth; ++col) {
      for (unsigned int h = 0; h < mWidth; ++h)
      dot_product_item += mArray[row][h] * matrix.mArray[h][col];
      result.mArray[row][col] = dot_product_item;
      dot_product_item = 0.0;
    }
  }
  
  return result;
}

/* transpose matrix */
Matrix Matrix::transpose() const
{
  // each ij element turns to a ji element
  Matrix result (mWidth, mHeight);
  for (unsigned int i = 0; i < mWidth; ++i) {
    for (unsigned int j = 0; j < mHeight; ++j) {
      result.mArray[i][j] = mArray[j][i];
    }
  }

  return result;
}

/* apply a function to every element of the matrix */
Matrix Matrix::applyFunction(double (*function)(double)) const
{
  Matrix result (mHeight, mWidth);
  for (unsigned int i = 0; i < mHeight; ++i) {
    for (unsigned int j = 0; j < mWidth; ++j) {
      result.mArray[i][j] = (*function)(mArray[i][j]);
    }
  }

  return result;
}

/* print of the matrix */
void Matrix::print(std::ostream &os) const
{
/*   // Get the max padding needed for each digit
  unsigned int max_width = 0;
  std::stringstream ss;
  std::string s;
  for (unsigned int i = 0; i < mArray.size(); ++i) {
    for (unsigned int j = 0; j < mArray.size(); ++j) {
      ss.str("");
      ss << mArray[i][j];
      s = ss.str();
      if (s.size() > max_width)
        max_width = s.size();
    }
  }
  
  for (unsigned int i = 0; i < mArray.size(); ++i) {
    for (unsigned int j = 0; j < mArray[0].size(); ++j) {
      ss.str("");  
      ss << mArray[i][j];
      s = ss.str();
      os << s << std::string (max_width - s.size() + 1, ' ');;
    }
    os << std::endl;
  } */
/*   unsigned int i,j;
  // const unsigned int mw = mWidth;
  int maxLength[mWidth];
  maxLength[mWidth] = {};
  std::stringstream ss;

  for (i=0 ; i<mHeight ; i++)
  {
      for (j=0 ; j<mWidth ; j++)
      {
          ss << mArray[i][j];
          if(maxLength[j] < ss.str().size())
          {
              maxLength[j] = ss.str().size();
          }
          ss.str(std::string());
      }
  }

  for (i=0 ; i<mHeight ; i++)
  {
      for (j=0 ; j<mWidth ; j++)
      {
          os << mArray[i][j];
          ss << mArray[i][j];
          for (int k=0 ; k<maxLength[j]-ss.str().size()+1 ; k++)
          {
              os << " ";
          }
          ss.str(std::string());
      }
      os << std::endl;
  } */
  std::stringstream ss;
  unsigned int padding = 0;
  for (unsigned int i = 0; i < mArray.size(); ++i) {
    for (unsigned int j = 0; j < mArray[i].size(); ++j) {
      ss << mArray[i][j];
      // Find maximum size of an item
      if (ss.str().size() > padding)
        padding = ss.str().size();
      ss.str(""); // Reset string stream
    }
  }

  for (unsigned int i = 0; i < mArray.size(); ++i) {
    for (unsigned int j = 0; j < mArray[i].size(); ++j) {
      os << std::setw(padding + 1) << mArray[i][j] << "  ";
    }
    os << std::endl;
  }
}

std::vector<std::vector<double> > Matrix::get_vector_matrix()
{
  return mArray;
}

std::ostream& operator<<(std::ostream &os, const Matrix &matrix)
{
    matrix.print(os);
    return os;
}