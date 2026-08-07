include(FetchContent)

FetchContent_Declare(
	Catch2
        GIT_REPOSITORY "https://github.com/catchorg/Catch2.git"
        GIT_TAG "v3.15.3"
)

add_subdirectory(catch2)

target_link_libraries(${PROJECT_NAME}-tests PRIVATE Catch2::Catch2WithMain)
