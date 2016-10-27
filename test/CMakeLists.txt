# cpp flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -g -O3 -std=c++11")
set(BUILD_TYPE DEBUG)
set(UNIT_TEST_LIB ImageProcessing)

find_package(Boost REQUIRED unit_test_framework)
find_package(OpenCV REQUIRED core imgproc highgui imgcodecs)

if (Boost_FOUND AND OpenCV_FOUND)
  include_directories(${CMAKE_SOURCE_DIR}/src)
  include_directories(${CMAKE_BINARY_DIR}/src)

  add_executable(test_util test_util.cpp)
  target_link_libraries(test_util ${Boost_LIBRARIES})

  add_executable(test_colorspace test_colorspace.cpp)
  add_custom_command(TARGET test_colorspace PRE_BUILD
    COMMAND python ${CMAKE_SOURCE_DIR}/script/download_image.py)
  target_link_libraries(test_colorspace ${Boost_LIBRARIES} ${OpenCV_LIBS}
    ${UNIT_TEST_LIB})

  add_test(test_util ./test_util)
  add_test(test_colorspace ./test_colorspace)
endif ()

find_package(OpenMP)
target_compile_options(ImageProcessing PRIVATE -DUSE_OPENMP -fopenmp)
