#ifndef SM_VECTOR_H
#define SM_VECTOR_H

#include <cstdint>
#include <stdexcept>
#include <string>
#include <vector>

namespace sm {

template<typename T>
class Vector
{
public:
  explicit Vector(uint32_t size) { values.resize(size); }

  explicit Vector() = default;

  void Resize(size_t size) { values.resize(size); }

  // Throws exception on out of range
  const T& operator[](const size_t index) const
  {
    return values.at(index);// To ensure bound check
  }

  T& operator[](size_t index)
  {
    return values.at(index);// To ensure bound check
  }

  Vector operator+(const Vector<T>& other) const
  {
    Vector res(values.size());

    uint32_t index = 0;
    for(auto& v: values) {
      res[index] = v + other.values[index];
      index++;
    }

    return res;
  }

  Vector& operator+=(const Vector<T>& other)
  {
    uint32_t index = 0;
    for(auto& v: values) {
      v += other.values[index];
      index++;
    }

    return *this;
  }

  Vector operator-(const Vector<T>& other) const
  {
    Vector res(values.size());

    uint32_t index = 0;
    for(auto& v: values) {
      res[index] = v - other.values[index];
      index++;
    }

    return res;
  }

  Vector& operator-=(const Vector<T>& other)
  {
    uint32_t index = 0;
    for(auto& v: values) {
      v += other.values[index];
      index++;
    }

    return *this;
  }

  size_t GetSize() const { return values.size(); }

private:
  std::vector<T> values;
};

template<typename T>
class VectorView
{
public:
  explicit VectorView(size_t size) { values.reserve(size); }
  explicit VectorView(const Vector<T>& vec)
  {
    values.reserve(vec.GetSize());
    Push(vec);
  }

  void PushValue(const T& valueRef) { values.push_back(&valueRef); }

  void Push(const Vector<T>& vec)
  {
    for(size_t i = 0; i < vec.GetSize(); i++) { PushValue(vec[i]); }
  }

  // Throws exception on out of range
  const T& operator[](uint32_t index) const
  {
    return *values.at(index);// To ensure bound check
  }

private:
  std::vector<const T*> values;
};

}// namespace sm

#endif// SM_VECTOR_H
