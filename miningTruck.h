#pragma once

enum class TruckState
{
    MINING               = 0,
    IN_TRANSIT_TO_UNLOAD = 1,
    WAITING_FOR_UNLOAD   = 2, // Waiting in line at a station
    UNLOADING            = 3,
    IN_TRANSIT_TO_MINE   = 4
};

class MiningTruck
{
public:
    // MiningTruck();
    MiningTruck(int truckID, TruckState status);
    ~MiningTruck();
    int getID() const;
    void setStatus(TruckState status);
    TruckState getStatus() const;

    // The idea is that in our loop we can have the trucks on their own go on mining missions
    // Our mining mission function will assign them to a set amount of time, we then can use that to put them into a queue
    // If we use a queue we can even adjust how fast we want to step through time per the simulation

    void goMining();

    // Stat accumulators — called by main at each state transition
    void recordMining(int minutes);
    void recordTransit(int minutes);
    void recordWaiting(int minutes);
    void recordUnloading(int minutes);
    void recordLoad();

    void printStats(int totalSimMinutes);

private:
    const int m_truckID; // Unique identifier no need for setter, set in constructor
    TruckState m_status;
    int m_loadsCompleted  = 0;
    int m_minutesMining   = 0;
    int m_minutesTransit  = 0;
    int m_minutesWaiting  = 0;
    int m_minutesUnloading = 0;
};
