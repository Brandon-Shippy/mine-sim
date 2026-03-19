#pragma once
#include <queue>
#include <string>

class UnloadSite
{
public:
    UnloadSite(std::string siteID);
    ~UnloadSite();
    std::string getID() const;

    bool isSiteEmpty();
    int getQueueLength();
    int getCurrentTruckID();
    void addToQueue(int id);
    void moveQueueForward();

    void recordTruckServed();
    void recordIdleMinutes(int minutes);
    void printStats(int totalSimMinutes);

private:
    // We could store our currTrucks 5 min counter in here, but it might be better to keep it in the main loop
    // Benefit why it might be better here is because of the fact we need to keep a running tab on which queue is the shortest
    const std::string m_siteID;
    std::queue<int> m_queue;
    int m_currTruckID = -1; // -1 means no truck currently unloading
    int m_trucksServed = 0;
    int m_minutesIdle = 0;
};
