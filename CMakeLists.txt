cmake_minimum_required(VERSION 3.15)
project(a_toolchain_test CXX)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -Wall")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O2 -Wall")

# Radar is just a template header so no build target to link against
# add_library(Radar libraries/Radar/radar.h)
# set_target_properties(Radar PROPERTIES LINKER_LANGUAGE CXX)

add_library(
        MyLED
        libraries/MyLED/MyLED.cpp
        libraries/MyLED/MyLED.h
)

add_library(
        CommandQueue
        libraries/CommandQueue/CommandQueue.cc
        libraries/CommandQueue/CommandQueue.h
)

add_library(
        LinkedList
        libraries/LinkedList/LinkedList.cc
        libraries/LinkedList/LinkedList.h
)

add_executable(a_toolchain_test src/main.cpp include/ArduinoInterface.h include/new.h src/new.cc)
add_executable(unit_tests tests/test_radar.cc tests/test_led.cc tests/test_linked_list.cc tests/test_command_queue.cc)

target_link_libraries(unit_tests gmock_main gtest MyLED CommandQueue)

include_directories(
        include libraries/Radar libraries/MyLED libraries/LiquidCrystal/src
        libraries/LinkedList libraries/CommandQueue
        cmake-build-debug/_deps/googletest-src/googlemock/include
        cmake-build-debug/_deps/googletest-src/googletest/include
)



target_compile_options(a_toolchain_test PUBLIC "$<$<CONFIG:DEBUG>:${DEBUG_OPTIONS}>")
target_compile_options(a_toolchain_test PUBLIC "$<$<CONFIG:RELEASE>:${RELEASE_OPTIONS}>")

if(U_UPLOAD)
    target_link_arduino_libraries(a_toolchain_test PRIVATE core)
    target_link_arduino_libraries(a_toolchain_test PRIVATE Wire)
    target_link_arduino_libraries(a_toolchain_test PRIVATE Servo)
    target_link_arduino_libraries(a_toolchain_test PRIVATE LiquidCrystal)
    target_link_arduino_libraries(a_toolchain_test PRIVATE MyLED)
    target_link_arduino_libraries(a_toolchain_test PRIVATE CommandQueue)
    # target_link_arduino_libraries(a_toolchain_test PRIVATE Radar)

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