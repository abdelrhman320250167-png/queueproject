#include "Server.h"

// Default constructor
Server::Server()
    : currentCustomer(nullptr),
      freeStatus(true),
      remainingTime(0),
      totalServedCount(0) {}

// Status checkers
bool Server::isFree() const { return freeStatus; }
bool Server::isBusy() const { return !freeStatus; }

// Getters
int Server::getRemainingTime() const { return remainingTime; }
int Server::getTotalServedTime() const { return totalServedCount; } // Note: Returns count as per UML
Customer* Server::getCurrentCustomer() const { return currentCustomer; }

// Assigns a new customer, sets server to busy, and updates customer status
void Server::assignCustomer(Customer* c) {
    if (!c) return;
    currentCustomer = c;
    freeStatus = false;
    remainingTime = c->getServiceTime();
    c->setStatus(Status::SERVING);
}

// Called each simulation tick to reduce remaining service time
void Server::updateTime() {
    if (!freeStatus && currentCustomer) {
        --remainingTime;
        if (remainingTime <= 0) {
            finishService();
        }
    }
}

// Completes service, marks customer as DONE, and frees the server
void Server::finishService() {
    if (currentCustomer) {
        currentCustomer->setStatus(Status::DONE);
        currentCustomer = nullptr;
        freeStatus = true;
        ++totalServedCount;
        remainingTime = 0;
    }
}