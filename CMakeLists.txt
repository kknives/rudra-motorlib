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
add_library(rml lib/sabertooth.cpp lib/smartelex.cpp)

# add_subdirectory(dist/rapidcheck)
# add_library(my_library src/my_library.cpp)

# find_package(Catch2 3 REQUIRED)
# add_executable(tests tests/sabertooth.cpp)
# target_link_libraries(tests PRIVATE Catch2::Catch2WithMain)
