#ifndef SM_MATRIX_H
#define SM_MATRIX_H

#include <stdexcept>
#include <string>

#include "Vector.h"

namespace sm {

template<typename T>
class Matrix;

template<typename T>
Matrix<T> ConvertToMatrix(sm::Vector<T> vec, bool rowMajor = true);

template<typename T>
class Matrix
{
public:
  explicit Matrix(size_t r, size_t c)
      : rows(r)
      , rowsCount(r)
      , columnsCount(c)
  {
    for(size_t i = 0; i < r; i++) rows[i].Resize(c);
  }
  explicit Matrix()
      : rows()
      , rowsCount(0)
      , columnsCount(0)
  {}

  void Resize(size_t r, size_t c)
  {
    rows.Resize(r);
    for(size_t i = 0; i < r; i++) rows[i].Resize(c);
  }

  size_t GetRows() const { return rowsCount; }
  size_t GetColumns() const { return columnsCount; }

  // Throws exception on out of range
  const Vector<T>& operator[](uint32_t index) const
  {
    return rows[index];// To ensure bound check
  }

  Vector<T>& operator[](uint32_t index)
  {
    return rows[index];// To ensure bound check
  }

  // We are using view since we don't want to copy our matrices data
  VectorView<T> GetRow(size_t index) const
  {
    return sm::VectorView<T>(rows[index]);
  }

  VectorView<T> GetColumn(size_t index) const
  {
    VectorView<T> column(rows.GetSize());

    for(size_t i = 0; i < rows.GetSize(); i++) {
      const Vector<T>& test = rows[i];
      column.PushValue(test[index]);
    }

    return column;
  }

  Matrix operator+(const Matrix<T>& other) const
  {
    if(rowsCount != other.rowsCount || columnsCount != other.columnsCount)
      throw std::invalid_argument("Matrix size mismatch");
    Matrix result(rowsCount, columnsCount);

    for(size_t i = 0; i < rowsCount; i++) {
      for(size_t j = 0; j < columnsCount; j++) {
        result[i][j] = rows[i][j] + other[i][j];
      }
    }

    return result;
  }

  Matrix operator+=(const Matrix<T>& other) const
  {
    if(rowsCount != other.rowsCount || columnsCount != other.columnsCount)
      throw std::invalid_argument("Matrix size mismatch");

    for(size_t i = 0; i < rowsCount; i++) {
      for(size_t j = 0; j < columnsCount; j++) { rows[i][j] += other[i][j]; }
    }

    return *this;
  }

  Matrix<T> operator-(const Matrix<T>& other) const
  {
    if(rowsCount != other.rowsCount || columnsCount != other.columnsCount)
      throw std::invalid_argument("Matrix size mismatch");
    Matrix result(rowsCount, columnsCount);

    for(size_t i = 0; i < rowsCount; i++) {
      for(size_t j = 0; j < columnsCount; j++) {
        result[i][j] = rows[i][j] + other[i][j];
      }
    }

    return result;
  }

  Matrix operator-=(const Matrix& other) const
  {
    if(rowsCount != other.rowsCount || columnsCount != other.columnsCount)
      throw std::invalid_argument("Matrix size mismatch");

    for(size_t i = 0; i < rowsCount; i++) {
      for(size_t j = 0; j < columnsCount; j++) { rows[i][j] -= other[i][j]; }
    }

    return *this;
  }

  Matrix operator*(const Matrix& other) const
  {
    if(columnsCount != other.rowsCount)
      throw std::invalid_argument("Matrix size mismatch");

    Matrix result(rowsCount, other.columnsCount);

    for(size_t i = 0; i < rowsCount; i++) {
      for(size_t j = 0; j < other.columnsCount; j++) {
        result[i][j] = 0;
        for(size_t k = 0; k < columnsCount; k++) {
          result[i][j] += rows[i][k] * other[k][j];
        }
      }
    }

    return result;
  }

  Matrix operator*(const Vector<T>& other) const
  {
    return *this * ConvertToMatrix<T>(other, false);
  }

private:
  Vector<Vector<T>> rows;
  size_t rowsCount;
  size_t columnsCount;
};

// Converting vector to 1xN or Nx1 matrix to simplify multiplication
// Not really the best way of doing things, but... whatever
template<typename T>
Matrix<T> ConvertToMatrix(Vector<T> vec, bool rowMajor)
{
  if(rowMajor) {
    Matrix<T> result(1, vec.GetSize());
    for(size_t i = 0; i < vec.GetSize(); i++) { result[0][i] = vec[i]; }
    return result;
  }

  Matrix<T> result(vec.GetSize(), 1);
  for(size_t i = 0; i < vec.GetSize(); i++) { result[i][0] = vec[i]; }
  return result;
}
}// namespace sm

template<typename T>
sm::Matrix<T> operator*(const sm::Vector<T>& left, const sm::Matrix<T>& right)
{
  return sm::ConvertToMatrix<T>(left, true) * right;
}

#endif// SM_MATRIX_H
