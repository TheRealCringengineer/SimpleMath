#include <chrono>

#include "Matrix.h"
#include "Vector.h"
#include <iostream>
#include <thread>

class Timer
{
public:
  explicit Timer(const std::string& task)
  {
    startPoint = std::chrono::high_resolution_clock::now();
  }

  ~Timer() { Stop(); }

  void Stop() const
  {
    auto endPoint = std::chrono::high_resolution_clock::now();

    auto start =
        std::chrono::time_point_cast<std::chrono::microseconds>(startPoint)
            .time_since_epoch()
            .count();

    auto end = std::chrono::time_point_cast<std::chrono::microseconds>(endPoint)
                   .time_since_epoch()
                   .count();

    auto duration = end - start;
    std::cout << taskName << " took " << duration << "us\n";
  }

private:
  std::chrono::time_point<std::chrono::high_resolution_clock> startPoint;
  std::string taskName;
};

sm::Matrix<float> GenerateBigMatrix(const size_t rows, const size_t cols)
{
  sm::Matrix<float> matrix(rows, cols);

  for(size_t i = 0; i < rows; i++) {
    for(size_t j = 0; j < cols; j++) {
      matrix[i][j] = static_cast<float>(rand()) / RAND_MAX * 100.0f;
    }
  }

  return matrix;
}

template<typename T>
sm::Matrix<T> MultithreadMultiply(const sm::Matrix<T>& a,
                                  const sm::Matrix<T>& b,
                                  size_t threadsCount = 1)
{
  if(a.GetColumns() != b.GetRows())
    throw std::invalid_argument("Matrix size mismatch");

  sm::Matrix<T> result(a.GetRows(), b.GetColumns());

  size_t elementsCount = a.GetRows() * b.GetColumns();

  std::vector<std::thread> threads;
  threads.reserve(threadsCount);
  for(size_t i = 0; i < threadsCount; i++) {

    // There is no need for synchronization since all cells in result are processed independently (no concurrency on write)
    // Also matrices a and b here are read only. So, we should be fine
    threads.emplace_back(
        std::thread([i, &a, &b, &result, elementsCount, threadsCount]() {
          for(size_t j = i; j < elementsCount; j += threadsCount) {
            int x = j / a.GetRows();
            int y = j % b.GetColumns();

            for(size_t k = 0; k < a.GetColumns(); k++) {
              result[y][x] += a[y][k] * b[k][x];
            }
          }
        }));
  }

  for(auto& thread: threads) { thread.join(); }

  return result;
}

int main(const int argc, char* argv[])
{
  int threadsCount = 0;
  int matricesSize = 0;
  if(argc == 1) {
    std::cout << "Start ./SimpleMath <matrices_size> <number_of_threads>"
              << std::endl;
    return 1;
  }
  if(argc < 3) {
    std::cerr << "Wrong number of arguments" << std::endl;
    return 1;
  }

  matricesSize = std::stoi(argv[1]);
  threadsCount = std::stoi(argv[2]);

  const auto a = GenerateBigMatrix(matricesSize, matricesSize);
  const auto b = GenerateBigMatrix(matricesSize, matricesSize);

  sm::Matrix<float> resultA;
  sm::Matrix<float> resultB;
  {
    Timer t("Matrix multiplication seq");
    resultA = a * b;
  }

  {
    Timer t("Matrix multiplication parallel");
    resultB = MultithreadMultiply(a, b, threadsCount);
  }

  for(size_t i = 0; i < resultA.GetRows(); i++) {
    for(size_t j = 0; j < resultB.GetColumns(); j++) {
      if(resultA[i][j] != resultB[i][j]) {
        std::cerr << "Matrix multiplication failed!" << std::endl;
        return 1;
      }
    }
  }

  return 0;
}
