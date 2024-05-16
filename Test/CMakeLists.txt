include(FetchContent)
FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG v1.14.0
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()
add_executable(Test
        vector_test.cpp
        scalar_test.cpp
        matrix_test.cpp
)

target_link_libraries(Test
        Engine
        GTest::gtest_main
)

target_include_directories(Test
        PUBLIC
        ../Engine
)

include(GoogleTest)
gtest_discover_tests(Test)