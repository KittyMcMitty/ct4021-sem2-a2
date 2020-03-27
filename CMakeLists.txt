cmake_minimum_required(VERSION 3.15)
project(a_toolchain_test CXX)

set(CMAKE_CXX_STANDARD 14)

include(FetchContent)
FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG        release-1.8.0
)
FetchContent_MakeAvailable(googletest)

add_executable(a_toolchain_test src/main.cpp)
add_executable(unit_tests tests/test_test.cpp)

add_library(SomeClass libraries/SomeClass/SomeClass.cpp libraries/SomeClass/SomeClass.h)

# target_link_libraries(a_toolchain_test SomeClass)
target_link_libraries(unit_tests gmock_main gtest SomeClass)

include_directories(libraries/SomeClass)

if(U_UPLOAD)
    target_link_arduino_libraries(a_toolchain_test PRIVATE core)
    target_link_arduino_libraries(a_toolchain_test PRIVATE SomeClass)
endif()

if(UNIT_TEST)
    add_test(test_runner COMMAND unit_tests)
endif()