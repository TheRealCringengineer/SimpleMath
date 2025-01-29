cmake_minimum_required(VERSION 3.16)
project(SimpleMath)

set(CMAKE_CXX_STANDARD 17)

if(NOT CMAKE_BUILD_TYPE)
  set(default_build_type "Debug")
  message(STATUS "Set the build type to `${default_build_type}` as none was specified.")
  set(CMAKE_BUILD_TYPE ${default_build_type} CACHE STRING "Chooce the build type." FORCE)
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS
  "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

message(STATUS "Build Type: ${CMAKE_BUILD_TYPE}")

add_executable(${PROJECT_NAME} main.cpp)

include(CTest)
enable_testing()
add_executable(Tests test.cpp)
add_test(NAME VectorsCreation COMMAND Tests 1)
add_test(NAME VectorsAddition COMMAND Tests 2)
add_test(NAME VectorsSubtraction COMMAND Tests 3)
add_test(NAME VectorsView COMMAND Tests 4)
add_test(NAME MatrixView COMMAND Tests 5)
add_test(NAME MatricesAdd COMMAND Tests 6)
add_test(NAME MatricesMul COMMAND Tests 7)
add_test(NAME MatrixVectorMul COMMAND Tests 8)
