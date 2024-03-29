cmake_minimum_required(VERSION 3.23)
project(SEMESTRAL)

set(CMAKE_CXX_STANDARD 17)

add_definitions("-Wall" "-g" "-o" "-std=c++11" "-lncurses")

include_directories(src)
include_directories(libs)
file(GLOB SRC_FILES src/*.h src/*.cpp src/*.hpp)
add_executable(SEMESTRAL ${SRC_FILES})

