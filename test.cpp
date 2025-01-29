#include "Vector.h"
#include <iostream>
#include <stdexcept>

#include "Matrix.h"

template<typename T>
bool AreSame(T a, T b)
{
  constexpr T EPSILON = 0.00001;
  return std::abs(a - b) < EPSILON;
}

// Vector creation + exceptions
int Test1()
{
  std::cout << "Creating simple vec10\n";
  sm::Vector<float> a(10);

  std::cout << "Filling the vector\n";
  for(size_t i = 0; i < a.GetSize(); i++) {
    a[i] = static_cast<float>(rand() % 100);
    // We don't really care about good random here
  }

  std::cout << "Trying to access value by incorrect index\n";

  try {
    a[11] = 25.0f;
  } catch(const std::out_of_range& e) {
    std::cout << "Got out_of_range exception with message " << e.what()
              << std::endl;
    return 0;
  }

  return 1;
}

// Simple vector addition
int Test2()
{
  std::cout << "Adding two vectors {1.1, 1.2, 1.3} + {1.4, 1.5, 1.6} expecting "
               "{2.5, 2.7, 2.9}\n";
  sm::Vector<float> a(3);
  a[0] = 1.1f;
  a[1] = 1.2f;
  a[2] = 1.3f;

  sm::Vector<float> b(3);
  b[0] = 1.4f;
  b[1] = 1.5f;
  b[2] = 1.6f;

  auto res = a + b;

  if(AreSame(res[0], 2.5f) && AreSame(res[1], 2.7f) && AreSame(res[2], 2.9f))
    return 0;

  return 1;
}

// Simple vector subtraction
int Test3()
{
  std::cout << "Subtracting {1.4, 1.5, 1.6} - {1.1, 1.2, 1.3} expecting {0.3, "
               "0.3, 0.3}\n";
  sm::Vector<double> a(3);
  a[0] = 1.4f;
  a[1] = 1.5f;
  a[2] = 1.6f;

  sm::Vector<double> b(3);
  b[0] = 1.1f;
  b[1] = 1.2f;
  b[2] = 1.3f;

  auto res = a - b;

  if(AreSame(res[0], 0.3) && AreSame(res[1], 0.3) && AreSame(res[2], 0.3))
    return 0;
  return 1;
}

// Test vector view
int Test4()
{
  sm::Vector<float> a(3);
  a[0] = 1.1f;
  a[1] = 1.2f;
  a[2] = 1.3f;

  std::cout << "Trying too get view into vector\n";
  const sm::VectorView b(a);

  if(!AreSame(b[1], 1.2f)) return 1;

  std::cout << "Modifying vector\n";
  a[1] = 25.0f;

  std::cout << "Checking if view is modified\n";
  if(!AreSame(b[1], 25.0f)) return 1;

  return 0;
}

// Matrix view test
int Test5()
{
  sm::Matrix<float> test(4, 4);

  test[0][0] = 1.0f;
  test[0][2] = 33.0f;
  test[3][3] = 25.0f;
  test[3][0] = 13.0f;

  std::cout << "Getting row view\n";
  auto viewRow = test.GetRow(0);
  if(!AreSame(viewRow[0], 1.0f) || !AreSame(viewRow[2], 33.0f)) return 1;

  std::cout << "Getting column view\n";
  auto viewColumn = test.GetColumn(0);
  if(!AreSame(viewColumn[0], 1.0f) || !AreSame(viewColumn[3], 13.0f)) return 1;

  return 0;
}

// Matrices addition test
int Test6()
{
  sm::Matrix<float> a(4, 4);
  a[0][0] = 1.0f;
  a[2][2] = 2.0f;
  a[3][2] = 4.0f;
  a[2][3] = 5.0f;

  sm::Matrix<float> b(4, 4);
  b[1][1] = 1.0f;
  b[2][2] = 2.0f;
  b[3][2] = 4.0f;
  b[2][3] = 5.0f;

  std::cout << "Adding two matrices 4x4\n";
  sm::Matrix<float> c = a + b;

  if(AreSame(c[0][0], 1.0f) && AreSame(c[1][1], 1.0f) && AreSame(c[2][2], 4.0f)
     && AreSame(c[3][2], 8.0f) && AreSame(c[2][3], 10.0f))
    return 0;

  return 1;
}

// Matrices multiplication test
int Test7()
{
  sm::Matrix<float> a(2, 2);
  a[0][0] = 1.0f;
  a[0][1] = 2.0f;
  a[1][0] = 3.0f;
  a[1][1] = 4.0f;

  sm::Matrix<float> b(2, 2);
  b[0][0] = 5.0f;
  b[0][1] = 6.0f;
  b[1][0] = 7.0f;
  b[1][1] = 8.0f;

  std::cout << "Multiplying matrices 2x2 and 2x2\n";
  sm::Matrix<float> c = a * b;

  if(!AreSame(c[0][0], 19.0f) || !AreSame(c[0][1], 22.0f)
     || !AreSame(c[1][0], 43.0f) || !AreSame(c[1][1], 50.0f))
    return 1;

  a[0][0] = 2.0f;
  a[0][1] = 3.0f;
  a[1][0] = 6.0f;
  a[1][1] = 5.0f;

  std::cout << "Multiplying matrices 2x2 and 3x2\n";
  sm::Matrix<float> d(2, 3);
  d[0][0] = 8.0f;
  d[0][1] = 9.0f;
  d[0][2] = 7.0f;
  d[1][0] = 5.0f;
  d[1][1] = 3.0f;
  d[1][2] = 5.0f;

  c = a * d;

  if(AreSame(c[0][0], 31.0f) && AreSame(c[0][1], 27.0f)
     && AreSame(c[0][2], 29.0f) && AreSame(c[1][0], 73.0f)
     && AreSame(c[1][1], 69.0f) && AreSame(c[1][2], 67.0f))
    return 0;

  return 1;
}

// Matrices * Vector test
int Test8()
{
  sm::Matrix<float> a(2, 2);
  a[0][0] = 5.0f;
  a[0][1] = 6.0f;
  a[1][0] = 7.0f;
  a[1][1] = 8.0f;

  sm::Vector<float> b(2);
  b[0] = 3.0f;
  b[1] = 4.0f;

  std::cout << "Testing matrix * vector (multiplication from right)\n";
  sm::Matrix<float> c = a * b;

  if(!AreSame(c[0][0], 39.0f) || !AreSame(c[1][0], 53.0f)) return 1;

  std::cout << "Testing vector * matrix (multiplication from left)\n";
  c = b * a;

  if(!AreSame(c[0][0], 43.0f) || !AreSame(c[0][1], 50.0f)) return 1;

  return 0;
}

int main(const int argc, char** argv)
{
  // No enough arguments
  if(argc == 1) return 1;

  switch(const int testNumber = std::stoi(argv[1])) {
  case 1:
    return Test1();
  case 2:
    return Test2();
  case 3:
    return Test3();
  case 4:
    return Test4();
  case 5:
    return Test5();
  case 6:
    return Test6();
  case 7:
    return Test7();
  case 8:
    return Test8();
  default:
    std::cout << "No test with number " << testNumber << "\n";
    return 1;
  }
}
