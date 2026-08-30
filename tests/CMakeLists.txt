cmake_minimum_required(VERSION 3.20)
project(MyProject)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#add_executable(my_app
#    main.cpp
#)


# GoogleTest
include(FetchContent)

FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.17.0.zip
)

FetchContent_MakeAvailable(googletest)

# Tests
enable_testing()

add_executable(tests
    src/lexer/lexer-1.cpp
)

target_link_libraries(tests
    PRIVATE
    GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(tests)