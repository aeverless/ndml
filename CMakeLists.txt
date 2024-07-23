cmake_minimum_required(VERSION 3.26)

project(ndml VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wpedantic -Wall -Werror")

add_library(${CMAKE_PROJECT_NAME} INTERFACE)
target_include_directories(${CMAKE_PROJECT_NAME} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)

set_target_properties(${CMAKE_PROJECT_NAME} PROPERTIES PUBLIC_HEADER include/ndml/ndml.hpp)
