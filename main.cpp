#include "unloadSite.h"
#include "miningTruck.h"
#include "simulationController.h"
#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <algorithm>

int main(int argc, char *argv[])
{
    //-------------------------------------------------------------------------------------------
    // Accept optional CLI args: ./MiningSimulation <num_trucks> <num_sites>
    // Falls back to defaults if not provided
    //-------------------------------------------------------------------------------------------
    const int NUM_TRUCKS = (argc > 1) ? std::stoi(argv[1]) : 5;
    const int NUM_SITES = (argc > 2) ? std::stoi(argv[2]) : 3;
    const int SIM_DURATION = 4320; // 72 hours in minutes
    const int TRAVEL_TIME = 30;
    const int UNLOAD_TIME = 5;
    const int MIN_MINE_TIME = 60, MAX_MINE_TIME = 300;

    SimulationController simulationController;

    //-------------------------------------------------------------------------------------------
    // Set up our pools — vectors own the objects by value, no heap allocation needed after setup
    // simulationController owns setting up and creating our objects
    // We know how many trucks and sites there are on runtime, so we need no dynamic allocations
    //-------------------------------------------------------------------------------------------
    std::vector<MiningTruck> truckPool;
    std::vector<UnloadSite> sitePool;
    simulationController.setSimulationTrucks(truckPool, NUM_TRUCKS);
    simulationController.setSimulationUnloadSites(sitePool, NUM_SITES);

    //-------------------------------------------------------------------------------------------
    // These will be our events that will go onto our priority queue
    // Define it by enum and with a struct because the event struct will be the one on the queue
    //-------------------------------------------------------------------------------------------
    enum class QueueEvent
    {
        DONE_MINING = 0,
        ARRIVED_AT_STATION = 1,
        DONE_UNLOADING = 2,
        ARRIVED_AT_MINE = 3
    };

    struct Event
    {
        int time;
        QueueEvent type;
        int truckID;
        int stationID = -1;
    };

    //-------------------------------------------------------------------------------------------
    // We want to set up our custom comparator for events
    // Also set up the pq for storing our events for the sim loop
    // Seed the queue with trucks starting at t = 0 since "trucks start at mine sites"
    //-------------------------------------------------------------------------------------------
    struct EventComparator
    {
        bool operator()(const Event &a, const Event &b) const { return a.time > b.time; }
    };

    std::priority_queue<Event, std::vector<Event>, EventComparator> eventQueue;

    for (MiningTruck &currTruck : truckPool)
    {
        int mineTime = simulationController.randomInt(MIN_MINE_TIME, MAX_MINE_TIME);
        currTruck.goMining();
        currTruck.recordMining(mineTime);                                        // Since we seed the mine time at start of mine time, could be edge case where at end of sim a trucks mine time is off
        eventQueue.push({mineTime, QueueEvent::DONE_MINING, currTruck.getID()}); // We seed it with the event trigger so in the pq when the next event pops we know
    }

    //-------------------------------------------------------------------------------------------
    // Station entry — same pattern as Event
    // freeAt is the sim minute when this station finishes its current backlog
    // Top of the PQ is always the station that clears soonest
    //-------------------------------------------------------------------------------------------
    struct StationEntry
    {
        int freeAt;
        int siteIdx;
    };

    struct StationComparator
    {
        bool operator()(const StationEntry &a, const StationEntry &b) const { return a.freeAt > b.freeAt; }
    };

    std::priority_queue<StationEntry, std::vector<StationEntry>, StationComparator> stationQueue;

    for (int i = 0; i < NUM_SITES; i++)
        stationQueue.push({0, i}); // All stations free at t=0

    //-------------------------------------------------------------------------------------------
    // Our main sim loop, go event by event
    //-------------------------------------------------------------------------------------------
    while (!eventQueue.empty())
    {
        Event currEvent = eventQueue.top();
        eventQueue.pop();

        if (currEvent.time >= SIM_DURATION)
            break; // Base case

        MiningTruck &currTruck = truckPool[currEvent.truckID]; // Find the current truck on the pq

        switch (currEvent.type) // Go and fulfill current event type
        {
        case QueueEvent::DONE_MINING: // When done mining the flow should be set to transit -> record -> push arrival to queue
            currTruck.setStatus(TruckState::IN_TRANSIT_TO_UNLOAD);
            currTruck.recordTransit(TRAVEL_TIME);
            eventQueue.push({currEvent.time + TRAVEL_TIME, QueueEvent::ARRIVED_AT_STATION, currEvent.truckID});
            break;

        case QueueEvent::ARRIVED_AT_STATION:
        {
            StationEntry entry = stationQueue.top();
            stationQueue.pop(); // Grab the soonest-free station
            int nextAvailSite = entry.siteIdx;
            UnloadSite &site = sitePool[nextAvailSite];
            bool wasIdle = entry.freeAt <= currEvent.time;
            site.addToQueue(currEvent.truckID);
            int newFreeAt;
            if (wasIdle)
            {
                site.recordIdleMinutes(currEvent.time - entry.freeAt);
                site.moveQueueForward();
                currTruck.setStatus(TruckState::UNLOADING);
                newFreeAt = currEvent.time + UNLOAD_TIME;
            }
            else
            {
                currTruck.setStatus(TruckState::WAITING_FOR_UNLOAD);
                currTruck.recordWaiting(entry.freeAt - currEvent.time);
                newFreeAt = entry.freeAt + UNLOAD_TIME;
            }
            currTruck.recordUnloading(UNLOAD_TIME);
            stationQueue.push({newFreeAt, nextAvailSite}); // Push back with updated time
            eventQueue.push({newFreeAt, QueueEvent::DONE_UNLOADING, currEvent.truckID, nextAvailSite});
            break;
        }

        case QueueEvent::DONE_UNLOADING:
            currTruck.recordLoad();
            sitePool[currEvent.stationID].recordTruckServed();
            sitePool[currEvent.stationID].moveQueueForward();
            currTruck.setStatus(TruckState::IN_TRANSIT_TO_MINE);
            currTruck.recordTransit(TRAVEL_TIME);
            eventQueue.push({currEvent.time + TRAVEL_TIME, QueueEvent::ARRIVED_AT_MINE, currEvent.truckID});
            break;

        case QueueEvent::ARRIVED_AT_MINE:
            int mineTime = simulationController.randomInt(MIN_MINE_TIME, MAX_MINE_TIME);
            currTruck.goMining();
            currTruck.recordMining(std::min(currEvent.time + mineTime, SIM_DURATION) - currEvent.time);
            if (currEvent.time + mineTime < SIM_DURATION)
            {
                eventQueue.push({currEvent.time + mineTime, QueueEvent::DONE_MINING, currEvent.truckID});
            }
            break;
        }
    }

    //-------------------------------------------------------------------------------------------
    // Flush our the station queue and record remaining idle minutes at end of sim
    //-------------------------------------------------------------------------------------------
    while (!stationQueue.empty())
    {
        StationEntry remaining = stationQueue.top();
        stationQueue.pop();
        if (SIM_DURATION - remaining.freeAt > 0)
            sitePool[remaining.siteIdx].recordIdleMinutes(SIM_DURATION - remaining.freeAt);
    }

    //-------------------------------------------------------------------------------------------
    // Print out all stats of all sites and trucks
    //-------------------------------------------------------------------------------------------
    for (auto &truck : truckPool)
        truck.printStats(SIM_DURATION);
    for (auto &site : sitePool)
        site.printStats(SIM_DURATION);

    return 0;
}
