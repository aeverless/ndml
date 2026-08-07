cmake_minimum_required(VERSION 3.31.6)

project(ndml VERSION 2.0.1 LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -Wpedantic -Wall -Werror --warning-suppression-mappings=${PROJECT_SOURCE_DIR}/suppression_mappings.txt")

include_directories(include)

add_library(${PROJECT_NAME} INTERFACE)
set_target_properties(${PROJECT_NAME} PROPERTIES PUBLIC_HEADER include/ndml/ndml.hpp)

add_executable(${PROJECT_NAME}-tests tests/vec.cpp tests/mat.cpp tests/quat.cpp)
target_compile_features(${PROJECT_NAME}-tests PRIVATE cxx_std_23)
set_target_properties(${PROJECT_NAME}-tests PROPERTIES EXCLUDE_FROM_ALL TRUE)

add_test(NAME tests COMMAND ${PROJECT_NAME}-tests)

add_subdirectory(thirdparty)

catch_discover_tests(${PROJECT_NAME}-tests)
