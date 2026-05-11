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
    int getRandomInt(int min, int max);

    // Helper: 15% chance for VIP status
    bool getRandomVIP();

public:
    // Constructor initializes RNG and state
    Simulation(int numServers = 3, int maxTime = 100);

    // Resets simulation to initial state without re-instantiation
    void initializeSimulation();

    // Main tick update method
    void updateSimulation();

    // Handles logic for potential new arrivals (80% chance per tick)
    void processArrival();

    // Updates server timers and tracks served count
    void updateServers();

    // Moves customers from queue to free servers
    void assignCustomersToServers();

    // Getters
    int getCurrentTime() const;
    int getMaxTime() const;
    int getArrivedCount() const;
    int getServedCount() const;
    bool getIsRunning() const;
    
    // Returns references to prevent copying
    const std::vector<Server>& getServers() const;
    const Queue& getQueue() const;
    const std::vector<int>& getQueueHistory() const;

    // Pause controls
    void togglePause();
    bool getIsPaused() const;
};