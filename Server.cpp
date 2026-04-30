#include "Server.h"
#include <iostream>

Server::Server() {
    currentCustomer = nullptr;
    busy = false;
    remainingTime = 0;
    servedCount = 0;
}

bool Server::isFree() const {
    return !busy;
}

bool Server::isBusy() const {
    return busy;
}

void Server::assignCustomer(Customer* c) {
    currentCustomer = c;
    busy = true;
    remainingTime = c->getTransactionTime();
    currentCustomer->setStatus(SERVING);
}

void Server::updateTime() {
    if (busy && remainingTime > 0) {
        remainingTime--;
        if (remainingTime == 0) {
            finishService();
        }
    }
}

void Server::finishService() {
    if (currentCustomer != nullptr) {
        currentCustomer->setStatus(DONE);
        currentCustomer = nullptr;
    }
    busy = false;
    servedCount++;
}

Customer* Server::getCurrentCustomer() const {
    return currentCustomer;
}

int Server::getRemainingTime() const {
    return remainingTime;
}

int Server::getTotalServedCount() const {
    return servedCount;
}

void Server::printStatus() const {
    if (busy) {
        std::cout << "Server is BUSY with Customer " << currentCustomer->getId() 
                  << " | Remaining Time: " << remainingTime << std::endl;
    } else {
        std::cout << "Server is FREE" << std::endl;
    }
}
