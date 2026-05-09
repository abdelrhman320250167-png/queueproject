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
    Server()
        : currentCustomer(nullptr),
          freeStatus(true),
          remainingTime(0),
          totalServedCount(0) {}

    // Status checkers
    bool isFree() const { return freeStatus; }
    bool isBusy() const { return !freeStatus; }
    
    // Getters
    int getRemainingTime() const { return remainingTime; }
    int getTotalServedTime() const { return totalServedCount; } // Note: Returns count as per UML
    Customer* getCurrentCustomer() const { return currentCustomer; }

    // Assigns a new customer, sets server to busy, and updates customer status
    void assignCustomer(Customer* c) {
        if (!c) return;
        currentCustomer = c;
        freeStatus = false;
        remainingTime = c->getServiceTime();
        c->setStatus(Status::SERVING);
    }

    // Called each simulation tick to reduce remaining service time
    void updateTime() {
        if (!freeStatus && currentCustomer) {
            --remainingTime;
            if (remainingTime <= 0) {
                finishService();
            }
        }
    }

    // Completes service, marks customer as DONE, and frees the server
    void finishService() {
        if (currentCustomer) {
            currentCustomer->setStatus(Status::DONE);
            currentCustomer = nullptr;
            freeStatus = true;
            ++totalServedCount;
            remainingTime = 0;
        }
    }
};