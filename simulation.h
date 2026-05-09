#ifndef SIMULATION_H
#define SIMULATION_H

#include "Queue.h"
#include "Server.h"
#include <vector>

class Simulation {
private:
    int currentTime;
    int maxTime;
    bool isRunning;
    int arrivedCount;
    int servedCount;
    std::vector<int> queueHistory;

    Queue q;
    std::vector<Server> servers;

    public:
    void processArrival();
    void updateServers();
    void assignCustomersToServers();

    Simulation(int numServers, int maxSimulationTime);

    void runTick();
    int getCurrentTime() const;

    const std::vector<Server>& getServers() const;
    const Queue& getQueue() const;
    const std::vector<int>& getQueueHistory() const;
    bool getIsRunning() const;
};

#endif
