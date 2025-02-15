cmake_minimum_required(VERSION 3.27)
project(lost_agent_localization)

set(CMAKE_CXX_STANDARD 20)

include_directories(src)

file(GLOB SRC_FILES src/*.h src/*.cpp)
add_executable(lost_agent_localization ${SRC_FILES})
