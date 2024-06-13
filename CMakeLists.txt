cmake_minimum_required(VERSION 3.5)
project(MyRayTracer)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(OpenCV REQUIRED)

include_directories( ${OpenCV_INCLUDE_DIRS} )

add_executable(MyRayTracer main.cpp)

target_link_libraries(MyRayTracer ${OpenCV_LIBS})

