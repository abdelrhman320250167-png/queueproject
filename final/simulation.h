#pragma once

// Simulation.h
// Defines the Simulation class orchestrating the bank queue model.

#include <vector>
#include <random>
#include "Queue.h"
#include "Server.h"

class Simulation {
private:
    int currentTime;              // Current simulation tick
    int maxSimulationTime;        // Maximum duration
    int totalCustomersArrived;    // Total arrivals count
    int totalCustomersServed;     // Total served count
    int nextArrivalTime;          // Next arrival time (extensibility)
    bool isRunning;               // Flag for simulation active state
    bool isPaused;                // Flag for pause state
    Queue customerQueue;          // Waiting queue (Uses Linked List Implementation)
    std::vector<Server> servers;  // Collection of servers
    std::vector<int> queueHistory;// Queue size history for GUI graph
    std::mt19937 rng;             // Random number generator

    // Helper: Generate random integer [min, max]
    int getRandomInt(int min, int max) {
        std::uniform_int_distribution<int> dist(min, max);
        return dist(rng);
    }

    // Helper: 15% chance for VIP status
    bool getRandomVIP() {
        return getRandomInt(0, 99) < 35;
    }

public:
    // Constructor initializes RNG and state
    Simulation(int numServers = 3, int maxTime = 100)
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
    void initializeSimulation() {
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
    void updateSimulation() {
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
    void processArrival() {
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
    void updateServers() {
        totalCustomersServed = 0;
        for (Server& s : servers) {
            s.updateTime();
            totalCustomersServed += s.getTotalServedTime();
        }
    }

    // Moves customers from queue to free servers
    void assignCustomersToServers() {
        for (Server& s : servers) {
            if (s.isFree() && !customerQueue.isEmpty()) {
                Customer* next = customerQueue.dequeueCustomer();
                s.assignCustomer(next);
            }
        }
    }

    // Getters
    int getCurrentTime() const { return currentTime; }
    int getMaxTime() const { return maxSimulationTime; }
    int getArrivedCount() const { return totalCustomersArrived; }
    int getServedCount() const { return totalCustomersServed; }
    bool getIsRunning() const { return isRunning; }
    
    // Returns references to prevent copying
    const std::vector<Server>& getServers() const { return servers; }
    const Queue& getQueue() const { return customerQueue; }
    const std::vector<int>& getQueueHistory() const { return queueHistory; }

    // Pause controls
    void togglePause() { isPaused = !isPaused; }
    bool getIsPaused() const { return isPaused; }
};