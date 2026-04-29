#ifndef SIMULATION_H
#define SIMULATION_H
#include "Queue.h"
#include "Server.h"
#include <vector>

class Simulation {
private:
    int currentTime;
    Queue q;
    std::vector<Server> servers;

public:
    Simulation(int numServers);
    void runTick(); 
    int getCurrentTime() const;
};
//3bdo_7anaka
#endif
