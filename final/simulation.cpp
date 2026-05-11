#include "Simulation.h"

// Helper: Generate random integer [min, max]
int Simulation::getRandomInt(int min, int max) {
    std::uniform_int_distribution<int> dist(min, max);
    return dist(rng);
}

// Helper: 15% chance for VIP status
bool Simulation::getRandomVIP() {
    return getRandomInt(0, 99) < 35; // 35 as per your original logic
}

// Constructor initializes RNG and state
Simulation::Simulation(int numServers, int maxTime)
    : currentTime(0),
    maxSimulationTime(maxTime),
    totalCustomersArrived(0),
    totalCustomersServed(0),
    nextArrivalTime(0),
    isRunning(true),
    isPaused(false),
    servers(numServers) {
    std::random_device rd;
    rng.seed(rd());
}

// Resets simulation to initial state without re-instantiation
void Simulation::initializeSimulation() {
    currentTime = 0;
    totalCustomersArrived = 0;
    totalCustomersServed = 0;
    nextArrivalTime = 0;
    isRunning = true;
    isPaused = false;
    
    // Reset queue using assignment operator
    customerQueue = Queue(); 
    queueHistory.clear();
    
    // Reset all servers
    for (Server& s : servers) {
        s = Server();
    }
}

// Main tick update method
void Simulation::updateSimulation() {
    if (!isRunning || isPaused) return;
    if (currentTime >= maxSimulationTime) {
        isRunning = false;
        return;
    }
    
    processArrival();
    updateServers();
    assignCustomersToServers();
    
    queueHistory.push_back(customerQueue.getQueueSize());
    customerQueue.updateWaitingTimes(currentTime);
    
    ++currentTime;
}

// Handles logic for potential new arrivals (80% chance per tick)
void Simulation::processArrival() {
    if (getRandomInt(0, 99) < 80) {
        ++totalCustomersArrived;
        int serviceTime = getRandomInt(5, 7);
        bool vip = getRandomVIP();
        
        // Note: Simulation handles allocation, Queue handles deallocation upon dequeue
        Customer* newCust = new Customer(totalCustomersArrived, currentTime, serviceTime, vip);
        customerQueue.enqueueCustomer(newCust);
    }
}

// Updates server timers and tracks served count
void Simulation::updateServers() {
    totalCustomersServed = 0;
    for (Server& s : servers) {
        s.updateTime();
        totalCustomersServed += s.getTotalServedTime();
    }
}

// Moves customers from queue to free servers
void Simulation::assignCustomersToServers() {
    for (Server& s : servers) {
        if (s.isFree() && !customerQueue.isEmpty()) {
            Customer* next = customerQueue.dequeueCustomer();
            s.assignCustomer(next);
        }
    }
}

// Getters
int Simulation::getCurrentTime() const { return currentTime; }
int Simulation::getMaxTime() const { return maxSimulationTime; }
int Simulation::getArrivedCount() const { return totalCustomersArrived; }
int Simulation::getServedCount() const { return totalCustomersServed; }
bool Simulation::getIsRunning() const { return isRunning; }

// Returns references to prevent copying
const std::vector<Server>& Simulation::getServers() const { return servers; }
const Queue& Simulation::getQueue() const { return customerQueue; }
const std::vector<int>& Simulation::getQueueHistory() const { return queueHistory; }

// Pause controls
void Simulation::togglePause() { isPaused = !isPaused; }
bool Simulation::getIsPaused() const { return isPaused; }