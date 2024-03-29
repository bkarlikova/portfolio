cmake_minimum_required(VERSION 3.27)
project(HW01)

set(CMAKE_CXX_STANDARD 14)

include_directories(src)

file(GLOB SRC_FILES src/*.h src/*.cpp)
add_executable(HW01 ${SRC_FILES})
