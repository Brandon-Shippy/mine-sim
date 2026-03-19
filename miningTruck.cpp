#include "miningTruck.h"
#include <iostream>
#include <iomanip>

// MiningTruck::MiningTruck()
// {
//     m_stringTruckID = "";
//     m_status = 0;
// }

MiningTruck::MiningTruck(int truckID, TruckState status)
    : m_truckID(truckID), // Need to assign in initilizer list because of references and consts
      m_status(status)
{
}

MiningTruck::~MiningTruck() {}

int MiningTruck::getID() const { return m_truckID; }
void MiningTruck::setStatus(TruckState status) { m_status = status; }
TruckState MiningTruck::getStatus() const { return m_status; }

void MiningTruck::goMining()
{
    m_status = TruckState::MINING;
}

void MiningTruck::recordMining(int minutes)   { m_minutesMining    += minutes; }
void MiningTruck::recordTransit(int minutes)  { m_minutesTransit   += minutes; }
void MiningTruck::recordWaiting(int minutes)  { m_minutesWaiting   += minutes; }
void MiningTruck::recordUnloading(int minutes){ m_minutesUnloading += minutes; }
void MiningTruck::recordLoad()                { m_loadsCompleted++; }

void MiningTruck::printStats(int totalSimMinutes)
{
    auto pct = [&](int minutes) { return (100.0 * minutes) / totalSimMinutes; };

    std::cout << std::fixed << std::setprecision(1);
    std::cout << "  Truck " << m_truckID
              << " | Loads: "     << m_loadsCompleted
              << " | Mining: "    << pct(m_minutesMining)    << "%"
              << " | Transit: "   << pct(m_minutesTransit)   << "%"
              << " | Waiting: "   << pct(m_minutesWaiting)   << "%"
              << " | Unloading: " << pct(m_minutesUnloading) << "%"
              << "\n";
}
