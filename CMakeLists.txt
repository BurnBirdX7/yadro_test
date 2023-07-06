cmake_minimum_required(VERSION 3.25)
project(yadro_test)

set(CMAKE_CXX_STANDARD 20)
set(CPP_SRC main.cpp Time.cpp Event.cpp Model.cpp)

add_executable(task ${CPP_SRC})

add_compile_options(task -Wall -Werror -fsanitize=address)
