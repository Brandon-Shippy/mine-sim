cmake_minimum_required(VERSION 3.14)
project(MiningSimulation)

set(CMAKE_CXX_STANDARD 17)

add_executable(MiningSimulation
    main.cpp
    simulationController.cpp
    miningTruck.cpp
    unloadSite.cpp
)

# ---- Tests ----
include(FetchContent)
FetchContent_Declare(googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(SimTests
    tests/test_miningTruck.cpp
    tests/test_unloadSite.cpp
    tests/test_simulationController.cpp
    miningTruck.cpp
    unloadSite.cpp
    simulationController.cpp)

target_link_libraries(SimTests gtest_main)

include(GoogleTest)
gtest_discover_tests(SimTests)