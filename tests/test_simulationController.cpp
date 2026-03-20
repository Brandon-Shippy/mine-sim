#include <gtest/gtest.h>
#include "../simulationController.h"

TEST(SimulationController, RandomIntAlwaysInBounds)
{
    SimulationController c;
    for (int i = 0; i < 1000; i++)
    {
        int v = c.randomInt(60, 300);
        EXPECT_GE(v, 60);
        EXPECT_LE(v, 300);
    }
}

TEST(SimulationController, RandomIntWorksForSingleValue)
{
    SimulationController c;
    EXPECT_EQ(c.randomInt(5, 5), 5);
}

TEST(SimulationController, SetSimulationTrucksCreatesCorrectCount)
{
    SimulationController c;
    std::vector<MiningTruck> truckPool;
    c.setSimulationTrucks(truckPool, 5);
    EXPECT_EQ((int)truckPool.size(), 5);
}

TEST(SimulationController, SetSimulationTrucksAssignsSequentialIDs)
{
    SimulationController c;
    std::vector<MiningTruck> truckPool;
    c.setSimulationTrucks(truckPool, 3);
    EXPECT_EQ(truckPool[0].getID(), 0);
    EXPECT_EQ(truckPool[1].getID(), 1);
    EXPECT_EQ(truckPool[2].getID(), 2);
}

TEST(SimulationController, SetSimulationUnloadSitesCreatesCorrectCount)
{
    SimulationController c;
    std::vector<UnloadSite> sitePool;
    c.setSimulationUnloadSites(sitePool, 3);
    EXPECT_EQ((int)sitePool.size(), 3);
}
