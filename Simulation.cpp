#include "Simulation.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
using namespace std;

Simulation::Simulation(int numServers, int maxSimulationTime) {
    this->currentTime = 0;
    this->maxTime = maxSimulationTime;
    this->isRunning = true;
    this->arrivedCount = 0;
    this->servedCount = 0;
    for (int i = 0; i < numServers; i++) {
        servers.push_back(Server(i + 1)); 
    }
    
    srand(time(0)); 
}

void Simulation::runTick() {
    if (!isRunning || currentTime >= maxTime) {
        isRunning = false;
        return;
    }
    processArrival();
    updateServers();
    assignCustomersToServers();
    queueHistory.push_back(q.getQueueSize());
    q.updateWaitingTimes(currentTime);

    currentTime++;
}

void Simulation::processArrival() {
    // 30% chance of arrival each tick
    if ((rand() % 100) < 30) {
        arrivedCount++;
        int serviceTime = (rand() % 8) + 3; 
        bool isVIP = (rand() % 100) < 15;    
  
        Customer* newCust = new Customer(arrivedCount, currentTime, serviceTime, isVIP);
        q.enqueueCustomer(newCust);
    }
}

void Simulation::updateServers() {
    for (size_t i = 0; i < servers.size(); i++) {
        if (!servers[i].isFree()) {
            servers[i].advanceTime(); 
            
            if (servers[i].getRemainingTime() <= 0) {
                servers[i].finishService();
                servedCount++;
            }
        }
    }
}

void Simulation::assignCustomersToServers() {
    for (size_t i = 0; i < servers.size(); i++) {
        if (servers[i].isFree() && !q.isEmpty()) {
            // dequeueCustomer returns a pointer (Customer*)
            Customer* next = q.dequeueCustomer();
            servers[i].serveCustomer(next); 
        }
    }
}

int Simulation::getCurrentTime() const {
    return currentTime;
}

const vector<Server>& Simulation::getServers() const {
    return servers;
}

const Queue& Simulation::getQueue() const {
    return q;
}

const vector<int>& Simulation::getQueueHistory() const {
    return queueHistory;
}

bool Simulation::getIsRunning() const {
    return isRunning;
}
