cmake_minimum_required(VERSION 3.8)
project(yadro_test)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CPP_SRC main.cpp Time.cpp Event.cpp Model.cpp)

add_executable(task ${CPP_SRC})

add_compile_options(task -Wall -Werror)
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    message(NOTICE "Adding debug options...")
    add_compile_options(-fsanitize=address -O0 -g)
endif ()
