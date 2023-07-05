cmake_minimum_required(VERSION 3.25)
project(yadro_test)

set(CMAKE_CXX_STANDARD 20)

set(CPP_SRC main.cpp Time.cpp Event.cpp Model.cpp Model.hpp)

add_executable(yadro_test ${CPP_SRC})
