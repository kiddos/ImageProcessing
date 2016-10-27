set(CMAKE_CXX_STANDARD 11)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

include_directories(${CMAKE_SOURCE_DIR}/src)
link_directories(${CMAKE_BINARY_DIR}/src)
add_executable(demo_tpcsltp demo_tpcsltp.cpp)
target_link_libraries(demo_tpcsltp ImageProcessing opencv_core opencv_highgui opencv_imgcodecs)


set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp -pthread")
find_package(OpenMP)
if (OPENMP_FOUND)
  target_compile_options(ImageProcessing PRIVATE -fopenmp -pthread)
endif ()
