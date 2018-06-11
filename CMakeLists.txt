cmake_minimum_required(VERSION 3.10)
project(focusStacking)

set(CMAKE_CXX_STANDARD 11)

add_subdirectory(googletest)

find_package(Threads REQUIRED)
find_package(OpenCV REQUIRED)

enable_testing()

include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

file(GLOB SOURCES "src/.h" "src/*.cpp")
file(GLOB TEST_SOURCES "test/.h" "test/*.cpp")

add_executable(focusStacking main.cpp ${SOURCES})
add_executable(runAllTests tests.cpp ${TEST_SOURCES} ${SOURCES})

target_link_libraries(focusStacking ${OpenCV_LIBS})
target_link_libraries(runAllTests gtest gtest_main gmock gmock_main)

add_test(AllTests runAllTests)

if (APPLE)
    add_definitions(-DGTEST_USE_OWN_TR1_TUPLE)
    add_definitions(-D__GLIBCXX__)
endif(APPLE)