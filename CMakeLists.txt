cmake_minimum_required(VERSION 3.8)
project(yadro_test)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(TASK_SRC
        src/Parse.cpp
        src/Time.cpp
        src/Event.cpp
        src/Model.cpp)

set(TEST_SRC
        test/TestTime.cpp
        test/TestEvent.cpp
        test/TestParse.cpp
        test/TestModel.cpp)

add_executable(task src/main.cpp ${TASK_SRC})
target_include_directories(task PUBLIC include)

target_compile_options(task PUBLIC -pedantic -Wall -Werror)
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    message(NOTICE "Adding debug options...")
    target_compile_options(task PUBLIC -fsanitize=address -O0 -g)
    target_link_options(task PUBLIC -fsanitize=address)
endif ()


# GTest:
include(FetchContent)
FetchContent_Declare(
        googletest
        GIT_REPOSITORY "https://github.com/google/googletest"
        GIT_TAG v1.13.0
)
FetchContent_MakeAvailable(googletest)

add_executable(test ${TASK_SRC} ${TEST_SRC})
target_link_libraries(test PUBLIC gtest_main)
target_include_directories(test PUBLIC include)

include(GoogleTest)
gtest_discover_tests(test)

