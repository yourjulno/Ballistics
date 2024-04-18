cmake_minimum_required(VERSION 3.22.1)
project(ballistics2023)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(src)
add_subdirectory(third_party)

message("TESTING: ${WITH_TESTS}")


    enable_testing()
    add_subdirectory(tests)


