cmake_minimum_required(VERSION 4.3.2)

project(ndml VERSION 3.0.0 LANGUAGES CXX)

include(CTest)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_MODULE_STD ON)

add_compile_options(-O3 -Wpedantic -Wall -Werror)

if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
	message(STATUS "Compiling with Clang. Suppressing `c2y-extensions` and `reserved-module-identifier` warnings.")
	add_compile_options(--warning-suppression-mappings=${PROJECT_SOURCE_DIR}/clang_suppressions.txt)
endif()

add_library(${PROJECT_NAME} STATIC)

target_sources(
	${PROJECT_NAME}
	PUBLIC
	FILE_SET CXX_MODULES
	FILES
		src/ndml.cppm

		src/vec/vec.cppm
		src/vec/types.cppm
		src/vec/operations.cppm

		src/mat/mat.cppm
		src/mat/types.cppm
		src/mat/operations.cppm
		src/mat/transforms.cppm

		src/quat/quat.cppm
		src/quat/types.cppm
		src/quat/operations.cppm
		src/quat/transforms.cppm

		src/meta/meta.cppm
		src/meta/burn.cppm
		src/meta/functional.cppm
)

if (BUILD_TESTING)
	add_executable(${PROJECT_NAME}-tests)

	target_compile_features(${PROJECT_NAME}-tests PRIVATE cxx_std_26)

	target_sources(
		${PROJECT_NAME}-tests
		PRIVATE
		FILE_SET CXX_MODULES
		FILES
			tests/common.cppm
			tests/vec.cppm
			tests/mat.cppm
			tests/quat.cppm
			tests/meta.cppm
	)

	target_link_libraries(${PROJECT_NAME}-tests PRIVATE ${PROJECT_NAME})

	add_subdirectory(thirdparty)

	catch_discover_tests(${PROJECT_NAME}-tests)
endif()
