#pragma once
#include <vector>
#include <random>
#include "miningTruck.h"
#include "unloadSite.h"

class SimulationController
{
public:
    SimulationController();
    ~SimulationController();

    void setSimulationTrucks(std::vector<MiningTruck> &truckPool, int amtTrucks);
    void setSimulationUnloadSites(std::vector<UnloadSite> &sitePool, int amtSites);

    // Dependency inject this where needed
    int randomInt(int min, int max); // Make this here to prevent having to create a global srand, plus this file is already doing sim orchestration

private:
    std::mt19937 m_rng;
};
