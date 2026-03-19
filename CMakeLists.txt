cmake_minimum_required(VERSION 3.10)
project(MiningSimulation)

add_executable(MiningSimulation
    main.cpp
    simulationController.cpp
    miningTruck.cpp
    unloadSite.cpp
)