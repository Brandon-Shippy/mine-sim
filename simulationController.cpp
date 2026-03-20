#include "simulationController.h"
#include <chrono>

SimulationController::SimulationController()
    // Seed with steady_clock so each run produces different mining durations.
    // Swap in a fixed integer (e.g. m_rng(42)) for reproducible test runs.
    : m_rng(static_cast<unsigned>(std::chrono::steady_clock::now().time_since_epoch().count()))
{
}

SimulationController::~SimulationController() {}

//-------------------------------------------------------------------------------------------
// We need to use reserve here to prevent re-allocation during runtime
// If it gets realloacted all the references become dangling, so reserve gurantees us it wont change
//-------------------------------------------------------------------------------------------
void SimulationController::setSimulationTrucks(std::vector<MiningTruck> &truckPool, int amtTrucks)
{
    truckPool.reserve(amtTrucks);
    for (int i = 0; i < amtTrucks; i++)
    {
        // Set new truck based off of the iterator, then store inside our pool
        truckPool.emplace_back(i, TruckState::MINING);
    }
}

void SimulationController::setSimulationUnloadSites(std::vector<UnloadSite> &sitePool, int amtSites)
{
    sitePool.reserve(amtSites);
    for (int i = 0; i < amtSites; i++)
    {
        // Set new site based off of the iterator, then store inside our pool
        std::string currSiteID = "site " + std::to_string(i);
        sitePool.emplace_back(currSiteID);
    }
}

int SimulationController::randomInt(int min, int max)
{
    std::uniform_int_distribution<int> dist(min, max);
    return dist(m_rng);
}
