cmake_minimum_required(VERSION 3.16)
project(Module27_opp2_task_3)

set(CMAKE_CXX_STANDARD 11)

option(ENABLE_TESTING "Enable test emulation" OFF)

add_executable(Module27_opp2_task_3)

target_sources(Module27_opp2_task_3
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include/random_int_generator.h

        ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/random_int_generator.cpp
        )

target_include_directories(Module27_opp2_task_3
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        )

target_compile_definitions(Module27_opp2_task_3
        PRIVATE
        ENABLE_TESTING=$<BOOL:${ENABLE_TESTING}>
        )

