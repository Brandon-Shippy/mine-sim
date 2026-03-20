#include <gtest/gtest.h>
#include "../miningTruck.h"

TEST(MiningTruck, StartsWithCorrectIDAndState)
{
    MiningTruck t(3, TruckState::MINING);
    EXPECT_EQ(t.getID(), 3);
    EXPECT_EQ(t.getStatus(), TruckState::MINING);
}

TEST(MiningTruck, SetStatusUpdatesState)
{
    MiningTruck t(0, TruckState::MINING);
    t.setStatus(TruckState::IN_TRANSIT_TO_UNLOAD);
    EXPECT_EQ(t.getStatus(), TruckState::IN_TRANSIT_TO_UNLOAD);
}

TEST(MiningTruck, GoMiningSetsStatusToMining)
{
    MiningTruck t(0, TruckState::UNLOADING);
    t.goMining();
    EXPECT_EQ(t.getStatus(), TruckState::MINING);
}

TEST(MiningTruck, StatsStartAtZero)
{
    MiningTruck t(0, TruckState::MINING);
    EXPECT_EQ(t.getLoadsCompleted(), 0);
    EXPECT_EQ(t.getMinutesMining(), 0);
    EXPECT_EQ(t.getMinutesTransit(), 0);
    EXPECT_EQ(t.getMinutesWaiting(), 0);
    EXPECT_EQ(t.getMinutesUnloading(), 0);
}

TEST(MiningTruck, StatsAccumulateCorrectly)
{
    MiningTruck t(0, TruckState::MINING);
    t.recordMining(180);
    t.recordTransit(30);
    t.recordWaiting(10);
    t.recordUnloading(5);
    t.recordLoad();

    EXPECT_EQ(t.getMinutesMining(), 180);
    EXPECT_EQ(t.getMinutesTransit(), 30);
    EXPECT_EQ(t.getMinutesWaiting(), 10);
    EXPECT_EQ(t.getMinutesUnloading(), 5);
    EXPECT_EQ(t.getLoadsCompleted(), 1);
}

TEST(MiningTruck, StatsAccumulateAcrossMultipleCalls)
{
    MiningTruck t(0, TruckState::MINING);
    t.recordMining(100);
    t.recordMining(80);
    EXPECT_EQ(t.getMinutesMining(), 180);
}
