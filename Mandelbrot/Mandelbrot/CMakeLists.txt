cmake_minimum_required (VERSION 2.6)
project (MANDELBROT)

find_package (Threads)

add_executable( mandelbrot Main.cpp)

SET(CMAKE_CXX_FLAGS "-std=c++1y")

target_link_libraries (mandelbrot ${CMAKE_THREAD_LIBS_INIT})

