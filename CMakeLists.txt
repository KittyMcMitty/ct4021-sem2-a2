cmake_minimum_required(VERSION 3.15)
project(a_toolchain_test CXX)

set(CMAKE_CXX_STANDARD 17)

add_library(Radar libraries/Radar/radar.cc libraries/Radar/radar.h)

add_executable(a_toolchain_test src/main.cpp include/ArduinoInterface.h)
add_executable(unit_tests tests/test_radar.cc include/ArduinoInterface.h)

target_link_libraries(unit_tests gmock_main gtest Radar)

include_directories(libraries/Radar include)

if(U_UPLOAD)
    target_link_arduino_libraries(a_toolchain_test PRIVATE core)
    target_link_arduino_libraries(a_toolchain_test PRIVATE Wire)
    target_link_arduino_libraries(a_toolchain_test PRIVATE Servo)
    target_link_arduino_libraries(a_toolchain_test PRIVATE LiquidCrystal)
    target_link_arduino_libraries(a_toolchain_test PRIVATE Radar)

    target_enable_arduino_upload(a_toolchain_test)

endif()

if(UNIT_TEST)
    include(FetchContent)
    FetchContent_Declare(
            googletest
            GIT_REPOSITORY https://github.com/google/googletest.git
            GIT_TAG        release-1.10.0
    )
    FetchContent_MakeAvailable(googletest)

    add_compile_definitions(UNIT_TEST)
    add_test(test_runner COMMAND unit_tests)
endif()