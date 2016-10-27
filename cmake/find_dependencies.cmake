# cpp flags
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -O3 -std=c++11")
if (NOT BUILD_TYPE)
  set(BUILD_TYPE DEBUG)
endif ()
if (BUILD_TYPE EQUAL DEBUG)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
endif()

include_directories(${CMAKE_SOURCE_DIR}/src)

# look for openmp
find_package(OpenMP)
if (OPENMP_FOUND)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp -pthread")
endif ()
