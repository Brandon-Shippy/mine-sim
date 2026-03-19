#include "unloadSite.h"
#include <iostream>
#include <iomanip>

UnloadSite::UnloadSite(std::string siteID)
    : m_siteID(siteID)
{
}

UnloadSite::~UnloadSite() {}

std::string UnloadSite::getID() const { return m_siteID; }

// Station is empty when nothing is unloading AND no trucks are waiting
bool UnloadSite::isSiteEmpty()
{
    return m_currTruckID == -1 && m_queue.empty();
}

// Total depth — current truck + waiting queue — used to find shortest queue
int UnloadSite::getQueueLength()
{
    int depth = static_cast<int>(m_queue.size());
    if (m_currTruckID != -1)
        depth++;
    return depth;
}

int UnloadSite::getCurrentTruckID() { return m_currTruckID; }

void UnloadSite::addToQueue(int id)
{
    m_queue.push(id);
}

void UnloadSite::moveQueueForward()
{
    if (!m_queue.empty())
    {
        m_currTruckID = m_queue.front();
        m_queue.pop();
    }
    else
    {
        m_currTruckID = -1;
    }
}

void UnloadSite::recordTruckServed() { m_trucksServed++; }
void UnloadSite::recordIdleMinutes(int minutes) { m_minutesIdle += minutes; }

void UnloadSite::printStats(int totalSimMinutes)
{
    double pctIdle = (100.0 * m_minutesIdle) / totalSimMinutes;
    std::cout << std::fixed << std::setprecision(1);
    std::cout << "  Site " << m_siteID
              << " | Trucks served: " << m_trucksServed
              << " | Idle: " << pctIdle << "%"
              << " | Active: " << (100.0 - pctIdle) << "%"
              << "\n";
}
