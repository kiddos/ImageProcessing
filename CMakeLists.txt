cmake_minimum_required(VERSION 2.8)

project(ImageProcessing)

add_subdirectory(src)

if (BUILD_TEST)
  enable_testing()
  add_subdirectory(test)
endif ()
