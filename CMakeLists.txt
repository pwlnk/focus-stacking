cmake_minimum_required(VERSION 3.10)
project(focusStacking)

set(CMAKE_CXX_STANDARD 11)

add_subdirectory(googletest)

find_package(Threads REQUIRED)
enable_testing()
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

file(GLOB SOURCES "src/.h" "src/*.cpp")
file(GLOB TEST_SOURCES "tests/*.cpp")
add_executable(runAllTests tests.cpp ${TEST_SOURCES} ${SOURCES})
add_executable(focusStacking main.cpp ${SOURCES})

target_link_libraries(runAllTests gtest gtest_main)
add_test(AllTests runAllTests)