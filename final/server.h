#pragma once
#include "Customer.h"

// Server.h
// Defines the Server class modeling a single teller in the simulation.

class Server {
private:
    Customer* currentCustomer;  // Pointer to the currently served customer (nullptr if free)
    bool freeStatus;            // True if available for a new customer
    int remainingTime;          // Ticks left to finish current service
    int totalServedCount;       // Number of fully served customers

public:
    // Default constructor
    Server();

    // Status checkers
    bool isFree() const;
    bool isBusy() const;
    
    // Getters
    int getRemainingTime() const;
    int getTotalServedTime() const; // Note: Returns count as per UML
    Customer* getCurrentCustomer() const;

    // Assigns a new customer, sets server to busy, and updates customer status
    void assignCustomer(Customer* c);

    // Called each simulation tick to reduce remaining service time
    void updateTime();

    // Completes service, marks customer as DONE, and frees the server
    void finishService();
};