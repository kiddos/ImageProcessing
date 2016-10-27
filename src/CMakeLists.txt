include(${CMAKE_SOURCE_DIR}/cmake/find_dependencies.cmake)
# sources
file(GLOB_RECURSE SOURCE *.h *.cpp)
add_library(ImageProcessing ${SOURCE})
