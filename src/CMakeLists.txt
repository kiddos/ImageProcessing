# cpp flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -O3 -std=c++11")
if (NOT BUILD_TYPE)
  set(BUILD_TYPE DEBUG)
endif ()
if (BUILD_TYPE EQUAL DEBUG)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
endif()
# look for openmp
find_package(OpenMP)

# sources
file(GLOB_RECURSE SOURCE *.h *.cpp)

include_directories(${CMAKE_SOURCE_DIR}/src)
add_library(ImageProcessing ${SOURCE})

if (OPENMP_FOUND)
  target_compile_options(ImageProcessing PRIVATE -DUSE_OPENMP -fopenmp)
endif ()
