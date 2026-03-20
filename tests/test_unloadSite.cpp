#include <gtest/gtest.h>
#include "../unloadSite.h"

TEST(UnloadSite, EmptyOnConstruct)
{
    UnloadSite s("site 0");
    EXPECT_TRUE(s.isSiteEmpty());
    EXPECT_EQ(s.getQueueLength(), 0);
    EXPECT_EQ(s.getCurrentTruckID(), -1);
}

TEST(UnloadSite, NotEmptyAfterTruckAdded)
{
    UnloadSite s("site 0");
    s.addToQueue(0);
    s.moveQueueForward();
    EXPECT_FALSE(s.isSiteEmpty());
    EXPECT_EQ(s.getCurrentTruckID(), 0);
}

TEST(UnloadSite, QueueLengthIncludesCurrentAndWaiting)
{
    UnloadSite s("site 0");
    s.addToQueue(0);
    s.moveQueueForward(); // truck 0 is current
    s.addToQueue(1);      // truck 1 waiting
    s.addToQueue(2);      // truck 2 waiting
    EXPECT_EQ(s.getQueueLength(), 3);
}

TEST(UnloadSite, MoveQueueForwardAdvancesCorrectly)
{
    UnloadSite s("site 0");
    s.addToQueue(0);
    s.addToQueue(1);
    s.moveQueueForward();
    EXPECT_EQ(s.getCurrentTruckID(), 0);
    s.moveQueueForward();
    EXPECT_EQ(s.getCurrentTruckID(), 1);
}

TEST(UnloadSite, BecomesIdleWhenQueueDrained)
{
    UnloadSite s("site 0");
    s.addToQueue(0);
    s.moveQueueForward();
    s.moveQueueForward(); // queue now empty
    EXPECT_TRUE(s.isSiteEmpty());
    EXPECT_EQ(s.getCurrentTruckID(), -1);
}

TEST(UnloadSite, StatsAccumulate)
{
    UnloadSite s("site 0");
    s.recordTruckServed();
    s.recordTruckServed();
    s.recordIdleMinutes(100);
    // No getters on stats yet — covered by printStats visually,
    // but recordIdleMinutes/recordTruckServed should not crash
}
