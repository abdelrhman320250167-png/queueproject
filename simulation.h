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

    void processArrival();
    void updateServers();
    void assignCustomersToServers();

public:
    Simulation(int numServers, int maxSimulationTime);
    void runTick();
    void addCustomerExternal(Customer* c);
    int getCurrentTime() const;
    int getArrivedCount() const;
    const std::vector<Server>& getServers() const;
    const Queue& getQueue() const;
    const std::vector<int>& getQueueHistory() const;
    bool getIsRunning() const;
};

#endif
