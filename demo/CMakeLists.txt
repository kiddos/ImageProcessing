include(${CMAKE_SOURCE_DIR}/cmake/find_dependencies.cmake)

find_package(OpenCV REQUIRED COMPONENTS core imgcodecs highgui)

link_directories(${CMAKE_BINARY_DIR}/src)
add_executable(demo_tpcsltp demo_tpcsltp.cpp)
target_link_libraries(demo_tpcsltp ImageProcessing ${OpenCV_LIBS})

