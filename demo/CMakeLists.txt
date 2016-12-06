include(${CMAKE_SOURCE_DIR}/cmake/find_dependencies.cmake)

find_package(OpenCV REQUIRED COMPONENTS core imgcodecs highgui videoio imgproc)

link_directories(${CMAKE_BINARY_DIR}/src)
add_executable(demo_tpcsltp demo_tpcsltp.cpp)
target_link_libraries(demo_tpcsltp ImageProcessing ${OpenCV_LIBS})

add_executable(demo_pixel demo_pixel.cpp)
target_link_libraries(demo_pixel ImageProcessing ${OpenCV_LIBS})

add_executable(demo_mixup mixup.cpp)
target_link_libraries(demo_mixup ImageProcessing ${OpenCV_LIBS})
