cmake_minimum_required(VERSION 3.5)
project(rudra-motorlib)

# Default to C++14
if(NOT CMAKE_CXX_STANDARD)
 set(CMAKE_CXX_STANDARD 17)
 set(CMAKE_CXX_STANDARD_REQUIRED ON)
endif()

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
# Include our own headers
include_directories(include)
add_library(rml SHARED lib/smartelex.cpp)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)
enable_testing()

add_executable(
  test_sabertooth
  tests/test_sabertooth.cpp
)
target_link_libraries(
  test_sabertooth
  rml
  GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(test_sabertooth)
# find_package(Catch2 3 REQUIRED)
# add_executable(tests tests/sabertooth.cpp)
# target_link_libraries(tests PRIVATE Catch2::Catch2WithMain)
